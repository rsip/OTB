//*****************************************************************************
// FILE: ossimLsrRay.cc
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
//  Class for representing rays in some local space rectangular (LSR)
//  coordinate system. This coordinate system is related to the ECEF system
//  by the ossimLsrSpace member object. This class simplifies coordinate
//  conversions between LSR and ECEF, and other LSR spaces.
//
//  An LSR ray is defined as having an LSR origin point and an LSR unit
//  direction vector radiating from the origin.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Log: ossimLsrRay.cpp,v $
//  Revision 1.6  2004/04/08 15:25:30  gpotts
//  *** empty log message ***
//
//  Revision 1.5  2004/04/08 14:12:39  gpotts
//  Fixe clog to ossimNotify
//
//  Revision 1.4  2002/10/21 21:51:46  gpotts
//  *** empty log message ***
//
//  Revision 1.3  2002/10/21 21:51:26  gpotts
//  *** empty log message ***
//
//  Revision 1.2  2001/12/20 22:26:41  bpayne
//  Added constructor accepting two ossimLsrPoint args
//
//  Revision 1.1  2001/08/13 21:29:18  okramer
//  Initial delivery of ECEF and LSR suite. (okramer@imagelinks.com)
//

#include "ossimLsrRay.h"

//*****************************************************************************
//  CONSTRUCTOR:    ossimLsrRay(ossimLsrPoint, ossimLsrVector)
//  
//  Constructs by transforming the given ray into the new space.
//  
//*****************************************************************************
ossimLsrRay::ossimLsrRay(const ossimLsrPoint&  origin,
                         const ossimLsrVector& direction)
   : theOrigin(origin),
     theDirection(direction.unitVector())
{
   if (origin.lsrSpace() != direction.lsrSpace())
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL -- ossimLsrRay(ossimLsrPoint,ossimLsrVector) Constructor:"
                                          << "\n   The origin and direction LSR quantities do not share the"
                                          << "\n   same LSR space. Setting to NAN. Check the data for errors." << std::endl;

      theOrigin    = ossimLsrPoint (OSSIM_NAN, OSSIM_NAN, OSSIM_NAN,
                                    origin.lsrSpace());
      theDirection = ossimLsrVector(OSSIM_NAN, OSSIM_NAN, OSSIM_NAN,
                                    direction.lsrSpace());
   }    
}

//*****************************************************************************
//  CONSTRUCTOR:    ossimLsrRay(ossimLsrPoint, ossimLsrPoint)
//  
//  Constructs by transforming the given ray into the new space.
//  
//*****************************************************************************
ossimLsrRay::ossimLsrRay(const ossimLsrPoint&  origin,
                         const ossimLsrPoint&  towards)
   : theOrigin(origin)
{
   if (origin.lsrSpace() != towards.lsrSpace())
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "ERROR -- ossimLsrRay(ossimLsrPoint,ossimLsrPoint) Constructor:"
                                          << "\n   The origin and direction LSR quantities do not share the"
                                          << "\n   same LSR space. Setting to NAN. Check the data for errors." << std::endl;
      
      theOrigin    = ossimLsrPoint (OSSIM_NAN, OSSIM_NAN, OSSIM_NAN,
                                    origin.lsrSpace());
      theDirection = ossimLsrVector(OSSIM_NAN, OSSIM_NAN, OSSIM_NAN,
                                    towards.lsrSpace());
   }

   else
   {
      theDirection = towards - origin;
      theDirection.normalize();
   }
}
