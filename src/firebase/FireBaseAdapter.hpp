/*********************************************************************

          File: FireBaseAdapter.hpp

   Description: FireBaseAdapter Header Part.

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

#ifndef _FireBaseAdapter_hpp_
#define _FireBaseAdapter_hpp_

#if defined(MAKE_TEST)

#include "FireBaseEngine.hpp"

#include "FireBaseConnectPool.hpp"
#include "FireBaseVideoPool.hpp"

// for Streaming
#include "ExternalStreamViewer.hpp"

#if defined(WINDOWS_SYS)
#include "ExConsole/ConsoleLogger.hpp"
#endif

class FireBaseAdapter {
private:
	FireBaseEngine *_FireBaseEngine;
  FireBaseConnectPool _FireBaseConnectPool;
  FireBaseVideoPool _FireBaseVideoPool;
  ExternalStreamViewer _ExternalStreamViewer;

	Thread _ParseCommandThread;
  Thread _VideoPassingThroughProcessingThread;

  queue<MessageInformations> _RecvCommandQueue;
  ThreadMutex _Mutex_RecvCommandQueue;
  SyncSignal _SyncSignal_RecvCommandQueue;

  queue<MessageInformations> _SendCommandQueue;
  ThreadMutex _Mutex_SendCommandQueue;
  SyncSignal _SyncSignal_SendCommandQueue;

  queue<Mat> _ProcessingVideoQueue;
  ThreadMutex _Mutex_ProcessingVideoQueue;
  SyncSignal _SyncSignal_ProcessingVideoQueue;

  vector<pair<int, int> > _ViewPoint;
  ThreadMutex _Mutex_ViewPoint;

  int _CamWidth;
  int _CamHeight;
  int _CamPort;

	bool _AdapterStarted;
	bool _EngineStarted;

  bool _ControlEnable;

#if defined(WINDOWS_SYS)
	ExConsoleLoggerEx _ExConsoleAllCallback;
	ExConsoleLoggerEx _ExConsoleFuncCallback;
	ExConsoleLoggerEx _ExConsoleTimeCallback;
	ExConsoleLoggerEx _ExConsoleDataCallback;
	ExConsoleLoggerEx _ExConsoleStateCallback;
	ExConsoleLoggerEx _ExConsoleData;
#endif

  void _Initialize_Members();
	void _Initialize();
	void _Deinitialize();

  void _Initialze_Pointers();
  void _Deinitialize_Pointers();

	void _ParseCommand(MessageInformations &__Msg);
	void _SetInfomation(Mat &__FrameData);
  template <typename T> bool _IsEmptyQueue(queue<T> __Queue, ThreadMutex &__Mutex);

	static void _FireBaseAdapter_FeatureSearchedResult(
#if !defined(MODE_ONLY_DETECTION)
			vector<FeatureData>
#else
      FeatureSets
#endif
			__Data);
	static void _FireBaseAdapter_AutoFocus(void);
  static void _FireBaseAdapter_LaneSearchedResult(LaneDetectData __Data);

  // Message Callback by "FireBaseConnectPool"
  static void _FireBaseAdapter_ExternalRecvMessageCallback(MessageInformations __Msg);
  static void _FireBaseAdapter_ExternalVideoCallback(Mat __View);

#if defined(LOG_WRITE_MODE)
	static void _AllLogCallback(const char *__CallValueString, const char *__Messages);
#endif
  static void *_FireBaseAdapter_VideoPassingThroughProcessingThread(void *Param);
	static void *_FireBaseAdapter_ParseCommandThread(void *Param);
  static void *_FireBaseAdapter_SendCommandThread(void *Param);

public:
	FireBaseAdapter();
  ~FireBaseAdapter();

  void Start_Video();
  void Stop_Video();
  void Start_Streaming();
  void Stop_Streaming();
	void Start_Engine();
	void Stop_Engine();

	void Start_GetCommand();
	void Stop_GetCommand();

  IMPLEMENT_GET(bool, AdapterStarted, _AdapterStarted)
};

#endif

#endif
