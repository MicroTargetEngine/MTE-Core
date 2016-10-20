/*********************************************************************

          File: LaneSearcher.hpp

   Description: LaneSearcher Header Part.

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

#include "modinclude.hpp"

#if defined(SET_CORE_ENGINE_MODULE_LANESEARCHER)

#ifndef _LaneSearcher_hpp_
#define _LaneSearcher_hpp_

#include "ImageStitcher.hpp"
#include "MathematicalFunc.hpp"

#include "CoreDefines.hpp"
#include "CoreVariables.hpp"

#include "SystemUtil.hpp" // using thread.

class LaneSearcher {
private:
  // 일감 분배용 작업 정보 구조체.
  typedef struct _LaneSearcherWorkingInformation {
    Mat _SceneMat;
    int _WorkingInfoIndex;
    vector<Vec4i> _HoughLineResult;
    LaneSearcher *_PFeatureSearcher;
  } LaneSearcherWorkingInformation;

  Thread _Thread;

  // Thread가 읽고, 외부에서 보내주는 공유 자원들.
  // 공유 자원은 항상 읽기/쓰기에 관한 처리가 되어야 함.
  queue<Mat> _InputSceneDataQueue;
  bool _LaneSearcherActivated;
  bool _LaneSearcherStarted;
  bool _LaneSearcherWorkAssignStarted;
  bool _LaneSearcherPaused; // 모든 동작을 멈추게 한다.

  //bool _NavigateDataUpdated;

  // Mutex들.
  ThreadMutex _Mutex_InputSceneDataQueue;

  SyncSignal _LaneSearcherActivatedSyncSignal;
  SyncSignal _InputSceneDataQueueSyncSignal;

  SyncSignal _WorkAssignSyncSignal;

  void _Initialize_Members();

  void _Initialize();
  void _Deinitialize();

  void _ThrowUpImageQueue();
  bool _IsEmptyImageQueue();

  static void *_LaneSearcher_WorkerThread(void *Param);
  static void *_LaneSearcher_WorkAssignThread(void *Param);
  static void *_LaneSearcher_MainThread(void *Param);

  template<typename T> inline void _QueueClear(queue<T> &_XQueue);

public:
  LaneSearcher();
  ~LaneSearcher();

  //LogD *ProgLog;
  typedef void(*_T_LANESEARCHEDRESULTCALLBACK)(LaneDetectData LaneSceneResult);
  _T_LANESEARCHEDRESULTCALLBACK TLaneSearchedResultCallback;

  void Start_LaneSearcher();
  void Stop_LaneSearcher();
  void Pause_LaneSearcher();
  void Resume_LaneSearcher();

  // Image를 Queue에 Push 하는 함수. Queue 전체가 Flush되는 상황이 있을 수 있으므로
  // 내부에 Mutex를 걸어준다.
  void Push_ImageToQueue(Mat &Scene);

  IMPLEMENT_GET(bool, LaneSearcherStart, _LaneSearcherStarted);
  IMPLEMENT_GET(bool, LaneSearcherPause, _LaneSearcherPaused);
};

template<typename T>
inline void LaneSearcher::_QueueClear(queue<T> &_XQueue) {
  queue<T> _TEmptyQueue;
  std::swap(_XQueue, _TEmptyQueue);
}

#endif

#endif