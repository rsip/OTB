//*****************************************************************************
// FILE: ossimPreferences.cc
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
// DESCRIPTION:
//   Contains implementation of class ossimPreferences. This class provides
//   a static keywordlist for global preferences. Objects needing access to
//   application-wide global parameters shall do so through this class.
//
// SOFTWARE HISTORY:
//>
//   23Apr2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************

#include <stdlib.h>
#include <iostream>
using namespace std;

#include <base/common/ossimPreferences.h>
#include <base/context/ossimNotifyContext.h>

//RTTI_DEF1(ossimPreferences, "ossimPreferences", ossimObject)

//***
// Define Trace flags for use within this file:
//***
#include "base/common/ossimTrace.h"
static ossimTrace traceExec  ("ossimPreferences:exec");
static ossimTrace traceDebug ("ossimPreferences:debug");

static const char* PREF_FILE_ENV_VAR_NAME = "OSSIM_PREFS_FILE";

ossimPreferences* ossimPreferences::theInstance = NULL;

ossimPreferences::ossimPreferences()
{
   loadPreferences();
}

ossimPreferences::~ossimPreferences()
{
        theInstance = NULL;
}

/*!****************************************************************************
 * METHOD: ossimPreferences::instance()
 *  
 *  This is the method by which run-time objects access this singleton instance
 *  
 *****************************************************************************/
ossimPreferences* ossimPreferences::instance()
{
   /*!
    * Simply return the instance if already created:
    */
   if (theInstance)
      return theInstance;

   /*!
    * Create the static instance of this class:
    */
   theInstance = new ossimPreferences();

   return theInstance;
}

/*!****************************************************************************
 * METHOD: loadPreferences()
 *  
 *  Loads the preferences file specified in the runtime environment.
 *  
 *****************************************************************************/
bool ossimPreferences::loadPreferences()
{
   static const char MODULE[] = "ossimPreferences::loadPreferences()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: " << MODULE << " entering...\n";

   bool parsed_ok = false;
   
   /*!
    * Fetch preferences file name from environment:
    */
   char* pref_filename = getenv(PREF_FILE_ENV_VAR_NAME);

   if (pref_filename)
   {
      /*!
       * Load the preferences file into the static keywordlist object:
       */
      thePrefFilename = pref_filename;
      parsed_ok = theKWL.addFile(pref_filename);

      /*!
       * Check for error opening KWL:
       */
      if (!parsed_ok)
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING: " << MODULE << ", an error was encountered loading the prefererences "
                                            << "file at \"" << thePrefFilename << "\" as specified by the "
                                            << "environment variable \"" << PREF_FILE_ENV_VAR_NAME << "\"."
                                            << "Preferences were not loaded.\n";
      }
   }

   else
   {
//       if (traceDebug())
//       {
//          // No ENV var found. Print warning:
//          CLOG << "WARNING: the preferences file environment variable \""
//               << PREF_FILE_ENV_VAR_NAME << "\" is not defined. No preferences "
//               << "were loaded. The environment variable should be set to "
//               << "the full path to the preferences keywordlist file desired."
//               << endl;
//       }
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: " << MODULE << "returning...\n";
   return parsed_ok;
}

/*!****************************************************************************
 * METHOD: loadPreferences(filename)
 *  
 *  Loads the preferences file specified in the arg.
 *  
 *****************************************************************************/
bool ossimPreferences::loadPreferences(const ossimFilename& pathname)
{
   static const char MODULE[] = "ossimPreferences::loadPreferences(filename)";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: " << MODULE << ", entering...\n";

   bool parsed_ok;

   /*!
    * First clear the existing KWL:
    */
   theKWL.clear();
   theInstanceIsModified = true;
   
   /*!
    * Load the preferences file into the static keywordlist object:
    */
   thePrefFilename = pathname;
   parsed_ok = theKWL.addFile(pathname);

   /*!
    * Check for error opening KWL:
    */
   if (!parsed_ok)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: " << MODULE << ", an error was encountered loading the prefererences "
                                         << "file at \"" << pathname << "\". Preferences were not "
                                         << "loaded.\n";
   }

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: " << MODULE<< ", returning...\n";
   return parsed_ok;
}

/*!****************************************************************************
 * METHOD: ossimPreferences::savePreferences()
 *  
 *  Saves KWL to the current filename.
 *  
 *****************************************************************************/
bool ossimPreferences::savePreferences() const
{
   static const char MODULE[] = "ossimPreferences::savePreferences()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: " << MODULE << ", entering...\n";
   
   bool success = true;
   
   /*!
    * Save the file to current preferences filename:
    */
   if (theInstanceIsModified)
   {
      theKWL.write(thePrefFilename);
      theInstanceIsModified = false;
   }

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:" << MODULE << ", returning...\n";
   return success;
}

/*!****************************************************************************
 * METHOD: ossimPreferences::savePreferences(filename)
 *  
 *  Saves KWL to the specified filename.
 *  
 *****************************************************************************/
bool ossimPreferences::savePreferences(const ossimFilename& pathname)
{
   static const char MODULE[] = "ossimPreferences::savePreferences()";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: "<< MODULE << ", entering...\n";
   
   bool success = true;
   
   /*!
    * Save the file to the specified preferences filename:
    */
   theKWL.write(pathname);

   thePrefFilename = pathname;
   theInstanceIsModified = false;
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: " << MODULE << ", returning...\n";
   return success;
}

void ossimPreferences::addPreference(const char* key, const char* value)
{
   theKWL.add(key, value, true);
   theInstanceIsModified = true;
}

void ossimPreferences::addPreferences(const ossimKeywordlist& kwl,
                                      const char* prefix,
                                      bool stripPrefix)
{
   theKWL.add(kwl, prefix, stripPrefix);
}
