//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id: ossimImageHandlerFactory.cpp 10104 2006-12-14 16:13:05Z gpotts $
#include <ossim/imaging/ossimImageHandlerSarFactory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimRadarSatTileSource.h>

static const ossimTrace traceDebug("ossimImageHandlerSarFactory:debug");

RTTI_DEF1(ossimImageHandlerSarFactory, "ossimImageHandlerSarFactory", ossimImageHandlerFactoryBase);

ossimImageHandlerSarFactory* ossimImageHandlerSarFactory::theInstance = 0;

ossimImageHandlerSarFactory::~ossimImageHandlerSarFactory()
{
   theInstance = (ossimImageHandlerSarFactory*)NULL;
}

ossimImageHandlerSarFactory* ossimImageHandlerSarFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimImageHandlerSarFactory;
   }

   return theInstance;
}

ossimImageHandler* ossimImageHandlerSarFactory::open(const ossimFilename& fileName)const
{
   
   ossimFilename copyFilename = fileName;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerSarFactory::open(filename) DEBUG: entered..."
         << std::endl
         << "Attempting to open file " << copyFilename << std::endl;
   }
   ossimImageHandler* result = NULL;

   // Check for empty file.
   copyFilename.trim();
   if (copyFilename.empty())
   {
      return result;
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying RadarSat"
         << std::endl;
   }
   result = new ossimRadarSatTileSource;
   if(result->open(copyFilename))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(filename) DEBUG: returning..." << std::endl;
   }
   return (ossimImageHandler*)NULL;
}

ossimImageHandler* ossimImageHandlerSarFactory::open(const ossimKeywordlist& kwl,
                                                  const char* prefix)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerSarFactory::open(kwl, prefix) DEBUG: entered..."
         << std::endl;
   }
   ossimImageHandler* result = NULL;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "trying RadarSat"
         << std::endl;
   }
   result  = new ossimRadarSatTileSource();
   if(result->loadState(kwl, prefix))
   {
      return result;
   }
   delete result;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerFactory::open(kwl, prefix) DEBUG: returning..."
         << std::endl;
   }
   return (ossimImageHandler*)NULL;
}

ossimObject* ossimImageHandlerSarFactory::createObject(const ossimString& typeName)const
{
   if(STATIC_TYPE_NAME(ossimRadarSatTileSource) == typeName)
   {
      return new ossimRadarSatTileSource();
   }
   return (ossimObject*)NULL;
}

void ossimImageHandlerSarFactory::getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const
{
   extensionList.push_back("001");
}

ossimObject* ossimImageHandlerSarFactory::createObject(const ossimKeywordlist& kwl,
                                                    const char* prefix)const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimImageHandlerSarFactory::createObject(kwl, prefix) DEBUG:"
         << " entering ..." << std::endl;
   }
   ossimObject* result = (ossimObject*)NULL;
   const char* type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

   if(type)
   {
      if (ossimString(type).trim() == STATIC_TYPE_NAME(ossimImageHandler))
      {
         const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);

         if (lookup)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "BEBUG: filename " << lookup << std::endl;
            }
            // Call the open that takes a filename...
            result = this->open(kwl, prefix);//ossimFilename(lookup));
         }
      }
      else
      {
         result = createObject(ossimString(type));
         if(result)
         {
            result->loadState(kwl, prefix);
         }
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageHandlerSarFactory::createObject(kwl, prefix) DEBUG: returning result ..." << std::endl;
   }
   return result;
}

void ossimImageHandlerSarFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimRadarSatTileSource));
}
