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

#if !defined(MODE_ONLY_DETECTION)
  _NavigateDataUpdated = false;
#endif
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

#if !defined(MODE_ONLY_DETECTION)
void *LaneSearcher::_ImageSearcher_WorkerThread(void *Param) {
	G_LogD->Logging("Func", "into _ImageSearcher_WorkerThread Function");

	WorkingInformation *_TWorkingInformation = (WorkingInformation *)Param;
	ImageSearcher *_TEngine = _TWorkingInformation->_PFeatureSearcher;

	// scene Feature Points
	FeatureSets _TObjectFeatures;

	_TObjectFeatures._Descriptor = _TWorkingInformation->_WorkingInfo.img_desc;
	_TObjectFeatures._Keypoints = _TWorkingInformation->_WorkingInfo.img_key;

	// Processing via FAST-ORB
  FeatureDesc _TFeatureDesc(FeatureDescEnum::S_FAST, FeatureDescEnum::S_ORB);
	int _TX = 0, _TY = 0;

	if (_TFeatureDesc.Find_Matchs(_TObjectFeatures, _TWorkingInformation->_SceneFeature, _TX, _TY) == true) {
		// 일단 나온 좌표부터 Setting. 나온 점은 Feature의 중간점 표시 위치.
		_TWorkingInformation->_WorkingInfo._DisplayPoint_X = _TX;
		_TWorkingInformation->_WorkingInfo._DisplayPoint_Y = _TY;

		//G_LogD->Logging("Data", "Name : %s -> X : %d, Y : %d", _TWorkingInformation->_WorkingInfo.comp_name_en.c_str(), _TX, _TY);

		_TEngine->_SortedData.push_back(_TWorkingInformation->_WorkingInfo);
	}
	return 0;
}
#endif

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

        //_TEngine->TLaneSearchedResultCallback(_TBinaryScene);

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