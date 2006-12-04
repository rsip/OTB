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
#include "otbRCC8VertexBase.h"

int otbRCC8VertexBase(int argc, char* argv[])
{
try
  {
    typedef unsigned short LabelType;
    unsigned int imageIndex = atoi(argv[1]);
    LabelType objectLabel = static_cast<LabelType>(atof(argv[2]));
    typedef otb::RCC8VertexBase<LabelType> RCC8VertexType;
    typedef RCC8VertexType::AttributesVectorType AttributesVectorType;
    // Instantiation
    RCC8VertexType::Pointer vertex1= RCC8VertexType::New();
    // Getters / setters tests
    vertex1->SetSegmentationImageIndex(imageIndex);
    vertex1->SetObjectLabelInImage(objectLabel);
    if(vertex1->GetSegmentationImageIndex()!=imageIndex)
      {
	std::cout<<"Test failed: vertex1->GetSegmentationImageIndex()!=imageIndex"<<std::endl;
	return EXIT_FAILURE;
      }
    if(vertex1->GetObjectLabelInImage()!=objectLabel)
      {
	std::cout<<"Test failed: vertex1->GetSegmentationImageIndex()!=imageIndex"<<std::endl;
	return EXIT_FAILURE;
      }
    // attributes vector set test
    AttributesVectorType attr1 = vertex1->GetAttributesVector(); 
    if(attr1[0].compare(std::string("SegmentationImageIndex"))!=0)
      {
	std::cout<<"Test failed: vertex1->GetAttributesVector()[0]!=\"SegmentationImageIndex\""<<std::endl;
	return EXIT_FAILURE;
      }
    if(attr1[2].compare(std::string("ObjectLabelInImage"))!=0)
      {
	std::cout<<"Test failed: vertex1->GetAttributesVector()[2]!=\"ObjectLabelInImage\""<<std::endl;
	return EXIT_FAILURE;
      }
    if(attr1[1].compare(std::string(argv[1]))!=0)
      {
	std::cout<<"Test failed: vertex1->GetAttributesVector()[1]!=std::string(argv[1])"<<std::endl;
	return EXIT_FAILURE;
      }
    if(attr1[3].compare(std::string(argv[2]))!=0)
      {
	std::cout<<"Test failed: vertex1->GetAttributesVector()[3]!=std::string(argv[2])"<<std::endl;
	return EXIT_FAILURE;
      }
    // attributes vector get test
    RCC8VertexType::Pointer vertex2 = RCC8VertexType::New();
    vertex2->SetAttributesVector(attr1);
    if(vertex1->GetSegmentationImageIndex()!=vertex2->GetSegmentationImageIndex())
      {
	std::cout<<"Test failed: vertex1->GetSegmentationImageIndex()!=vertex2->GetSegmentationImageIndex()"<<std::endl;
	return EXIT_FAILURE;
      }
    if(vertex1->GetObjectLabelInImage()!=vertex2->GetObjectLabelInImage())
      {
	std::cout<<"Test failed: vertex1->GetObjectLabelInImage()!=vertex2->GetObjectLabelInImage()"<<std::endl;
	return EXIT_FAILURE;
      }
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
