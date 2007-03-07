//*******************************************************************
// Copyright (C) 2000 Intelligence Data Systems. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
//
// Description: Nitf support class for RPC00A -
// Rational Polynomial Coefficient extension.
//
//********************************************************************
// $Id: ossimNitfRpcATag.h,v 1.3 2005/09/28 18:29:29 dburken Exp $
#ifndef ossimNitfRpcATag_HEADER
#define ossimNitfRpcATag_HEADER

#include <support_data/nitf/ossimNitfRpcBase.h>

/**
 * The layout of RPC00B is the same as RPC00A.  The difference is how the
 * coefficients are used; hence, all the code is in the ossimNitfRpcBase class.
 */
class OSSIM_DLL ossimNitfRpcATag : public ossimNitfRpcBase
{
public:
   
   ossimNitfRpcATag();

   /** @return "RPC00A" as an ossimString. */
   virtual ossimString getRegisterTagName() const;
   
protected:
   
TYPE_DATA   
};

#endif
