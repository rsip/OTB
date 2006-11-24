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
#include "itkExceptionObject.h"

#include "otbMorphologicalPyramidAnalyseFilter.h"
#include "otbMorphologicalPyramidMRToMSConverter.h"
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbMorphologicalPyramidMRToMSConverter(int argc, char * argv[])
{
  try
    {
      const char * inputFilename = argv[1];
      const char * outputFilename1 = argv[2];
      const char * outputFilename2 = argv[3];
      const char * outputFilename3 = argv[4];
      const unsigned int numberOfIterations = atoi(argv[5]);
      const float subSampleScale = atof(argv[6]);
      
      const unsigned int Dimension = 2;
      typedef unsigned char InputPixelType;
      typedef unsigned char OutputPixelType;

      typedef otb::Image<InputPixelType,Dimension> InputImageType;
      typedef otb::Image<OutputPixelType,Dimension> OutputImageType;

      typedef otb::ImageFileReader<InputImageType> ReaderType;
      typedef otb::ImageFileWriter<OutputImageType> WriterType;

      typedef itk::BinaryBallStructuringElement<InputPixelType,Dimension> StructuringElementType;
      typedef otb::OpeningClosingMorphologicalFilter<InputImageType,InputImageType,StructuringElementType>
	OpeningClosingFilterType;
      typedef otb::MorphologicalPyramidAnalyseFilter<InputImageType,InputImageType,OpeningClosingFilterType>
	PyramidAnalyseFilterType;
      typedef otb::morphologicalPyramid::MRToMSConverter<InputImageType,OutputImageType> MRToMSConverterType;


      // Reading input image
      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(inputFilename);

      // Analysis
      PyramidAnalyseFilterType::Pointer pyramidAnalyse = PyramidAnalyseFilterType::New();
      pyramidAnalyse->SetNumberOfIterations(numberOfIterations);
      pyramidAnalyse->SetSubSampleScale(subSampleScale);
      pyramidAnalyse->SetInput(reader->GetOutput());
      pyramidAnalyse->Update();

      // From multi resolution to multi scale
      MRToMSConverterType::Pointer mrtoms = MRToMSConverterType::New();
      mrtoms->SetInput(pyramidAnalyse->GetOutput());
      mrtoms->SetNumberOfIterations(numberOfIterations);
      mrtoms->SetSupFiltre(pyramidAnalyse->GetSupFiltre());
      mrtoms->SetSupDeci(pyramidAnalyse->GetSupDeci());
      mrtoms->SetInfFiltre(pyramidAnalyse->GetInfFiltre());
      mrtoms->SetInfDeci(pyramidAnalyse->GetInfDeci());
      mrtoms->Update();

      // Writing the output images
      WriterType::Pointer writer1 = WriterType::New();
      writer1->SetFileName(outputFilename1);
      writer1->SetInput(mrtoms->GetSupFiltreFullResolution()->Back());
      writer1->Update();

      WriterType::Pointer writer2 = WriterType::New();
      writer2->SetFileName(outputFilename2);
      writer2->SetInput(mrtoms->GetInfFiltreFullResolution()->Back());
      writer2->Update();

      WriterType::Pointer writer3 = WriterType::New();
      writer3->SetFileName(outputFilename3);
      writer3->SetInput(mrtoms->GetOutput()->Back());
      writer3->Update();
      
    }
      catch( itk::ExceptionObject & err ) 
	{ 
	  std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
	  std::cout << err << std::endl; 
	  return EXIT_FAILURE;
	} 

      catch( ... ) 
	{ 
	  std::cout << "Unknown exception thrown !" << std::endl; 
	  return EXIT_FAILURE;
	} 

      return EXIT_SUCCESS;
    }
