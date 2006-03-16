/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   10 mars 2006
  Version   :   
  Role      :   
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"

#include "otbImageFileReader.h"
#include "otbHuImageFunction.h"

int otbHuImage( int argc, char ** argv )
{
  try 
    { 
        const char * inputFilename  = argv[1];
        unsigned int  Number = 1;
       
        typedef unsigned char                                   InputPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef itk::Image< InputPixelType,  Dimension >	              InputImageType;
        typedef otb::ImageFileReader< InputImageType  >                       ReaderType;  
        typedef std::complex<float>                                           ComplexType;
	typedef otb::HuImageFunction<InputImageType,ComplexType>   FunctionType;
  
        InputImageType::RegionType   region;
        InputImageType::SizeType     size;
        InputImageType::IndexType    start;

        start.Fill( 0 );
        size[0] = 50;
        size[1] = 50;

        ReaderType::Pointer reader         = ReaderType::New();
	FunctionType::Pointer function =FunctionType::New();
	
        reader->SetFileName( inputFilename  );
	
	InputImageType::Pointer image = reader->GetOutput();

        region.SetIndex( start );
        region.SetSize( size );
	
	image->SetRegions(region);
	image->Update();
	
	function->SetInputImage( image );
	function->SetQ(Number);
	
	InputImageType::IndexType index;
	index[0]=10;
	index[1]=10;
	
	ComplexType Result;
	
        Result = function->EvaluateAtIndex( index );
	std::cout << "Hu("<<Number<<") = "<< Result <<std::endl;
	
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}

