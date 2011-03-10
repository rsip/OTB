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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbReciprocalCovarianceToReciprocalCoherencyImageFilter.h"
#include "otbComplexToVectorImageCastFilter.h"

int otbReciprocalCovarianceToReciprocalCoherencyImageFilter(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef double                   PixelType;
  typedef std::complex<PixelType>  InputPixelType;
 
  typedef otb::VectorImage<PixelType>      RealImageType;
  typedef otb::VectorImage<InputPixelType> ImageType;

  typedef otb::ReciprocalCovarianceToReciprocalCoherencyImageFilter<ImageType, ImageType> FilterType;


  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<RealImageType> WriterType;
  typedef otb::ComplexToVectorImageCastFilter<ImageType, RealImageType> CasterType;

  ReaderType::Pointer reader = ReaderType::New();
  CasterType::Pointer caster = CasterType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());

  caster->SetInput(filter->GetOutput());

  writer->SetFileName(outputFilename);
  writer->SetInput(caster->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
