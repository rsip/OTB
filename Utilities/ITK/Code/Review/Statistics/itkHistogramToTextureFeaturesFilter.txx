/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHistogramToTextureFeaturesFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-08 16:55:05 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHistogramToTextureFeaturesFilter_txx
#define __itkHistogramToTextureFeaturesFilter_txx

#include "itkHistogramToTextureFeaturesFilter.h"

#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace itk {
namespace Statistics {

//constructor 
template< class THistogram >
HistogramToTextureFeaturesFilter< THistogram >::
HistogramToTextureFeaturesFilter( void )
{
  this->ProcessObject::SetNumberOfRequiredInputs(1);

  // allocate the data objects for the outputs which are
  // just decorators real types
  for (int i=0; i < 8; ++i)
    {
    this->ProcessObject::SetNthOutput( i, this->MakeOutput(i) );
    }
}

template< class THistogram >
void
HistogramToTextureFeaturesFilter< THistogram >
::SetInput( const HistogramType * histogram )
{
  this->ProcessObject::SetNthInput(0, 
                                   const_cast< HistogramType* >( histogram ) );
}

template< class THistogram >
const typename 
HistogramToTextureFeaturesFilter<THistogram>::HistogramType *
HistogramToTextureFeaturesFilter< THistogram >
::GetInput( ) const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const HistogramType * >
  (this->ProcessObject::GetInput(0) );
}


template<class THistogram>
typename
HistogramToTextureFeaturesFilter<THistogram>::DataObjectPointer
HistogramToTextureFeaturesFilter<THistogram>
::MakeOutput( unsigned int itkNotUsed( idx ) )
{
  return static_cast<DataObject*>(MeasurementObjectType::New().GetPointer());
}


template< class THistogram >
void
HistogramToTextureFeaturesFilter< THistogram >::
GenerateData( void )
{
  typedef typename HistogramType::ConstIterator HistogramIterator;

  const HistogramType * inputHistogram = this->GetInput();

  //Normalize the absolute frequencies and populate the relative frequency
  //container
  TotalRelativeFrequencyType    totalFrequency = 
               static_cast< TotalRelativeFrequencyType >(inputHistogram->GetTotalFrequency());

  m_RelativeFrequencyContainer.clear();

  for (HistogramIterator hit = inputHistogram->Begin();
       hit != inputHistogram->End(); ++hit)
    {
    AbsoluteFrequencyType  frequency = hit.GetFrequency();
    RelativeFrequencyType  relativeFrequency =  frequency / totalFrequency;
    m_RelativeFrequencyContainer.push_back( relativeFrequency );
    }
   
  // Now get the various means and variances. This is takes two passes
  // through the histogram.
  double pixelMean;
  double marginalMean;
  double marginalDevSquared;
  double pixelVariance;

  this->ComputeMeansAndVariances(pixelMean, marginalMean, marginalDevSquared,
                                 pixelVariance);
  
        
  // Finally compute the texture features. Another one pass.
  MeasurementType energy      = NumericTraits< MeasurementType>::Zero;
  MeasurementType entropy     = NumericTraits< MeasurementType>::Zero;
  MeasurementType correlation = NumericTraits< MeasurementType>::Zero;

  MeasurementType inverseDifferenceMoment      = 
                              NumericTraits< MeasurementType>::Zero;

  MeasurementType inertia             = NumericTraits< MeasurementType>::Zero;
  MeasurementType clusterShade        = NumericTraits< MeasurementType>::Zero;
  MeasurementType clusterProminence   = NumericTraits< MeasurementType>::Zero;
  MeasurementType haralickCorrelation = NumericTraits< MeasurementType>::Zero;
 
  double pixelVarianceSquared = pixelVariance * pixelVariance;
  double log2 = vcl_log(2.0);

  typename RelativeFrequencyContainerType::const_iterator rFreqIterator = 
                                         m_RelativeFrequencyContainer.begin(); 

  for (HistogramIterator hit = inputHistogram->Begin();
       hit != inputHistogram->End(); ++hit)
    {
    RelativeFrequencyType frequency = *rFreqIterator; 
    ++rFreqIterator;
    if (frequency == 0)
      {
      continue; // no use doing these calculations if we're just multiplying by zero.
      }
    
    IndexType index = inputHistogram->GetIndex(hit.GetInstanceIdentifier());
    energy += frequency * frequency;
    entropy -= (frequency > 0.0001) ? frequency * vcl_log(frequency) / log2 : 0;
    correlation += ( (index[0] - pixelMean) * (index[1] - pixelMean) * frequency)
      / pixelVarianceSquared;
    inverseDifferenceMoment += frequency /
      (1.0 + (index[0] - index[1]) * (index[0] - index[1]) );
    inertia += (index[0] - index[1]) * (index[0] - index[1]) * frequency;
    clusterShade += vcl_pow((index[0] - pixelMean) + (index[1] - pixelMean), 3) *
      frequency;
    clusterProminence += vcl_pow((index[0] - pixelMean) + (index[1] - pixelMean), 4) *
      frequency;
    haralickCorrelation += index[0] * index[1] * frequency;
    }
  
  haralickCorrelation = (haralickCorrelation - marginalMean * marginalMean) /
    marginalDevSquared;

  MeasurementObjectType* energyOutputObject=
                   static_cast<MeasurementObjectType*>(this->ProcessObject::GetOutput(0));
  energyOutputObject->Set( energy );

  MeasurementObjectType* entropyOutputObject=
                   static_cast<MeasurementObjectType*>(this->ProcessObject::GetOutput(1));
  entropyOutputObject->Set( entropy );

  MeasurementObjectType* correlationOutputObject=
                   static_cast<MeasurementObjectType*>(this->ProcessObject::GetOutput(2));
  correlationOutputObject->Set( correlation );

  MeasurementObjectType* inverseDifferenceMomentOutputObject=
                   static_cast<MeasurementObjectType*>(this->ProcessObject::GetOutput(3));
  inverseDifferenceMomentOutputObject->Set( inverseDifferenceMoment );

  MeasurementObjectType* inertiaOutputObject=
                   static_cast<MeasurementObjectType*>(this->ProcessObject::GetOutput(4));
  inertiaOutputObject->Set( inertia );

  MeasurementObjectType* clusterShadeOutputObject=
                   static_cast<MeasurementObjectType*>(this->ProcessObject::GetOutput(5));
  clusterShadeOutputObject->Set( clusterShade );

  MeasurementObjectType* clusterProminenceOutputObject=
                   static_cast<MeasurementObjectType*>(this->ProcessObject::GetOutput(6));
  clusterProminenceOutputObject->Set( clusterProminence );

  MeasurementObjectType* haralickCorrelationOutputObject=
                   static_cast<MeasurementObjectType*>(this->ProcessObject::GetOutput(7));
  haralickCorrelationOutputObject->Set( haralickCorrelation );
}

template< class THistogram >
void
HistogramToTextureFeaturesFilter< THistogram >::
ComputeMeansAndVariances( double &pixelMean, double &marginalMean, 
                          double &marginalDevSquared, double &pixelVariance )
{
  // This function takes two passes through the histogram and two passes through
  // an array of the same length as a histogram axis. This could probably be
  // cleverly compressed to one pass, but it's not clear that that's necessary.
  
  typedef typename HistogramType::ConstIterator HistogramIterator;
  
  const HistogramType * inputHistogram =  this->GetInput();

  // Initialize everything
  typename HistogramType::SizeValueType binsPerAxis = inputHistogram->GetSize(0);
  double *marginalSums = new double[binsPerAxis];
  for (double *ms_It = marginalSums; 
       ms_It < marginalSums + binsPerAxis; ms_It++)
    {
    *ms_It = 0;
    }
  pixelMean = 0;
  
  typename RelativeFrequencyContainerType::const_iterator rFreqIterator = 
                                         m_RelativeFrequencyContainer.begin(); 

  // Ok, now do the first pass through the histogram to get the marginal sums
  // and compute the pixel mean
  HistogramIterator hit = inputHistogram->Begin(); 
  while( hit != inputHistogram->End() )
    {
    RelativeFrequencyType frequency = *rFreqIterator;
    IndexType index = inputHistogram->GetIndex(hit.GetInstanceIdentifier());
    pixelMean += index[0] * frequency;
    marginalSums[index[0]] += frequency;
    ++hit;
    ++rFreqIterator;
    }
  
  /*  Now get the mean and deviaton of the marginal sums.
      Compute incremental mean and SD, a la Knuth, "The  Art of Computer 
      Programming, Volume 2: Seminumerical Algorithms",  section 4.2.2. 
      Compute mean and standard deviation using the recurrence relation:
      M(1) = x(1), M(k) = M(k-1) + (x(k) - M(k-1) ) / k
      S(1) = 0, S(k) = S(k-1) + (x(k) - M(k-1)) * (x(k) - M(k))
      for 2 <= k <= n, then
      sigma = vcl_sqrt(S(n) / n) (or divide by n-1 for sample SD instead of
      population SD).
  */
  marginalMean = marginalSums[0];
  marginalDevSquared = 0;
  for (unsigned int arrayIndex = 1; arrayIndex < binsPerAxis; arrayIndex++)
    {
    int k = arrayIndex + 1;
    double M_k_minus_1 = marginalMean;
    double S_k_minus_1 = marginalDevSquared;
    double x_k = marginalSums[arrayIndex];
    
    double M_k = M_k_minus_1 + (x_k - M_k_minus_1) / k;
    double S_k = S_k_minus_1 + (x_k - M_k_minus_1) * (x_k - M_k);
    
    marginalMean = M_k;
    marginalDevSquared = S_k;
    }
  marginalDevSquared = marginalDevSquared / binsPerAxis;

  rFreqIterator = m_RelativeFrequencyContainer.begin(); 
  // OK, now compute the pixel variances.
  pixelVariance = 0;
  for (hit = inputHistogram->Begin(); hit != inputHistogram->End(); ++hit)
    {
    RelativeFrequencyType frequency = *rFreqIterator;
    IndexType index = inputHistogram->GetIndex(hit.GetInstanceIdentifier());
    pixelVariance += (index[0] - pixelMean) * (index[0] - pixelMean) * frequency;
    ++rFreqIterator;
    }

  delete [] marginalSums;
}

template<class THistogram >
const
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementObjectType*
HistogramToTextureFeaturesFilter<THistogram>
::GetEnergyOutput() const
{
  return static_cast< const MeasurementObjectType*>(this->ProcessObject::GetOutput(0));
}

template<class THistogram >
const
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementObjectType*
HistogramToTextureFeaturesFilter<THistogram>
::GetEntropyOutput() const
{
  return static_cast< const MeasurementObjectType*>(this->ProcessObject::GetOutput(1));
}

template<class THistogram >
const
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementObjectType*
HistogramToTextureFeaturesFilter<THistogram> 
::GetCorrelationOutput() const
{
  return static_cast< const MeasurementObjectType*>(this->ProcessObject::GetOutput(2));
}

template<class THistogram >
const
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementObjectType*
HistogramToTextureFeaturesFilter<THistogram> 
::GetInverseDifferenceMomentOutput() const
{
  return static_cast< const MeasurementObjectType*>(this->ProcessObject::GetOutput(3));
}

template<class THistogram >
const
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementObjectType*
HistogramToTextureFeaturesFilter<THistogram> 
::GetInertiaOutput() const
{
  return static_cast< const MeasurementObjectType*>(this->ProcessObject::GetOutput(4));
}

template<class THistogram >
const
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementObjectType*
HistogramToTextureFeaturesFilter<THistogram> 
::GetClusterShadeOutput() const
{
  return static_cast< const MeasurementObjectType*>(this->ProcessObject::GetOutput(5));
}

template<class THistogram >
const
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementObjectType*
HistogramToTextureFeaturesFilter<THistogram> 
::GetClusterProminenceOutput() const
{
  return static_cast< const MeasurementObjectType*>(this->ProcessObject::GetOutput(6));
}

template<class THistogram >
const
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementObjectType*
HistogramToTextureFeaturesFilter<THistogram> 
::GetHaralickCorrelationOutput() const
{
  return static_cast<const MeasurementObjectType*>(this->ProcessObject::GetOutput(7));
}

template<class THistogram >
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementType
HistogramToTextureFeaturesFilter<THistogram>
::GetEnergy() const 
{
  return this->GetEnergyOutput()->Get();
}

template<class THistogram >
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementType
HistogramToTextureFeaturesFilter<THistogram>
::GetEntropy() const 
{
  return this->GetEntropyOutput()->Get();
}

template<class THistogram >
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementType
HistogramToTextureFeaturesFilter<THistogram>
::GetCorrelation() const 
{
  return this->GetCorrelationOutput()->Get();
}

template<class THistogram >
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementType
HistogramToTextureFeaturesFilter<THistogram>
::GetInverseDifferenceMoment() const 
{
  return this->GetInverseDifferenceMomentOutput()->Get();
}

template<class THistogram >
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementType
HistogramToTextureFeaturesFilter<THistogram>
::GetInertia() const 
{
  return this->GetInertiaOutput()->Get();
}

template<class THistogram >
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementType
HistogramToTextureFeaturesFilter<THistogram>
::GetClusterShade() const 
{
  return this->GetClusterShadeOutput()->Get();
}

template<class THistogram >
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementType
HistogramToTextureFeaturesFilter<THistogram>
::GetClusterProminence() const 
{
  return this->GetClusterProminenceOutput()->Get();
}

template<class THistogram >
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementType
HistogramToTextureFeaturesFilter<THistogram>
::GetHaralickCorrelation() const 
{
  return this->GetHaralickCorrelationOutput()->Get();
}

template<class THistogram >
typename HistogramToTextureFeaturesFilter< THistogram >::MeasurementType
HistogramToTextureFeaturesFilter<THistogram>
::
GetFeature(TextureFeatureName feature)
{
  switch(feature)
    {
    case Energy:
      return this->GetEnergy();
    case Entropy:
      return this->GetEntropy();
    case Correlation:
      return this->GetCorrelation();
    case InverseDifferenceMoment:
      return this->GetInverseDifferenceMoment();
    case Inertia:
      return this->GetInertia();
    case ClusterShade:
      return this->GetClusterShade();
    case ClusterProminence:
      return this->GetClusterProminence();
    case HaralickCorrelation:
      return this->GetHaralickCorrelation();
    default:
      return 0;
    }
}

template< class THistogram >
void
HistogramToTextureFeaturesFilter< THistogram >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end of namespace Statistics 
} // end of namespace itk 


#endif
