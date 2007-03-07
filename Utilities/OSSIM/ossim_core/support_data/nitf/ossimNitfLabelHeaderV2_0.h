//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
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
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfLabelHeaderV2_0.h,v 1.3 2004/05/23 18:42:33 dburken Exp $

#ifndef ossimNitfLabelHeaderV2_0_HEADER
#define ossimNitfLabelHeaderV2_0_HEADER
#include <support_data/nitf/ossimNitfLabelHeader.h>

class OSSIMDLLEXPORT ossimNitfLabelHeaderV2_0 : public ossimNitfLabelHeader
{
public:
   ossimNitfLabelHeaderV2_0();
   virtual ~ossimNitfLabelHeaderV2_0(){}
   
   virtual void parseStream(istream &in);
   virtual std::ostream& print(std::ostream &out)const;
   virtual long getDisplayLevel()const;
   virtual ossimDrect getImageRect()const;

TYPE_DATA
private:
   void clearFields();

   /*!
    * This is a required 2 byte field and has the value
    * LA
    */
   char theFilePartType[3];

   /*!
    * This is a required 10 byte field.
    */
   char theLabelId[11];

   /*!
    * This is a required 1 byte field and will have
    * a value of either:
    *
    * T    Top secret
    * S    Secret
    * C    Confidential
    * R    Restricted
    * U    Unclassified
    */
   char theLabelSecurityClassification[2];

   /*!
    * optional 40 byte field
    */
   char theLabelCodewords[41];

   /*!
    * optional 40 byte field
    */
   char theLabelControlAndHandling[41];

   /*!
    * optional 40 byte field
    */
   char theLabelReleasingInstructions[41];

   /*!
    * optional 20 byte field
    */
   char theLabelClassificationAuthority[21];

   /*!
    * optional 20 byte field
    */
   char theLabelSecurityControlNumber[21];

   /*!
    * optional 6 byte field
    */
   char theLabelSecurityDowngrade[7];

   /*!
    * conditional 40 byte field.  It exists only if
    * theLabelSecurityDowngrade is = 999998
    */
   char theLabelDowngradingEvent[41];

   /*!
    * required 1 byte field and can have value
    * 0  = not encrypted
    * 1  = encrypted
    */
   char theLabelEncryption[2];

   /*!
    * required 1 byte field.  Is reserved for future
    * use.  Will contain 1 blank.
    */
   char theLabelFontStyle[2];

   /*!
    * optional 2 byte field.  Ranges from 1-99 
    */
   char theLabelCellWidth[3];

   /*!
    * optional 2 byte field. Ranges from 1-99
    */
   char theLabelCellHeight[3];

   /*!
    * Required 3 byte field.  Ranges from 1-999.
    */
   char theLabelDisplayLevel[4];

   /*!
    * Required 3 byte field.  Ranges from 1-998.
    */
   char theLabelAttachmentLevel[4];

   /*!
    * Required 10 byte field. Has format:
    *
    * rrrrrccccc  the first five chars is the row
    *             location and the second is the
    *             column location.
    */
   char theLabelLocation[11];

   /*!
    * Required 3 byte field.  Has format:
    *
    * RGB   First byte is red, second is blue, and
    *       third is green
    */
   char theLabelTextColor[4];

   /*!
    * Required 3 byte field.  Has format:
    *
    * RGB   First byte is red, second is blue, and
    *       third is green
    */
   char theLabelBackgroundColor[4];

   /*!
    * Required 5 byte field.  Ranges from 0-09747
    */
   char theExtendedHeaderDataLength[6];

   /*!
    * Conditional 3 byte field.  Will exist if
    * theExtendedHeaderDataLength is not 0
    */
   char theExtendedSubheaderOverflow[4];
   
};
#endif
