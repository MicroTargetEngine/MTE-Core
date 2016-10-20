/*********************************************************************

          File: SystemDefines.hpp

   Description: SystemDefines Header Part.

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

#ifndef _SystemDefines_hpp_
#define _SystemDefines_hpp_

#include "SystemCheck.hpp"

// 추후에 Test로 Compile시 Process Check할 때 유용.
#if defined(WINDOWS_SYS)
  #if defined(USING_QT)
    #define EXEC_FILENAME art.exe
  #elif defined(TEST_MODE)
    // for Visual Studio.
    #define EXEC_FILENAME art_test.exe
  #endif
#elif defined(POSIX_SYS)
  #if defined(USING_QT)
    #define EXEC_FILENAME art
  #endif
#endif

#define ENGINE_EXEC_VER "0.1.0"

#define IP_ADDR_LOCAL "127.0.0.1"
#define ART_TCP_PORT 25413
#define TCP_LISTEN_QUEUE 200

#define TCP_MAX_CONNECTIONS TCP_LISTEN_QUEUE
#define POLL_MAX_CONNECTIONS (TCP_MAX_CONNECTIONS+1)

#endif
