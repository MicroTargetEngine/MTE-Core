/*********************************************************************

          File: CoreDependency.hpp

   Description: CoreDependency Header Part.

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

#ifndef _CoreDependency_hpp_
#define _CoreDependency_hpp_

#define CLIENT_MOBILE_ALL_MAX_THREAD 64 // Mobile 상에서의 최대 Thread 갯수.
#define CLIENT_IMAGE_SEARCHER_MAXIMUM_THREAD 4 // Thread의 최대 갯수.
//#define CLIENT_MAXIMUM_WORKNUMBER 4 // 일감의 최대 수.
#define CLIENT_IMAGE_SEARCHER_QUEUE_LIMIT 24 // Image Searcher의 Queue 한계 값.
#define CLIENT_DATA_QUEUE_LIMIT 60 // Data Queue의 한계 값.

#define CLIENT_MINIMUM_DATA_COUNT 5

#define CLIENT_DATA_MEANING_SECOND 2.0 // Data의 유의미 지속 시간.
#define CLIENT_STOP_LIMIT_SECOND 2.0 // Callback이 멈춰 있는 한계 시간.
#define CLIENT_DATA_VOID_LIMIT_COUNT 100 // Data가 비어져 있는 한계 Count.

//#include "SystemDependency.hpp"
#endif