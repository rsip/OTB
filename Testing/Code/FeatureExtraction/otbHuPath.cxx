/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   24 mars 2006
  Version   :   
  Role      :   
  $Id$

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImageFileReader.h"
#include "otbHuPathFunction.h"
#include "itkPolyLineParametricPath.h"
#include "itkExceptionObject.h"

int otbHuPath( int argc, char ** argv )
{
  try 
    { 
        unsigned int              Number;
        const   unsigned int      Dimension = 2;
	typedef itk::PolyLineParametricPath< Dimension >     PathType;
	typedef otb::HuPathFunction<PathType>                FunctionType;
	typedef FunctionType::RealType                       RealType;
  
		
        // Dessiner un carr�:
	PathType::ContinuousIndexType cindex;
	PathType::Pointer pathElt = PathType::New();

 	pathElt->Initialize();

        cindex[0]=30;
        cindex[1]=30;
        pathElt->AddVertex(cindex);
        cindex[0]= 30;
        cindex[1]=130;
        pathElt->AddVertex(cindex);
        cindex[0]=130;
        cindex[1]=130;
        pathElt->AddVertex(cindex);
        cindex[0]=130;
        cindex[1]= 30;
        pathElt->AddVertex(cindex);

	FunctionType::Pointer function =FunctionType::New();

	RealType Result;
	
	for (Number = 1 ;Number<8;Number++)
	  {
	   function->SetNumber(Number);
           Result = function->Evaluate( *pathElt );
	   std::cout << "Hu("<<Number<<") = "<< Result <<std::endl;
	  }
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

