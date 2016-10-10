/*********************************************************************

          File: LaneSearcher.cpp

   Description: LaneSearcher Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/10/03

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

#include "LaneSearcher.hpp"

#if defined(_LaneSearcher_hpp_)

#include "LogD.hpp"

LaneSearcher *G_LaneSearcher;

LaneSearcher::LaneSearcher() {
  _Initialize_Members();
  _Initialize();
}

LaneSearcher::~LaneSearcher(){
  _Deinitialize();
  _Initialize_Members();
}

void LaneSearcher::_Initialize_Members() {
  _LaneSearcherActivated = false;
  _LaneSearcherStarted = false;
  _LaneSearcherWorkAssignStarted = false;
  _LaneSearcherPaused = false;
}

void LaneSearcher::_Initialize() {
  G_LaneSearcher = this;
  TLaneSearchedResultCallback = NULL;

  // Clear ImageQueue.
  _QueueClear(_InputSceneDataQueue);
}

void LaneSearcher::_Deinitialize() {
  // Clear ImageQueue.
  _QueueClear(_InputSceneDataQueue);
  G_LaneSearcher = NULL;
}

void LaneSearcher::_ThrowUpImageQueue() {
  __MUTEXLOCK(_Mutex_InputSceneDataQueue);
  _QueueClear(_InputSceneDataQueue);
  __MUTEXUNLOCK(_Mutex_InputSceneDataQueue);
}

bool LaneSearcher::_IsEmptyImageQueue() {
  bool _TResult = false;

  __MUTEXLOCK(_Mutex_InputSceneDataQueue);
  _TResult = _InputSceneDataQueue.empty();
  __MUTEXUNLOCK(_Mutex_InputSceneDataQueue);

  return _TResult;
}

void *LaneSearcher::_LaneSearcher_WorkerThread(void *Param) {
	G_LogD->Logging("Func", "into _LaneSearcher_WorkerThread Function");

  LaneSearcherWorkingInformation *_TWorkingInformation = (LaneSearcherWorkingInformation *)Param;
  LaneSearcher *_TEngine = _TWorkingInformation->_PFeatureSearcher;

  Mat _TGrayMat;
  Mat _TSobelX, _TSobelY;
  Mat _TSobelABSX, _TSobelABSY;
  Mat _TSobelResult, _TCannyResult;
  Mat _THoughResult;
  vector<Vec4i> _THoughLineResult;

  cvtColor(_TWorkingInformation->_SceneMat, _TGrayMat, CV_BGR2GRAY);

  medianBlur(_TGrayMat, _TGrayMat, 3);
  GaussianBlur(_TGrayMat, _TGrayMat, Size(3,3), 0, 0, BORDER_DEFAULT);

  // using Sobel mask of Grad X
  cv::Sobel(_TGrayMat, _TSobelX, CV_16S, 1, 0);
  convertScaleAbs(_TSobelX, _TSobelABSX);
  // using Sobel mask of Grad Y
  cv::Sobel(_TGrayMat, _TSobelY, CV_16S, 0, 1);
  convertScaleAbs(_TSobelY, _TSobelABSY);

  // Weighted Sobel Result
  addWeighted(_TSobelABSX, 0.5, _TSobelABSY, 0.5, 0, _TSobelResult);

  // using Canny Edge in Image
  Canny(_TSobelResult, _TCannyResult, 100, 150);
  _THoughResult = _TWorkingInformation->_SceneMat.clone();

  // using Hough Line in Image
  HoughLinesP(_TCannyResult, _TWorkingInformation->_HoughLineResult, 1, CV_PI/180, 50, 20, 10);

	return 0;
}

void *LaneSearcher::_LaneSearcher_WorkAssignThread(void *Param) {
  G_LogD->Logging("Func", "into _ImageSearcher_WorkAssignThread Function");

  LaneSearcher *_TEngine = (LaneSearcher *)Param;

  _TEngine->_LaneSearcherWorkAssignStarted = true;

  while (_TEngine->_LaneSearcherActivated) {
    //G_LogD->Logging("Func", "into _ImageSearcherActivated");
    if (_TEngine->_LaneSearcherPaused != true) {
      if (_TEngine->_IsEmptyImageQueue() != true) {
        //Time _TTime;
        //G_LogD->Logging("Time", "Start of Match");
        //_TTime.Start();
        // 집어 넣어진 것을 꺼내어 보고 앞에껀 pop 해버린다.
        __MUTEXLOCK(_TEngine->_Mutex_InputSceneDataQueue);
        Mat _TBinaryScene = _TEngine->_InputSceneDataQueue.front();
        _TEngine->_InputSceneDataQueue.pop();
        __MUTEXUNLOCK(_TEngine->_Mutex_InputSceneDataQueue);

        // Do Program.
        Thread _TThread[CLIENT_IMAGE_SEARCHER_MAXIMUM_THREAD];
        LaneSearcherWorkingInformation _TWorkInfo[CLIENT_IMAGE_SEARCHER_MAXIMUM_THREAD];
        int _TMinX = 0, _TMinY = 0, _TMaxX =0, _TMaxY = 0;

        // Working Infomation 생성.
        for (register int i=0; i<CLIENT_IMAGE_SEARCHER_MAXIMUM_THREAD; i++) {
          if (i==0) { _TMinX = 0; _TMinY = 0; _TMaxX = (_TBinaryScene.cols/2)-1; _TMaxY = (_TBinaryScene.rows/2)-1; }
          else if (i==1) { _TMinX = _TBinaryScene.cols/2; _TMinY = 0; _TMaxX = _TBinaryScene.cols; _TMaxY = (_TBinaryScene.rows/2)-1; }
          else if (i==2) { _TMinX = 0; _TMinY = _TBinaryScene.rows/2; _TMaxX = (_TBinaryScene.cols/2)-1; _TMaxY = _TBinaryScene.rows; }
          else if (i==3) { _TMinX = _TBinaryScene.cols/2; _TMinY = _TBinaryScene.rows/2; _TMaxX = _TBinaryScene.cols; _TMaxY = _TBinaryScene.rows; }
          Rect _TRectROISrc(_TMinX, _TMinY, _TMaxX, _TMaxY);
          Mat _TROISrc(_TBinaryScene, _TRectROISrc);

          _TWorkInfo[i]._SceneMat = _TROISrc.clone();
          _TWorkInfo[i]._WorkingInfoIndex = i;
          _TWorkInfo[i]._PFeatureSearcher = _TEngine;
        }

        // Thread를 Joinable하게 만들고, Start 시킨다.
        for(register int i = 0; i < CLIENT_IMAGE_SEARCHER_MAXIMUM_THREAD; i++) {
          // thread Join Mode.
          _TThread[i].AttacheMode = true;
          _TThread[i].StartThread(_LaneSearcher_WorkerThread, &_TWorkInfo[i]);
        }

        // Join한다. 기다리는데 시간은 보장할 수 없다.
        for(register int i = 0; i < CLIENT_IMAGE_SEARCHER_MAXIMUM_THREAD; i++)
          _TThread[i].JoinThread();

        ImageStitcher _TImageStitcher;
        Mat _TH1, _TH2, _TResScene;

        // stitching image
        _TH1 = _TImageStitcher.Do_StitchingHorizontallyFromTwoImage(_TWorkInfo[0]._SceneMat, _TWorkInfo[1]._SceneMat);
        _TH2 = _TImageStitcher.Do_StitchingHorizontallyFromTwoImage(_TWorkInfo[2]._SceneMat, _TWorkInfo[3]._SceneMat);
        _TResScene = _TImageStitcher.Do_StitchingVerticallyFromTwoImage(_TH1, _TH2);

        vector<Vec4i> _THoughLineResult;

        for (int i=0; i<CLIENT_IMAGE_SEARCHER_MAXIMUM_THREAD; i++) {
          for (int j=0; j<_TWorkInfo[i]._HoughLineResult.size(); j++)
            _THoughLineResult.push_back(_TWorkInfo[i]._HoughLineResult[j]);
        }

        /*
        // set line in image at Hough Line result
        for (size_t i=0; i<_THoughLineResult.size(); i++) {
          Vec4i _TLine = _THoughLineResult[i];
          line(_THoughResult, Point(_TLine[0], _TLine[1]), Point(_TLine[2], _TLine[3]), Scalar(0,0,255), 2);
        }
        */
        LaneDetectData _TLaneDetectData;
        MathematicalFunc _TMathematicalFunc;

        double _TAngle = 0.0f;

        _TLaneDetectData.ImageData = _TResScene;

        // calculation Data.
        for (int i=0; i<_THoughLineResult.size(); i++){
          Vec4i _TLine = _THoughLineResult[i];
          _TAngle += _TMathematicalFunc.Get_AngleToPointByImagePosition(_TLine[0],_TLine[1], _TLine[2], _TLine[3]);
        }
        _TAngle /= _THoughLineResult.size();
        _TLaneDetectData.AngleData = _TAngle;

        _TEngine->TLaneSearchedResultCallback(_TLaneDetectData);
      }
      else
        _TEngine->_InputSceneDataQueueSyncSignal.Wait();
    }
    else {
      // 멈추었을때 딱 한번만 실행하게 해야한다.
      // _InputImageQueue를 Clear한다. 이 상황은 Image Searching Engine이 멈추었을 때이다.
      // Image Searching을 멈춘다는 것은, 메뉴를 열어 검색을 하기 위함이다.
      // 그러므로, Image들이 아예 필요가 없어져서 Engine안에 있는 Image를 몸땅 버려도 된다.
      _TEngine->_ThrowUpImageQueue();
      // Paused 하면, 대기한다.
      _TEngine->_WorkAssignSyncSignal.Wait();
    }
  }

  _TEngine->_LaneSearcherWorkAssignStarted = true;

  return 0;
}

void *LaneSearcher::_LaneSearcher_MainThread(void *Param) {
  G_LogD->Logging("Func", "into _ImageSearcher_MainThread Function");

  LaneSearcher *_TEngine = (LaneSearcher *)Param;
  Thread _TImageSearcherWorkAssignThread;
  _TEngine->_LaneSearcherStarted = true;
  // Semapore를 먼저 Initialize 시킨다.
  __MUTEXINIT(_TEngine->_Mutex_InputSceneDataQueue);

  _TImageSearcherWorkAssignThread.StartThread(_LaneSearcher_WorkAssignThread, _TEngine);

  // _ImageSearcher_WorkAssignThread Thread가 signal 받기 전까지 대기한다.
  // CPU의 점유를 없에기 위함(예전의 while은 항상 어느정도의 CPU를 점유한다).
  _TEngine->_LaneSearcherActivatedSyncSignal.Wait();
  /*
  while (_TEngine->_ImageSearcherActivated == true) {
    // CPU 점유율을 낮추기 위해 Sleep을 걸어줌.
    // 약 100ms.
    m_sleep(33 * 3);
  }
  */
  // Thread 안전종료 전, Wait이 걸려 있는것이 있는지 없는지 확인한다.
  // 멈추어 있으면, 절대로 종료하지 않기 때문에, 무한 대기 상태로 빠진다.
  // 그래서 확인 후, '무조건' 대기 상태를 풀어준다.
  if (_TEngine->_WorkAssignSyncSignal.Get_IsWaiting() == true)
    _TEngine->_WorkAssignSyncSignal.Signal();

  // Thread 안전 종료. 다 처리될 때까지 기다린다.
  while (_TEngine->_LaneSearcherWorkAssignStarted == true) m_sleep(33);

  __MUTEXDESTROY(_TEngine->_Mutex_InputSceneDataQueue);


  _TEngine->_LaneSearcherStarted = false;

  return 0;
}

void LaneSearcher::Start_LaneSearcher() {
  G_LogD->Logging("Func", "into Start_ImageSearcher Function");

  if (_LaneSearcherActivated != true) {
    _LaneSearcherActivated = true;
    // Searcher Thread Start.
    _Thread.StartThread(_LaneSearcher_MainThread, this);
  }
}

void LaneSearcher::Stop_LaneSearcher() {
  G_LogD->Logging("Func", "into Stop_ImageSearcher Function");

  if (_LaneSearcherActivated != false) {
    _LaneSearcherActivated = false;
    if (_LaneSearcherActivatedSyncSignal.Get_IsWaiting() == true)
      _LaneSearcherActivatedSyncSignal.Signal();
  }
}

void LaneSearcher::Pause_LaneSearcher() {
  G_LogD->Logging("Func", "into Pause_ImageSearcher Function");

  if (_LaneSearcherPaused != true)
    _LaneSearcherPaused = true;
}

void LaneSearcher::Resume_LaneSearcher() {
  G_LogD->Logging("Func", "into Resume_ImageSearcher Function");

  if (_LaneSearcherPaused != false) {
    _LaneSearcherPaused = false;
    // Pause 되면, 무조건 Thread가 Wait되어 있기 때문에
    // 이것을 풀어 주어야 한다.
    _WorkAssignSyncSignal.Signal();
  }
}

void LaneSearcher::Push_ImageToQueue(Mat &Scene) {
  G_LogD->Logging("Func", "into Push_ImageToQueue Function, %d %d", _LaneSearcherPaused, _LaneSearcherActivated);

  // 중간에 SearchEngine이 중단될 수 있는 상황(_InputImageQueue전체가 Flush되는 상황)이
  // 벌어질 수도 있기 때문에 반드시 Mutex 걸어준다.
  // Engine이 중단된 상황, Engine이 활성화 되지 않은 상황에서는 집어 넣어서는 안된다.
  if (_LaneSearcherPaused == false
      && _LaneSearcherActivated == true) {
    cv::Mat _TGrayMat;
    cv::cvtColor(Scene, _TGrayMat, CV_BGR2GRAY);

    //G_LogD->Logging("Func", "Feature Size : %d", _TSceneMatSize);
    __MUTEXLOCK(_Mutex_InputSceneDataQueue);
    // Queue Frame 수 제한.
    if (_InputSceneDataQueue.size() > CLIENT_IMAGE_SEARCHER_QUEUE_LIMIT)
      _InputSceneDataQueue.pop();

    _InputSceneDataQueue.push(_TGrayMat);

    if (_InputSceneDataQueueSyncSignal.Get_IsWaiting() == true) {
      _InputSceneDataQueueSyncSignal.Signal();
    }

    __MUTEXUNLOCK(_Mutex_InputSceneDataQueue);
  }
}

#endif