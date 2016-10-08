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

#ifndef _ImageSearcherModuleDependency_hpp_
#define _ImageSearcherModuleDependency_hpp_

// Feature Detection Parameter
/* ** for Binary Description Detector & Extractor ** */
#define DETECTOR_THRESHOLD 50
#define EXTRACTOR_THRESHOLD 32

#define MATCHER_NEIGHBORS_K 2

/* ** for Matcher ** */
#define NNR_RATIO 0.8

// 오브젝트 키포인트 대 매칭 포인트
// 실험에 의하면, detecting 된 keypoint와 scene과 matching된 key 값의 비율이 거의 80%를 넘어선다!!
// 그러니까, 80%를 넘어선 object는 scene상에 해당 object가 존재한다고 봐도 된다.
// 이건 해당 비율을 조정하여 다중으로 매칭 가능하게 할 수도 있다.
// 실제 다중 Matching이 성공 하였다(5개 까지!!).
// 단, 이것의 전제 조건은 Matching 할 때 knnmatch, knnsearch를 써서 matching 할 때이다.
#define OBJKEY_N_MATCHPOINT_RATIO 0.45 // 약 80% -> 70%까지 변경 가능할듯.

// 매칭이 이루어 진 후, 이 매칭이 제대로 이루어 졌는지를 판단 가능한 키값의 최소값.
// 이 값은, Scene안에 Object와의 특징점 매칭 갯수가 얼마나 있는지를 나타내는 최소의 값이다.
// 이 이하로 가면, 제대로 매칭이 안되었거나 혹은 다른 사진이다.
#define MINIMUM_INLINERS 30

/* ** for Homography ** */
#define RANSAC_REPROJECTIONTHRESHOLD 1.0 // default 3.0

#endif