/*********************************************************************

          File: FireBaseEngine.cpp

   Description: FireBaseEngine Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/30

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

#include "FireBaseEngine.hpp"

#if defined(LOG_WRITE_MODE)
#include "LogD.hpp"
#endif

FireBaseEngine *G_Engine;

// Area of Constructor & Destructor.
#pragma region Constructor & Destructor
FireBaseEngine::FireBaseEngine() {
	_Initialize_Members();
	_Initialize();
}

FireBaseEngine::~FireBaseEngine() {
	_Deinitialize();
	_Initialize_Members();
}
#pragma endregion Constructor & Destructor

// Area of Private Methods.
#pragma region Private Methods
void FireBaseEngine::_Initialize_Members() {
	_EngineStarted = false;
	_EngineActivated = false;
	_EnginePaused = false;

	_QueueClear(_SortedFeatureSearchResult);
	_QueueClear(_SortedLaneSearchResult);
}

void FireBaseEngine::_Initialize() {
	G_Engine = this;

	_FeatureSearcher = NULL;
  _LaneSearcher = NULL;

	TExternalFeatureSearchingResultPassingCallback = NULL;
	TExternalLaneSearchingResultPassingCallback = NULL;
	TExternalForceAutofocusCallback = NULL;

	_Initialze_Pointers();
}

void FireBaseEngine::_Deinitialize() {
	// Engine 삭제시, 안전 종료 처리.
	if (_EngineActivated == true){
    Stop_FireBaseEngine();
		while(_EngineStarted == true) m_sleep(33);
	}

	_Deinitialze_Pointers();

	if (G_Engine != NULL)	G_Engine = NULL;

	if (_FeatureSearcher != NULL) _FeatureSearcher = NULL;
	if (_LaneSearcher != NULL) _LaneSearcher = NULL;
}

void FireBaseEngine::_Initialze_Pointers() {
	_FeatureSearcher = new FeatureSearcher();
  _LaneSearcher = new LaneSearcher();
}

void FireBaseEngine::_Deinitialze_Pointers() {
	delete _FeatureSearcher;
	delete _LaneSearcher;
}

void FireBaseEngine::_FireBaseEngine_FeatureSearchedResult(
#if !defined(MODE_ONLY_DETECTION)
    vector<FeatureData> __SortedData
#else
    FeatureSets __SceneFeatureSet
#endif
) {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into _ClientEngine_ImageSearchedResult Function");
#endif
#if !defined(MODE_ONLY_DETECTION)
	// 이것은 Engine 내부의 Queue로 가며, Engine 내부 Thread에서 검색의 결과를 다시 바깥단으로 발송한다.
	// 유의미 Data란, Callback에 Data가 들어온 시각을 기준으로 Data에 Time Stamp를 주고
	// 나중에 다시 들어왔을때 이를 점검하여 Data의 시각을 Check 하여 그 시각에서 유의미한 범위에 있는
	// Data인지 아닌지의 여부를 판별하여 유의미 Data를 판별.

	static vector<pair<FeatureData, long> > _TSummnationData;
	static int _TVoidCount = 0;
	static clock_t _TBeforeTime = 0;

	clock_t _TClock = clock();

	// 갑자기 잘 쌓다가 오랜시간 정지해 버린 경우, 시간을 비교하여 모든 Vector List를 날려버린다.
	// 한마디로, Callback이 오랫동안 오지 않은 경우(Engine의 Pause, Image Searcher의 Pause 등으로..)
	// 기존에 있던 Data가 남아 있기 때문에 지속적으로 표시 되는 것을 방지하기 위해서이다.
	if (_TBeforeTime == 0) {
		// _TBeforeTime이 0인 경우는 무조건적으로 처음 이 Callback이 호출될 때이다.
		_TBeforeTime = _TClock;
	}
	else {
		// 2초(Data 유의미 시각)동안 Callback이 들어오지 않으면? 삭제.
		if ((double)((_TClock - _TBeforeTime) / CLOCKS_PER_SEC) > CLIENT_STOP_LIMIT_SECOND) {
			_TSummnationData.clear();

		}
		//G_LogD->Logging("Data", "Passed by %lf Second.\n", (double)((_TClock - _TBeforeTime) / CLOCKS_PER_SEC));
	}

	// 변화량 Data 생성.
	if (__SortedData.size() != 0) {
		for_IterToEnd(vector, InnerFormalizaionData, __SortedData, i) {
			bool _TExistData = false;
			// find.
			for (int j = 0; j < _TSummnationData.size(); j++) {
				if (strcmp(_TSummnationData[j].first.Key.c_str(), _i->Key.c_str()) == 0) {
					// 그리고 나서 break.
					_TSummnationData[j].second = _TClock;
					_TSummnationData[j].first = *_i;
					_TExistData = true;
					break;
				}
			}

			if (_TExistData == false) {
				// 새로 들어 가야할 Data.
				pair<FeatureData,
				 long // 시간 Data(이전 시각).
				> _TData;
				// Data를 넣는다.
				_TData.first = *_i;
				_TData.second = _TClock;
				_TSummnationData.push_back(_TData);
			}
		}

		// 변화량 없는 Data 거르기.
		// 있으면 이전 시간을 비교하고, 이전시간에 비해 차이가 굉장히 적은 수치(유의미 수치)의 차이를 결정한다.
		// 이전 시각과 현재 시각이 2초(Data 유의미 시각) 이상 차이 나면, 변화가 없다고 판단하여 삭제한다.
		vector<pair<FeatureData, long> >::iterator _TIterator;
		for (_TIterator = _TSummnationData.begin();
			_TIterator != _TSummnationData.end(); ) {
			long _TTimeCheck = _TClock - _TIterator->second;

			if (_TTimeCheck > 0 && (double)(_TTimeCheck / CLOCKS_PER_SEC) > CLIENT_DATA_MEANING_SECOND) {
				_TIterator = _TSummnationData.erase(_TIterator);
			}
			else
				_TIterator++;
		}

		_TVoidCount = 0;
	}
	else {
		if (_TVoidCount > CLIENT_DATA_VOID_LIMIT_COUNT) {
			// Data가 비어 있는 횟수가 100회를 넘으면.. 다음 상황을 가르킨다.
			// ** 화면에 현재 찾을수 있는 Data는 없다(그것도 100회 이상).
			// 화면에 보이는 유의미 데이터는 없으므로, 무조건 삭제한다.
			_TSummnationData.clear();
			_TVoidCount = 0;
		}
		else
			_TVoidCount++;
	}

	vector<FeatureData> _TCompleteData;

	// 다 정렬된 Data는, InnerFormalizaionData들만 꺼내어 집어 넣는다.
	vector<pair<FeatureData, long> >::iterator _TIterator;
	for (_TIterator = _TSummnationData.begin();
		_TIterator != _TSummnationData.end(); _TIterator++) {
		_TCompleteData.push_back(_TIterator->first);
	}

	// 시간 갱신.
	_TBeforeTime = _TClock;
#else
	// 지금부터 코딩 신나게 들어가야함.
#endif

  // 다 완료 되었으면, Queue에 집어 넣는다.
  if (G_Engine->_SortedFeatureSearchResult.size() > CLIENT_DATA_QUEUE_LIMIT)
    G_Engine->_SortedFeatureSearchResult.pop();
  G_Engine->_SortedFeatureSearchResult.push(
#if !defined(MODE_ONLY_DETECTION)
      _TCompleteData
#else
      __SceneFeatureSet
#endif
      );

  // 0개 일 때 대기하고 있던 Thread를 Data가 들어오면 '무조건' 깨운다.
  if (G_Engine->_FeatureSearchedExternalCallSyncSignal.Get_IsWaiting() == true)
    G_Engine->_FeatureSearchedExternalCallSyncSignal.Signal();
}

void FireBaseEngine::_FireBaseEngine_LaneSearchedResult(LaneDetectData __LaneSceneResult) {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into _ClientEngine_DatabaseSearchedResult Function");
#endif
  // 지금부터 코딩 신나게 들어가야함.
}

void FireBaseEngine::_FireBaseEngine_ForcedAutoFocus() {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into _ClientEngine_ForcedAutoFocus Function");
#endif
	if (G_Engine->TExternalForceAutofocusCallback != NULL)
		G_Engine->TExternalForceAutofocusCallback();
}

void *FireBaseEngine::_FireBaseEngine_FeatureSearchedExternalCallThread(void *Param) {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into _ClientEngine_ImageSearchedExternalCallThread Function");
#endif
	FireBaseEngine *_TEngine = (FireBaseEngine *)Param;

	while (_TEngine->_EngineActivated == true) {
		if (_TEngine->_SortedFeatureSearchResult.size() != 0) {
			if (G_Engine->TExternalFeatureSearchingResultPassingCallback != NULL)
				_TEngine->TExternalFeatureSearchingResultPassingCallback(_TEngine->_SortedFeatureSearchResult.front());
			_TEngine->_SortedFeatureSearchResult.pop();
		}
		else {
			// 0개 일때는, Thread를 대기상태로 둔다.
			_TEngine->_FeatureSearchedExternalCallSyncSignal.Wait();
		}
	}

	return 0;
}

void *FireBaseEngine::_FireBaseEngine_LaneSearchedExternalCallThread(void *Param) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _ClientEngine_ImageSearchedExternalCallThread Function");
#endif
  FireBaseEngine *_TEngine = (FireBaseEngine *)Param;

  while (_TEngine->_EngineActivated == true) {
    if (_TEngine->_SortedFeatureSearchResult.size() != 0) {
      if (G_Engine->TExternalLaneSearchingResultPassingCallback != NULL)
        _TEngine->TExternalLaneSearchingResultPassingCallback(_TEngine->_SortedLaneSearchResult.front());
      _TEngine->_SortedFeatureSearchResult.pop();
    }
    else {
      // 0개 일때는, Thread를 대기상태로 둔다.
      _TEngine->_LaneSearchedExternalCallSyncSignal.Wait();
    }
  }

  return 0;
}

void *FireBaseEngine::_FireBaseEngine_MainProcessingThread(void *Param) {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into _ClientEngine_MainProcessingThread Function");
#endif
	FireBaseEngine *_TEngine = (FireBaseEngine *)Param;
	Thread _TFeatureSearchedToExternalThread, _TLaneSearchedToExternalThread;

	/*
		Engine의 실행 순서.
		1. Thread Start에 대한 등록.
		2. Pointer Initialize.
		3. Engines(Image Searcher) Start.
		4. Engines(Image Searcher) Stop.
		5. Engines(Image Searcher) 종료 대기.
		6. Pointer Deinitialize.
		7. Thread End에 대한 등록.
	*/

	_TEngine->_EngineStarted = true;
	// 1. 실행 할 Class에 대한 Initialize.
	//_TEngine->_Initialze_Pointers();

	// 2. Engine에서의 중요 요소들을 Start 시킴.
  _TEngine->_FeatureSearcher->Start_FeatureSearcher();
	_TEngine->_LaneSearcher->Start_LaneSearcher();

	// ImageSearcher에 있는 Callback을 연결한다.
	_TEngine->_FeatureSearcher->TFeatureSearchedResultCallback = _FireBaseEngine_FeatureSearchedResult;
	_TEngine->_FeatureSearcher->TForceAutoFocusCallback = _FireBaseEngine_ForcedAutoFocus;
	// DataAutomator에 있는 Callback을 연결한다.
	_TEngine->_LaneSearcher->TLaneSearchedResultCallback = _FireBaseEngine_LaneSearchedResult;


	// Thread Attatched(내부/외부 Data 전달용 Thread).
	_TFeatureSearchedToExternalThread.AttacheMode = true;
	_TLaneSearchedToExternalThread.AttacheMode = true;
  _TFeatureSearchedToExternalThread.StartThread(_TEngine->_FireBaseEngine_FeatureSearchedExternalCallThread, _TEngine);
	_TLaneSearchedToExternalThread.StartThread(_TEngine->_FireBaseEngine_LaneSearchedExternalCallThread, _TEngine);

	_TEngine->_EngineActivatedSyncSignal.Wait();
	/*
	while (_TEngine->_EngineActivated) {
		// CPU 점유율을 낮추기 위해 Sleep을 걸어줌.
		// 약 100ms.
		m_sleep(33 * 3);
	}
	*/
	// 먼저 _ClientEngine_ImageSearchedExternalCallThread는 Join을 한다.
	// Join이 완료 되면 완전히 정상 종료 된 것이다.
	// 내부/외부 Data 전달용 Thread부터 정상 종료 확인 한 후 다른 Engine을 종료한다.
	_TFeatureSearchedToExternalThread.JoinThread();
	_TLaneSearchedToExternalThread.JoinThread();

	// 3. Engine에서의 중요 요소들을 Stop 시킴.
	_TEngine->_LaneSearcher->Stop_LaneSearcher();
	_TEngine->_FeatureSearcher->Stop_FeatureSearcher();

	// 4. 종료 대기(혹시나 모를 비정상 종료).
	while (_TEngine->_FeatureSearcher->Get_FeatureSearcherStart()) m_sleep(20);
	while (_TEngine->_LaneSearcher->Get_LaneSearcherStart()) m_sleep(20);

	// 5. 종료할 Class에 대한 Deinitialize.
	//_TEngine->_Deinitialze_Pointers();

	_TEngine->_EngineStarted = false;

	return 0;
}
#pragma endregion Private Methods

// Area of Public Methods.
#pragma region Public Methods
void FireBaseEngine::Start_FireBaseEngine() {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into Start_ClientEngine Function");
#endif
	if (_EngineActivated != true) {
		_EngineActivated = true;
		// Engine Thread Start.
    _Thread.StartThread(_FireBaseEngine_MainProcessingThread, this);
	}
}

void FireBaseEngine::Stop_FireBaseEngine() {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into Stop_ClientEngine Function");
#endif
	if (_EngineActivated != false) {
		_EngineActivated = false;
		if (_EngineActivatedSyncSignal.Get_IsWaiting() == true)
			_EngineActivatedSyncSignal.Signal();
	}
}

void FireBaseEngine::Pause_FireBaseEngine() {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into Pause_ClientEngine Function");
#endif
	if (_EnginePaused == false) {
		_EnginePaused = true;

		// 둘다 사용자에 의한 일시정지 요청.
    _FeatureSearcher->Pause_FeatureSearcher();
    _LaneSearcher->Pause_LaneSearcher();
	}
}

void FireBaseEngine::Resume_FireBaseEngine() {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into Resume_ClientEngine Function");
#endif
	if (_EnginePaused == true) {
		_EnginePaused = false;

		// 사용자에 의한 재시작 요청.
    _FeatureSearcher->Resume_FeatureSearcher();
    _LaneSearcher->Resume_LaneSearcher();
	}
}

void FireBaseEngine::Push_NowFrameImage(Mat NowFrameImage) {
//#if defined(LOG_WRITE_MODE)
//	G_LogD->Logging("Func", "into Push_NowFrameImage Function");
//#endif
	// _ImageSearcher가 NULL 이거나, _ImageSearcher의 Engine이
	// Pause이거나, Engine의 _EngineStarted가 true 되어 있지 않을때 절대로
	// Image를 Push 해서는 안된다.
	if (_FeatureSearcher != NULL) {
		if (_FeatureSearcher->Get_FeatureSearcherStart() == true
				&& _FeatureSearcher->Get_FeatureSearcherPause() == false) {
			//_ImageSearcher->_InputImageQueue.push(NowFrameImage);
			_FeatureSearcher->Push_ImageToQueue(NowFrameImage);
		}
	}
}

/*
vector<ImageSearchedResult> Engine::Get_ImageSearcherMakedData() {
	return _ImageSearcher->Get_RelatedSortData();
}
*/
#pragma endregion Public Methods
