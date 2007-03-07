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
//  $Id: ossimReadmeFileWriter.h,v 1.5 2005/08/08 22:00:24 dburken Exp $

#ifndef ossimReadmeFileWriter_H
#define ossimReadmeFileWriter_H

#include <fstream>
#include <imaging/metadata/ossimMetadataFileWriter.h>

/** ossimReadmeFileWriter */
class OSSIMDLLEXPORT ossimReadmeFileWriter : public ossimMetadataFileWriter
{
public:

   /** default constructor */
   ossimReadmeFileWriter();

   /** virtual destructor */
   virtual ~ossimReadmeFileWriter();

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
   virtual bool writeFile();
   
TYPE_DATA
};

#endif /* End of #ifndef ossimReadmeFileWriter_H */
