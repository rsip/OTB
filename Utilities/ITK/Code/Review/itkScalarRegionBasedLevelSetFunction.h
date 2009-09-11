/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarRegionBasedLevelSetFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-05-13 14:23:05 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkScalarRegionBasedLevelSetFunction_h
#define __itkScalarRegionBasedLevelSetFunction_h

#include "itkRegionBasedLevelSetFunction.h"
#include "itkNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"

namespace itk {


/** \class ScalarRegionBasedLevelSetFunction
 *
 * \brief LevelSet function that computes a speed image based on regional integrals
 *
 * This class implements a level set function that computes the speed image by
 * integrating values on the image domain.
 *
 * Based on the paper:
 *
 *        "An active contour model without edges"
 *         T. Chan and L. Vese.
 *         In Scale-Space Theories in Computer Vision, pages 141–151, 1999.
 *
 * \author Mosaliganti K., Smith B., Gelas A., Gouaillard A., Megason S.
 *
 *  This code was taken from the Insight Journal paper:
 *
 *      "Cell Tracking using Coupled Active Surfaces for Nuclei and Membranes"
 *      http://www.insight-journal.org/browse/publication/642
 *      http://hdl.handle.net/10380/3055
 *
 *  That is based on the papers:
 *
 *      "Level Set Segmentation: Active Contours without edge"
 *      http://www.insight-journal.org/browse/publication/322
 *      http://hdl.handle.net/1926/1532
 *
 *      and
 *
 *      "Level set segmentation using coupled active surfaces"
 *      http://www.insight-journal.org/browse/publication/323
 *      http://hdl.handle.net/1926/1533
 *
 *
 */
template < class TInputImage, class TFeatureImage, class TSharedData >
class ITK_EXPORT ScalarRegionBasedLevelSetFunction
: public RegionBasedLevelSetFunction< TInputImage, TFeatureImage, TSharedData >
{
public:
  typedef ScalarRegionBasedLevelSetFunction         Self;
  typedef RegionBasedLevelSetFunction<
    TInputImage, TFeatureImage, TSharedData >       Superclass;
  typedef SmartPointer<Self>                        Pointer;
  typedef SmartPointer<const Self>                  ConstPointer;

  // itkNewMacro() is purposely not provided since this is an abstract class.

  /** Run-time type information (and related methods) */
  itkTypeMacro( ScalarRegionBasedLevelSetFunction, RegionBasedLevelSetFunction );

  itkStaticConstMacro( ImageDimension, unsigned int, TFeatureImage::ImageDimension );

  typedef typename Superclass::InputImageType         InputImageType;
  typedef typename Superclass::InputImageConstPointer InputImageConstPointer;
  typedef typename Superclass::InputImagePointer      InputImagePointer;
  typedef typename Superclass::InputPixelType         InputPixelType;
  typedef typename Superclass::InputIndexType         InputIndexType;
  typedef typename Superclass::InputIndexValueType    InputIndexValueType;
  typedef typename Superclass::InputSizeType          InputSizeType;
  typedef typename Superclass::InputSizeValueType     InputSizeValueType;
  typedef typename Superclass::InputRegionType        InputRegionType;
  typedef typename Superclass::InputPointType         InputPointType;

  typedef typename Superclass::FeatureImageType       FeatureImageType;
  typedef typename FeatureImageType::ConstPointer     FeatureImageConstPointer;
  typedef typename Superclass::FeaturePixelType       FeaturePixelType;
  typedef typename Superclass::FeatureIndexType       FeatureIndexType;
  typedef typename Superclass::FeatureOffsetType      FeatureOffsetType;

  typedef typename Superclass::ScalarValueType        ScalarValueType;
  typedef typename Superclass::NeighborhoodType       NeighborhoodType;
  typedef typename Superclass::FloatOffsetType        FloatOffsetType;
  typedef typename Superclass::RadiusType             RadiusType;
  typedef typename Superclass::TimeStepType           TimeStepType;
  typedef typename Superclass::GlobalDataStruct       GlobalDataStruct;
  typedef typename Superclass::PixelType              PixelType;
  typedef typename Superclass::VectorType             VectorType;

  typedef typename Superclass::SharedDataType         SharedDataType;
  typedef typename Superclass::SharedDataPointer      SharedDataPointer;

  typedef ImageRegionIteratorWithIndex< InputImageType >      ImageIteratorType;
  typedef ImageRegionConstIteratorWithIndex< InputImageType > ConstImageIteratorType;
  typedef ImageRegionIteratorWithIndex< FeatureImageType >    FeatureImageIteratorType;
  typedef ImageRegionConstIterator< FeatureImageType >        ConstFeatureIteratorType;

  typedef std::list< unsigned int >                 ListPixelType;
  typedef typename ListPixelType::const_iterator    ListPixelConstIterator;
  typedef typename ListPixelType::iterator          ListPixelIterator;
  typedef Image< ListPixelType, itkGetStaticConstMacro(ImageDimension) >
                                                    ListImageType;

  /** \brief Performs the narrow-band update of the Heaviside function for each
  voxel. The characteristic function of each region is recomputed (note the
  shared data which contains information from the other level sets). Using the
  new H values, the previous c_i are updated. */
  void UpdatePixel( const unsigned int& idx,
    NeighborhoodIterator<TInputImage> &iterator,
    InputPixelType &newValue,
    bool &status );

protected:
  ScalarRegionBasedLevelSetFunction() : Superclass(){}
  ~ScalarRegionBasedLevelSetFunction(){}

  ScalarValueType ComputeOverlapParameters( const FeatureIndexType& featIndex,
    ScalarValueType& product );

  virtual void UpdateSharedDataInsideParameters( const unsigned int& iId,
    const bool& iBool, const FeaturePixelType& iVal, const ScalarValueType& iH ) = 0;
  virtual void UpdateSharedDataOutsideParameters( const unsigned int& iId,
    const bool& iBool, const FeaturePixelType& iVal, const ScalarValueType& iH ) = 0;

private:
  ScalarRegionBasedLevelSetFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkScalarRegionBasedLevelSetFunction.txx"
#endif

#endif