/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAsinImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:08 $
  Version:   $Revision: 1.24 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAsinImageFilter_h
#define __itkAsinImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{
  
/** \class AsinImageFilter
 * \brief Computes the vcl_asin(x) pixel-wise
 *
 * This filter is templated over the pixel type of the input image
 * and the pixel type of the output image. 
 *
 * The filter will walk over all the pixels in the input image, and for
 * each one of them it will do the following: 
 *
 * - cast the pixel value to \c double, 
 * - apply the \c vcl_asin() function to the \c double value
 * - cast the \c double value resulting from \c vcl_asin() to the pixel type of the output image 
 * - store the casted value into the output image.
 * 
 * The filter expect both images to have the same dimension (e.g. both 2D, or both 3D, or both ND)
 * 
 * \ingroup IntensityImageFilters   Multithreaded
 */
namespace Functor {  
  
template< class TInput, class TOutput>
class Asin
{
public:
  Asin() {};
  ~Asin() {};
  bool operator!=( const Asin & ) const
    {
    return false;
    }
  bool operator==( const Asin & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput & A ) const
    {
    return static_cast<TOutput>(
      vcl_asin(
        static_cast<double>(A)
        )
      );
    }
}; 

}
template <class TInputImage, class TOutputImage>
class ITK_EXPORT AsinImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Functor::Asin< 
  typename TInputImage::PixelType, 
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef AsinImageFilter           Self;
  typedef UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                                  Functor::Asin< 
    typename TInputImage::PixelType, 
    typename TOutputImage::PixelType>   
  >                                 Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(AsinImageFilter, 
               UnaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputConvertibleToDoubleCheck,
    (Concept::Convertible<typename TInputImage::PixelType, double>));
  itkConceptMacro(DoubleConvertibleToOutputCheck,
    (Concept::Convertible<double, typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  AsinImageFilter() {}
  virtual ~AsinImageFilter() {}

private:
  AsinImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
