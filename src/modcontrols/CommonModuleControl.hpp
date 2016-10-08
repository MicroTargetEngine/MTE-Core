/*********************************************************************

          File: CommonModuleControl.hpp

   Description: CommonModuleControl Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/29

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

#ifndef _CommonModuleControl_hpp_
#define _CommonModuleControl_hpp_

#include "CoreModuleControl.hpp"
#include "TargetControl.hpp"

#if defined(USING_COMMON_MODULE_DATABASE)
  #define SET_COMMON_MODULE_DATABASE
#endif

#if defined(USING_COMMON_MODULE_ETHERNET)
  #define SET_COMMON_MODULE_ETHERNET
#endif

#if defined(USING_COMMON_MODULE_HASH)
#define SET_COMMON_MODULE_HASH
#endif

#if defined(USING_COMMON_MODULE_I18N)
  #define SET_COMMON_MODULE_I18N
#endif

#if defined(USING_COMMON_MODULE_LOG)
  #define SET_COMMON_MODULE_LOG
#endif

#if defined(USING_COMMON_MODULE_MATHEMATICAL)
  #define SET_COMMON_MODULE_MATHEMATICAL
#endif

#if defined(USING_COMMON_MODULE_POSITION)
  #define SET_COMMON_MODULE_POSITION
#endif

#if defined(USING_COMMON_MODULE_RANDOM)
#define SET_COMMON_MODULE_RANDOM
#endif

#if defined(USING_COMMON_MODULE_STRINGTOOLS)
  #define SET_COMMON_MODULE_STRINGTOOLS
#endif

#if defined(USING_COMMON_MODULE_THREAD)
  #define SET_COMMON_MODULE_THREAD
#endif

#if defined(USING_COMMON_MODULE_TIME)
  #define SET_COMMON_MODULE_TIME
#endif

#if defined(USING_COMMON_MODULE_TYPE)
  #define SET_COMMON_MODULE_TYPE
#endif

#endif
