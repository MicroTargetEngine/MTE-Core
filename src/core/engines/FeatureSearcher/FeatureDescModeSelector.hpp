/*********************************************************************

          File: ImageSearcherModuleDependency.hpp

   Description: ImageSearcherModuleDependency Header Part.

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

#include "CoreModuleControl.hpp"

#ifndef _ImageSearcherModeSelector_hpp_
#define _ImageSearcherModeSelector_hpp_

/*************************************************************************/
/*    Combinations of Feature Detector & Extractor & Matcher             */
/*    Sorted & Tested By Doohoon Kim                                     */
/*************************************************************************/
/*  ( * : Now Using, + : Testing Candidate, ({Number}) : Testing Rate )  */
/*************************************************************************/
/*     Detector | Extractor |  Matcher  |  Method  | Enable? | Using     */
/*************************************************************************/
/* 1.    FAST   |  ORB      | BFMatcher |  NORM    | YES     |  +(4)     */
/* 2.    FAST   |  ORB      | FLANN     |  NORM    | NO      |  +(5)     */
/* 3.    FAST   |  ORB      | BFMatcher |  KNN     | YES     |  +(2)     */
/* 4.    FAST   |  ORB      | FLANN     |  KNN     | YES     |  +(3)     */
/* 5.    FAST   |  BRIEF    | BFMatcher |  NORM    | YES     |  +(1)     */
/* 6.    FAST   |  BRIEF    | FLANN     |  NORM    | NO      |           */
/* 7.    FAST   |  BRIEF    | BFMatcher |  KNN     | YES     |  *        */
/* 8.    FAST   |  BRIEF    | FLANN     |  KNN     | YES     |           */
/* 9.    FAST   |  FREAK    | BFMatcher |  NORM    | YES     |           */
/* 10.   FAST   |  FREAK    | FLANN     |  NORM    | NO      |           */
/* 11.   FAST   |  FREAK    | BFMatcher |  KNN     | YES     |           */
/* 12.   FAST   |  FREAK    | FLANN     |  KNN     | YES     |           */
/* 13.   ORB    |  ORB      | BFMatcher |  NORM    | YES     |           */
/* 14.   ORB    |  ORB      | FLANN     |  NORM    | NO      |           */
/* 15.   ORB    |  ORB      | BFMatcher |  KNN     | YES     |           */
/* 16.   ORB    |  ORB      | FLANN     |  KNN     | YES     |           */
/* 17.   ORB    |  BRIEF    | BFMatcher |  NORM    | YES     |           */
/* 18.   ORB    |  BRIEF    | FLANN     |  NORM    | NO      |           */
/* 19.   ORB    |  BRIEF    | BFMatcher |  KNN     | YES     |           */
/* 20.   ORB    |  BRIEF    | FLANN     |  KNN     | YES     |           */
/*************************************************************************/

// Feature Detection Modes.
#if defined(USING_MODE_FAST_DETECTOR)
  #define MODE_FAST_DETECTOR
#elif defined(USING_MODE_ORB_DETECTOR)
  #define MODE_ORB_DETECTOR // nop
#endif

// this setting is only detection mode.
// It's enabling if you not set "DataAutomator"
#if defined(USING_MODE_ONLY_DETECTION)
#define MODE_ONLY_DETECTION
#endif

#if !defined(MODE_ONLY_DETECTION)
  #if defined(USING_MODE_ORB_EXTRACTOR)
    // FeatureDesc Module is default extractor of "ORB"
    #define MODE_ORB_EXTRACTOR
  #endif

  #if defined(USING_MODE_NORMAL_METHOD)
    #define MODE_NORMAL_METHOD // nop
  #elif defined(USING_MODE_KNN_METHOD)
    #define MODE_KNN_METHOD
  #endif

  #if defined(USING_MODE_BF_MATCHER)
    #define MODE_BF_MATCHER
  #elif defined(USING_MODE_FLANN_MATCHER)
    #define MODE_FLANN_MATCHER // nop
  #endif
#endif

#endif // _ImageSearcherModeSelector_hpp_
