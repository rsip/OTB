//*************************************************************************
// Copyright (C) 2004 Intelligence Data Systems, Inc.  All rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
//**************************************************************************
// $Id: ossimVpfAnnotationCoverageInfo.h,v 1.8 2004/12/21 20:12:40 gpotts Exp $
#ifndef ossimVpfAnnotationCoverageInfo_HEADER
#define ossimVpfAnnotationCoverageInfo_HEADER
#include <imaging/formats/vpf/ossimVpfAnnotationFeatureInfo.h>
#include <vec/vpf/ossimVpfCoverage.h>
class ossimVpfLibrary;

class ossimRgbImage;

class ossimVpfAnnotationCoverageInfo
{
public:
  ossimVpfAnnotationCoverageInfo(const ossimString& name="")
    :theName(name)
  {
    
  }
  virtual ~ossimVpfAnnotationCoverageInfo()
  {
    deleteAllFeatures();
  }
  bool hasRenderableFeature()const;
  void getEnabledFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& result);
  void getAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& result);

  void transform(ossimProjection* proj);
  ossimIrect getBoundingProjectedRect()const;
  void buildCoverage();
  void buildCoverage(const ossimString& feature);
  void setName(const ossimString& name)
  {
    theName = name;
  }
  ossimString getName()const
     {
        return theName;
     }
  void setLibrary(ossimVpfLibrary* library)
  {
    theLibrary = library;
  }
  ossimVpfLibrary* getLibrary()
  {
    return theLibrary;
  }
  const ossimVpfLibrary* getLibrary()const
  {
    return theLibrary;
  }
  void drawAnnotations(ossimRgbImage* tile);
   void deleteAllFeatures();

  virtual bool saveState(ossimKeywordlist& kwl,
			 const char* prefix=0)const;
  virtual bool loadState(const ossimKeywordlist& kwl,
			 const char* prefix=0);
protected:
  ossimString theName;

  /*!
   * Not owned.
   */
  ossimVpfLibrary* theLibrary;
  std::vector<ossimVpfAnnotationFeatureInfo*>           theFeatureInfoArray;
};

#endif
