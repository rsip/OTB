//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Image handler factory for ossim plugins plugin.
//
//----------------------------------------------------------------------------
// $Id$

#include <ossimPluginReaderFactory.h>
#include <ossimTerraSarTiffReader.h>
#include <ossimAlosPalsarReader.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>

static const ossimTrace traceDebug("ossimPluginReaderFactory:debug");

RTTI_DEF1(ossimPluginReaderFactory,
          "ossimPluginReaderFactory",
          ossimImageHandlerFactoryBase);

ossimPluginReaderFactory* ossimPluginReaderFactory::theInstance = 0;

ossimPluginReaderFactory::~ossimPluginReaderFactory()
{
   theInstance = 0;
}

ossimPluginReaderFactory* ossimPluginReaderFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimPluginReaderFactory;
   }
   return theInstance;
}

ossimImageHandler* ossimPluginReaderFactory::open(
   const ossimFilename& fileName)const
{
   ossimImageHandler* reader;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPluginReaderFactory::open(filename) DEBUG: entered..."
         << "\nTrying ossimAlosPalsarReader"
         << std::endl;
   }

   reader = new ossimAlosPalsarReader();
   if (reader->open(fileName)) 
   {
      return reader;
   }
   delete reader;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPluginReaderFactory::open(filename) DEBUG: entered..."
         << "\nTrying ossimTerraSarTiffReader"
         << std::endl;
   }

   reader = new ossimTerraSarTiffReader();
   if (reader->open(fileName)) 
   {
      return reader;
   }
   delete reader;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPluginReaderFactory::open(filename) DEBUG: leaving..."
         << std::endl;
   }

   return reader;
}

// Yet to be modified for ALOS
ossimImageHandler* ossimPluginReaderFactory::open(const ossimKeywordlist& kwl,
                                                  const char* prefix)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPluginReaderFactory::open(kwl, prefix) DEBUG: entered..."
         << "Trying ossimTerraSarTiffReader"
         << std::endl;
   }

   ossimImageHandler* reader = new ossimTerraSarTiffReader;
   if(reader->loadState(kwl, prefix) == false)
   {
      delete reader;
      reader = 0;
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPluginReaderFactory::open(kwl, prefix) DEBUG: leaving..."
         << std::endl;
   }

   return reader;
}

ossimObject* ossimPluginReaderFactory::createObject(
   const ossimString& typeName)const
{
   if (typeName == "ossimTerraSarTiffReader")
   {
      return new ossimTerraSarTiffReader;
   }

   if (typeName == "ossimAlosPalsarReader")
   {
      return new ossimAlosPalsarReader;
   }
   
   return 0;
}

ossimObject* ossimPluginReaderFactory::createObject(const ossimKeywordlist& kwl,
                                                 const char* prefix)const
{
   return this->open(kwl, prefix);
}

void ossimPluginReaderFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{   
   typeList.push_back(ossimString("ossimAlosPalsarReader"));
   typeList.push_back(ossimString("ossimTerraSarTiffReader"));
}

void ossimPluginReaderFactory::getSupportedExtensions(
   ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   extensionList.push_back(ossimString("xml"));
}

ossimPluginReaderFactory::ossimPluginReaderFactory(){}

ossimPluginReaderFactory::ossimPluginReaderFactory(const ossimPluginReaderFactory&){}

void ossimPluginReaderFactory::operator=(const ossimPluginReaderFactory&){}
