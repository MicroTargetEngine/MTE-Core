/*********************************************************************

          File: CoreModuleControl.hpp

   Description: CoreModuleControl Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/27

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

#ifndef _CoreModuleControl_hpp_
#define _CoreModuleControl_hpp_

#include "SystemCheck.hpp"

#if defined(USING_CORE_ENGINE_MODULE_DATAAUTOMATOR)
  // DataAutomator is required set of "Common-Position,
  // Common-Thread(if using pthread, Required Library-pthread, pthread is default) and Library-Boost and (Library-mysql or Library-sqlite)"
  #define SET_CORE_ENGINE_MODULE_DATAAUTOMATOR

  // Common modules.
  #ifndef USING_COMMON_MODULE_POSITION
    #define USING_COMMON_MODULE_POSITION
  #endif
  #ifndef USING_COMMON_MODULE_DATABASE
    #define USING_COMMON_MODULE_DATABASE
  #endif
  #ifndef USING_COMMON_MODULE_THREAD
    #define USING_COMMON_MODULE_THREAD
  #endif

  // Libraries.
  #ifndef USING_LIB_BOOST
    #define USING_LIB_BOOST
  #endif
  #if defined(WINDOWS_SYS) && !defined(USING_LIB_PTHREAD)
    // you are using windows.
    // you will using winthread.
  #else
    #define USING_LIB_PTHREAD
  #endif
  #if defined(USING_LIB_SQLITE) && defined(USING_LIB_MARIADB)
    // you are using sqlite & MariaDB.
  #elif !defined(USING_LIB_SQLITE)
    // DataAutomator default database using "sqlite"
    #define USING_LIB_SQLITE
  #else
    #define USING_LIB_MARIADB
  #endif

#else
  // set default mode "USING_MODE_ONLY_DETECTION"
  // if you not using "DataAutomator", you are not going extraction, matching.
  #define USING_MODE_ONLY_DETECTION
#endif // USING_CORE_ENGINE_MODULE_DATAAUTOMATOR

#if defined(USING_CORE_ENGINE_MODULE_IMAGESEARCHER)
  // FeatureSearcher is required set of "Common-Thread, Common-Time and Library-OpenCV"
  #define SET_CORE_ENGINE_MODULE_FEATURESEARCHER

  // Common modules.
  #ifndef USING_COMMON_MODULE_THREAD
    #define USING_COMMON_MODULE_THREAD
  #endif
  #ifndef USING_COMMON_MODULE_TIME
    #define USING_COMMON_MODULE_TIME
  #endif

  // Libraries.
  #ifndef USING_LIB_OPENCV
    #define USING_LIB_OPENCV
  #endif
  #if defined(WINDOWS_SYS) && !defined(USING_LIB_PTHREAD)
    // you are using windows.
    // you will using winthread.
  #else
    #define USING_LIB_PTHREAD
  #endif

#endif // USING_CORE_ENGINE_MODULE_IMAGESEARCHER

#if defined(USING_CORE_ENGINE_MODULE_LANESEARCHER)
  // LaneSearcher is required set of "Common-Thread, Common-Time and Library-OpenCV"
  #define SET_CORE_ENGINE_MODULE_LANESEARCHER

  // Common modules.
  #ifndef USING_COMMON_MODULE_THREAD
    #define USING_COMMON_MODULE_THREAD
  #endif
  #ifndef USING_COMMON_MODULE_TIME
    #define USING_COMMON_MODULE_TIME
  #endif

  // Libraries.
  #ifndef USING_LIB_OPENCV
    #define USING_LIB_OPENCV
  #endif
#endif // USING_CORE_ENGINE_MODULE_LANESEARCHER

#if defined(USING_SUB_SUPPORT_MODULE_STREAMING)
  // LaneSearcher is required set of "Common-Thread and Library-GStreamer"
  #define SET_SUB_SUPPORT_MODULE_STREAMING

  // Common modules.
  #ifndef USING_COMMON_MODULE_THREAD
    #define USING_COMMON_MODULE_THREAD
  #endif

  // Libraries.
  #ifndef USING_LIB_GSTREAMER
    #define USING_LIB_GSTREAMER
  #endif
#endif // USING_CORE_ENGINE_MODULE_STREAMING

#endif // _CoreModuleControl_hpp_
