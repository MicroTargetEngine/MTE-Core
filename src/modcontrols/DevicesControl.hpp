/*********************************************************************

          File: DevicesControl.hpp

   Description: DevicesControl Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/28

 Copyright (c) 2016, Team "FireBase", Open Robot Marathon, IRC.

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

*********************************************************************/

#ifndef _DevicesControl_hpp_
#define _DevicesControl_hpp_

#include "CommonModuleControl.hpp"

/* 장치 관련 */
/*
** 장치 목록
1. mmal(Must requied for Embedded Target)
2. serial(Must requied for Embedded Target)
 */
// 1. mmal(Must requied for Embedded Target)
#if defined(SET_TARGET_ARM)
  #if defined(USING_DEVICE_MMAL)
  #define SET_DEVICE_MMAL
  #endif
#endif

// 2. serial(Must requied for Embedded Target)
#if defined(USING_DEVICE_SERIAL)
#define SET_DEVICE_SERIAL
#endif

#endif
