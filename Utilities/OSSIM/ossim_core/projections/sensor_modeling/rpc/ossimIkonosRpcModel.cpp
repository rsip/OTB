//*****************************************************************************
// FILE: ossimIkonosRpcModel.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains implementation of class ossimIkonosRpcModel. This 
//    derived class implements the capability of reading Ikonos RPC support
//    data.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimIkonosRpcModel.cpp,v 1.10 2004/04/08 13:27:34 gpotts Exp $
#include <projections/sensor_modeling/rpc/ossimIkonosRpcModel.h>
#include <base/context/ossimNotifyContext.h>

RTTI_DEF1(ossimIkonosRpcModel, "ossimIkonosRpcModel", ossimRpcModel);

#include <base/data_types/ossimKeywordlist.h>
#include <base/common/ossimKeywordNames.h>

//***
// Define Trace flags for use within this file:
//***
#include <base/common/ossimTrace.h>
static ossimTrace traceExec  ("ossimIkonosRpcModel:exec");
static ossimTrace traceDebug ("ossimIkonosRpcModel:debug");

static const char* MODEL_TYPE        = "ossimIkonosRpcModel";
static const char* META_DATA_FILE    = "meta_data_file";
static const char* RPC_DATA_FILE     = "rpc_data_file";
static const char* LINE_OFF_KW       = "LINE_OFF";
static const char* SAMP_OFF_KW       = "SAMP_OFF";
static const char* LAT_OFF_KW        = "LAT_OFF";
static const char* LONG_OFF_KW       = "LONG_OFF";
static const char* HEIGHT_OFF_KW     = "HEIGHT_OFF";
static const char* LINE_SCALE_KW     = "LINE_SCALE";
static const char* SAMP_SCALE_KW     = "SAMP_SCALE";
static const char* LAT_SCALE_KW      = "LAT_SCALE";
static const char* LONG_SCALE_KW     = "LONG_SCALE";
static const char* HEIGHT_SCALE_KW   = "HEIGHT_SCALE";
static const char* LINE_NUM_COEFF_KW = "LINE_NUM_COEFF_";
static const char* LINE_DEN_COEFF_KW = "LINE_DEN_COEFF_";
static const char* SAMP_NUM_COEFF_KW = "SAMP_NUM_COEFF_";
static const char* SAMP_DEN_COEFF_KW = "SAMP_DEN_COEFF_";

//*****************************************************************************
//  CONSTRUCTOR: ossimIkonosRpcModel
//  
//  Constructs given a geometry file that specifies the filenames for the
//  metadata and RPC data files.
//  
//*****************************************************************************
ossimIkonosRpcModel::ossimIkonosRpcModel(const ossimFilename& geom_file)
   :  ossimRpcModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel Constructor #1: entering..." << std::endl;

   ossimKeywordlist kwl(geom_file);
   const char* value;
   
   //***
   // Assure this keywordlist contains correct type info:
   //***
   value = kwl.find(ossimKeywordNames::TYPE_KW);
   if (!value || (strcmp(value, "ossimIkonosRpcModel")))
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG  ossimIkonosRpcModel Constructor #1:"
                                             << "\nFailed attempt to construct. sensor type \""<<value
                                             << "\" does not match \"ossimIkonosRpcModel\"." << std::endl;
      }

      theErrorStatus++;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG  ossimIkonosRpcModel Constructor #1: returning..." << std::endl;
      return;
   }

   //***
   // Read meta data filename from geom file:
   //***
   value = kwl.find(META_DATA_FILE);
   if (!value)
   {
      theErrorStatus++;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel Constructor #1: returning..." << std::endl;
      return;
   }

   ossimFilename metadata (value);

   //***
   // Read RPC data filename from geom file:
   //***
   value = kwl.find(RPC_DATA_FILE);
   if (!value)
   {
      theErrorStatus++;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG  ossimIkonosRpcModel Constructor #1: returning..." << std::endl;
      return;
   }
   ossimFilename rpcdata (value);

   parseMetaData(metadata);
   parseRpcData (rpcdata);
   finishConstruction();

   ossimString drivePart;
   ossimString pathPart;
   ossimString filePart;
   ossimString extPart;
   geom_file.split(drivePart,
                   pathPart,
                   filePart,
                   extPart);

   

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG returning..." << std::endl;
   
   return;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimIkonosRpcModel
//  
//  Constructs given filenames for metadata and RPC data.
//  
//*****************************************************************************
ossimIkonosRpcModel::ossimIkonosRpcModel(const ossimFilename& metadata,
                                         const ossimFilename& rpcdata)
   :
      ossimRpcModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel Constructor #2: entering..." << std::endl;

   parseMetaData(metadata);
   parseRpcData (rpcdata);
   finishConstruction();

   //***
   // Save current state in RPC model format:
   //***
   ossimString drivePart;
   ossimString pathPart;
   ossimString filePart;
   ossimString extPart;
   metadata.split(drivePart,
                  pathPart,
                  filePart,
                  extPart);
   
   ossimFilename init_rpc_geom;
   init_rpc_geom.merge(drivePart,
                       pathPart,
                       ossimRpcModel::INIT_RPC_GEOM_FILENAME,
                       "");
//      (metadata.path().dirCat(ossimRpcModel::INIT_RPC_GEOM_FILENAME));
   ossimKeywordlist kwl (init_rpc_geom);
   saveState(kwl);
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel Constructor #2: returning..." << std::endl;
}

//*****************************************************************************
//  METHOD: ossimIkonosRpcModel::finishConstruction()
//  
//*****************************************************************************
void ossimIkonosRpcModel::finishConstruction()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel finishConstruction(): entering..." << std::endl;

   //***
   // Assign other data members:
   //***
   thePolyType      = B; // This may not be true for early RPC imagery
   theRefImgPt.line = theLineOffset;
   theRefImgPt.samp = theSampOffset;
   theRefGndPt.lat  = theLatOffset;
   theRefGndPt.lon  = theLonOffset;
   theRefGndPt.hgt  = theHgtOffset;
   theMeanGSD       = (theGSD.x + theGSD.y)/2.0;

   //***
   // Assign the bounding image space rectangle:
   //***
   theImageClipRect = ossimDrect(0.0, 0.0,
                                 theImageSize.samp-1, theImageSize.line-1);

   //***
   // Assign the bounding ground polygon:
   //***
   ossimGpt v0, v1, v2, v3;
   ossimDpt ip0 (0.0, 0.0);
   lineSampleHeightToWorld(ip0, 0.0, v0);
   ossimDpt ip1 (theImageSize.samp-1.0, 0.0);
   lineSampleHeightToWorld(ip1, 0.0, v1);
   ossimDpt ip2 (theImageSize.samp-1.0, theImageSize.line-1.0);
   lineSampleHeightToWorld(ip2, 0.0, v2);
   ossimDpt ip3 (0.0, theImageSize.line-1.0);
   lineSampleHeightToWorld(ip3, 0.0, v3);
   theBoundGndPolygon
      = ossimPolygon (ossimDpt(v0), ossimDpt(v1), ossimDpt(v2), ossimDpt(v3));
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel finishConstruction(): returning..." << std::endl;
   return;
}

//*****************************************************************************
// PROTECTED METHOD: ossimIkonosRpcModel::parseMetaData()
//  
//  Parses the Ikonos metadata file.
//  
//*****************************************************************************
void ossimIkonosRpcModel::parseMetaData(const ossimFilename& data_file)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel::parseMetaData(data_file): entering..." << std::endl;

   FILE* fptr = fopen (data_file, "r");
   if (!fptr)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseMetaData(data_file): Could not open Meta data file <" << data_file << ">. "
                                          << "Aborting..." << std::endl;
      theErrorStatus++;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel::parseMetaData(data_file): returning with error..." << std::endl;
      return;
   }

   char* strptr;
   char linebuf[80];
   char dummy[80], name[80];

   //***
   // Read the file into a buffer:
   //***
   char filebuf[5000];
   fread(filebuf, 1, 5000, fptr);
   
   //***
   // Image ID:
   //***
   strptr = strstr(filebuf, "\nSource Image ID:");
   if (!strptr)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseMetaData(data_file): "
                                          << "\n\tAborting construction. Error encountered parsing "
                                          << "presumed meta-data file." << endl;
      
      return;
   }
      
   sscanf(strptr, "%17c %s", dummy, name);
   theImageID = name;

   //***
   // Sensor Type:
   //***
   strptr = strstr(strptr, "\nSensor:");
   if (!strptr)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseMetaData(data_file): "
                                             << "\n\tAborting construction. Error encountered parsing "
                                             << "presumed meta-data file." << endl;
         
         return;
      }
   }
   sscanf(strptr, "%8c %s", dummy, name);
   theSensorID = name;

   //***
   // GSD:
   //***
   strptr = strstr(strptr, "\nPixel Size X:");
   if (!strptr)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseMetaData(data_file): "
                                             << "\n\tAborting construction. Error encountered parsing "
                                             << "presumed meta-data file." << endl;
         
         return;
      }
   }
   sscanf(strptr, "%14c %lf", dummy, &theGSD.samp);
   strptr = strstr(strptr, "\nPixel Size Y:");
   if (!strptr)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseMetaData(data_file): "
                                             << "\n\tAborting construction. Error encountered parsing "
                                             << "presumed meta-data file." << endl;
         
         return;
      }
   }
   sscanf(linebuf, "%14c %lf", dummy, &theGSD.line);

   //***
   // Image size:
   //***
   strptr = strstr(strptr, "\nColumns:");
   if (!strptr)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseMetaData(data_file): "
                                             << "\n\tAborting construction. Error encountered parsing "
                                             << "presumed meta-data file." << endl;
         
         return;
      }
   }
   sscanf(strptr, "%s %d", dummy, &theImageSize.samp);
   strptr = strstr(strptr, "\nRows:");
   if (!strptr)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseMetaData(data_file): "
                                             << "\n\tAborting construction. Error encountered parsing "
                                             << "presumed meta-data file." << endl;
         
         return;
      }
   }
   sscanf(strptr, "%s %d", dummy, &theImageSize.line);
           
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel::parseMetaData(data_file): returning..." << std::endl;
   return;
}


//*****************************************************************************
// PROTECTED METHOD: ossimIkonosRpcModel::parseRpcData()
//  
//  Parses the Ikonos RPC data file.
//  
//*****************************************************************************
void ossimIkonosRpcModel::parseRpcData(const ossimFilename& data_file)
{
   if (traceExec())   ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel::parseRpcData(data_file): entering..." << std::endl;

   //***
   // The Ikonos RPC data file is conveniently formatted as KWL file:
   //***
   ossimKeywordlist kwl (data_file);
   if (kwl.getErrorStatus())
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "ERROR ossimIkonosRpcModel::parseRpcData(data_file): Could not open RPC data file <" << data_file << ">. "
                                          << "Aborting..." << std::endl;
      theErrorStatus++;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "returning with error..." << std::endl;
      return;
   }

   const char* buf;
   const char* keyword;
   
   //***
   // Parse data from KWL:
   //***
   keyword = LINE_OFF_KW;
   buf = kwl.find(keyword);
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                          << "\nAborting construction. Error looking up keyword: "
                                          << keyword << std::endl;
      return;
   }
   theLineOffset = atof(buf);
      
   keyword = SAMP_OFF_KW;
   buf = kwl.find(keyword);
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                          << "\nAborting construction. Error looking up keyword: "
                                          << keyword << std::endl;
      return;
   }
   theSampOffset = atof(buf);

   keyword = LAT_OFF_KW;
   buf = kwl.find(keyword);
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                          << "\nAborting construction. Error looking up keyword: "
                                          << keyword << std::endl;
      return;
   }
      
   theLatOffset = atof(buf);
   
   keyword = LONG_OFF_KW;
   buf = kwl.find(keyword);
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                          << "\nAborting construction. Error looking up keyword: "
                                          << keyword << std::endl;
      return;
   }
   theLonOffset = atof(buf);

   keyword = HEIGHT_OFF_KW;
   buf = kwl.find(keyword);
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                          << "\nAborting construction. Error looking up keyword: "
                                          << keyword << std::endl;
      return;
   }
      
   theHgtOffset = atof(buf);

   keyword = LINE_SCALE_KW;
   buf = kwl.find(keyword);
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                          << "\nAborting construction. Error looking up keyword: "
                                          << keyword << std::endl;
      return;
   }
   theLineScale = atof(buf);
   
   keyword = SAMP_SCALE_KW;
   buf = kwl.find(keyword);
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                          << "\nAborting construction. Error looking up keyword: "
                                          << keyword << std::endl;
      return;
   }
   theSampScale = atof(buf);
   
   keyword = LAT_SCALE_KW;
   buf = kwl.find(keyword);
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                          << "\nAborting construction. Error looking up keyword: "
                                          << keyword << std::endl;
      return;
   }

   theLatScale = atof(buf);
   
   keyword = LONG_SCALE_KW;
   buf = kwl.find(keyword);
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                          << "\nAborting construction. Error looking up keyword: "
                                          << keyword << std::endl;
      return;
   }
   theLonScale = atof(buf);
   
   keyword = HEIGHT_SCALE_KW;
   buf = kwl.find(keyword);
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                          << "\nAborting construction. Error looking up keyword: "
                                          << keyword << std::endl;
      return;
   }
      
   theHgtScale = atof(buf);
   
   char kwbuf[32];
   keyword = kwbuf;
   for(int i=1; i<=20; i++)
   {
      sprintf(kwbuf, "%s%d", LINE_NUM_COEFF_KW, i);
      buf = kwl.find(keyword);
      if (!buf)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                             << "\nAborting construction. Error looking up keyword: "
                                             << keyword << std::endl;
         return;
      }
      
      theLineNumCoef[i-1] = atof(buf);
      
      sprintf(kwbuf, "%s%d", LINE_DEN_COEFF_KW, i);
      buf = kwl.find(keyword);
      if (!buf)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                             << "\nAborting construction. Error looking up keyword: "
                                             << keyword << std::endl;
         return;
      }
      theLineDenCoef[i-1] = atof(buf);
      
      sprintf(kwbuf, "%s%d", SAMP_NUM_COEFF_KW, i);
      buf = kwl.find(keyword);
      if (!buf)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                             << "\nAborting construction. Error looking up keyword: "
                                             << keyword << std::endl;
         return;
      }
      theSampNumCoef[i-1] = atof(buf);
      
      sprintf(kwbuf, "%s%d", SAMP_DEN_COEFF_KW, i);
      buf = kwl.find(keyword);
      if (!buf)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimIkonosRpcModel::parseRpcData(data_file):"
                                             << "\nAborting construction. Error looking up keyword: "
                                             << keyword << std::endl;
         return;
      }
      theSampDenCoef[i-1] = atof(buf);
   }

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel::parseRpcData(data_file): returning..." << std::endl;
   return;

   theErrorStatus++;
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimIkonosRpcModel::parseRpcData(data_file): returning with error..." << std::endl;
   
   return;
}

//*****************************************************************************
//  METHOD: ossimIkonosRpcModel::writeGeomTemplate()
//  
//   Writes a template of an ossimIkonosRpcModel geometry file.
//  
//*****************************************************************************
void ossimIkonosRpcModel::writeGeomTemplate(ostream& os)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::writeGeomTemplate(os): entering..." << std::endl;

   os <<
      "//**************************************************************\n"
      "// Template for Ikonos RPC geometry keywordlist\n"
      "//\n"
      "// NOTE: It is preferable to select the full RPC geometry KWL \n"
      "//       that should have been created with the first use of the\n"
      "//       derived model type ossimIkonosRpcModel. Using this KWL \n"
      "//       implies that an initial geometry is being constructed \n"
      "//       with all adjustable parameters initialized to 0. \n"
      "//**************************************************************\n"
      << ossimKeywordNames::TYPE_KW << ": " << MODEL_TYPE << endl;
   os << META_DATA_FILE << ": <string>\n"
      << RPC_DATA_FILE  << ": <string>\n" << endl;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::writeGeomTemplate(os): returning..." << std::endl;
   return;
}

bool ossimIkonosRpcModel::saveState(ossimKeywordlist& kwl,
				    const char* prefix)const
{
  ossimRpcModel::saveState(kwl, prefix);

  // this model just sets the base class values so
  // we do not need to re-construct this model so 
  // specify the type as the base class type
  //
  kwl.add(prefix,
	  ossimKeywordNames::TYPE_KW,
	  STATIC_TYPE_NAME(ossimRpcModel),
	  true);

  return true;
}
