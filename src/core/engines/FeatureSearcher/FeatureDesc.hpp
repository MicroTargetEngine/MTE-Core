/*********************************************************************

          File: FeatureDesc.hpp

   Description: FeatureDesc Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/28

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

#include "modinclude.hpp"

#if defined(SET_CORE_ENGINE_MODULE_FEATURESEARCHER)

#ifndef _FeatureDesc_hpp_
#define _FeatureDesc_hpp_

// dependent module param & mode.
#include "FeatureDescParam.hpp"
#include "FeatureDescMode.hpp"

// System Defines.
#include "SystemCommon.hpp"

// Core-Engine Defines.
#include "CoreDefines.hpp"

// Library Supports.
#include "OpenCVSupport.hpp"

typedef struct _FeatureSets {
	Mat _Descriptor;
	vector<KeyPoint> _Keypoints;
	int _Cols, _Rows;

	_FeatureSets() {
		_Cols = -1;
		_Rows = -1;
	}

	void Set_ImageLength(int Cols, int Rows) {
		_Cols = Cols;
		_Rows = Rows;
	}
} FeatureSets;

typedef struct _FastParam {
  int _Threshold;
  bool _NonmaxSuppression;
  int _Type;

  _FastParam() {
    _Threshold = 10;
    _NonmaxSuppression = true;
    _Type = FastFeatureDetector::TYPE_9_16;
  }
  IMPLEMENT_GET(int, Threshold, _Threshold)
  IMPLEMENT_SET(int, Threshold, _Threshold)
  IMPLEMENT_GET(bool, NonmaxSuppression, _NonmaxSuppression)
  IMPLEMENT_SET(bool, NonmaxSuppression, _NonmaxSuppression)
  IMPLEMENT_GET(int, Type, _Type)
  IMPLEMENT_SET(int, Type, _Type)
} FastParam;

typedef struct _ORBParam {
  int _Nfeatures;
  float _ScaleFactor;
  int _Nlevels;
  int _EdgeThreshold;
  int _FirstLevel;
  int _WTA_K;
  int _ScoreType;
  int _PatchSize;
  int _FastThreshold;

  _ORBParam() {
    _Nfeatures=500;
    _ScaleFactor=1.2f;
    _Nlevels=8;
    _EdgeThreshold=31;
    _FirstLevel=0;
    _WTA_K=2;
    _ScoreType=ORB::HARRIS_SCORE;
    _PatchSize=31;
    _FastThreshold=20;
  }
  IMPLEMENT_GET(int, Nfeatures, _Nfeatures)
  IMPLEMENT_SET(int, Nfeatures, _Nfeatures)
  IMPLEMENT_GET(float, ScaleFactor, _ScaleFactor)
  IMPLEMENT_SET(float, ScaleFactor, _ScaleFactor)
  IMPLEMENT_GET(int, Nlevels, _Nlevels)
  IMPLEMENT_SET(int, Nlevels, _Nlevels)
  IMPLEMENT_GET(int, EdgeThreshold, _EdgeThreshold)
  IMPLEMENT_SET(int, EdgeThreshold, _EdgeThreshold)
  IMPLEMENT_GET(int, FirstLevel, _FirstLevel)
  IMPLEMENT_SET(int, FirstLevel, _FirstLevel)
  IMPLEMENT_GET(int, WTA_K, _WTA_K)
  IMPLEMENT_SET(int, WTA_K, _WTA_K)
  IMPLEMENT_GET(int, ScoreType, _ScoreType)
  IMPLEMENT_SET(int, ScoreType, _ScoreType)
  IMPLEMENT_GET(int, PatchSize, _PatchSize)
  IMPLEMENT_SET(int, PatchSize, _PatchSize)
  IMPLEMENT_GET(int, FastThreshold, _FastThreshold)
  IMPLEMENT_SET(int, FastThreshold, _FastThreshold)
} ORBParam;

namespace FeatureDescEnum {
// for binary Detectors.
  enum SpicesDetector {
    S_FAST = 0, S_ORB = 1
  };

  enum SpicesMatcher {
    S_BF_L2 = 0, S_BF_L1 = 1, S_BF_H1 = 2, S_BF_H2 = 3, S_FLANN = 4
  };
}

class FeatureDesc {
private:
  int _DetectorKind;
#if !defined(MODE_ONLY_DETECTION)
  int _ExtractorKind;
  int _MatcherKind;
#endif

  FastParam _FastParameter;
  ORBParam _ORBParameter;

  Ptr<Feature2D> _Detector;
#if !defined(MODE_ONLY_DETECTION)
  Ptr<Feature2D> _Extractor;
  Ptr<DescriptorMatcher> _Matcher;
#endif
  void _Set_FAST(Ptr<FastFeatureDetector> &__FastDetector);
  void _Set_ORB(Ptr<ORB> &__ORBDetector);

  void _Initialize(int __DetectorKind
#if !defined(MODE_ONLY_DETECTION)
      , int __ExtractorKind, int __MatcherKind
#endif
      );
  void _SetDetector(int __DetectorKind);

#if !defined(MODE_ONLY_DETECTION)
  void _SetExtractor(int __ExtractorKind);
  void _SetMatcher(int __DescriptorMatcherKind);
#endif

public:
  FeatureDesc() :
      _DetectorKind(
#if defined(SET_MODE_FAST_DETECTOR)
          FeatureDescEnum::S_FAST
#elif defined(SET_MODE_ORB_DETECTOR)
          DetectorsEnum::S_ORB
#endif
      )
#if !defined(MODE_ONLY_DETECTION)
      ,
      _ExtractorKind(
#if defined(SET_MODE_FAST_DETECTOR)
          FeatureDescEnum::S_FAST
#elif defined(SET_MODE_ORB_DETECTOR)
          DetectorsEnum::S_ORB
#endif
      ),
      _MatcherKind(
#if defined(SET_MODE_BF_MATCHER)
          FeatureDescEnum::S_BF_H1
#endif
      )
#endif
  {
    _Initialize(_DetectorKind
#if !defined(MODE_ONLY_DETECTION)
        , _ExtractorKind, _MatcherKind
#endif
        );
  }

  FeatureDesc(int __DetectorKind = FeatureDescEnum::S_ORB) :
      _DetectorKind(__DetectorKind
#if !defined(MODE_ONLY_DETECTION)
      ),
      _ExtractorKind(FeatureDescEnum::S_ORB),
      _MatcherKind(
#if (MATCHER_NEIGHBORS_K < 3)
          FeatureDescEnum::S_BF_H1
#else
          DetectorsEnum::S_BF_H2
#endif
#endif
      ){}
#if !defined(MODE_ONLY_DETECTION)
  FeatureDesc(int __DetectorKind = FeatureDescEnum::S_ORB,
   int __ExtractorKind = FeatureDescEnum::S_ORB) :
      _DetectorKind(__DetectorKind),
      _ExtractorKind(__ExtractorKind),
      _MatcherKind(
#if (MATCHER_NEIGHBORS_K < 3)
          FeatureDescEnum::S_BF_H1
#else
          DetectorsEnum::S_BF_H2
#endif
      ){}

  FeatureDesc(int __DetectorKind = -1, int __ExtractorKind = -1, int __MatcherKind = -1) :
      _DetectorKind(__DetectorKind),
      _ExtractorKind(__ExtractorKind),
      _MatcherKind(__MatcherKind)
  {}
#endif
  ~FeatureDesc() {}

  IMPLEMENT_GET(FastParam, FastParameter, _FastParameter)
  IMPLEMENT_SET(FastParam, FastParameter, _FastParameter)

  IMPLEMENT_GET(ORBParam, ORBParameter, _ORBParameter)
  IMPLEMENT_SET(ORBParam, ORBParameter, _ORBParameter)

  void Go_Initialize();

  void Go_Detect(Mat &__Image, vector<KeyPoint> &__Keypoints);

#if !defined(MODE_ONLY_DETECTION)
  void Go_Extract(Mat &__Image, vector<KeyPoint> &__Keypoints, Mat &__Descriptors);
  void Go_DetectAndExtract(Mat &__Image, vector<KeyPoint> &__Keypoints, Mat &__Descriptors);
  void Go_Matching(Mat &__Object, Mat &__Scene,
#if defined(SET_MODE_FLANN_MATCHER)
      Mat &__Results, Mat &__Dists,
#endif
#if defined(SET_MODE_NORMAL_METHOD)
      vector<DMatch>
#elif defined(SET_MODE_KNN_METHOD)
                   vector<vector<DMatch> >
#endif
  &__Matches);

  bool Find_Matchs(FeatureSets &Object, FeatureSets &Scene, int &Scene_X, int &Scene_Y);
#endif
};

#endif

#endif
/*
class ImageDetector {
private:
#if defined(SET_MODE_FAST_DETECTOR)
	int _DetectorThreshold;
#endif
#if defined(SET_MODE_BRIEF_EXTRACTOR)
	int _ExtractorThreshold;
#endif
	double _NNDRatio;

#if defined(SET_MODE_FAST_DETECTOR)
	FastFeatureDetector
#elif defined(SET_MODE_ORB_DETECTOR)
	OrbFeatureDetector
#endif
		_Detector;

#if defined(SET_MODE_ORB_EXTRACTOR)
	ORB
#endif
		_Extractor;

public:
	ImageDetector() :
#if defined(SET_MODE_FAST_DETECTOR)
		_DetectorThreshold(DETECTOR_THRESHOLD),
#endif
		_NNDRatio(NNR_RATIO) {}

	ImageDetector(
#if defined(SET_MODE_FAST_DETECTOR)
		int DetectorThreshold = DETECTOR_THRESHOLD,
#endif
		double NNDRatio = NNR_RATIO);
	FeatureDetectoror();

	void Go_Detect(Mat &__Image, vector<KeyPoint> &__Keypoints);
	void Go_Extract(Mat &__Image, vector<KeyPoint> &__Keypoints, Mat &__Descriptors);
	void Go_DetectAndExtract(Mat &__Image, vector<KeyPoint> &__Keypoints, Mat &__Descriptors);
	void Go_Matching(Mat &__Object, Mat &__Scene,
#if defined(SET_MODE_FLANN_MATCHER)
		Mat &__Results, Mat &__Dists,
#endif
#if defined(SET_MODE_NORMAL_METHOD)
		vector<DMatch>
#elif defined(SET_MODE_KNN_METHOD)
		vector<vector<DMatch> >
#endif
		&__Matches);

	bool Find_Matchs(FeatureSets &Object, FeatureSets &Scene, int &Scene_X, int &Scene_Y);
};
*/