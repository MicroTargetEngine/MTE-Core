/*********************************************************************

          File: LibrariesControl.hpp

   Description: LibrariesControl Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

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

#ifndef _LibrariesControl_hpp_
#define _LibrariesControl_hpp_

#include "CommonModuleControl.hpp"

/* 외부 Library 관련 */
/*
** 외부 Library 목록.
1. pthread(Must required)
2. MariaDB(Client : Don't Use, Server : Must required)
3. SQLite(Must required)
6. Boost(Must required)
7. OpenCV(Must required)
*/

// 1. pthread(Must required)
#if defined(USING_LIB_PTHREAD)
#define SET_LIB_PTHREAD
#endif

// 2. MariaDB(Client : Don't Use, Server : Must required)
#if defined(USING_LIB_MARIADB) // Maria DB를 쓸 것인지 아닌지.
#define SET_LIB_MARIADB
#endif

// 3. SQLite(Must required)
#if defined(USING_LIB_SQLITE) // SQLite를 쓸 것인지 아닌지.
#define SET_LIB_SQLITE
#endif

// 4. Boost(Must required)
#if defined(USING_LIB_BOOST)
#define SET_LIB_BOOST
#endif

// 5. OpenCV(Must required)
#if defined(USING_LIB_OPENCV)
#define SET_LIB_OPENCV
#endif

// 6. GStreamer(Must required)
#if defined(USING_LIB_GSTREAMER)
#define SET_LIB_GSTREAMER
#endif

#endif