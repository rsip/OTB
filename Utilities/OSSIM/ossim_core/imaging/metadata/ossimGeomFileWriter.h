//*******************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Kenneth Melero (kmelero@sanz.com)
//
//*******************************************************************
//  $Id: ossimGeomFileWriter.h,v 1.7 2005/08/08 22:00:24 dburken Exp $

#ifndef ossimGeomFileWriter_H
#define ossimGeomFileWriter_H

#include <imaging/metadata/ossimMetadataFileWriter.h>

/** ossimGeomFileWriter */
class OSSIMDLLEXPORT ossimGeomFileWriter : public ossimMetadataFileWriter
{
public:
   /** default constructor */
   ossimGeomFileWriter();

   /** virtual destructor */
   virtual ~ossimGeomFileWriter();

   /**
    * Satisfies pure virtual from ossimMetadataWriter base.
    *
    * Appends the writers image types to the "metadatatypeList".
    * 
    * @param metadatatypeList stl::vector<ossimString> list to append to.
    *
    * @note Appends to the list, does not clear it first.
    */
   virtual void getMetadatatypeList(
      std::vector<ossimString>& metadatatypeList) const;

   /**
    * Satisfies pure virtual from ossimMetadataWriter base.
    *
    * @param imageType string representing image type.
    *
    * @return true if "metadataType" is supported by writer.
    */
   virtual bool hasMetadataType(const ossimString& metadataType)const;

private:

   /** Write a geometry file to "theFilename". */
   virtual bool writeFile();
   
TYPE_DATA
};

#endif /* End of #ifndef ossimGeomFileWriter_H */
