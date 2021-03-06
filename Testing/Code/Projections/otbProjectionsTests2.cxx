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

// this file defines the otbProjectionsTest for the test driver
// and all it expects is that you have a function called RegisterTests


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbOrthoRectificationFilterNew);
  REGISTER_TEST(otbOrthoRectificationFilter);
  REGISTER_TEST(otbOrthoRectificationComplexFilter);
  REGISTER_TEST(otbOrthoRectificationFilterWithDEM);
  REGISTER_TEST(otbOrthoRectificationMonoThreadFilter);
  REGISTER_TEST(otbGCPsToRPCSensorModelImageFilterNew);
  REGISTER_TEST(otbGCPsToRPCSensorModelImageFilterWithoutDEM);
  REGISTER_TEST(otbGCPsToRPCSensorModelImageFilterAndOrtho);
  REGISTER_TEST(otbLeastSquareAffineTransformEstimatorNew);
  REGISTER_TEST(otbLeastSquareAffineTransformEstimator);
  REGISTER_TEST(otbGCPsToRPCSensorModelImageFilterCheckRpcModel);
}
