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

#include <cstdlib>
#include "otbPathListToPathListFilter.h"
#include "itkPolyLineParametricPath.h"

int otbPathListToPathListFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef itk::PolyLineParametricPath<Dimension>  PathType;
  typedef otb::PathListToPathListFilter<PathType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
