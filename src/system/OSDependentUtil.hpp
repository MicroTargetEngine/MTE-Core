/*********************************************************************

          File: OSDependentUtil.hpp

   Description: OSDependentUtil Header Part.

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

#ifndef _OSDependentUtil_hpp_
#define _OSDependentUtil_hpp_

#include "SystemCheck.hpp"

#if defined(WINDOWS_SYS)
//#if !defined(SET_MARIA_DB) // MariaDB windows library 자체가 windows.h를 포함.
#include <windows.h>
//#endif
#else
#include <unistd.h>
#endif

// Millisecond sleep.
#define m_sleep __msleep_
#if defined(WINDOWS_SYS) 
	#define __msleep_(__msec) Sleep(__msec)
#else
	#define __msleep_(__msec) sleep((unsigned int)__msec)
#endif

// Microsecond sleep.
#define u_sleep __usleep_
#if defined(WINDOWS_SYS) 
#else
	#define __usleep_(__msec) usleep((unsigned int)__msec)
#endif

/*
class OSDependentUtil {
private:
public:
	void m_sleep(int __msec);
	void u_sleep(unsigned long __usec);
};
*/
#endif
