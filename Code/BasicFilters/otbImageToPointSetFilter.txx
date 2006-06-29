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
#ifndef _otbImageToPointSetFilter_txx
#define _otbImageToPointSetFilter_txx

#include "otbImageToPointSetFilter.h"


namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::ImageToPointSetFilter()
{
  this->itk::ProcessObject::SetNumberOfRequiredInputs(1);

  OutputPointSetPointer output
    = dynamic_cast<OutputPointSetType*>(this->MakeOutput(0).GetPointer()); 

  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  this->itk::ProcessObject::SetNthOutput(0, output.GetPointer());

}

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::~ImageToPointSetFilter()
{
}
  

/**
 *   Make Ouput
 */
template <class TInputImage, class TOutputPointSet>
itk::DataObject::Pointer
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::MakeOutput(unsigned int)
{
  OutputPointSetPointer  outputPointSet = OutputPointSetType::New();
  return dynamic_cast< itk::DataObject *>( outputPointSet.GetPointer() );
}




/**
 *
 */
template <class TInputImage, class TOutputPointSet>
void 
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::SetInput(unsigned int idx,const InputImageType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->itk::ProcessObject::SetNthInput(idx, 
                                   const_cast< InputImageType * >(input) );
}


  
/**
 *
 */
template <class TInputImage, class TOutputPointSet>
const typename ImageToPointSetFilter<TInputImage,TOutputPointSet>::InputImageType *
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::GetInput(unsigned int idx) 
{
  return dynamic_cast<const InputImageType*>
    (this->itk::ProcessObject::GetInput(idx));
}

 
/**
 *
 */
template <class TInputImage, class TOutputPointSet>
typename ImageToPointSetFilter<TInputImage,TOutputPointSet>::OutputPointSetType *
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::GetOutput(void) 
{
  return dynamic_cast<OutputPointSetType*>
    (this->itk::ProcessObject::GetOutput(0));
}


/**
 *
 */
template <class TInputImage, class TOutputPointSet>
void 
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}



/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the 
 * ProcessObject version to be called
 */
template <class TInputImage, class TOutputPointSet>
void 
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::GenerateOutputInformation()
{
}


} // end namespace otb

#endif
