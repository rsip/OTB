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
#ifndef _otbBreakAngularPathListFilter_txx
#define _otbBreakAngularPathListFilter_txx

#include "otbBreakAngularPathListFilter.h"
#include "vcl_cmath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath>
BreakAngularPathListFilter<TPath>
::BreakAngularPathListFilter()
{
}


template <class TPath>
void
BreakAngularPathListFilter<TPath>
::BreakAngularPath(const double maxAngle, const PathPointerType inputPath, PathListPointerType outputPathList)
{
        typename PathType::VertexListType::ConstPointer  vertexList = inputPath->GetVertexList();
        typename PathType::VertexListType::ConstIterator pathIt = vertexList->Begin();
        
        typename PathType::VertexType pixel1, pixel2, pixel3;
        
        // Initialisation 
        PathPointerType newPath = PathType::New();  
        newPath->Initialize();
        
        double alpha1(0.), alpha2(0.);
        
        while ( pathIt != vertexList->End() ) 
        {
                // Add Pixel 1
                otbMsgDevMacro(<<"Add Pixel 1"<<pathIt.Value()<< " -> Path : "<<newPath);
                newPath->AddVertex(pathIt.Value());
                pixel1=pathIt.Value();
                ++pathIt;
                if (pathIt != vertexList->End())
                {
                        pixel2=pathIt.Value();
                        ++pathIt;
                        if (pathIt != vertexList->End())
                        {
                                pixel3=pathIt.Value();
                               
                                alpha1 = vcl_atan2((pixel1[1]-pixel2[1]),(pixel1[0]-pixel2[0]));
                                alpha2 = vcl_atan2((pixel2[1]-pixel3[1]),(pixel2[0]-pixel3[0]));
                                if (vcl_abs(alpha1-alpha2) > maxAngle)
/*                                alpha1 = atan2((pixel1[1]-pixel2[1]),(pixel1[0]-pixel2[0]));
                                alpha2 = atan2((pixel2[1]-pixel3[1]),(pixel2[0]-pixel3[0]));
                                if (std::abs(alpha1-alpha2) > maxAngle)*/
                                {
                                        // Add Pixel 2
                                        newPath->AddVertex(pixel3);
                otbMsgDevMacro(<<"BREAK test : Add Pixel 2 : "<<pixel3<<" -> Path : "<<newPath);
                                        //Create new PathType in the out list
                                        outputPathList->PushBack(newPath);
                                        // Reinit
                                        newPath = PathType::New();
                otbMsgDevMacro(<<"reinit outputPathList -> Path : "<<newPath<<"\n Fin reinit outputPath");
                                        
                                }
                                --pathIt; // Return previous pixel
                        }
                        else
                        {
                                // Add Pixel 2
                                newPath->AddVertex(pixel2);
                        }
                }
        }
        //Create new PathType in the out list
        outputPathList->PushBack(newPath);
        otbMsgDevMacro(<<"AJOUT dernier PATH -> Path : "<<newPath<<"\n Fin AJOUT outputPathList");
        otbMsgDevMacro(<<"\nObjetcList : "<<outputPathList);
}


template <class TPath>
void
BreakAngularPathListFilter<TPath>
::GenerateData()
{
        const PathListType *  inputPathList  = this->GetInput();
        PathListType *  outputPathList = this->GetOutput();

        typename PathListType::ConstIterator listIt = inputPathList->Begin();
        outputPathList->Clear();
        
        PathListPointerType newTempPathList = PathListType::New(); 
        while( listIt != inputPathList->End())
        {
                (void)BreakAngularPath(m_MaxAngle, listIt.Get(), outputPathList);
                ++listIt;
        }
}

/**
 * PrintSelf Method
 */
template <class TPath>
void
BreakAngularPathListFilter<TPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Angular max value : "<<m_MaxAngle<<std::endl;
}

} // End namespace otb
#endif
