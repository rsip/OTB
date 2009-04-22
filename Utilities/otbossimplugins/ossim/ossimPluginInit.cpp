//*******************************************************************
// Copyright (C) 2005 David Burken, all rights reserved.
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id$
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossimPluginConstants.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include "ossimPluginProjectionFactory.h"
static void setDescription(ossimString& description)
{
   description = "OSSIM Plugin\n\n";
   std::vector<ossimString> projectionTypes;
   
   ossimPluginProjectionFactory::instance()->getTypeNameList(projectionTypes);
   ossim_uint32 idx = 0;
   description = "Projecitons Supported:\n\n";
   for(idx = 0; idx < projectionTypes.size();++idx)
   {
      description += projectionTypes[idx] + "\n";
   }
}


extern "C"
{
   ossimSharedObjectInfo  myInfo;
   ossimString theDescription;
   std::vector<ossimString> theObjList;
   const char* getDescription()
   {
      return theDescription.c_str();
   }
   int getNumberOfClassNames()
   {
      return (int)theObjList.size();
   }
   const char* getClassName(int idx)
   {
      if(idx < (int)theObjList.size())
      {
         return theObjList[0].c_str();
      }
      return (const char*)0;
   }

   /* Note symbols need to be exported on windoze... */ 
   OSSIM_PLUGINS_DLL void ossimSharedLibraryInitialize(
      ossimSharedObjectInfo** info)
   {    
      myInfo.getDescription = getDescription;
      myInfo.getNumberOfClassNames = getNumberOfClassNames;
      myInfo.getClassName = getClassName;
      
      *info = &myInfo;
      ossimProjectionFactoryRegistry::instance()->registerFactory(ossimPluginProjectionFactory::instance(), true);
     setDescription(theDescription);
  }

   /* Note symbols need to be exported on windoze... */ 
  OSSIM_PLUGINS_DLL void ossimSharedLibraryFinalize()
  {
     
  }
}