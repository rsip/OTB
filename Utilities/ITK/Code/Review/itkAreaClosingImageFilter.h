/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAreaClosingImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-02-20 17:33:47 $
  Version:   $Revision: 1.3 $

  Copyright ( c ) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkAreaClosingImageFilter_h
#define __itkAreaClosingImageFilter_h

#include "itkAttributeMorphologyBaseImageFilter.h"

namespace itk
{

/**
 * \class AreaClosingImageFilter
 * \brief Morphological closing by attributes
 *
 * This is the base class for morphology attribute
 * operations. Attribute openings remove blobs according to criteria
 * such as area. When applied to grayscale images it has the effect of
 * trimming peaks based on area while leaving the rest of the image
 * unchanged. It is possible to use attributes besides area, but no
 * others are implemented yet. This filter uses some dodgy coding
 * practices - most notably copying the image data to a linear buffer
 * to allow direct implementation of the published algorithm. It
 * should therefore be quite a good candidate to carry out tests of
 * itk iterator performance with randomish access patterns. 
 *
 * This filter is implemented using the method of Wilkinson, "A
 * comparison of algorithms for Connected set openings and Closings",
 * A. Meijster and M. H. Wilkinson, PAMI, vol 24, no. 4, April 2002.
 * Attempts at implementing the method from ISMM 2000 are also
 * included, but operation appears incorrect. Check the ifdefs if you
 * are interested.
 *
 * \author Richard Beare. Department of Medicine, Monash University, Melbourne, Australia.
 *
 */
template <class TInputImage, class TOutputImage, class TAttribute=ITK_TYPENAME TInputImage::SpacingType::ValueType>
class ITK_EXPORT AreaClosingImageFilter :
    public AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, TAttribute, std::less<typename TInputImage::PixelType> >

{
public:
  typedef AreaClosingImageFilter Self;
  typedef AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, TAttribute, std::less<typename TInputImage::PixelType> >
                                 Superclass;

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /**
   * Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same.
   */
  typedef typename TOutputImage::PixelType         OutputPixelType;
  typedef typename TOutputImage::InternalPixelType OutputInternalPixelType;
  typedef typename TInputImage::PixelType          InputPixelType;
  typedef typename TInputImage::InternalPixelType  InputInternalPixelType;
  typedef typename TInputImage::IndexType          IndexType;
  typedef typename TInputImage::OffsetType         OffsetType;
  typedef typename TInputImage::SizeType           SizeType;
  typedef TAttribute                               AttributeType;

  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(AreaClosingImageFilter, 
               AttributeMorphologyBaseImageFilter);

  /**
   * Set/Get whether the image spacing is used or not - defaults to true.
   */
  itkSetMacro(UseImageSpacing, bool);
  itkGetConstReferenceMacro(UseImageSpacing, bool);
  itkBooleanMacro(UseImageSpacing);

protected:
  AreaClosingImageFilter()
    {
    m_UseImageSpacing = true;
    }
  virtual ~AreaClosingImageFilter() {}

  void GenerateData()
    {
    this->m_AttributeValuePerPixel = 1;
    if( m_UseImageSpacing )
      {
      // compute pixel size
      double psize = 1.0;
      for( unsigned i=0; i<ImageDimension; i++)
        {
        psize *= this->GetInput()->GetSpacing()[i];
        }
      this->m_AttributeValuePerPixel = static_cast<AttributeType>( psize );
      // std::cout << "m_AttributeValuePerPixel: " << this->m_AttributeValuePerPixel << std::endl;
      // and call superclass implementation of GenerateData()
      }
    Superclass::GenerateData();
    }

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf(os,indent);
    os << indent << "UseImageSpacing: "  << m_UseImageSpacing << std::endl;
    }

private:

  AreaClosingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_UseImageSpacing;

};

} // namespace itk
#endif
