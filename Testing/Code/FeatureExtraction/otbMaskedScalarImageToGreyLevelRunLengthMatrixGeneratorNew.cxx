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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"
#include <iostream>

#include "otbMaskedScalarImageToGreyLevelRunLengthMatrixGenerator.h"

int otbMaskedScalarImageToGreyLevelRunLengthMatrixGeneratorNew(int argc, char* argv[])
{
  typedef unsigned char InputPixelType;
  typedef double        OutputPixelType;
  const unsigned int Dimension = 2;

  typedef itk::Image<InputPixelType,  Dimension> InputImageType;
  typedef itk::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::MaskedScalarImageToGreyLevelRunLengthMatrixGenerator<InputImageType> FilterType;

  FilterType::Pointer FilterMatrixGenerator = FilterType::New();

  return EXIT_SUCCESS;
}