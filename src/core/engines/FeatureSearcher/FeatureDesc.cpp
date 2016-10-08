/*********************************************************************

          File: ImageDetector.cpp

   Description: ImageDetector Source Part.

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

#include "FeatureDesc.hpp"

#if defined(SET_CORE_ENGINE_MODULE_FEATURESEARCHER)

#if defined(LOG_WRITE_MODE)
#include "LogD.hpp"
#endif

void FeatureDesc::_Set_FAST(Ptr<FastFeatureDetector> &__FastDetector) {
  __FastDetector->setThreshold(_FastParameter._Threshold);
  __FastDetector->setNonmaxSuppression(_FastParameter._NonmaxSuppression);
  __FastDetector->setType(_FastParameter._Type);
}

void FeatureDesc::_Set_ORB(Ptr<ORB> &__ORBDetector) {
  __ORBDetector->setMaxFeatures(_ORBParameter._Nfeatures);
  __ORBDetector->setScaleFactor(_ORBParameter._ScaleFactor);
  __ORBDetector->setNLevels(_ORBParameter._Nlevels);
  __ORBDetector->setEdgeThreshold(_ORBParameter._EdgeThreshold);
  __ORBDetector->setFirstLevel(_ORBParameter._FirstLevel);
  __ORBDetector->setWTA_K(_ORBParameter._WTA_K);
  __ORBDetector->setScoreType(_ORBParameter._ScoreType);
  __ORBDetector->setPatchSize(_ORBParameter._PatchSize);
  __ORBDetector->setFastThreshold(_ORBParameter._FastThreshold);
}

void FeatureDesc::_Initialize(int __DetectorKind
#if !defined(MODE_ONLY_DETECTION)
    , int __ExtractorKind, int __MatcherKind
#endif
) {
  _SetDetector(__DetectorKind);
#if !defined(MODE_ONLY_DETECTION)
  _SetExtractor(__ExtractorKind);
  _SetMatcher(__MatcherKind);
#endif
}

void FeatureDesc::_SetDetector(int __DetectorKind) {
  if(__DetectorKind == FeatureDescEnum::S_FAST) {
    Ptr<FastFeatureDetector> _FastFeatureDetector = FastFeatureDetector::create();
    _Set_FAST(_FastFeatureDetector);
    _Detector = _FastFeatureDetector;
  }
  else if(__DetectorKind == FeatureDescEnum::S_ORB) {
    Ptr<ORB> _ORB = ORB::create();
    _Set_ORB(_ORB);
    _Detector = _ORB;
  }
/*  switch(__DetectorKind) {
    case DetectorsEnum::S_FAST :
      Ptr<FastFeatureDetector> _FastFeatureDetector = FastFeatureDetector::create();
      _Set_FAST(_FastFeatureDetector);
      _Detector = _FastFeatureDetector;
      break;
    case DetectorsEnum::S_ORB :
      Ptr<ORB> _ORB = ORB::create();
      _Set_ORB(_ORB);
      _Detector = _ORB;
      break;
    default:
      // not initialize.
      break;
  }*/
}

#if !defined(MODE_ONLY_DETECTION)
void FeatureDesc::_SetExtractor(int __ExtractorKind) {
  if(__ExtractorKind == FeatureDescEnum::S_FAST) {
    Ptr<FastFeatureDetector> _FastFeatureDetector = FastFeatureDetector::create();
    _Set_FAST(_FastFeatureDetector);
    _Detector = _FastFeatureDetector;
  }
  else if(__ExtractorKind == FeatureDescEnum::S_ORB) {
    Ptr<ORB> _ORB = ORB::create();
    _Set_ORB(_ORB);
    _Detector = _ORB;
  }
  /*
  switch(__ExtractorKind) {
    case DetectorsEnum::S_FAST :
      Ptr<FastFeatureDetector> _FastFeatureDetector = FastFeatureDetector::create();
      _Set_FAST(_FastFeatureDetector);
      _Extractor = _FastFeatureDetector;
      break;
    case DetectorsEnum::S_ORB :
      Ptr<ORB> _ORB = ORB::create();
      _Set_ORB(_ORB);
      _Extractor =_ORB;
      break;
    default:
      // not initialize.
      break;
  }*/
}

void FeatureDesc::_SetMatcher(int __DescriptorMatcherKind) {
  string _TMatcherStr = "";
  switch(__DescriptorMatcherKind) {
#if defined(SET_MODE_BF_MATCHER)
#if defined(SET_MODE_SIFT) || defined(SET_MODE_SURF)
    case DetectorsEnum::S_BF_L2: // use SIFT, SURF
      _TMatcherStr = "BruteForce";
      break;
    case DetectorsEnum::S_BF_L1: // use SIFT, SURF
      _TMatcherStr = "BruteForce-L1";
      break;
#elif defined(SET_MODE_BRIEF_EXTRACTOR) || defined(SET_MODE_FREAK_EXTRACTOR) || (defined(SET_MODE_ORB_EXTRACTOR) && (MATCHER_NEIGHBORS_K < 3))
    case FeatureDescEnum::S_BF_H1 : // use ORB, BRISK, BRIEF,
      _TMatcherStr = "BruteForce-Hamming";
      break;
#elif defined(SET_MODE_ORB_EXTRACTOR)
    case DetectorsEnum::S_BF_H2 : // use ORB(if use, WTA_K=3 or 4)
      _TMatcherStr = "BruteForce-Hamming(2)";
      break;
#endif
#elif defined(SET_MODE_FLANN_MATCHER)
    case DetectorsEnum::S_FLANN :
      _TMatcherStr = "FlannBased";
      break;
#endif
    default:
      // not initialize.
      break;
  }
  _Matcher = DescriptorMatcher::create(_TMatcherStr);
}
#endif

void FeatureDesc::Go_Initialize() {
#if defined(LOG_WRITE_MODE)
  //G_LogD->Logging("Func", "into Initialize Function");
#endif
  _Initialize(_DetectorKind
#if !defined(USING_MODE_ONLY_DETECTION)
      , _ExtractorKind, _MatcherKind
#endif
      );
}

void FeatureDesc::Go_Detect(Mat &__Image, vector<KeyPoint> &__Keypoints) {
#if defined(LOG_WRITE_MODE)
  //G_LogD->Logging("Func", "into Go_Detect Function");
#endif
  // this threshold function made when OpenCV Version 3.1.0
  _Detector->detect(__Image, __Keypoints);
}

#if !defined(MODE_ONLY_DETECTION)
void FeatureDesc::Go_Extract(Mat &__Image, vector<KeyPoint> &__Keypoints, Mat &__Descriptors) {
#if defined(LOG_WRITE_MODE)
  //G_LogD->Logging("Func", "into Go_Extract Function");
#endif
  _Extractor->compute(__Image, __Keypoints, __Descriptors);
}

void FeatureDesc::Go_DetectAndExtract(Mat &__Image, vector<KeyPoint> &__Keypoints, Mat &__Descriptors) {
#if defined(LOG_WRITE_MODE)
  //G_LogD->Logging("Func", "into Go_DetectAndExtract Function");
#endif
  Go_Detect(__Image, __Keypoints);
  Go_Extract(__Image, __Keypoints, __Descriptors);
}

void FeatureDesc::Go_Matching(Mat &__Object, Mat &__Scene,
#if defined(SET_MODE_FLANN_MATCHER)
    Mat &__Results, Mat &__Dists,
#endif
#if defined(SET_MODE_NORMAL_METHOD)
    vector<DMatch>
#elif defined(SET_MODE_KNN_METHOD)
    vector<vector<DMatch> >
#endif
&__Matches) {
  int _TObjMatSize = (int)(__Object.cols * __Object.rows * __Object.elemSize());
  int _TSceneMatSize = (int)(__Scene.cols * __Scene.rows * __Scene.elemSize());
#if defined(LOG_WRITE_MODE)
  //G_LogD->Logging("Func", "into Go_Matching Function");
	//G_LogD->Logging("Func", "Mem1 Size : %d, Mem2 Size : %d", (int)_TObjMatSize, (int)_TSceneMatSize);
#endif
#if defined(SET_MODE_KNN_METHOD)
  int _TK = MATCHER_NEIGHBORS_K;
#endif

#if defined(SET_MODE_NORMAL_METHOD)
  #	if defined(SET_MODE_BF_MATCHER)
	_Matcher->match(__Object, __Scene, __Matches);
# else
	printf("You are not activate Matching Process.\ Please change another match method & matcher.");
#	endif
#elif defined(SET_MODE_KNN_METHOD)
  #if defined(SET_MODE_BF_MATCHER)
  //Time _TTime;
	//_TTime.Start();
  _Matcher->knnMatch(__Object, __Scene, __Matches, _TK);
	//_TTime.End();
	//printf("BFMatcher Time elapsed : %lf\n", _TTime.Get_ElapsedTime());
  #elif defined(SET_MODE_FLANN_MATCHER)
  _Matcher->knnSearch(__Object, __Results, __Dists, _TK, cv::flann::SearchParams());
  #endif
#endif
}

bool FeatureDesc::Find_Matchs(FeatureSets &Object, FeatureSets &Scene, int &Scene_X, int &Scene_Y) {
  bool _TResult = false;
#if defined(LOG_WRITE_MODE)
  //G_LogD->Logging("Func", "into Find_Matchs Function");
#endif
  double _TMax_Dist = 0.0f, _TMin_Dist = 100.0f;
#if defined(SET_MODE_FLANN_MATCHER)
  cv::Mat _TResults;
	cv::Mat _TDists;
#endif
#if defined(SET_MODE_NORMAL_METHOD)
  vector<DMatch>
#elif defined(SET_MODE_KNN_METHOD)
  vector<vector<DMatch> >
#endif
      _TMatches;
  vector<Point2f> _TObjectPoint, _TScenePoint;
  vector<int> _TIndexObject, _TIndexScene;

  Go_Matching(Object._Descriptor, Scene._Descriptor,
#if defined(SET_MODE_FLANN_MATCHER)
      _TResults,	_TDists,
#endif
              _TMatches);

  //printf("Maches : %d\n", _TMatches.size());
  float _TMatchesSum = 0.0;
  for (register int i = 0; i <
#if defined(SET_MODE_BF_MATCHER)
    _TMatches.size();
#elif defined(SET_MODE_FLANN_MATCHER)
    Object._Descriptor.rows;
#endif
       i++) {
    //_TMatchesSum += _TMatches.at(i).distance;
    if (
#if defined(SET_MODE_BF_MATCHER)
#	if defined(SET_MODE_NORMAL_METHOD)
        _TMatches.size() <= NNR_RATIO * _TMatches.at(i).distance
#	elif defined(SET_MODE_KNN_METHOD)
        _TMatches.at(i).size() == 2 && _TMatches.at(i).at(0).distance <= NNR_RATIO * _TMatches.at(i).at(1).distance
#	endif
#elif defined(SET_MODE_FLANN_MATCHER)
      #	if defined(SET_MODE_KNN_METHOD)
			_TResults.at<int>(i, 0) >= 0 && _TResults.at<int>(i, 0) >= 0 &&
			_TDists.at<float>(i, 0) <= NNR_RATIO * _TDists.at<float>(i, 1)
#	endif
#endif
        )	{
      _TObjectPoint.push_back(Object._Keypoints.at(
#if defined(SET_MODE_BF_MATCHER)
#	if defined(SET_MODE_NORMAL_METHOD)
          _TMatches.at(0).queryIdx
#	elif defined(SET_MODE_KNN_METHOD)
          _TMatches.at(i).at(0).queryIdx
#	endif
#elif defined(SET_MODE_FLANN_MATCHER)
          i
#endif
      ).pt);
      _TIndexObject.push_back(
#if defined(SET_MODE_BF_MATCHER)
#	if defined(SET_MODE_NORMAL_METHOD)
          _TMatches.at(0).queryIdx
#	elif defined(SET_MODE_KNN_METHOD)
          _TMatches.at(i).at(0).queryIdx
#	endif
#elif defined(SET_MODE_FLANN_MATCHER)
          i
#endif
      );
      _TScenePoint.push_back(Scene._Keypoints.at(
#if defined(SET_MODE_BF_MATCHER)
#	if defined(SET_MODE_NORMAL_METHOD)
          _TMatches.at(0).trainIdx
#	elif defined(SET_MODE_KNN_METHOD)
          _TMatches.at(i).at(0).trainIdx
#	endif
#elif defined(SET_MODE_FLANN_MATCHER)
          _TResults.at<int>(i, 0)
#endif
      ).pt);

      _TIndexScene.push_back(
#if defined(SET_MODE_BF_MATCHER)
#	if defined(SET_MODE_NORMAL_METHOD)
          _TMatches.at(0).trainIdx
#	elif defined(SET_MODE_KNN_METHOD)
          _TMatches.at(i).at(0).trainIdx
#	endif
#elif defined(SET_MODE_FLANN_MATCHER)
          _TResults.at<int>(i, 0)
#endif
      );
    }
  }

  //printf("Matches Sum : %f\n", _TMatchesSum);
  vector<uchar> _TOutMask;
  float _TMinX = 65535.0, _TMinY = 65535.0, _TMaxX = 0.0, _TMaxY = 0.0;

  //printf("obj size : %d, Obj point : %d, Ratio : %lf\n" ,_TObjectPoint.size(), Object._Keypoints.size(), ((double)_TObjectPoint.size() / (double)Object._Keypoints.size()));
  double _TNowKeyNMatchRatio = ((double)_TObjectPoint.size() / (double)Object._Keypoints.size());
#if defined(LOG_WRITE_MODE)
  //	if (_TNowKeyNMatchRatio > 0.5f)
//		G_LogD->Logging("Data", "Key Match Ratio : %lf, Minimum Inliner : %d", _TNowKeyNMatchRatio, _TObjectPoint.size());
#endif
  if (_TNowKeyNMatchRatio >= OBJKEY_N_MATCHPOINT_RATIO
      && _TObjectPoint.size() >= MINIMUM_INLINERS) {
    // find homography
    Mat _THomography = findHomography(_TObjectPoint, _TScenePoint, CV_RANSAC, RANSAC_REPROJECTIONTHRESHOLD, _TOutMask);

    if (_TOutMask.size() != 0) {
      for (register int i = 0; i < _TScenePoint.size(); i++) {
        if (_TOutMask.at(i)) {
          float _TNowX = _TScenePoint.at(i).x;
          float _TNowY = _TScenePoint.at(i).y;

          if (_TNowX < _TMinX) _TMinX = _TNowX;
          if (_TNowX > _TMaxX) _TMaxX = _TNowX;
          if (_TNowY < _TMinY) _TMinY = _TNowY;
          if (_TNowY > _TMaxY) _TMaxY = _TNowY;
        }
      }

      float _TX1 = _TMinX, _TY1 = _TMinY, // A1
          _TX2 = _TMaxX, _TY2 = _TMaxY, // A2
          _TX3 = _TMinX, _TY3 = _TMaxY, // B1
          _TX4 = _TMaxX, _TY4 = _TMinY; // B2


      float _TBaseLine = (((_TY4 - _TY3) * (_TX2 - _TX1)) - ((_TX4 - _TX3) * (_TY2 - _TY1))); // �Ϸ�

      if (_TBaseLine != 0.0) {

        double _T_T = (((_TX4 - _TX3) * (_TY1 - _TY3)) - ((_TY4 - _TY3) * (_TX1 - _TX3)));
        double _T_S = (((_TX2 - _TX1) * (_TY1 - _TY3)) - ((_TY2 - _TY1) * (_TX1 - _TX3)));
        double _TT = _T_T / _TBaseLine;
        double _TS = _T_S / _TBaseLine;

        if (_TT < 0.0 || _TT > 1.0 || _TS < 0.0 || _TS > 1.0) return false;
        if (_T_T == 0.0 && _T_S == 0.0) return false;

        Scene_X = _TX1 + _TT * (_TX2 - _TX1);
        Scene_Y = _TY1 + _TT * (_TY2 - _TY1);
        _TResult = true;
        //G_LogD->Logging("Data", "End.");
      }
    }
  }

  return _TResult;
}
#endif

#endif
/*
ImageDetector::ImageDetector(
#if defined(SET_MODE_FAST_DETECTOR)
														 int DetectorThreshold,
#endif
														 double NNDRatio) {
#if defined(SET_MODE_FAST_DETECTOR)
	_DetectorThreshold = DetectorThreshold;
#endif
	_NNDRatio = NNDRatio;
}

ImageDetector::FeatureDetectoror() {
#if defined(SET_MODE_FAST_DETECTOR)
	_DetectorThreshold = 0;
#endif
	_NNDRatio = 0.0;
}

void ImageDetector::Go_Detect(Mat &__Image, vector<KeyPoint> &__Keypoints) {
#if defined(LOG_WRITE_MODE)
	//G_LogD->Logging("Func", "into Go_Detect Function");
#endif
#if defined(SET_MODE_FAST_DETECTOR)
	FastFeatureDetector
#elif defined(SET_MODE_ORB_DETECTOR)
	OrbFeatureDetector
#endif
		_TDetector;

	_Detector = _TDetector;
	// this threshold function made when OpenCV Version 3.1.0
	_Detector.setThreshold(_DetectorThreshold);
	_Detector.detect(__Image, __Keypoints);
}

void ImageDetector::Go_Extract(Mat &__Image, vector<KeyPoint> &__Keypoints, Mat &__Descriptors) {
#if defined(LOG_WRITE_MODE)
	//G_LogD->Logging("Func", "into Go_Extract Function");
#endif
#if defined(SET_MODE_ORB_EXTRACTOR)
	ORB
#endif
		_TExtractor;

	_Extractor = _TExtractor;
	_Extractor.compute(__Image, __Keypoints, __Descriptors);
}

void ImageDetector::Go_DetectAndExtract(Mat &__Image, vector<KeyPoint> &__Keypoints, Mat &__Descriptors) {
#if defined(LOG_WRITE_MODE)
	//G_LogD->Logging("Func", "into Go_DetectAndExtract Function");
#endif
	Go_Detect(__Image, __Keypoints);
	Go_Extract(__Image, __Keypoints, __Descriptors);
}

void ImageDetector::Go_Matching(Mat &__Object, Mat &__Scene,
#if defined(SET_MODE_FLANN_MATCHER)
																Mat &__Results, Mat &__Dists,
#endif
#if defined(SET_MODE_NORMAL_METHOD)
																vector<DMatch>
#elif defined(SET_MODE_KNN_METHOD)
																vector<vector<DMatch> >
#endif	
																&__Matches) {
	int _TObjMatSize = (int)(__Object.cols * __Object.rows * __Object.elemSize());
	int _TSceneMatSize = (int)(__Scene.cols * __Scene.rows * __Scene.elemSize());
#if defined(LOG_WRITE_MODE)
	//G_LogD->Logging("Func", "into Go_Matching Function");
	//G_LogD->Logging("Func", "Mem1 Size : %d, Mem2 Size : %d", (int)_TObjMatSize, (int)_TSceneMatSize);
#endif
#if defined(SET_MODE_KNN_METHOD)
	int _TK = MATCHER_NEIGHBORS_K;
#endif

#if defined(SET_MODE_BF_MATCHER)
	cv::BFMatcher
#elif defined(SET_MODE_FLANN_MATCHER)
	cv::flann::Index
#endif
#if defined(SET_MODE_BF_MATCHER)
		_TMatcher(
#if defined(SET_MODE_BRIEF_EXTRACTOR) || defined(SET_MODE_FREAK_EXTRACTOR)
		NORM_HAMMING
#elif defined(SET_MODE_ORB_EXTRACTOR)
		NORM_HAMMING2 
#endif
		);
#elif defined(SET_MODE_FLANN_MATCHER)
		_TMatcher(__Scene, cv::flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);
#endif

#if defined(SET_MODE_NORMAL_METHOD)
#	if defined(SET_MODE_BF_MATCHER)
	_TMatcher.match(__Object, __Scene, __Matches);
# else
	printf("You are not activate Matching Process.\ Please change another match method & matcher.");
#	endif
#elif defined(SET_MODE_KNN_METHOD)
#	if defined(SET_MODE_BF_MATCHER)
	//Time _TTime;
	//_TTime.Start();
	_TMatcher.knnMatch(__Object, __Scene, __Matches, _TK);
	//_TTime.End();
	//printf("BFMatcher Time elapsed : %lf\n", _TTime.Get_ElapsedTime());
#elif defined(SET_MODE_FLANN_MATCHER)
	_TMatcher.knnSearch(__Object, __Results, __Dists, _TK, cv::flann::SearchParams());
#	endif
#endif
}

bool ImageDetector::Find_Matchs(FeatureSets &Object, FeatureSets &Scene, int &Scene_X, int &Scene_Y) {
	bool _TResult = false;
#if defined(LOG_WRITE_MODE)
	//G_LogD->Logging("Func", "into Find_Matchs Function");
#endif
	double _TMax_Dist = 0.0f, _TMin_Dist = 100.0f;
#if defined(SET_MODE_FLANN_MATCHER)
	cv::Mat _TResults;
	cv::Mat _TDists;
#endif
#if defined(SET_MODE_NORMAL_METHOD)
	vector<DMatch>
#elif defined(SET_MODE_KNN_METHOD)
	vector<vector<DMatch> >
#endif	
		_TMatches;
	vector<Point2f> _TObjectPoint, _TScenePoint;
	vector<int> _TIndexObject, _TIndexScene;

	Go_Matching(Object._Descriptor, Scene._Descriptor,
#if defined(SET_MODE_FLANN_MATCHER)
		_TResults,	_TDists,
#endif
		_TMatches);

	//printf("Maches : %d\n", _TMatches.size());
	float _TMatchesSum = 0.0;
	for (register int i = 0; i <
#if defined(SET_MODE_BF_MATCHER)
		_TMatches.size();
#elif defined(SET_MODE_FLANN_MATCHER)
		Object._Descriptor.rows;
#endif
		i++) {
		//_TMatchesSum += _TMatches.at(i).distance;
		if (
#if defined(SET_MODE_BF_MATCHER)
#	if defined(SET_MODE_NORMAL_METHOD)
			_TMatches.size() <= NNR_RATIO * _TMatches.at(i).distance
#	elif defined(SET_MODE_KNN_METHOD)
			_TMatches.at(i).size() == 2 && _TMatches.at(i).at(0).distance <= NNR_RATIO * _TMatches.at(i).at(1).distance
#	endif
#elif defined(SET_MODE_FLANN_MATCHER)
#	if defined(SET_MODE_KNN_METHOD)
			_TResults.at<int>(i, 0) >= 0 && _TResults.at<int>(i, 0) >= 0 &&
			_TDists.at<float>(i, 0) <= NNR_RATIO * _TDists.at<float>(i, 1)
#	endif
#endif
			)	{
			_TObjectPoint.push_back(Object._Keypoints.at(
#if defined(SET_MODE_BF_MATCHER)
#	if defined(SET_MODE_NORMAL_METHOD)
				_TMatches.at(0).queryIdx
#	elif defined(SET_MODE_KNN_METHOD)
				_TMatches.at(i).at(0).queryIdx
#	endif
#elif defined(SET_MODE_FLANN_MATCHER)
				i
#endif
				).pt);
			_TIndexObject.push_back(
#if defined(SET_MODE_BF_MATCHER)
#	if defined(SET_MODE_NORMAL_METHOD)
				_TMatches.at(0).queryIdx
#	elif defined(SET_MODE_KNN_METHOD)
				_TMatches.at(i).at(0).queryIdx
#	endif
#elif defined(SET_MODE_FLANN_MATCHER)
				i
#endif
				);
			_TScenePoint.push_back(Scene._Keypoints.at(
#if defined(SET_MODE_BF_MATCHER)
#	if defined(SET_MODE_NORMAL_METHOD)
				_TMatches.at(0).trainIdx
#	elif defined(SET_MODE_KNN_METHOD)
				_TMatches.at(i).at(0).trainIdx
#	endif
#elif defined(SET_MODE_FLANN_MATCHER)
				_TResults.at<int>(i, 0)
#endif
				).pt);

			_TIndexScene.push_back(
#if defined(SET_MODE_BF_MATCHER)
#	if defined(SET_MODE_NORMAL_METHOD)
				_TMatches.at(0).trainIdx
#	elif defined(SET_MODE_KNN_METHOD)
				_TMatches.at(i).at(0).trainIdx
#	endif
#elif defined(SET_MODE_FLANN_MATCHER)
				_TResults.at<int>(i, 0)
#endif
				);
		}
	}

	//printf("Matches Sum : %f\n", _TMatchesSum);
	std::vector<uchar> _TOutMask;
	float _TMinX = 65535.0, _TMinY = 65535.0, _TMaxX = 0.0, _TMaxY = 0.0;

	//printf("obj size : %d, Obj point : %d, Ratio : %lf\n" ,_TObjectPoint.size(), Object._Keypoints.size(), ((double)_TObjectPoint.size() / (double)Object._Keypoints.size()));
	double _TNowKeyNMatchRatio = ((double)_TObjectPoint.size() / (double)Object._Keypoints.size());
#if defined(LOG_WRITE_MODE)
//	if (_TNowKeyNMatchRatio > 0.5f)
//		G_LogD->Logging("Data", "Key Match Ratio : %lf, Minimum Inliner : %d", _TNowKeyNMatchRatio, _TObjectPoint.size());
#endif
	if (_TNowKeyNMatchRatio >= OBJKEY_N_MATCHPOINT_RATIO
		&& _TObjectPoint.size() >= MINIMUM_INLINERS) {
		// find homography
		Mat _THomography = findHomography(_TObjectPoint, _TScenePoint, CV_RANSAC, RANSAC_REPROJECTIONTHRESHOLD, _TOutMask);

		if (_TOutMask.size() != 0) {
			for (register int i = 0; i < _TScenePoint.size(); i++) {
				if (_TOutMask.at(i)) {
					float _TNowX = _TScenePoint.at(i).x;
					float _TNowY = _TScenePoint.at(i).y;

					if (_TNowX < _TMinX) _TMinX = _TNowX;
					if (_TNowX > _TMaxX) _TMaxX = _TNowX;
					if (_TNowY < _TMinY) _TMinY = _TNowY;
					if (_TNowY > _TMaxY) _TMaxY = _TNowY;
				}
			}

			float _TX1 = _TMinX, _TY1 = _TMinY, // A1
				_TX2 = _TMaxX, _TY2 = _TMaxY, // A2
				_TX3 = _TMinX, _TY3 = _TMaxY, // B1
				_TX4 = _TMaxX, _TY4 = _TMinY; // B2


			float _TBaseLine = (((_TY4 - _TY3) * (_TX2 - _TX1)) - ((_TX4 - _TX3) * (_TY2 - _TY1))); // �Ϸ�

			if (_TBaseLine != 0.0) {

				double _T_T = (((_TX4 - _TX3) * (_TY1 - _TY3)) - ((_TY4 - _TY3) * (_TX1 - _TX3)));
				double _T_S = (((_TX2 - _TX1) * (_TY1 - _TY3)) - ((_TY2 - _TY1) * (_TX1 - _TX3)));
				double _TT = _T_T / _TBaseLine;
				double _TS = _T_S / _TBaseLine;

				if (_TT < 0.0 || _TT > 1.0 || _TS < 0.0 || _TS > 1.0) return false;
				if (_T_T == 0.0 && _T_S == 0.0) return false;

				Scene_X = _TX1 + _TT * (_TX2 - _TX1);
				Scene_Y = _TY1 + _TT * (_TY2 - _TY1);
				_TResult = true;
				//G_LogD->Logging("Data", "End.");
			}
		}
	}
	
	return _TResult;
}
*/