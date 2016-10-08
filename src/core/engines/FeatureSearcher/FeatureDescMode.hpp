/*********************************************************************

          File: DetectionMode.hpp

   Description: DetectionMode Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18 13:34

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

#ifndef _DetectionMode_hpp_
#define _DetectionMode_hpp_

#include "FeatureDescModeSelector.hpp"

// Detection
#if defined(MODE_FAST_DETECTOR)
#define SET_MODE_FAST_DETECTOR
#endif

#if defined(MODE_ORB_DETECTOR)
#define SET_MODE_ORB_DETECTOR
#endif

// Extractor
#if defined(MODE_ORB_EXTRACTOR)
#define SET_MODE_ORB_EXTRACTOR
#endif

#if defined(MODE_BRIEF_EXTRACTOR)
#define SET_MODE_BRIEF_EXTRACTOR
#endif

#if defined(MODE_FREAK_EXTRACTOR)
#define SET_MODE_FREAK_EXTRACTOR
#endif

// Matcher
#if defined(MODE_NORMAL_METHOD)
#define SET_MODE_NORMAL_METHOD
#endif

#if defined(MODE_KNN_METHOD)
#define SET_MODE_KNN_METHOD
#endif

#if defined(MODE_BF_MATCHER)
#define SET_MODE_BF_MATCHER
#elif defined(MODE_FLANN_MATCHER)
#define SET_MODE_FLANN_MATCHER
#endif

#if defined(MODE_FLANN_MATCHER)
#define SET_MODE_FLANN_MATCHER
#endif

#endif