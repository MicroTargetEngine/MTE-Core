/*********************************************************************

          File: FireBaseEngine.hpp

   Description: FireBaseEngine Header Part.

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

#ifndef _FireBaseEngine_hpp_
#define _FireBaseEngine_hpp_

#include "SystemUtil.hpp"

#if defined(SET_CORE_ENGINE_MODULE_FEATURESEARCHER)
#include "FeatureSearcher.hpp"
#endif
#if defined(SET_CORE_ENGINE_MODULE_LANESEARCHER)
#include "LaneSearcher.hpp"
#endif
#if defined(SET_CORE_ENGINE_MODULE_DATAAUTOMATOR)
#include "DataAutomator.hpp"
#else
#include "StringTokenizer.hpp"
#endif
class FireBaseEngine {
private:
	// 임의 조작이 불가능한 변수들.
	queue<
#if !defined(MODE_ONLY_DETECTION)
      vector<FeatureData>
#else
      FeatureSets
#endif
  > _SortedFeatureSearchResult;
	queue<LaneDetectData > _SortedLaneSearchResult;
	Thread _Thread;
	// 임의 조작이 불가능한 변수들. Get Function에 의해 가능한 것들의 집합.
	bool _EngineStarted;
	bool _EngineActivated;
	bool _EnginePaused; // 모든 동작을 멈추게 한다.

  SyncSignal _EngineActivatedSyncSignal;

	SyncSignal _FeatureSearchedExternalCallSyncSignal;
  SyncSignal _LaneSearchedExternalCallSyncSignal;

	// 임의 조작 가능한 Pointer들.
#if defined(SET_CORE_ENGINE_MODULE_FEATURESEARCHER)
	FeatureSearcher *_FeatureSearcher;
#endif
#if defined(SET_CORE_ENGINE_MODULE_LANESEARCHER)
  LaneSearcher *_LaneSearcher;
#endif
#if defined(SET_CORE_ENGINE_MODULE_DATAAUTOMATOR)
	DataAutomator *_DataAutomator;
#endif

#pragma region Private Functions
	void _Initialize_Members();
	void _Initialize();
	void _Deinitialize();

	void _Initialze_Pointers();
	void _Deinitialze_Pointers();

	// Internal Callback Functions(실제 Static 함수 등록. Any to Engine).
	// 주로 외부에서 정형화 완료된 것들.
	// 이미지 검색의 결과, 텍스트 검색의 결과 등.
	static void _FireBaseEngine_FeatureSearchedResult(
#if !defined(MODE_ONLY_DETECTION)
      vector<FeatureData> __SortedData
#else
      FeatureSets __SceneFeatureSet
#endif
  );
	static void _FireBaseEngine_ForcedAutoFocus(void);
  static void _FireBaseEngine_LaneSearchedResult(LaneDetectData LaneSceneResult);

	// thread functions.
	static void *_FireBaseEngine_FeatureSearchedExternalCallThread(void *Param);
	static void *_FireBaseEngine_LaneSearchedExternalCallThread(void *Param);
	static void *_FireBaseEngine_MainProcessingThread(void *Param);

	template <typename T> inline void _QueueClear(queue<T> &_XQueue);
#pragma endregion Private Functions
public:
	FireBaseEngine();
	~FireBaseEngine();

	// External Callback(외부->Core 등록용)
	// for Feature Search(Result)
	typedef void(*_T_EXTERNALFEATURESEARCHINGRESULTPASSINGCALLBACK)(
#if !defined(MODE_ONLY_DETECTION)
      vector<FeatureData> SortedData
#else
      FeatureSets SceneFeatureSet
#endif
  );
	_T_EXTERNALFEATURESEARCHINGRESULTPASSINGCALLBACK TExternalFeatureSearchingResultPassingCallback;
	// for Feature Search(Autofocus)
	typedef void(*_T_EXTERNALFORCEAUTOFOCUSSETCALLBACK)(void);
	_T_EXTERNALFORCEAUTOFOCUSSETCALLBACK TExternalForceAutofocusCallback;
	// for Lane Search.
  typedef void(*_T_EXTERNALLANESEARCHINGRESULTPASSINGCALLBACK)(LaneDetectData LaneSceneResult);
  _T_EXTERNALLANESEARCHINGRESULTPASSINGCALLBACK TExternalLaneSearchingResultPassingCallback;

	// 바깥에서 제어 가능한 중요 함수들.
	void Start_FireBaseEngine();
	void Stop_FireBaseEngine();
	void Pause_FireBaseEngine();
	void Resume_FireBaseEngine();

#if defined(SET_CORE_ENGINE_MODULE_DATAAUTOMATOR)
	void Select_NativeDataSearchMode(bool ModeSelect);
#endif

	// ** ImageSearcher가 필요로 하는 것.
	void Push_NowFrameImage(Mat NowFrameImage); // Image Searcher에만 적용.

	IMPLEMENT_GET(bool, EngineStart, _EngineStarted);
	IMPLEMENT_GET(bool, EngineActivate, _EngineActivated);
	IMPLEMENT_GET(bool, EnginePause, _EnginePaused);
#if defined(SET_CORE_ENGINE_MODULE_FEATURESEARCHER)
	IMPLEMENT_GET(FeatureSearcher *, FeatureSearcherEngine, _FeatureSearcher);
#endif
#if defined(SET_CORE_ENGINE_MODULE_LANESEARCHER)
  IMPLEMENT_GET(LaneSearcher *, LaneSearcherEngine, _LaneSearcher);
#endif
#if defined(SET_CORE_ENGINE_MODULE_DATAAUTOMATOR)
	IMPLEMENT_GET(DataAutomator *, DataAutomatorEngine, _DataAutomator);
#endif
};

template <typename T>
inline void FireBaseEngine::_QueueClear(queue<T> &_XQueue) {
	queue<T> _TEmptyQueue;
	std::swap(_XQueue, _TEmptyQueue);
}

#endif