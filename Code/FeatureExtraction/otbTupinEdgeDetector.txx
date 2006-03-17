/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   14 mars 2006
  Role      :   Filter of detection of linear features 
  $Id$ 

=========================================================================*/
#ifndef __otbTupinEdgeDetector_txx
#define __otbTupinEdgeDetector_txx

#include "otbTupinEdgeDetector.h"

#include "itkDataObject.h"
#include "itkExceptionObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkProgressReporter.h"
#include <math.h>

#define M_PI 3.14159265358979323846

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class InterpolatorType >
TupinEdgeDetector<TInputImage, TOutputImage, InterpolatorType>::TupinEdgeDetector()
{
  m_Radius.Fill(1);
  m_Interpolator = InterpolatorType::New(); 
}

template <class TInputImage, class TOutputImage, class InterpolatorType>
void TupinEdgeDetector<TInputImage, TOutputImage, InterpolatorType>::GenerateInputRequestedRegion() throw (itk::InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  typename Superclass::InputImagePointer inputPtr   =  const_cast< TInputImage * >( this->GetInput() );
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
  
  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( m_Radius );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    
    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << static_cast<const char *>(this->GetNameOfClass())
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

template< class TInputImage, class TOutputImage, class InterpolatorType>
void TupinEdgeDetector< TInputImage, TOutputImage, InterpolatorType>
::ThreadedGenerateData(	
			const 	OutputImageRegionType& 		outputRegionForThread,
                       	int 	threadId
		       )
{
  unsigned int i;
  itk::ZeroFluxNeumannBoundaryCondition<InputImageType> 	nbc;
  itk::ConstNeighborhoodIterator<InputImageType> 		bit;
  typename itk::ConstNeighborhoodIterator<InputImageType>::OffsetType	off;
  itk::ImageRegionIterator<OutputImageType> 			it;
  
  // Allocate output
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInput();
  
  m_Interpolator->SetInputImage(input);
   
  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType 		faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator 	fit;

 // Define the size of the region by the radius
  m_Radius[0] = static_cast<unsigned int>(0.5*( (3*m_WidthLine + 2) - 1 )); 
  m_Radius[1] = static_cast<unsigned int>(0.5*(m_LengthLine-1));
  
//std::cout <<"Radius " << m_Radius[0] << " " << m_Radius[1] << std::endl; 
  
  
  // Define the size of the facelist by taking into account the rotation of the region
  m_FaceList[0] = sqrt((m_Radius[0]*m_Radius[0])+ (m_Radius[1]*m_Radius[1]));
  m_FaceList[1] = m_FaceList[0];

  itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(input, outputRegionForThread, m_FaceList);


  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  
  typename TInputImage::IndexType     bitIndex;
  typename InterpolatorType::ContinuousIndexType Index;


  // --------------------------------------------------------------------------
  
  // Number of direction
  const int NB_DIR = 4;
  // Number of zone	  
  const int NB_ZONE = 3;  
  // Definition of the 4 directions
  double Theta[NB_DIR];
  
  Theta[0] = 0.;
  Theta[1] = M_PI / 4. ;
  Theta[2] = M_PI / 2. ;
  Theta[3] = 3*M_PI / 4. ;

  // Number of the zone 
  int zone;
   
  // Contains for the 4 directions the sum of the pixels belonging to each zone
  double Sum[NB_DIR][NB_ZONE];
  // Mean of zone 1, 2 and 3
  double M1, M2, M3;
  
  // Result of the filter for each direction
  double R12_theta[NB_DIR];
  double R13_theta[NB_DIR];
  // Result in one direction
  double R12, R13;
  // Intensity of the linear feature
  double R;

  // Pixel location in the input image
  int X, Y;
  
  // Pixel location in the system axis of the region after rotation 
  double xout, yout;
  
   // Pixel location in the input image after rotation
  TPoint point;
   
  // location of the pixel central in the input image
  int Xc, Yc;

  // location of the pixel central between zone 1 and 2 and between zone 1 and 3
  int Xc12, Xc13;
  
  //---------------------------------------------------------------------------
 
  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    { 
    bit = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius, input, *fit);
    unsigned int neighborhoodSize = bit.Size();
      
    it = itk::ImageRegionIterator<OutputImageType>(output, *fit);
    
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
    
//std::cout <<"***"<< neighborhoodSize << std::endl; 


    while ( ! bit.IsAtEnd() )
      {
std::cout << "Xc,Yc " << bit.GetIndex() << std::endl;

      // Initialisations
      for (int dir=0; dir<NB_DIR; dir++)
        {
        for (int z=0; z<NB_ZONE; z++) 
          Sum[dir][z] = 0.;
        }
        
     
      // Location of the central pixel of the region
      bitIndex = bit.GetIndex();
      
      Xc = bitIndex[0];
      Yc = bitIndex[1];
      
      // Location of the central pixel between zone 1 and zone 2
      Xc12 = ( Xc - (m_WidthLine-1)/2 ) - 1 ;
      
      // Location of the central pixel between zone 1 and zone 3
      Xc13 = ( Xc + (m_WidthLine-1)/2 ) + 1 ;
          
      // Loop on the region 
      for (i = 0; i < neighborhoodSize; ++i)
        {
std::cout << "---"<< i <<" "<< bit.GetIndex(i)<< std::endl;
std::cout << "val(X,Y) "<< static_cast<double>(bit.GetPixel(i)) << " " << std::endl;

        bitIndex = bit.GetIndex(i);
        X = bitIndex[0];
        Y = bitIndex[1];
 
//std::cout << "---"<< i <<" "<< static_cast<double>(bit.GetPixel(i))<< std::endl;     
      
        // We determine in the vertical direction with which zone the pixel belongs. 
         
        if ( X < Xc12 )
      	  zone = 1;
        else if ( ( Xc12 < X ) && ( X < Xc13 ) )
          zone = 0;
        else if ( X < Xc13 )
          zone = 2;
    
        Sum[0][zone] += static_cast<double>(bit.GetPixel(i));
         
        // Loop on the 3 other directions
        for ( int dir=1; dir<NB_DIR; dir++ )
          {      
            ROTATION( (X-Xc), (Y-Yc), Theta[dir], xout, yout);
            
           // point[0] = double(xout + Xc);
           // point[1] = double(yout + Yc);
            
            Index[0] = static_cast<float>(xout + Xc);
            Index[1] = static_cast<float>(yout + Yc);
                        
std::cout << "X' Y' "<< (xout + Xc) << " " << (yout + Yc) << std::endl;
std::cout << "val(X',Y') "<< static_cast<double>(m_Interpolator->Evaluate( point )) << std::endl;
           
            // Sum[dir][zone] += static_cast<double>(m_Interpolator->Evaluate( point ));
            Sum[dir][zone] += static_cast<double>(m_Interpolator->EvaluateAtContinuousIndex( Index ));

             
          }      

        } // end of the loop on the pixels of the region           

//std::cout << static_cast<double>(Sum[0][0])/ double(WidthZone*HeightZone) << std::endl;    
           
      // Loop on the 4 directions
      for ( int dir=0; dir<NB_DIR; dir++ )
        {
        	
        // Calculation of the averages of the 3 zones	
        M1 = Sum[dir][0] / double(m_WidthLine*m_LengthLine);
        M2 = Sum[dir][1] / double(m_WidthLine*m_LengthLine);
        M3 = Sum[dir][2] / double(m_WidthLine*m_LengthLine);
     	
        // Calculation of the intensity of the linear feature
        if (( M1 != 0 ) && (M2 != 0)) 
          R12_theta[dir] = static_cast<double>( 1 - MIN( (M1/M2), (M2/M1) ) );
        else
	  R12_theta[dir] = 0.;
	  
	if (( M1 != 0 ) && (M3 != 0)) 
          R13_theta[dir] = static_cast<double>( 1 - MIN( (M1/M3), (M3/M1) ) );
        else
	  R13_theta[dir] = 0.;
      		
        } // end of the loop on the directions
      
      // Determination of the maximum intensity of the linear feature
      R12 = R12_theta[0];
      R13 = R13_theta[0];
      
      for (int dir=1; dir<NB_DIR; dir++)
        {
        R12 = static_cast<double>( MAX( R12, R12_theta[dir] ) );
        R13 = static_cast<double>( MAX( R13, R13_theta[dir] ) );
	}
	
      // Intensity of the linear feature
      R = MIN ( R12, R13 );	
 
//std::cout << "R = " << R << std::endl; 

      // Assignment of this value to the output pixel
      it.Set( static_cast<OutputPixelType>(R) );  

      ++bit;
      ++it;
      progress.CompletedPixel();  
      
      }
      
    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput, class InterpolatorType>
void 
TupinEdgeDetector<TInputImage, TOutput, InterpolatorType>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Radius: " << m_Radius << std::endl;
}


} // end namespace otb


#endif
