/*********************************************************************

          File: FeatureSearcher.hpp

   Description: FeatureSearcher Header Part.

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

#include "modinclude.hpp"

#if defined(SET_CORE_ENGINE_MODULE_FEATURESEARCHER)

#ifndef _FeatureSearcher_hpp_
#define _FeatureSearcher_hpp_

#include "FeatureDesc.hpp"

#include "CoreDefines.hpp"
#include "CoreVariables.hpp"

#include "SystemUtil.hpp" // using thread.

class FeatureSearcher {
private:
#if !defined(MODE_ONLY_DETECTION)
	// 일감 분배용 작업 정보 구조체.
	typedef struct _WorkingInformation {
		FeatureSets _SceneFeature;
		FeatureData _WorkingInfo;
		int _WorkingInfoIndex;
		FeatureSearcher *_PFeatureSearcher;
	} WorkingInformation;
#endif
	Thread _Thread;
	
	// Thread가 읽고, 외부에서 보내주는 공유 자원들.
	// 공유 자원은 항상 읽기/쓰기에 관한 처리가 되어야 함.
	queue<FeatureSets> _InputSceneDataQueue;
#if !defined(MODE_ONLY_DETECTION)
	vector<FeatureData> _NavigateData;
	vector<FeatureData> _SortedData; // 내부에서 정렬된 값. 요청에 의해서 버려질 수도 있음.
#endif
	bool _FeatureSearcherActivated;
	bool _FeatureSearcherStarted;
	bool _FeatureSearcherWorkAssignStarted;
	bool _FeatureSearcherPaused; // 모든 동작을 멈추게 한다.
#if !defined(MODE_ONLY_DETECTION)
	bool _NavigateDataUpdated;
#endif

	// Mutex들.
	ThreadMutex _Mutex_InputSceneDataQueue;
#if !defined(MODE_ONLY_DETECTION)
	ThreadMutex _Mutex_NavigateData;
	ThreadMutex _Mutex_SortedData;
#endif

	SyncSignal _FeatureSearcherActivatedSyncSignal;
	SyncSignal _FeatureQueueSyncSignal;

	SyncSignal _WorkAssignSyncSignal;

	void _Initialize_Members();
	void _Initialize();
	void _Deinitialize();

	void _ThrowUpImageQueue();
	bool _IsEmptyImageQueue();
#if !defined(MODE_ONLY_DETECTION)
	int _GetRandomWorkNumber(vector<int> __WorkChkArr);
	vector<int> _MakeWorkNumberList(vector<int> &__WorkChkArr, int __WorkCnt);
	vector<FeatureData> _MakeWorkList(vector<int> __WorkListArr);
#endif
	static void *_FeatureSearcher_WorkerThread(void *Param);
	static void *_FeatureSearcher_WorkAssignThread(void *Param);
	static void *_FeatureSearcher_MainThread(void *Param);

	template <typename T> inline void _QueueClear(queue<T> &_XQueue);

	//IMPLEMENT_GET_SET
public:
	FeatureSearcher();
	~FeatureSearcher();
	
	typedef void(*_T_FEATURESEARCHEDRESULTCALLBACK)(
#if !defined(MODE_ONLY_DETECTION)
      vector<FeatureData> SortedData
#else
      FeatureSets SceneFeatureSet
#endif
  );
  _T_FEATURESEARCHEDRESULTCALLBACK TFeatureSearchedResultCallback;

  // for Camera Auto-Focus
	typedef void(*_T_FORCEAUTOFOCUSCALLBACK)(void);
	_T_FORCEAUTOFOCUSCALLBACK TForceAutoFocusCallback;

	void Start_FeatureSearcher();
	void Stop_FeatureSearcher();
	void Pause_FeatureSearcher();
	void Resume_FeatureSearcher();

	// Image를 Queue에 Push 하는 함수. Queue 전체가 Flush되는 상황이 있을 수 있으므로
	// 내부에 Mutex를 걸어준다.
	void Push_ImageToQueue(Mat &Scene);
#if !defined(MODE_ONLY_DETECTION)
	void Push_SearchableData(vector<FeatureData> SearchedData);
#endif

	// Sort 된, 1frame에서 발견된 연관 Data를 가져다 주는 함수.
	//vector<ImageSearchedResult> Get_RelatedSortData();

	IMPLEMENT_GET(bool, FeatureSearcherStart, _FeatureSearcherStarted);
	//IMPLEMENT_GET(bool, ImageSearcherActivate, _ImageSearcherActivated);
	IMPLEMENT_GET(bool, FeatureSearcherPause, _FeatureSearcherPaused);
};

template <typename T>
inline void FeatureSearcher::_QueueClear(queue<T> &_XQueue) {
	queue<T> _TEmptyQueue;
	std::swap(_XQueue, _TEmptyQueue);
}

#endif

#endif