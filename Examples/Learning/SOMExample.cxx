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

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROI_QB_MUL_1.png} 
//    OUTPUTS: {ROI_QB_MUL_SOM.png}, {ROI_QB_MUL_SOMACT.png}
//    8 8 8 8 20 1.0 0.1 128
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
// This example illustrates the use of the
// \doxygen{otb}{SOM} class for building Kohonen's Self Organizing
// Maps.
// The first thing to do is include the header file for the
// class. We will also need the header files for the map itself and
// the activation map builder whose utility will be explained at the
// end of the example.
//
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbSOMMap.h"
#include "otbSOM.h"
#include "otbSOMActivationBuilder.h"
// Software Guide : EndCodeSnippet

#include "itkExceptionObject.h"
#include "otbImage.h"
#include "itkRGBPixel.h"
#include "itkVectorExpandImageFilter.h"
#include "itkVectorNearestNeighborInterpolateImageFunction.h"

#include "itkExpandImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

//  Software Guide : BeginLatex
// Since the \doxygen{otb}{SOM} class uses a distance, we will need to
// include the header file for the one we want to use
//
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet

#include "itkEuclideanDistance.h"

// Software Guide : EndCodeSnippet
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageToListAdaptor.h"

int main(int argc, char* argv[])
{
  if(argc!=12)
    {
    std::cout << "Usage: " << argv[0] << "inputImage outputMap activationMap";
    std::cout << "sizeX sizeY neighborX neighborY iterations ";
    std::cout << "beta0 betaEnd initValue" << std::endl;
    return 1;
    }


    char * inputFileName = argv[1];
    char * outputFileName = argv[2];
    char * actMapFileName = argv[3];
    unsigned int sizeX = atoi(argv[4]);
    unsigned int sizeY = atoi(argv[5]);
    unsigned int neighInitX = atoi(argv[6]);
    unsigned int neighInitY= atoi(argv[7]);
    unsigned int nbIterations= atoi(argv[8]);
    double betaInit = atof(argv[9]);
    double betaEnd= atof(argv[10]);
    float initValue = atof(argv[11]);

//  Software Guide : BeginLatex
// 
// The Self Organizing Map itself is actually an N-dimensional image
// where each pixel contains a neuron. In our case, we decide to build
// a 2-dimensional SOM, where the neurons store RGB values with
// floating point precision.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
    

    const unsigned int Dimension = 2;
    typedef unsigned char ComponentType;
    typedef itk::RGBPixel<ComponentType> PixelType;

// Software Guide : EndCodeSnippet    
//  Software Guide : BeginLatex
// 
// The distance that we want to apply between the RGB values is the
// Euclidean one. Of course we could choose to use other type of
// distance, as for instance, a distance defined in any other color space.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
    
    typedef itk::Statistics::EuclideanDistance<PixelType> DistanceType;

// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// We can now define the type for the map. The \doxygen{otb}{SOMMap}
// class is templated over the neuron type -- \code{PixelType} here
// --, the distance type and the number of dimensions. Note that the
// number of dimensions of the map could be different from the one of
// the images to be processed.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
    
    
    typedef otb::SOMMap<PixelType,DistanceType,Dimension> MapType;

// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// We are going to perform the learning directly on the pixels of the
// input image. Therefore, the image type is defined using the same
// pixel type as we used for the map. We also define the type for the
// imge file reader.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
    
    
    typedef otb::Image<PixelType,Dimension> ImageType;
    typedef otb::ImageFileReader<ImageType> ReaderType;

// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// Since the \doxygen{otb}{SOM} class works on lists of samples, it
// will need to access the input image through an adaptor. Its type is
// defined as follows:
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
    
    typedef itk::Statistics::ImageToListAdaptor<ImageType> ListAdaptorType;

// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// We can now define the type for the SOM, which is templated over the
// input sample list and the type of the map to be produced.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
    
    typedef otb::SOM<ListAdaptorType,MapType> SOMType;

// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// Since the map is itself an image, we can write it to disk with an
// \doxygen{otb}{ImageFileWriter}. 
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
    
    typedef otb::ImageFileWriter<MapType> WriterType;

// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// We can now start building the pipeline. The first step is to
// instantiate the reader and pass its output to the adaptor.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet    
    
    
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFileName);
    reader->Update();

    ListAdaptorType::Pointer adaptor = ListAdaptorType::New();
    adaptor->SetImage(reader->GetOutput());

// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// We can now instantiate the SOM algorithm and set the sample list as input.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet        

    SOMType::Pointer som = SOMType::New();
    som->SetListSample(adaptor);

// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// We use a \code{SOMType::SizeType} array in order to set the sizes
// of the map.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet    
    
    SOMType::SizeType size;
    size[0]=sizeX;
    size[1]=sizeY;
    som->SetMapSize(size);

// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// The initial size of the neighborhood of each neuron is set in the
// same way.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet        
    
    
    SOMType::SizeType radius;
    radius[0] = neighInitX;
    radius[1] = neighInitY;
    som->SetNeighborhoodSizeInit(radius);

// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// The other parameters are the number of iterations, the initial and
// the final values for the learning rate -- $\beta$ -- and the
// maximum initial value for the neurons (the map will be randomly
// initialized). 
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet        
    
    
    som->SetNumberOfIterations(nbIterations);
    som->SetBetaInit(betaInit);
    som->SetBetaEnd(betaEnd);
    som->SetMaxWeight(static_cast<ComponentType>(initValue));
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
// 
// Finally, we set up the las part of the pipeline where the plug the
// output of the SOM into the writer. The learning procedure is
// triggered by calling the \code{Update()} method on the writer.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet        

    
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFileName);
    writer->SetInput(som->GetOutput());
    writer->Update();

// Software Guide : EndCodeSnippet          

    //Just for visualization purposes, we zoom the image.
    typedef itk::VectorExpandImageFilter< MapType, MapType > ExpandType;
    typedef itk::VectorNearestNeighborInterpolateImageFunction< MapType,
                                        double > InterpolatorType;

    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    ExpandType::Pointer expand = ExpandType::New();
    expand->SetInput(som->GetOutput());
    expand->SetExpandFactors( 20 );
    expand->SetInterpolator( interpolator );
    PixelType pix;
    pix[0]= 255;
    pix[1]= 255;
    pix[2]= 255;
    expand->SetEdgePaddingValue(pix);
    writer->SetInput(expand->GetOutput());
    writer->Update();



//  Software Guide : BeginLatex
// Figure \ref{fig:SOMMAP} shows the result of the SOM learning. Since
// we have performed a learning on RGB pixel values, the produced SOM
// can be interpreted as an optimal color table for the input image.
// \begin{figure}
// \center
// \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1.eps}
// \includegraphics[width=0.2\textwidth]{ROI_QB_MUL_SOM.eps}
// \includegraphics[width=0.2\textwidth]{ROI_QB_MUL_SOMACT.eps}
// \itkcaption[SOM Image Classification]{Result of the SOM
// learning. Left: RGB image. Center: SOM. Right: Activation map} 
// \label{fig:SOMMAP}
// \end{figure}
//  Software Guide : EndLatex


//  Software Guide : BeginLatex
// 
// We can now compute the activation map for the input image. The
// activation map tells us how many times a given neuron is activated
// for the set of examples given to the map. The activation map is
// stored as a scalar image and an integer pixel type is usually enough.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet        
    

    typedef unsigned char OutputPixelType;

    typedef otb::Image<OutputPixelType,Dimension> OutputImageType;
    typedef otb::ImageFileWriter<OutputImageType> ActivationWriterType;

// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
// 
// In a similar way to the \doxygen{otb}{SOM} class the
// \doxygen{otb}{SOMActivationBuilder} is templated over the sample
// list given as input, the SOM map type and the activation map to be
// built as output.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet        
    
    typedef otb::SOMActivationBuilder<ListAdaptorType,MapType,
                             OutputImageType> SOMActivationBuilderType;

// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
// 
// We instantiate the activation map builder and set as input the SOM
// map build before and the image (using the adaptor).
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
    
    SOMActivationBuilderType::Pointer somAct
                                   = SOMActivationBuilderType::New();
    somAct->SetInput(som->GetOutput());
    somAct->SetListSample(adaptor);

// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
// 
// The final step is to write the activation map to a file.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
    
    
    ActivationWriterType::Pointer actWriter = ActivationWriterType::New();
    actWriter->SetFileName(actMapFileName);
    actWriter->SetInput(somAct->GetOutput());
    actWriter->Update();
// Software Guide : EndCodeSnippet    

//  Software Guide : BeginLatex
// The righthand side of figure \ref{fig:SOMMAP} shows the activation
// map obtained.
//
//  Software Guide : EndLatex     

    //Just for visualization purposes, we zoom the image.
    typedef itk::ExpandImageFilter< OutputImageType, OutputImageType > ExpandType2;
    typedef itk::NearestNeighborInterpolateImageFunction< OutputImageType,                                        double > InterpolatorType2;

    InterpolatorType2::Pointer interpolator2 = InterpolatorType2::New();
    ExpandType2::Pointer expand2 = ExpandType2::New();
    expand2->SetInput(somAct->GetOutput());
    expand2->SetExpandFactors( 20 );
    expand2->SetInterpolator( interpolator2 );
    expand2->SetEdgePaddingValue(255);
    actWriter->SetInput(expand2->GetOutput());
    actWriter->Update();



 return EXIT_SUCCESS;
}
