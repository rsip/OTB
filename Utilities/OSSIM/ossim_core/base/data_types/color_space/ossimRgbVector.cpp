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
// Author: Garrett Potts (gpotts@remotesensing.org)
// Description:
//
//*************************************************************************
// $Id: ossimRgbVector.cpp,v 1.7 2005/09/08 13:35:22 gpotts Exp $
#include "ossimRgbVector.h"
#include "ossimJpegYCbCrVector.h"
#include "ossimHsiVector.h"
#include "ossimHsvVector.h"
#include "ossimCmyVector.h"
#include "base/common/ossimCommon.h"
#include <math.h>

ossimRgbVector::ossimRgbVector(const ossimJpegYCbCrVector& YCbCr)
{
   theBuf[0] = static_cast<unsigned char>(clamp(irint(YCbCr.getY() +
                                                      1.402 * (YCbCr.getCr()-128.0))));
   theBuf[1] = static_cast<unsigned char>(clamp(irint(YCbCr.getY() -
                                                      0.34414 *(YCbCr.getCb()-128.0) -
                                                      0.71414*(YCbCr.getCr()-128.0))));
   theBuf[2] = static_cast<unsigned char>(clamp(irint(YCbCr.getY() +
                                                      1.772 * ( YCbCr.getCb()-128.0))));
   
}

ossimRgbVector::ossimRgbVector(const ossimHsiVector& hsi)
{
   float h = hsi.getH();
   float s = hsi.getS();
   float i = hsi.getI();

   float r=0;
   float g=0;
   float b=0;
   
   if(h <= 120.0)
   {
      b = i*(1-s);

      r = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
      g = 3*i - (r+b);
   }
    else if(h <= 240.0)
    {
       h-=120;

       r = i*(1-s);
       g = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
       b = 3*i - (r+g);
    }
    else if(h <= 360.0)
    {
       h-=240;

       g = i*(1-s);
       b = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
       r = 3*i - (g+b);      
    }
   
   theBuf[0] = clamp((long)(r*255));
   theBuf[1] = clamp((long)(g*255));
   theBuf[2] = clamp((long)(b*255));
}

ossimRgbVector::ossimRgbVector(const ossimHsvVector& hsv)
{
   // H is given on [0, 6] or UNDEFINED. S and V are given on [0, 1]. 
      // RGB are each returned on [0, 1]. 
   float h = hsv.getH(), // unnormalize it
         s = hsv.getS(),
         v = hsv.getV();
   float m, n, f; 
   int i; 
   if(h == ossimHsvVector::OSSIM_HSV_UNDEFINED)
   {
      theBuf[0] = clamp(irint(v*255));
      theBuf[1] = clamp(irint(v*255));
      theBuf[2] = clamp(irint(v*255));
   }
   else
   {
      h*=6.0; // unnormalize h
      i = (int)floor(h); 
      f = h - i; 
      if(!(i & 1)) f = 1 - f; // if i is even 
      m = v * (1 - s); 
      n = v * (1 - s * f); 
      switch (i)
      { 
      case 6: 
      case 0:
      {
         theBuf[0] = clamp(irint(v*255));
         theBuf[1] = clamp(irint(n*255));
         theBuf[2] = clamp(irint(m*255));
         break;
      }
      case 1:
      {
         theBuf[0] = clamp(irint(n*255));
         theBuf[1] = clamp(irint(v*255));
         theBuf[2] = clamp(irint(m*255));
         break;
      }
      case 2:
      {
         theBuf[0] = clamp(irint(m*255));
         theBuf[1] = clamp(irint(v*255));
         theBuf[2] = clamp(irint(n*255));
         break;
      }
      case 3: 
      {
         theBuf[0] = clamp(irint(m*255));
         theBuf[1] = clamp(irint(n*255));
         theBuf[2] = clamp(irint(v*255));
         break;
      }
      case 4:
      {
         theBuf[0] = clamp(irint(n*255));
         theBuf[1] = clamp(irint(m*255));
         theBuf[2] = clamp(irint(v*255));
         break;
      }
      case 5: 
      {
         theBuf[0] = clamp(irint(v*255));
         theBuf[1] = clamp(irint(m*255));
         theBuf[2] = clamp(irint(n*255));
         break;
      }
      }
   }
}

ossimRgbVector::ossimRgbVector(const ossimCmyVector& cmy)
{
   theBuf[0] = 255 - cmy.getC();
   theBuf[1] = 255 - cmy.getM();
   theBuf[2] = 255 - cmy.getY();
}

const ossimRgbVector& ossimRgbVector::operator =(const ossimHsvVector& hsv)
{
   // H is given on [0, 6] or UNDEFINED. S and V are given on [0, 1]. 
      // RGB are each returned on [0, 1]. 
   float h = hsv.getH(), // unnormalize it
         s = hsv.getS(),
         v = hsv.getV();
   float m, n, f; 
   int i; 
   if(h == ossimHsvVector::OSSIM_HSV_UNDEFINED)
   {
      theBuf[0] = clamp(irint(v*255));
      theBuf[1] = clamp(irint(v*255));
      theBuf[2] = clamp(irint(v*255));
   }
   else
   {
      h*=6.0; // unnormalize h
      i = (int)floor(h); 
      f = h - i; 
      if(!(i & 1)) f = 1 - f; // if i is even 
      m = v * (1 - s); 
      n = v * (1 - s * f); 
      switch (i)
      { 
      case 6: 
      case 0:
      {
         theBuf[0] = clamp(irint(v*255));
         theBuf[1] = clamp(irint(n*255));
         theBuf[2] = clamp(irint(m*255));
         break;
      }
      case 1:
      {
         theBuf[0] = clamp(irint(n*255));
         theBuf[1] = clamp(irint(v*255));
         theBuf[2] = clamp(irint(m*255));
         break;
      }
      case 2:
      {
         theBuf[0] = clamp(irint(m*255));
         theBuf[1] = clamp(irint(v*255));
         theBuf[2] = clamp(irint(n*255));
         break;
      }
      case 3: 
      {
         theBuf[0] = clamp(irint(m*255));
         theBuf[1] = clamp(irint(n*255));
         theBuf[2] = clamp(irint(v*255));
         break;
      }
      case 4:
      {
         theBuf[0] = clamp(irint(n*255));
         theBuf[1] = clamp(irint(m*255));
         theBuf[2] = clamp(irint(v*255));
         break;
      }
      case 5: 
      {
         theBuf[0] = clamp(irint(v*255));
         theBuf[1] = clamp(irint(m*255));
         theBuf[2] = clamp(irint(n*255));
         break;
      }
      }
   }
   return *this;
}

const ossimRgbVector& ossimRgbVector::operator =(const ossimJpegYCbCrVector& YCbCr)
{
   theBuf[0] = static_cast<unsigned char>(clamp(irint(YCbCr.getY() +
                                                      1.402 * (YCbCr.getCr()-128.0))));
   theBuf[1] = static_cast<unsigned char>(clamp(irint(YCbCr.getY() -
                                                      0.34414 *(YCbCr.getCb()-128.0) -
                                                      0.71414*(YCbCr.getCr()-128.0))));
   theBuf[2] = static_cast<unsigned char>(clamp(irint(YCbCr.getY() +
                                                      1.772 * ( YCbCr.getCb()-128.0))));

   return *this;
}

const ossimRgbVector& ossimRgbVector::operator =(const ossimHsiVector& hsi)
{
   float h = hsi.getH();
   float s = hsi.getS();
   float i = hsi.getI();

   float r=0;
   float g=0;
   float b=0;
   
   if(h <= 120.0)
   {
      b = i*(1-s);

      r = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
      g = 3*i - (r+b);
   }
    else if(h <= 240.0)
    {
       h-=120;

       r = i*(1-s);
       g = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
       b = 3*i - (r+g);
    }
    else if(h <= 360.0)
    {
       h-=240;

       g = i*(1-s);
       b = i*(1 + s*cos(RAD_PER_DEG*h)/cos((60-h)*RAD_PER_DEG));
       r = 3*i - (g+b);      
    }
   
   theBuf[0] = clamp((long)(r*255));
   theBuf[1] = clamp((long)(g*255));
   theBuf[2] = clamp((long)(b*255));
   
   return *this;
}

const ossimRgbVector& ossimRgbVector::operator =(const ossimCmyVector& cmy)
{
   theBuf[0] = 255 - cmy.getC();
   theBuf[1] = 255 - cmy.getM();
   theBuf[2] = 255 - cmy.getY();

   return *this;
}
