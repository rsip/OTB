//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class declaration for TiffWriter.
//
//*******************************************************************
//  $Id: ossimNBandLutDataObject.cpp,v 1.7 2005/11/14 11:58:06 gpotts Exp $
#include <sstream>
#include <base/misc/lookup_tables/ossimScalarTypeLut.h>
#include <base/common/ossimNBandLutDataObject.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimFilename.h>
#include <base/context/ossimNotifyContext.h>

RTTI_DEF1(ossimNBandLutDataObject, "ossimNBandLutDataObject", ossimObject);

ostream& operator <<(ostream& out,
                     const ossimNBandLutDataObject& lut)
{
   if(lut.theLut)
   {
      out << ossimKeywordNames::NUMBER_ENTRIES_KW << ": " << lut.theNumberOfEntries << std::endl;
      out << ossimKeywordNames::NUMBER_BANDS_KW   << ": " << lut.theNumberOfBands << std::endl;
      out << "null_pixel_index: " << lut.theNullPixelIndex << std::endl;
      for(ossim_uint32 idx = 0; idx < lut.theNumberOfEntries; ++idx)
      {
         const ossimNBandLutDataObject::LUT_ENTRY_TYPE *bandPtr =lut[idx];
         ossim_uint32 bandIdx = 0;
         out << ossimKeywordNames::ENTRY_KW << idx << ": ";
         for(bandIdx = 0; bandIdx < lut.theNumberOfBands; ++bandIdx)
         {
            out << bandPtr[bandIdx] << " ";
         }
         if(idx < (lut.theNumberOfEntries-1))
         {
            out << std::endl;
         }
      }
   }
   return out;
}

ossimNBandLutDataObject::ossimNBandLutDataObject(ossim_uint32 numberOfEntries,
                                                 ossim_uint32 numberOfBands,
                                                 ossimScalarType bandScalarType,
                                                 ossim_int32 nullPixelIndex)
   :theLut(0),
    theNumberOfEntries(0),
    theNumberOfBands(0),
    theBandScalarType(bandScalarType),
    theNullPixelIndex(nullPixelIndex)
{
   create(numberOfEntries, numberOfBands);
}

ossimNBandLutDataObject::ossimNBandLutDataObject(const ossimNBandLutDataObject& lut)
   :theLut(0),
    theNumberOfEntries(lut.theNumberOfEntries),
    theNumberOfBands(lut.theNumberOfBands),
    theBandScalarType(lut.theBandScalarType),
    theNullPixelIndex(lut.theNullPixelIndex)
{
   *this = lut;
   
}

ossimNBandLutDataObject::~ossimNBandLutDataObject()
{
   if(theLut)
   {
      delete [] theLut;
      theLut = 0;
   }
   theNumberOfEntries = 0;
   theNumberOfBands   = 0;
}

void ossimNBandLutDataObject::create(ossim_uint32 numberOfEntries,
                                     ossim_uint32 numberOfBands)
{
   if(theLut)
   {
      delete [] theLut;
      theLut = 0;
   }
   if(numberOfEntries&&numberOfBands)
   {
      theLut = new ossimNBandLutDataObject::LUT_ENTRY_TYPE[numberOfEntries*numberOfBands];
      theNumberOfEntries = numberOfEntries;
      theNumberOfBands   = numberOfBands;
   }
   else
   {
      theNumberOfEntries = 0;
      theNumberOfBands   = 0;
   }
}

ossim_uint32 ossimNBandLutDataObject::findIndex(ossimNBandLutDataObject::LUT_ENTRY_TYPE* bandValues)const
{
   ossim_float64 distance = 1.0/DBL_EPSILON; 
   ossim_uint32  result   = 0;

   if(theNumberOfEntries > 0)
   {
      ossim_uint32 idx = 0;
      ossim_uint32 bandIdx = 0;
      ossimNBandLutDataObject::LUT_ENTRY_TYPE* lutPtr = theLut;
      for(idx = 0; idx < theNumberOfEntries; ++idx,lutPtr+=theNumberOfBands)
      {
         ossim_float64 sumSquare = 0.0;

         for(bandIdx = 0; bandIdx < theNumberOfBands; ++bandIdx)
         {
            ossim_int64 delta = lutPtr[bandIdx] - bandValues[bandIdx];
            sumSquare += (delta*delta);
         }
         if((ossimNBandLutDataObject::LUT_ENTRY_TYPE)sumSquare == 0)
         {
            return idx;
         }
         else if( sumSquare < distance)
         {
            result = idx;
            distance = sumSquare;
         }
      }
   }

   return result;
}

void ossimNBandLutDataObject::clearLut()
{
   if(theLut)
   {
      memset(theLut, '\0', theNumberOfEntries*theNumberOfBands*sizeof(ossimNBandLutDataObject::LUT_ENTRY_TYPE));
   }
}

void ossimNBandLutDataObject::getMinMax(ossim_uint32 band,
                                        ossimNBandLutDataObject::LUT_ENTRY_TYPE& minValue,
                                        ossimNBandLutDataObject::LUT_ENTRY_TYPE& maxValue)
{
   minValue = 0;
   maxValue = 0;
   ossim_uint32 idx = 0;
   LUT_ENTRY_TYPE *bandPtr = theLut+theNumberOfBands;
   if((band < theNumberOfBands)&&
      (theNumberOfEntries > 0))
   {
      minValue = theLut[band];
      maxValue = theLut[band];
      
      for(idx = 1; idx < theNumberOfEntries; ++idx,bandPtr+=theNumberOfBands)
      {
         if((ossim_int32)idx != theNullPixelIndex)
         {
            if(bandPtr[band] < minValue)
            {
               minValue = bandPtr[band];
            }
            if(bandPtr[band] > maxValue)
            {
               maxValue = bandPtr[band];
            }
         }
      }
   }
}

const ossimNBandLutDataObject& ossimNBandLutDataObject::operator =(const ossimNBandLutDataObject& lut)
{
   if(theNumberOfEntries != lut.theNumberOfEntries)
   {
      delete [] theLut;
      theLut = 0;
   }

   theNullPixelIndex = lut.theNullPixelIndex;
   theBandScalarType = lut.theBandScalarType;
   create(lut.theNumberOfEntries,
          lut.theNumberOfBands);

   if(theLut)
   {
      memcpy(theLut, lut.theLut, (theNumberOfEntries*theNumberOfBands)*sizeof(ossimNBandLutDataObject::LUT_ENTRY_TYPE));
   }

   return *this;
}

bool ossimNBandLutDataObject::operator ==(const ossimNBandLutDataObject& lut)const
{
   if(theNumberOfEntries != lut.theNumberOfEntries)
   {
      return false;
   }

   if(!theLut && !lut.theLut) return true;
   if(theNullPixelIndex != lut.theNullPixelIndex) return false;
   if(theBandScalarType != lut.theBandScalarType) return false;
   
   if(theLut&&lut.theLut)
   {
      return (memcmp(theLut, lut.theLut, theNumberOfEntries*theNumberOfBands*sizeof(ossimNBandLutDataObject::LUT_ENTRY_TYPE)) == 0);
   }
   return false;
}

bool ossimNBandLutDataObject::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           getClassName(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_ENTRIES_KW,
           ossimString::toString(theNumberOfEntries).c_str(),
           true);
   kwl.add(prefix,
           ossimKeywordNames::NUMBER_BANDS_KW,
           theNumberOfBands,
           true);
   kwl.add(prefix,
           ossimKeywordNames::NULL_VALUE_KW,
           theNullPixelIndex,
           true);
   kwl.add(prefix,
           ossimKeywordNames::SCALAR_TYPE_KW,
           ossimScalarTypeLut::instance()->getEntryString(theBandScalarType),
           true);
          
   ossimNBandLutDataObject::LUT_ENTRY_TYPE* lutPtr = theLut;
   for(ossim_uint32 idx = 0; idx < theNumberOfEntries; ++idx, lutPtr+=theNumberOfBands)
   {
      ossimString newPrefix = ossimKeywordNames::ENTRY_KW;
      newPrefix += ossimString::toString(idx);
      std::ostringstream ostr;
      ossim_uint32 bandIdx = 0;
      for(bandIdx = 0; bandIdx < theNumberOfBands; ++bandIdx)
      {
         ostr << lutPtr[bandIdx]
              << " ";
      }
      kwl.add(prefix,
              newPrefix,
              ostr.str().c_str(),
              true);
   }

   return true;
}

bool ossimNBandLutDataObject::open(const ossimFilename& lutFile)
{
   ossimKeywordlist kwl;
   kwl.addFile(lutFile);
   
   return loadState(kwl);
}

bool ossimNBandLutDataObject::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   const char* nullPixelIndex = kwl.find(prefix, ossimKeywordNames::NULL_VALUE_KW);
   const char* lutFile = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   ossimKeywordlist fileLut;
   const ossimKeywordlist* tempKwl = &kwl;
   ossimString tempPrefix = prefix;

   // check to see if we should open an external file
   // if so point the fileLut to the one that we use
   if(lutFile)
   {
      ossimFilename filename(lutFile);
      if(filename.exists())
      {
         fileLut.addFile(filename.c_str());
         tempKwl = &fileLut;
         tempPrefix = "";
      }
   }

   if(nullPixelIndex)
   {
      theNullPixelIndex = ossimString(nullPixelIndex).toInt32();
   }
   else
   {
      theNullPixelIndex = -1;
   }
   int scalar = ossimScalarTypeLut::instance()->getEntryNumber(kwl, prefix);

   if (scalar != ossimLookUpTable::NOT_FOUND)
   {
      theBandScalarType = static_cast<ossimScalarType>(scalar);
   }
   const char* numberOfBands   = tempKwl->find(tempPrefix, ossimKeywordNames::NUMBER_BANDS_KW);
   const char* numberOfEntries = tempKwl->find(tempPrefix, ossimKeywordNames::NUMBER_ENTRIES_KW);
   
   create(ossimString(numberOfEntries).toUInt32(),
          ossimString(numberOfBands).toUInt32());
   clearLut();
   ossimNBandLutDataObject::LUT_ENTRY_TYPE* lutPtr = theLut;

   if(lutPtr)
   {
      ossim_uint32 entryIdx;
      ossim_uint32 bandIdx;
      for(entryIdx = 0; entryIdx < theNumberOfEntries; ++entryIdx)
      {
         ossimString newPrefix = "entry";
         newPrefix += ossimString::toString(entryIdx);
         ossimString v = tempKwl->find(tempPrefix, newPrefix.c_str());
         v = v.trim();
         if(v != "")
         {
            std::istringstream istr(v);
            
            for(bandIdx = 0; bandIdx < theNumberOfBands; ++bandIdx)
            {
               if(!istr.fail())
               {
                  istr >> lutPtr[bandIdx];
               }
            }
         }
         lutPtr += theNumberOfBands;
      }
   }
   
   return true;
}
