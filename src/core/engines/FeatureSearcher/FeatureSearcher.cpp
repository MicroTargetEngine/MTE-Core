/*********************************************************************

          File: FeatureSearcher.cpp

   Description: FeatureSearcher Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18

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

#include "FeatureSearcher.hpp"

#if defined(_FeatureSearcher_hpp_)

#include "LogD.hpp"

FeatureSearcher *G_ImageSearcher;

FeatureSearcher::FeatureSearcher() {
	_Initialize_Members();
	_Initialize();
}

FeatureSearcher::~FeatureSearcher(){
	_Deinitialize();
	_Initialize_Members();
}

void FeatureSearcher::_Initialize_Members() {
	_FeatureSearcherActivated = false;
	_FeatureSearcherStarted = false;
	_FeatureSearcherWorkAssignStarted = false;
	_FeatureSearcherPaused = false;

#if !defined(MODE_ONLY_DETECTION)
	_NavigateDataUpdated = false;
#endif
}

void FeatureSearcher::_Initialize() {
	G_ImageSearcher = this;
	TFeatureSearchedResultCallback = NULL;
	TForceAutoFocusCallback = NULL;

	// Clear ImageQueue.
	_QueueClear(_InputSceneDataQueue);
#if !defined(MODE_ONLY_DETECTION)
	_NavigateData.clear();
	_SortedData.clear();
#endif
}

void FeatureSearcher::_Deinitialize() {
#if !defined(MODE_ONLY_DETECTION)
	_SortedData.clear();
	_NavigateData.clear();
#endif
	// Clear ImageQueue.
	_QueueClear(_InputSceneDataQueue);
	G_ImageSearcher = NULL;
}

void FeatureSearcher::_ThrowUpImageQueue() {
	__MUTEXLOCK(_Mutex_InputSceneDataQueue);
	_QueueClear(_InputSceneDataQueue);
	__MUTEXUNLOCK(_Mutex_InputSceneDataQueue);
}

bool FeatureSearcher::_IsEmptyImageQueue() {
	bool _TResult = false;

	__MUTEXLOCK(_Mutex_InputSceneDataQueue);
	_TResult = _InputSceneDataQueue.empty();
	__MUTEXUNLOCK(_Mutex_InputSceneDataQueue);
	
	return _TResult;
}

#if !defined(MODE_ONLY_DETECTION)
int FeatureSearcher::_GetRandomWorkNumber(vector<int> __WorkChkArr) {
	// 랜덤하게 뽑은놈 하나의 번호만 리턴.
	int _TNumber = -1;

	if (__WorkChkArr.size() == 0)
		return _TNumber;

	srand(time(NULL));
	return rand() % __WorkChkArr.size();
}

vector<int> FeatureSearcher::_MakeWorkNumberList(vector<int> &__WorkChkArr, int __WorkCnt) {
	vector<int> _TWorkArr;
	int _TWorkCnt = __WorkCnt;

	while (_TWorkCnt > 0) {
		int _TWorkNumber = _GetRandomWorkNumber(__WorkChkArr);

		if (_TWorkNumber == -1) break;
		else {
			_TWorkArr.push_back(__WorkChkArr.at(_TWorkNumber));
			__WorkChkArr.erase(__WorkChkArr.begin() + _TWorkNumber);
		}

		_TWorkCnt--;
	}
	return _TWorkArr;
}

vector<FeatureData> FeatureSearcher::_MakeWorkList(vector<int> __WorkListArr) {
	vector<FeatureData> _TWorkLists;

	for_IterToEnd(vector, int, __WorkListArr, i) {
		// i의 값인 'Index'가 중요함.
		_TWorkLists.push_back(_NavigateData[*_i]);
	}

	return _TWorkLists;
}

void *FeatureSearcher::_FeatureSearcher_WorkerThread(void *Param) {
	G_LogD->Logging("Func", "into _ImageSearcher_WorkerThread Function");

	WorkingInformation *_TWorkingInformation = (WorkingInformation *)Param;
	FeatureSearcher *_TEngine = _TWorkingInformation->_PFeatureSearcher;
	
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

void *FeatureSearcher::_FeatureSearcher_WorkAssignThread(void *Param) {
	G_LogD->Logging("Func", "into _ImageSearcher_WorkAssignThread Function");

	FeatureSearcher *_TEngine = (FeatureSearcher *)Param;

	_TEngine->_FeatureSearcherWorkAssignStarted = true;
	
	while (_TEngine->_FeatureSearcherActivated) {
		//G_LogD->Logging("Func", "into _ImageSearcherActivated");
		if (_TEngine->_FeatureSearcherPaused != true) {
			if (_TEngine->_IsEmptyImageQueue() != true) {
				//Time _TTime;
				//G_LogD->Logging("Time", "Start of Match");
				//_TTime.Start();
				// 집어 넣어진 것을 꺼내어 보고 앞에껀 pop 해버린다.
				__MUTEXLOCK(_TEngine->_Mutex_InputSceneDataQueue);
				FeatureSets _TSceneFeatureSet = _TEngine->_InputSceneDataQueue.front();
				_TEngine->_InputSceneDataQueue.pop();
				__MUTEXUNLOCK(_TEngine->_Mutex_InputSceneDataQueue);

#if !defined(MODE_ONLY_DETECTION)
				vector<int> _TWorkListNumbers;

				__MUTEXLOCK(_TEngine->_Mutex_NavigateData);
				for (register int i = 0; i < _TEngine->_NavigateData.size(); i++)
					_TWorkListNumbers.push_back(i);
				__MUTEXUNLOCK(_TEngine->_Mutex_NavigateData);
				// 일단 1 frame이 추출이 되었고, 현재 반경데이터가 있을 때,
				// 기본적으로 이를 다 완료 될 때까지 처리하고 들어온 1 frame에 대한 n개의 정보에 대한 분석을 끝낸다.
				// 그러기 위해서는 일감을 전달해야하는 routine이 필요하다.
				while (1) {
					// 가장 먼저, 일감이 업데이트 되어있는지를 살핀다. 만약 업데이트가 되었다면,
					// 모바일에 넘겨줄 기존 Data를 파기하고, 새 Data를 찾아서 주어야 한다.
					if (_TEngine->_NavigateDataUpdated == true) {
						__MUTEXLOCK(_TEngine->_Mutex_SortedData);
						_TEngine->_SortedData.clear();
						__MUTEXUNLOCK(_TEngine->_Mutex_SortedData);
						_TEngine->_NavigateDataUpdated = false;
					}
					// 일감을 전달한다. 중간에 일감을 Update할 경우에는,
					// 기존에 돌던걸 Flush하고, while loop를 종료하는 것을 원칙으로 한다.
					Thread _TThread[CLIENT_IMAGE_SEARCHER_MAXIMUM_THREAD];
					WorkingInformation _TWorkInfo[CLIENT_IMAGE_SEARCHER_MAXIMUM_THREAD];
					//int _TNoWorkRange = -1;

					vector<int> _TWorkListNumberWithThreadNumbers = _TEngine->_MakeWorkNumberList(_TWorkListNumbers, CLIENT_IMAGE_SEARCHER_MAXIMUM_THREAD);
					vector<FeatureData> _TWorkList = _TEngine->_MakeWorkList(_TWorkListNumberWithThreadNumbers);

					// Working Infomation 생성.
					for(register int i = 0; i < _TWorkListNumberWithThreadNumbers.size(); i++) {
						_TWorkInfo[i]._SceneFeature = _TSceneFeatureSet;
						_TWorkInfo[i]._WorkingInfo = _TWorkList.at(i);
						_TWorkInfo[i]._WorkingInfoIndex = _TWorkListNumberWithThreadNumbers.at(i);
						_TWorkInfo[i]._PFeatureSearcher = _TEngine;
					}
					// Thread를 Joinable하게 만들고, Start 시킨다.
					for(register int i = 0; i < _TWorkListNumberWithThreadNumbers.size(); i++) {
						// thread Join Mode.
						_TThread[i].AttacheMode = true;
						_TThread[i].StartThread(_ImageSearcher_WorkerThread, &_TWorkInfo[i]);
					}

					// Join한다. 기다리는데 시간은 보장할 수 없다.
					for(register int i = 0; i < _TWorkListNumberWithThreadNumbers.size(); i++)
						_TThread[i].JoinThread();

					// 일감을 다 채웠는가? 다 채웠으면 break.
					if (_TWorkListNumbers.size() == 0)
						break;
				}

				if (_TEngine->_SortedData.size() != 0) {
					// 완료된 결과는 외부로 보낸다.
					vector<FeatureData> _TSortedData;

					//__MUTEXLOCK(_TEngine->_Mutex_SortedData);
					_TSortedData = _TEngine->_SortedData;
					//__MUTEXUNLOCK(_TEngine->_Mutex_SortedData);
					_TEngine->TImageSearchedResultCallback(_TSortedData);
					_TEngine->_SortedData.clear();
				}
				//_TTime.End();
				//G_LogD->Logging("Time", "End of Match Time : %lf", _TTime.Get_ElapsedTime());
#else
        _TEngine->TFeatureSearchedResultCallback(_TSceneFeatureSet);
#endif
			}
			else
				_TEngine->_FeatureQueueSyncSignal.Wait();
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

	_TEngine->_FeatureSearcherWorkAssignStarted = true;

	return 0;
}

void *FeatureSearcher::_FeatureSearcher_MainThread(void *Param) {
	G_LogD->Logging("Func", "into _ImageSearcher_MainThread Function");

	FeatureSearcher *_TEngine = (FeatureSearcher *)Param;
	Thread _TImageSearcherWorkAssignThread;
	_TEngine->_FeatureSearcherStarted = true;
	// Semapore를 먼저 Initialize 시킨다.
	__MUTEXINIT(_TEngine->_Mutex_InputSceneDataQueue);
#if !defined(MODE_ONLY_DETECTION)
	__MUTEXINIT(_TEngine->_Mutex_NavigateData);
	__MUTEXINIT(_TEngine->_Mutex_SortedData);
#endif

  _TImageSearcherWorkAssignThread.StartThread(_FeatureSearcher_WorkAssignThread, _TEngine);

  // _ImageSearcher_WorkAssignThread Thread가 signal 받기 전까지 대기한다.
  // CPU의 점유를 없에기 위함(예전의 while은 항상 어느정도의 CPU를 점유한다).
	_TEngine->_FeatureSearcherActivatedSyncSignal.Wait();
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
	while (_TEngine->_FeatureSearcherWorkAssignStarted == true) m_sleep(33);

	__MUTEXDESTROY(_TEngine->_Mutex_InputSceneDataQueue);
#if !defined(MODE_ONLY_DETECTION)
	__MUTEXDESTROY(_TEngine->_Mutex_NavigateData);
	__MUTEXDESTROY(_TEngine->_Mutex_SortedData);
#endif

	_TEngine->_FeatureSearcherStarted = false;

	return 0;
}

void FeatureSearcher::Start_FeatureSearcher() {
	G_LogD->Logging("Func", "into Start_ImageSearcher Function");

	if (_FeatureSearcherActivated != true) {
		_FeatureSearcherActivated = true;
		// Searcher Thread Start.
    _Thread.StartThread(_FeatureSearcher_MainThread, this);
	}
}

void FeatureSearcher::Stop_FeatureSearcher() {
	G_LogD->Logging("Func", "into Stop_ImageSearcher Function");

	if (_FeatureSearcherActivated != false) {
		_FeatureSearcherActivated = false;
		if (_FeatureSearcherActivatedSyncSignal.Get_IsWaiting() == true)
			_FeatureSearcherActivatedSyncSignal.Signal();
	}
}

void FeatureSearcher::Pause_FeatureSearcher() {
	G_LogD->Logging("Func", "into Pause_ImageSearcher Function");

	if (_FeatureSearcherPaused != true)
		_FeatureSearcherPaused = true;
}

void FeatureSearcher::Resume_FeatureSearcher() {
	G_LogD->Logging("Func", "into Resume_ImageSearcher Function");

	if (_FeatureSearcherPaused != false) {
		_FeatureSearcherPaused = false;
		// Pause 되면, 무조건 Thread가 Wait되어 있기 때문에
		// 이것을 풀어 주어야 한다.
		_WorkAssignSyncSignal.Signal();
	}
}

void FeatureSearcher::Push_ImageToQueue(Mat &Scene) {
	static bool _TIsAutoFocus = false;
	G_LogD->Logging("Func", "into Push_ImageToQueue Function, %d %d", _FeatureSearcherPaused, _FeatureSearcherActivated);

	// 중간에 SearchEngine이 중단될 수 있는 상황(_InputImageQueue전체가 Flush되는 상황)이
	// 벌어질 수도 있기 때문에 반드시 Mutex 걸어준다.
	// Engine이 중단된 상황, Engine이 활성화 되지 않은 상황에서는 집어 넣어서는 안된다.
	if (_FeatureSearcherPaused == false
			&& _FeatureSearcherActivated == true) {
		cv::Mat _TGrayMat;
		cv::cvtColor(Scene, _TGrayMat, CV_BGR2GRAY);

#if !defined(MODE_ONLY_DETECTION)
		// Feature detect-extract via FAST-ORB.
		FeatureDesc _TImageDetector(FeatureDescEnum::S_FAST, FeatureDescEnum::S_ORB);
#else
    // Feature detect via FAST.
    FeatureDesc _TImageDetector(FeatureDescEnum::S_FAST);
#endif
		FeatureSets _TFeatureSet;

#if !defined(MODE_ONLY_DETECTION)
		_TImageDetector.Go_DetectAndExtract(_TGrayMat, _TFeatureSet._Keypoints, _TFeatureSet._Descriptor);
#else
    _TImageDetector.Go_Detect(_TGrayMat, _TFeatureSet._Keypoints);
#endif
		_TFeatureSet._Cols = _TGrayMat.cols;
		_TFeatureSet._Rows = _TGrayMat.rows;

    // 없는 Descritor가 들어오는 것을 미리 방지
    int _TSceneMatSize = (int)(_TFeatureSet._Descriptor.cols * _TFeatureSet._Descriptor.rows *
#if !defined(MODE_ONLY_DETECTION)
        _TFeatureSet._Descriptor.elemSize()
#else
        _TFeatureSet._Keypoints.size()
#endif
    );

		//G_LogD->Logging("Func", "Feature Size : %d", _TSceneMatSize);
		if (_TSceneMatSize != 0) {
			__MUTEXLOCK(_Mutex_InputSceneDataQueue);
			// Queue Frame 수 제한.
			if (_InputSceneDataQueue.size() > CLIENT_IMAGE_SEARCHER_QUEUE_LIMIT)
				_InputSceneDataQueue.pop();

			_InputSceneDataQueue.push(_TFeatureSet);
			
			if (_FeatureQueueSyncSignal.Get_IsWaiting() == true) {
				_FeatureQueueSyncSignal.Signal();
			}

			__MUTEXUNLOCK(_Mutex_InputSceneDataQueue);
			_TIsAutoFocus = false;
		}
		else {
			if (_TSceneMatSize == 0 && _TIsAutoFocus == false) {
				G_ImageSearcher->TForceAutoFocusCallback();
				_TIsAutoFocus = true;
			}
		}
	}
}

#if !defined(MODE_ONLY_DETECTION)
void FeatureSearcher::Push_SearchableData(vector<FeatureData> SearchedData) {
	G_LogD->Logging("Func", "into Push_SearchableData Function");

	// 중간에 SearchEngine이 읽는 상황이 발생할 수 있기 때문에 반드시 Mutex 처리한다.
	// Engine이 중단된 상황, Engine이 활성화 되지 않은 상황에서는 집어 넣어서는 안된다.
	if (_FeatureSearcherPaused == false
			&& _FeatureSearcherActivated == true) {
		if (SearchedData.size() != 0) {
			__MUTEXLOCK(_Mutex_NavigateData);
			_NavigateData = SearchedData;
			_NavigateDataUpdated = true;
			__MUTEXUNLOCK(_Mutex_NavigateData);
		}
	}
}
#endif

#endif