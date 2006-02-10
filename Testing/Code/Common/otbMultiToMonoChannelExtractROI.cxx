/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Test l'extraction d'une ROI dans une image mono ou multi canal, dont les valeurs sont cod�es en "unsigned char"
                Les parametres de la ROI ne sont pas obligatoire, tout comme les canaux. Dans ce cas, les valauers par d�faut 
                de la classe sont utilis�es
                
  $Id$

=========================================================================*/

#include "itkExceptionObject.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiToMonoChannelExtractROI.h"

template < typename  InputPixelType, typename OutputPixelType  >
int generic_otbMultiToMonoChannelExtractROI ( int argc, char ** argv, const char * inputFilename,const char * outputFilename)
{
  try 
    { 
        typedef otb::MultiToMonoChannelExtractROI< InputPixelType, 
                                             OutputPixelType >  ExtractROIFilterType;

        typename ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
        int cpt(0),nbcanaux(0);

	while ( argv[cpt] != NULL )
	{
		std::string strArgv(argv[cpt]);
		if ( strArgv == "-startX" ) 		{ extractROIFilter->SetStartX((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetStartX("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-startY" ) 	{ extractROIFilter->SetStartY((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetStartY("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-sizeX" ) 	{ extractROIFilter->SetSizeX((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetSizeX("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-sizeY" ) 	{ extractROIFilter->SetSizeY((unsigned long)::atoi(argv[cpt+1]));std::cout <<" ->SetSizeY("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
		else if ( strArgv == "-channel" )       { extractROIFilter->SetChannel((unsigned int)::atoi(argv[cpt+1]));std::cout <<" ->SetChannel("<<::atoi(argv[cpt+1])<<")"<<std::endl;cpt += 2;}
	}

	// Resume de la ligne de commande
	std::cout << " ROI selectionnee : startX "<<extractROIFilter->GetStartX()<<std::endl;
	std::cout << "                    startY "<<extractROIFilter->GetStartY()<<std::endl;
	std::cout << "                    sizeX  "<<extractROIFilter->GetSizeX()<<std::endl;
	std::cout << "                    sizeY  "<<extractROIFilter->GetSizeY()<<std::endl;
	std::cout << " Canal selectionne : ("<<extractROIFilter->GetChannel()<<") : ";

        typedef otb::ImageFileReader< typename ExtractROIFilterType::InputImageType, itk::DefaultConvertPixelTraits< InputPixelType >  >       ReaderType;
        typedef otb::ImageFileWriter< typename ExtractROIFilterType::OutputImageType >           WriterType;
        typename ReaderType::Pointer reader = ReaderType::New();
        typename WriterType::Pointer writer = WriterType::New();

        reader->SetFileName( inputFilename  );
        reader->Update(); //Necessaire pour connaitre le nombre de canaux dans l'image
        writer->SetFileName( outputFilename );
        extractROIFilter->SetInput( reader->GetOutput() );
        
        writer->SetInput( extractROIFilter->GetOutput() );
        writer->Update(); 
        std::cout << " Nb canaux dans l'image d'entree : "<< reader->GetOutput()->GetNumberOfComponentsPerPixel()<<std::endl;
        std::cout << " Nb canaux dans l'image de sortie : "<<extractROIFilter->GetOutput()->GetNumberOfComponentsPerPixel() <<std::endl;
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

int otbMultiToMonoChannelExtractROI ( int argc, char ** argv )
{
        std::string linputPixelType;
        std::string loutputPixelType;
        const char * inputFilename;
        const char * outputFilename;
        int cpt(1);
        //Si le format n'est pas sp�cif�, alors celui par defaut
        if( argv[cpt][0] == '-' )
        {
                linputPixelType = std::string(argv[cpt]);cpt++;
                inputFilename  = argv[cpt];cpt++;
        }
        else
        {
                linputPixelType = std::string("-uchar");
                inputFilename  = argv[cpt];cpt++;
        }
        if( argv[cpt][0] == '-' )
        {
                loutputPixelType = std::string(argv[cpt]);cpt++;
                outputFilename  = argv[cpt];cpt++;
        }
        else
        {
                loutputPixelType = std::string("-uchar");
                outputFilename  = argv[cpt];cpt++;
        }

        argc -= cpt;
        argv += cpt;
        std::cout << " -> "<<linputPixelType<<" pour "<<inputFilename<<std::endl;
        std::cout << " -> "<<loutputPixelType<<" pour "<<outputFilename<<std::endl;
        std::string pixelType;
        if (  (linputPixelType=="-uchar")&&(loutputPixelType=="-uchar") )               return (generic_otbMultiToMonoChannelExtractROI< unsigned char, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-ushort")&&(loutputPixelType=="-ushort") )        return (generic_otbMultiToMonoChannelExtractROI< unsigned short, unsigned short >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-uint")&&(loutputPixelType=="-uint") )            return (generic_otbMultiToMonoChannelExtractROI< unsigned int, unsigned int >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-float")&&(loutputPixelType=="-float") )          return (generic_otbMultiToMonoChannelExtractROI<float, float >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-double")&&(loutputPixelType=="-double") )        return (generic_otbMultiToMonoChannelExtractROI<double, double >( argc,argv,inputFilename,outputFilename) );
        // Type -> uchar
        else if (  (linputPixelType=="-ushort")&&(loutputPixelType=="-uchar") )         return (generic_otbMultiToMonoChannelExtractROI< unsigned short, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-uint")&&(loutputPixelType=="-uchar") )           return (generic_otbMultiToMonoChannelExtractROI< unsigned int, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-float")&&(loutputPixelType=="-uchar") )          return (generic_otbMultiToMonoChannelExtractROI< float, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else if (  (linputPixelType=="-double")&&(loutputPixelType=="-uchar") )         return (generic_otbMultiToMonoChannelExtractROI< double, unsigned char >( argc,argv,inputFilename,outputFilename) );
        else 
        {       
                std::cout << " Erreur : le format des images en entr�e est mal pr�cis� dans la ligne de commande !!!"<<std::endl;
                std::cout << "          valeurs autoris�es : -uchar, -ushort, -uint, -float, -double"<<std::endl;
                std::cout << "          valeurs par defaut : -uchar"<<std::endl;
                return EXIT_FAILURE;
        }

return EXIT_FAILURE;
     
}


