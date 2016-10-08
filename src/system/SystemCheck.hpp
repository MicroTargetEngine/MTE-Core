/*********************************************************************

          File: SystemDependency.hpp

   Description: SystemDependency Header Part.

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

#ifndef _SystemDependency_hpp_
#define _SystemDependency_hpp_

/*
	Compile 시어느 정도의 자동 판별을 가능케 하는 "System Dependent"한
	Compile 구역 지정 구문.
 	DO NOT TOUCH THIS CODE!!
*/

/* System 및 Compiler 환경 구분 관련 */
// Windows인지, POSIX인지.
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32)
// using windows.
#define WINDOWS_SYS
#	ifdef __MINGW32__
// using MinGW(for windows).
#	define MINGW_USING
#		if defined(__GNUC__)
#		define WIN_QT_USING
#		define USING_QT
#		endif
#	endif
// Qt mingw g++ mkspecs using Always.
#else
// using Linux, FreeBSD, Mac OSX.
#	define POSIX_SYS
// 추후 수정
#	if defined(__GNUC__) && (defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_MAC) || defined(Q_OS_UNIX) \
		|| defined(Q_OS_CYGWIN) || defined(Q_OS_DARWIN))
#	define POSIX_QT_USING
#	define USING_QT
#	endif
#endif

// QT를 이용하는지 하지 않는지.
#if defined(USING_QT)
#define MAKE_ARTFAO
#endif

/* Debug Mode 관련 */
// Debug Mode인지, 아닌지.
// If defined Debug mode by compiler(M$ Visual Studio & Qt Creator).
#if defined(_DEBUG) || !defined(QT_NO_DEBUG)
#define DEBUG_MODE
#endif

/* Test Mode 관련 */
// Test Build를 할 것인지 아닌지.
#if defined(MAKE_TEST)
#define TEST_MODE
#endif

#if defined(MAKE_LOG_WRITE)
#define LOG_WRITE_MODE
#endif
#endif
