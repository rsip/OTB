//*******************************************************************
// Copyright (C) 2003 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Histogram remapper class declaration.  Derived from ossimTableRemapper.
//
// Remaps a tile based on mode and histogram clip points.  This object uses
// a precomputed remap table for speed; hence, derived from the
// ossimTableRemapper
//
// NOTE on bands:
// 
// - All methods take zero based bands.  In other words the first
//   band is band "0".  
//
// - This objects band 0 is the same band 0 as it's input connection.  If
//   the band order was changed by a ossimBandSelector then this band 0 will
//   not map to the histogram files band 0.  To handle this,
//   the "getOutputBandList()" method is used to get the correct histogram for
//   the input band.
//   
// - The "get" methods that do not take a band, simply return the average of
//   all bands.  This really only makes sense if all bands are the same
//   but is provided for convenience.
//   
//*************************************************************************
// $Id: ossimHistogramRemapper.h,v 1.17 2005/09/12 13:15:52 gpotts Exp $
#ifndef ossimHistogramRemapper_HEADER
#define ossimHistogramRemapper_HEADER

#include <imaging/tile_sources/ossimTableRemapper.h>

class ossimMultiResLevelHistogram;
class ossimHistogram;

class OSSIMDLLEXPORT ossimHistogramRemapper : public ossimTableRemapper
{
public:
   enum StretchMode
   {
      LINEAR_ONE_PIECE      = 0,
      LINEAR_1STD_FROM_MEAN = 1,
      LINEAR_2STD_FROM_MEAN = 2,
      LINEAR_3STD_FROM_MEAN = 3,
      LINEAR_AUTO_MIN_MAX   = 4,
      STRETCH_UNKNOWN = 5 // Alway last as used for number of modes method.
      
   };
   
   ossimHistogramRemapper(ossimObject* owner=NULL);
   ossimHistogramRemapper(ossimImageSource* inputSource);

   /**
    * NOTE:
    * - If input chip source is mutiband this will set all band to same
    *   percentage of clip.
    * - low_clip  is a normalized percentage, 0.01 = one percent low clip.
    * - high_clip is a normalized percentage, 0.99 = one percent high clip.
    */
   ossimHistogramRemapper(ossimImageSource* inputSource,
                          ossimMultiResLevelHistogram* histogram,
                          bool own_histogram = true,
                          ossim_float64 low_clip = 0.0,
                          ossim_float64 high_clip = 1.0);
   
   
   virtual ~ossimHistogramRemapper();

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();

   /**
    * - Disables this source.
    * - Sets all clip points to default.
    * - Removes current table.
    */
   void reset();
   
   /**
    * Sets remap mode to mode.
    */
   void setStretchMode(StretchMode mode);

   /**
    * Returns the current enumerated node.
    */
   StretchMode   getStretchMode() const;

   /**
    * Returns the string for current remap mode.
    */
   ossimString getStretchModeString() const;

   /**
    * Returns the number of stretch modes.
    */
   ossim_uint32 getNumberOfStretchModes() const;

   /**
    * Returns the mode string for a given index.
    * Returns "stretch_unknown" if index is out of range.
    * 
    * NOTE:
    * - If you're building gui buttons you can:
    *   // pseudo code...
    *   ossim_uint32 number_of_modes = remapper->getNumberOfModes()
    *   for (ossim_uint32 i = 0; i < number_of_modes; ++i)
    *   {
    *      addButton(getModeString(i).c_str());
    *   }
    */
   ossimString getStretchModeString(ossim_uint32 index) const;
   
   /**
    * Sets the low clip point.
    *
    * Note on null bin:
    * - The null bin is ignored when considering a percent of penetration from
    *   the low end.
    *
    * Notes on clip point:
    * - If input chip source is mutiband this will set all band to same
    *   percentage of clip.
    * - Clip points should be normalized between zero and one.
    * - A low clip point of 0.0 means no penetration.
    * - A low clip point of 0.02 means penetrate in two percent.
    */
   void setLowNormalizedClipPoint(const ossim_float64& clip);

   /**
    * Sets the low clip point for band.
    *
    * Note on null bin:
    * - The null bin is ignored when considering a percent of penetration from
    *   the low end.
    *
    * Notes on clip points:
    * - Clip point should be normalized between zero and one.
    * - A low clip point of 0.0 means no penetration.
    * - A low clip point of 0.02 means penetrate in two percent.
    */
   void setLowNormalizedClipPoint(const ossim_float64& clip,
                                  ossim_uint32 zero_based_band);

   /**
    * Sets the high clip point.
    * 
    * Notes on clip points:
    * - If input chip source is mutiband this will set all band to same
    *   percentage of clip.
    * - Clip point should be normalized between zero and one.
    * - A high clip point of 1.0 means no penetration.
    * - A high clip point of .98 mean a 2 % penetration from the high end.
    */
   void setHighNormalizedClipPoint(const ossim_float64& clip);

   /**
    * Sets the high clip point.
    *
    * Notes on clip points:
    * - Clip point should be normalized between zero and one.
    * - A high clip point of 1.0 means no penetration.
    * - A high clip point of .98 mean a 2 % penetration from the high end.
    */
   void setHighNormalizedClipPoint(const ossim_float64& clip,
                                   ossim_uint32 zero_based_band);

   /**
    * Sets the low clip point.
    *
    * Notes on clip point:
    * - If input chip source is mutiband this will set all band to same
    *   percentage of clip.
    * - Clip points are in DN values.
    * - If histogram is not set no action is taken. 
    */
   void setLowClipPoint(const ossim_float64& clip);

   /**
    * Sets the low clip point for band.
    *
    * Notes on clip points:
    * - Clip points are DN values.
    * - If histogram is not set no action is taken. 
    */
   void setLowClipPoint(const ossim_float64& clip,
                        ossim_uint32 zero_based_band);

   /**
    * Sets the high clip point.
    * 
    * Notes on clip points:
    * - If input chip source is mutiband this will set all band to same
    *   percentage of clip.
    * - Clip points are DN values.
    * - If histogram is not set no action is taken. 
    */
   void setHighClipPoint(const ossim_float64& clip);

   /**
    * Sets the high clip point.
    *
    * Notes on clip points:
    * - Clip points are DN values.
    * - If histogram is not set no action is taken. 
    */
   void setHighClipPoint(const ossim_float64& clip,
                         ossim_uint32 zero_based_band);

   /**
    * Sets the mid clip point.
    * 
    * Notes on clip points:
    * - If input chip source is mutiband this will set all band to same
    *   percentage of clip.
    */
   void setMidPoint(const ossim_float64& value);

   /**
    * Sets the mid clip point for band.
    */
   void setMidPoint(const ossim_float64& value,
                    ossim_uint32 zero_based_band);

   /**
    * Sets the min output value.
    * 
    * Notes on clip points:
    * - If input chip source is mutiband this will set all band to same
    *   percentage of clip.
    */
   void setMinOutputValue(const ossim_float64& value);

   /**
    * Sets the min output value for band.
    */
   void setMinOutputValue(const ossim_float64& value,
                          ossim_uint32 zero_based_band);

   /**
    * Sets the max output value.
    * 
    * Notes on clip points:
    * - If input chip source is mutiband this will set all band to same
    *   percentage of clip.
    */
   void setMaxOutputValue(const ossim_float64& value);

   /**
    * Sets the min output value for band.
    */
   void setMaxOutputValue(const ossim_float64& value,
                          ossim_uint32 zero_based_band);
   
   /**
    * Returns the normalized low clip point for band.
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    * - Histogram has not been set.
    */
   ossim_float64 getLowNormalizedClipPoint(ossim_uint32 zero_based_band) const;

   /**
    * Returns the normalized low clip point which is the average of all bands.
    * 
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    * - Histogram has not been set.
    */
   ossim_float64 getLowNormalizedClipPoint() const;

   /**
    * Returns the normalized high clip point for band.
    *
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    * - Histogram has not been set.
    */
   ossim_float64 getHighNormalizedClipPoint(ossim_uint32 zero_based_band) const;
   /**
    * Returns the normalized High clip point which is the average of all bands.
    * 
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    * - Histogram has not been set.
    */
   ossim_float64 getHighNormalizedClipPoint() const;

   /**
    * Returns the low clip point for band.
    * Clip points are DN values relative to the scalar type.
    *
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    * - Histogram has not been set.
    */
   ossim_float64 getLowClipPoint(ossim_uint32 zero_based_band) const;

   /**
    * Returns the low clip point which is the average of all bands.
    * 
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    * - Histogram has not been set.
    */
   ossim_float64 getLowClipPoint() const;

   /**
    * Returns the high clip point for band.
    * Clip points are DN values relative to the scalar type.
    *
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    * - Histogram has not been set.
    */
   ossim_float64 getHighClipPoint(ossim_uint32 zero_based_band) const;

   /**
    * Returns the high clip point which is the average of all bands.
    * 
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    * - Histogram has not been set.
    */
   ossim_float64 getHighClipPoint() const;

   
   /**
    * Returns the mid point for band
    * 
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    * - Histogram has not been set.
    */
   ossim_float64 getMidPoint(ossim_uint32 zero_based_band) const;
   
   /**
    * Returns the mid clip point which is the average of all bands.
    * 
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    * - Histogram has not been set.
    */
   ossim_float64 getMidPoint() const;
   
   /**
    * Returns the minimum output value for band.
    * 
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    */
   ossim_float64 getMinOutputValue(ossim_uint32 zero_based_band) const;

   /**
    * Returns the minimum output value which is the average of all bands.
    * 
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    */
   ossim_float64 getMinOutputValue() const;

   /**
    * Returns the maximum output value for band.
    * 
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    */
   ossim_float64 getMaxOutputValue(ossim_uint32 zero_based_band) const;

   /**
    * Returns the maximum output value which is the average of all bands.
    * 
    * Will return OSSIM_DBL_NAN if:
    * - Band is out of range.
    * - Connection is not complete.
    */
   ossim_float64 getMaxOutputValue() const;
   
   /**
    * Sets the histogram.
    * If (own_histogram == true) this object will delete theHistogram
    * on destruction.
    */
   void setHistogram(ossimMultiResLevelHistogram* histogram,
                     bool own_histogram = true);

   /**
    * Returns pointer to histogram for band and reduced res level.
    * - Band is zero based relative to the input connection.  This will be
    *   remapped to the histogram band using the "getOutputBandList()" to
    *   get the correct histogram band.
    *   
    * Will return NULL if:
    * - band is out range
    * - res_level is out of range
    * - theHistogram has not been initialized yet.
    */
   ossimHistogram* getHistogram(ossim_uint32 zero_based_band,
                                ossim_uint32 res_level=0) const;

   /**
    * Open the histogram file.
    * With this method the histogram is owned by this object.
    * Returns true on success, false on error.
    */
   bool openHistogram(const ossimFilename& histogram_file);

   /**
    * Returns the currently opened histogram.
    * Returns ossimFilename::NIL if no histogram is loaded.
    */
   ossimFilename getHistogramFile() const;
   
   /**
    * Method to the save the state to a keyword list.
    * Returns true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.
    * Returns true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   
   virtual ostream& print(ostream& os) const;

   friend ostream& operator <<(ostream& os, const ossimHistogramRemapper& hr);
   
private:
   // Do not allow copy constructor, operator=.
   ossimHistogramRemapper(const ossimHistogramRemapper& hr);
   ossimHistogramRemapper& operator=(const ossimHistogramRemapper& hr);

   void buildTable();
   void buildLinearTable();
   void buildAutoLinearMinMaxTable();
   template <class T> void buildLinearTable(T dummy);
   template <class T> void buildAutoLinearMinMaxTableTemplate(T dummy);

   /**
    * Sets clip points using mean and standard deviations then calls
    * buildLinearTable method.
    */
   void buildLinearTableStdFromMean();
   
   /**
    * Uses getNumberOfInputBands() to determine BANDS then calls
    * initializeClips(BANDS)
    */
   void initializeClips();

   /**
    * Initializes data members to some default state and size.
    */
   void initializeClips(ossim_uint32 bands);

   /*
    * Sets the count of the null bin to 0 so that clips from the low end
    * ignore the null bin.
    */
   void setNullCount() const;

   /**
    * Initialized base class (ossimTableRemapper) values:
    * - theTableBinCount
    * - theTableBandCount
    * - theTableType
    * - theTable (resizes if not correct size.
    */
   void setupTable();

   void verifyEnabled();

   /**
    * Returns the histogram band that maps to the input band.
    * Note:
    * - This uses the current band list from "getOutputBandList()".
    */
   ossim_uint32 getHistogramBand(ossim_uint32 input_band) const;


   StretchMode                   theStretchMode;
   bool                          theDirtyFlag;
   bool                          theHistogramOwnerFlag;
   ossimMultiResLevelHistogram*  theHistogram;
   ossim_uint32                  theTableSizeInBytes;
   vector<ossim_float64>         theNormalizedLowClipPoint;
   vector<ossim_float64>         theNormalizedHighClipPoint;
   vector<ossim_float64>         theMidPoint;
   vector<ossim_float64>         theMinOutputValue;
   vector<ossim_float64>         theMaxOutputValue;

   // Maps zero based band to histogram band.
   vector<ossim_uint32>          theBandList;
   
   TYPE_DATA
};

#endif  /* #ifndef ossimHistogramRemapper_HEADER */
