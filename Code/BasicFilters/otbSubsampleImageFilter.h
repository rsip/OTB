/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSubsampleImageFilter_h
#define __otbSubsampleImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"

// This include is needed to define InverseOrForwardTransformationEnum only...
#include "otbGenericMapProjection.h"

namespace otb {

/** \class SubsampleImageFilter
 * \brief Performs a down sampling of an image
 *
 * The third template is a FORWARD/INVERSE InverseOrForwardTransformationEnum 
 * enumeration type. With FORWARD, the output image is of the same size of the one of 
 * the input image. But the output image contains only co-located subsample, the reset being 0.
 * When INVERSE is used, the input image is considered to as the sub-sampled. then, the output
 * image is larger. Inital pixel values are preserved but the output image is interleaved with
 * 0.
 *
 * \sa ResampleImageFilter
 * \sa SubsampleImageRegionConstIterator
 * \sa DecimateIMageFilter
 */
template < class TInputImage, class TOutputImage, 
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
class ITK_EXPORT SubsampleImageFilter :
  public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef SubsampleImageFilter Self;
  typedef itk::ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SubsampleImageFilter, ImageToImageFilter);

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Direction definition */
  typedef InverseOrForwardTransformationEnum DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation,DirectionOfTransformationEnumType,TDirectionOfTransformation);

  /** Image typedef support. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::IndexType InputImageIndexType;

  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::SizeType OutputImageSizeType;
  typedef typename OutputImageType::IndexType OutputImageIndexType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  /** Set/Get the SubsampleFactor */
  itkGetMacro(SubsampleFactor,unsigned int);
  itkSetMacro(SubsampleFactor,unsigned int);

protected:
  SubsampleImageFilter ()
  {
    m_SubsampleFactor = 1;
  }
  virtual ~SubsampleImageFilter() {}

  /** Since input and output image are very likely to be of different size. 
   * Region estimation functions has to be reimplemented
   */
  virtual void CallCopyOutputRegionToInputRegion 
    ( InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion );
  virtual void CallCopyInputRegionToOutputRegion
    ( OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion );	 

  /** Output image region size is not of the same dimension as the input.
   * That is why GenerateOutputInformation has to be redefined.
   */
  virtual void GenerateOutputInformation();

  /** Allows multithreading */
  virtual void ThreadedGenerateData 
    ( const OutputImageRegionType & outputRegionForThread, int threadId );

  virtual void PrintSelf( std::ostream & os, itk::Indent indent ) const;

private:
  SubsampleImageFilter ( const Self & ); // purposely not implemented
  void operator= ( const Self & ); // purposely not implemented

  unsigned int m_SubsampleFactor;
}; // end of class 

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSubsampleImageFilter.txx"
#endif



#endif

