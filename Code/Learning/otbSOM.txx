/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _otbSOM_txx
#define _otbSOM_txx

#include "otbSOM.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkContinuousIndex.h"
#include "itkRandomImageSource.h"
#include "otbMacro.h"
#include "itkImageRegionIterator.h"

namespace otb
{
/** 
 * Constructor 
 */
template <class TListSample,class TMap>  
SOM<TListSample,TMap>
::SOM()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  m_MapSize.Fill(10);
  m_NumberOfIterations=10;
  m_BetaInit = 1.0;
  m_BetaEnd = 0.2;
  m_NeighborhoodSizeInit.Fill(3);
  m_MinWeight=static_cast<ValueType>(0.0);
  m_MaxWeight=static_cast<ValueType>(128.0);
  m_RandomInit=false;
}
/** 
 * Destructor 
 */
template <class TListSample,class TMap>  
SOM<TListSample,TMap>
::~SOM()
{
}
/**
 * Update the output map with a new sample.
 * \param sample The new sample to learn,
 * \param beta The learning coefficient,
 * \param radius The radius of the nieghbourhood.
 */
template <class TListSample,class TMap>  
void
SOM<TListSample,TMap>
::UpdateMap(const NeuronType& sample, double beta, SizeType& radius)
{
  // output map pointer
  MapPointerType map = this->GetOutput();

  // typedefs
  typedef itk::ImageRegionIteratorWithIndex<MapType> IteratorType;
  typedef itk::ContinuousIndex<double,MapType::ImageDimension> ContinuousIndexType;
  typedef itk::Statistics::EuclideanDistance<ContinuousIndexType> DistanceType; 
  typename DistanceType::Pointer distance = DistanceType::New();

  // winner index in the map
  IndexType position = map->GetWinner(sample);
  NeuronType winner = map->GetPixel(position);

  // Local neighborhood definition
  RegionType localRegion;
  IndexType  localIndex = position-radius;
  SizeType   localSize;
  for(unsigned int i=0;i<MapType::ImageDimension;++i)
    {
      localSize[i]= 2*radius[i]+1;
    }
  localRegion.SetIndex(localIndex);
  localRegion.SetSize(localSize);
  localRegion.Crop(map->GetLargestPossibleRegion());
  IteratorType it(map,localRegion);

  // Walk through the map, and evolve each neuron depending on its 
  // distance to the winner.
  for(it.GoToBegin();!it.IsAtEnd();++it)
    {
      NeuronType tempNeuron = it.Get();
      NeuronType newNeuron;
      double tempBeta = beta/(1+distance->Evaluate(ContinuousIndexType(position),ContinuousIndexType(it.GetIndex())));
      for(unsigned int i = 0; i<NeuronType::Length;++i)
	{
	 newNeuron[i] = tempNeuron[i]+static_cast<typename NeuronType::ValueType>((sample[i]-tempNeuron[i])*tempBeta);
	}
      it.Set(newNeuron);
    }
}
/**
 * Step one iteration.
 */
template <class TListSample,class TMap>  
void 
SOM<TListSample,TMap>
::Step(unsigned int currentIteration)
{  
  InputImagePointerType image = const_cast<InputImageType*>(this->GetInput());

  // Compute the new learning coefficient
  double newBeta = (m_BetaEnd-m_BetaInit)/static_cast<double>(m_NumberOfIterations)
    *static_cast<double>(currentIteration)+m_BetaInit;

  // Compute the new neighborhood size
  SizeType newSize;
  newSize[0]=m_NeighborhoodSizeInit[0]-static_cast<int>((static_cast<float>(currentIteration)
  /static_cast<float>(m_NumberOfIterations))*static_cast<float>(m_NeighborhoodSizeInit[0]-2));

  newSize[1]=m_NeighborhoodSizeInit[1]-static_cast<int>((static_cast<float>(currentIteration)
  /static_cast<float>(m_NumberOfIterations))*static_cast<float>(m_NeighborhoodSizeInit[1]-2));

  // define an iterator on the training set 
  typedef itk::ImageRegionIterator<InputImageType> IteratorType;
  IteratorType it(image,image->GetLargestPossibleRegion());

 // update the neurons map with each example of the training set.
 otbMsgDebugMacro(<<"Beta: "<<newBeta<<", radius: "<<newSize);
 for(it.GoToBegin();!it.IsAtEnd();++it)
   {
     UpdateMap(it.Get(),newBeta,newSize);
   }
}
/** 
 * Main computation method 
 */
template <class TListSample,class TMap>  
void
SOM<TListSample,TMap>
::GenerateData(void)
{
  // output neuron map fill
  MapPointerType map = this->GetOutput();
  RegionType region;
  IndexType index;
  index.Fill(0);
  region.SetIndex(index);
  region.SetSize(m_MapSize);
  map->SetRegions(region);
  map->Allocate();
  
  if(m_RandomInit)
    {
      

    }
  else
    {
      NeuronType neuronInit;
      neuronInit.Fill(m_MaxWeight);
      map->FillBuffer(neuronInit);
    }
  
  // Step through the iterations
  for(unsigned int i = 0;i<m_NumberOfIterations;++i)
    {
      otbMsgDebugMacro(<<"Step "<<i+1<<" / "<<m_NumberOfIterations);
      Step(i);
    }
}
/** 
 *PrintSelf method 
 */
template <class TListSample,class TMap>  
void 
SOM<TListSample,TMap>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}
} // end namespace otb
#endif
