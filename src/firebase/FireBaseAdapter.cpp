/*********************************************************************

          File: FireBaseAdapter.cpp

   Description: FireBaseAdapter Source Part.

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

#if defined(MAKE_TEST)

#include "FireBaseAdapter.hpp"

#include "StringListIter.hpp"

#if defined(LOG_WRITE_MODE)
#include "LogD.hpp"
#endif

FireBaseAdapter *G_Adapter;

FireBaseAdapter::FireBaseAdapter() {
  _Initialize_Members();
  _Initialize();
}

FireBaseAdapter::~FireBaseAdapter() {
  _Deinitialize();
  _Initialize_Members();
}

void FireBaseAdapter::_Initialize_Members() {
  _CamWidth = _FireBaseVideoPool.Get_CamWidth();
  _CamHeight = _FireBaseVideoPool.Get_CamHeight();

  _AdapterStarted = false;
  _EngineStarted = false;

  _ControlEnable = false;
}

void FireBaseAdapter::_Initialize() {
  G_Adapter = this;

  // console creates
#if defined(WINDOWS_SYS)
  //_ExConsoleAllCallback.Create("All Logs");
  //_ExConsoleAllCallback.cls(ExConsoleLoggerEx::COLOR_BACKGROUND_BLUE);

  //_ExConsoleFuncCallback.Create("Func Logs");
  //_ExConsoleFuncCallback.cls(ExConsoleLoggerEx::COLOR_BACKGROUND_BLACK);
  //_ExConsoleTimeCallback.Create("Processing Time Logs");
  //_ExConsoleTimeCallback.cls(ExConsoleLoggerEx::COLOR_BACKGROUND_BLACK);
  _ExConsoleDataCallback.Create("See Data Logs");
  _ExConsoleDataCallback.cls(ExConsoleLoggerEx::COLOR_BACKGROUND_BLACK);

  _ExConsoleData.Create("Data Sorted Logs");
  _ExConsoleData.cls(ExConsoleLoggerEx::COLOR_BACKGROUND_GREEN);
#endif
  _Initialze_Pointers();
}

void FireBaseAdapter::_Deinitialize() {
  _Deinitialize_Pointers();

  if (G_Adapter != NULL) G_Adapter = NULL;

  if (_FireBaseEngine != NULL) _FireBaseEngine = NULL;
}

void FireBaseAdapter::_Initialze_Pointers() {
  _FireBaseEngine = new FireBaseEngine();
}

void FireBaseAdapter::_Deinitialize_Pointers() {
  delete _FireBaseEngine;
}

void FireBaseAdapter::_ParseCommand(MessageInformations &__Msg) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _ParseCommand Function");
#endif
  string _TID = __Msg.UserInformation.UserID;
  string _TSendingCommand = "";

  StringTokenizer *_TStringTokenizer = new StringTokenizer();

  _TStringTokenizer->Set_InputCharString(__Msg.RecvMessage.c_str());
  _TStringTokenizer->Set_SingleToken(" ");

  if (_TStringTokenizer->Go_StringToken() == false) {
    delete _TStringTokenizer;
    return;
  }

  for_IterToEndC(list, string, _TStringTokenizer->Get_TokenedStringList(), i) {
    StringTokenizer *_TAtomicValues = new StringTokenizer();
    if (strcmp(_i->c_str(), "Engine_Start") == 0) {
      Start_Engine();
    }
    else if (strcmp(_i->c_str(), "Engine_Stop") == 0) {
      Stop_Engine();
    }
    else if (strcmp(_i->c_str(), "Pause_Engine") == 0) {
      _FireBaseEngine->Pause_FireBaseEngine();
    }
    else if (strcmp(_i->c_str(), "Resume_Engine") == 0) {
      _FireBaseEngine->Resume_FireBaseEngine();
    }
    else if (strcmp(_i->c_str(), "Quit") == 0) {
      Stop_GetCommand();
    }
    else if (strcmp(_i->c_str(), "Set_Fall") == 0) {
      _FireBaseEngine->Pause_FireBaseEngine();
    }
    else if (strcmp(_i->c_str(), "Set_Raise") == 0) {
      _FireBaseEngine->Resume_FireBaseEngine();
    }
    else if (strcmp(_i->c_str(), "Do_Control") == 0) {
      _ControlEnable = true;
      _TSendingCommand.append("Control_Started");
    }
    else if (strcmp(_i->c_str(), "No_Control") == 0) {
      _ControlEnable = false;
      _TSendingCommand.append("Control_Stopped");
    }
    else if (strcmp(_i->c_str(), "Initial_Connect") == 0) {
      // 접속자를 찾고, 접속자 안에서 동일한 ID가 존재하면 보낸다.
      // 단, Socket은 이더넷에 한해서 처리한다.
      // Your_ID가 여기에 들어간다.Y
      _TSendingCommand.append("Your_ID=");
      _TSendingCommand.append(_TID);
    }
    else {
      _TAtomicValues->Set_InputCharString((const char *) _i->c_str());
      _TAtomicValues->Set_SingleToken("=");

      if (_TAtomicValues->Go_StringToken() != true) {
        // Database String이 잘못 되었을 때.
        delete _TAtomicValues; // 중간에 나가기 때문에 반드시 delete 해준다.
        return;
      }

      StringListIter<char *> *_TIterationSeeker = new StringListIter<char *>(
          _TAtomicValues->Get_TokenedCharListArrays());

      if (strcmp((const char *) *_TIterationSeeker->Get_NowStringIter(), "Set_CamResolution") == 0) {
        StringTokenizer *_TCamResValues = new StringTokenizer();

        _TIterationSeeker->Move_NextStringIter();

        _TCamResValues->Set_InputCharString((const char *) *_TIterationSeeker->Get_NowStringIter());
        _TCamResValues->Set_SingleToken(",");

        if (_TCamResValues->Go_StringToken() == false) {
          delete _TCamResValues;
          return;
        }

        StringListIter<char *> *_TCamResValueSeeker = new StringListIter<char *>(
            _TCamResValues->Get_TokenedCharListArrays());

        _CamWidth = atoi((const char *) *_TCamResValueSeeker->Get_NowStringIter());
        _TCamResValueSeeker->Move_NextStringIter();
        _CamHeight = atoi((const char *) *_TCamResValueSeeker->Get_NowStringIter());

        delete _TCamResValueSeeker;
        delete _TCamResValues;

        // 엔진 일시 정지 및 Cam 잠시 정지.
        _FireBaseEngine->Pause_FireBaseEngine();

        // Camera Disable after Setting and Enable
        Stop_Streaming();

        Stop_Video();

        // Set Camera Resolution.
        _FireBaseVideoPool.Set_Resolution(_CamWidth, _CamHeight);
        // Restart Video.
        Start_Video();
        // Set Streaming Resolution.
        _ExternalStreamViewer.Set_Resolution(_CamWidth, _CamHeight);
        // Restart Sereaming.
        Start_Streaming();

        // 다시 시작.
        _FireBaseEngine->Resume_FireBaseEngine();
      }
      else if (strcmp((const char *) *_TIterationSeeker->Get_NowStringIter(), "Set_CamNum") == 0) {
        _TIterationSeeker->Move_NextStringIter();
        _CamPort = atoi((const char *) *_TIterationSeeker->Get_NowStringIter());

        // 엔진 일시 정지 및 Cam 잠시 정지.
        _FireBaseEngine->Pause_FireBaseEngine();

        // Camera Disable after Setting and Enable
        Stop_Streaming();
        Stop_Video();
        // Set Camera Port.
        _FireBaseVideoPool.Set_CameraPort(_CamPort);
        // Restart Video.
        Start_Video();
        Start_Streaming();

        // 다시 시작.
        _FireBaseEngine->Resume_FireBaseEngine();
      }
    }

    delete _TAtomicValues;
  }
  delete _TStringTokenizer;
}

void FireBaseAdapter::_SetInfomation(Mat &__FrameData) {
  // Image에 정보를 그린다.
  // 추후에 Data가 다 나오고 나서.
  __MUTEXLOCK(_Mutex_ViewPoint);
  for (int i = 0; i < _ViewPoint.size(); i++)
    cv::circle(__FrameData, Point(_ViewPoint.at(i).first, _ViewPoint.at(i).second), 10, Scalar(0, 0, 255), CV_FILLED);
  __MUTEXUNLOCK(_Mutex_ViewPoint);
}

template<typename T>
bool FireBaseAdapter::_IsEmptyQueue(queue<T> __Queue, ThreadMutex &__Mutex) {
  bool _TResult = false;

  __MUTEXLOCK(__Mutex);
  _TResult = __Queue.empty();
  __MUTEXUNLOCK(__Mutex);

  return _TResult;
}

/* Start SearchedResult Callback */
void FireBaseAdapter::_FireBaseAdapter_FeatureSearchedResult(
#if !defined(MODE_ONLY_DETECTION)
    vector<FeatureData>
#else
    FeatureSets
#endif
__Data) {
  __MUTEXLOCK(G_Adapter->_Mutex_ViewPoint);
  G_Adapter->_ViewPoint.clear();
  __MUTEXUNLOCK(G_Adapter->_Mutex_ViewPoint);
#if !defined(MODE_ONLY_DETECTION)
  for_IterToEnd(vector, FeatureData, __Data, i) {
#if defined(WINDOWS_SYS)
    G_Adapter->_ExConsoleData.cprintf("ImageSearching -> Name : %s, X : %d, Y : %d, Category : %s, Tel : %s\n",
      _i->comp_name_en.c_str(), _i->_DisplayPoint_X, _i->_DisplayPoint_Y, _i->comp_category.c_str(), _i->comp_tel.c_str());
    pair<int, int> _TPoint;
    _TPoint.first = _i->_DisplayPoint_X;
    _TPoint.second = _i->_DisplayPoint_Y;
    //__MUTEXLOCK(G_Adapter->_ViewPointMutex);
    G_Adapter->_ViewPoint.push_back(_TPoint);
    //__MUTEXUNLOCK(G_Adapter->_ViewPointMutex);
#endif
  }
#endif
}

void FireBaseAdapter::_FireBaseAdapter_AutoFocus(void) {

}

void FireBaseAdapter::_FireBaseAdapter_LaneSearchedResult(LaneDetectData __Data) {
  __MUTEXLOCK(G_Adapter->_Mutex_ViewPoint);
  G_Adapter->_ViewPoint.clear();
  __MUTEXUNLOCK(G_Adapter->_Mutex_ViewPoint);

}
/* End SearchedResult Callback */

/* Start External Callback */
// Message
void FireBaseAdapter::_FireBaseAdapter_ExternalRecvMessageCallback(MessageInformations __Msg) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseAdapter_ExternalRecvMessageCallback Function");
#endif
  // 큐에 집어 넣고 메세지가 왔다는 시그널을 준다.
  G_Adapter->_RecvCommandQueue.push(__Msg);
  G_Adapter->_SyncSignal_RecvCommandQueue.Signal();
}

// Video
void FireBaseAdapter::_FireBaseAdapter_ExternalVideoCallback(Mat __View) {
  if (G_Adapter->_FireBaseEngine->Get_EngineStart() == true) {
    G_Adapter->_FireBaseEngine->Push_NowFrameImage(__View);
  }

  G_Adapter->_ProcessingVideoQueue.push(__View);
  G_Adapter->_SyncSignal_ProcessingVideoQueue.Signal();
}

/* End External Callback */

void *FireBaseAdapter::_FireBaseAdapter_VideoPassingThroughProcessingThread(void *Param) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseAdapter_VideoPassingThroughProcessingThread Function");
#endif
  FireBaseAdapter *_TAdapter = (FireBaseAdapter *) Param;

  while (_TAdapter->_EngineStarted == true) {
    if (_TAdapter->_IsEmptyQueue(_TAdapter->_ProcessingVideoQueue, _TAdapter->_Mutex_ProcessingVideoQueue) != true) {
      Mat _TMat = _TAdapter->_ProcessingVideoQueue.front();

      // Processing my Result for View Drawing.

      // Go Streaming.
      if (_TAdapter->_ExternalStreamViewer.Get_ExternalStreamViewerStarted() == true)
        _TAdapter->_ExternalStreamViewer.Push_ImageQueue(_TMat);
      _TAdapter->_ProcessingVideoQueue.pop();
    }
    else
      _TAdapter->_SyncSignal_ProcessingVideoQueue.Wait();
  }
  return 0;
}

void *FireBaseAdapter::_FireBaseAdapter_ParseCommandThread(void *Param) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseAdapter_ParseCommandThread Function");
#endif
  FireBaseAdapter *_TAdapter = (FireBaseAdapter *) Param;

  // 모든 커넥션을 생성하고, 이것들을 깨운다.
  // 여기를 없에면, FireBase의 모든 Processing이 종료 된다.
  // for Connect pool Callbacks.
  _TAdapter->_FireBaseConnectPool.TRecvMessageCallback = _FireBaseAdapter_ExternalRecvMessageCallback;
  _TAdapter->_FireBaseConnectPool.Start_ConnectPool();

  // 커멘드가 되자마자 엔진 시작.
  // 엔진이 올라가면 엥간한 기능들이 전부 작동한다.
  //_TAdapter->Start_Engine();

  // 커넥션 큐에서 들어온 것들을 처리한다.
  while (_TAdapter->_AdapterStarted == true) {
    if (_TAdapter->_IsEmptyQueue(_TAdapter->_RecvCommandQueue, _TAdapter->_Mutex_RecvCommandQueue) != true) {
      MessageInformations _TRecvInfo = _TAdapter->_RecvCommandQueue.front();

      _TAdapter->_ParseCommand(_TRecvInfo);

      if (_TRecvInfo.SendMessage != "")
        _TAdapter->_FireBaseConnectPool.Send_Message(_TRecvInfo);

      _TAdapter->_RecvCommandQueue.pop();
    }
    else
      _TAdapter->_SyncSignal_RecvCommandQueue.Wait();
  }

  // 끝날때는 일단 엔진부터 종료.
  //_TAdapter->Stop_Engine();

  // 파서가 종료하면, 명령을 받을 수 있는 모든 것을 해제한다.
  _TAdapter->_FireBaseConnectPool.TRecvMessageCallback = NULL;
  _TAdapter->_FireBaseConnectPool.Stop_ConnectPool();

  return 0;
}

void *FireBaseAdapter::_FireBaseAdapter_SendCommandThread(void *Param) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseAdapter_SendCommandThread Function");
#endif
  FireBaseAdapter *_TAdapter = (FireBaseAdapter *) Param;

  // 커넥션 큐에서 들어온 것들을 처리한다.
  while (_TAdapter->_AdapterStarted == true) {
    if (_TAdapter->_IsEmptyQueue(_TAdapter->_SendCommandQueue, _TAdapter->_Mutex_SendCommandQueue) != true) {
      MessageInformations _TSendInfo = _TAdapter->_RecvCommandQueue.front();

      if (_TSendInfo.SendMessage != "")
        _TAdapter->_FireBaseConnectPool.Send_Message(_TSendInfo);

      _TAdapter->_RecvCommandQueue.pop();
    }
    else
      _TAdapter->_SyncSignal_RecvCommandQueue.Wait();
  }
  return 0;
}

void FireBaseAdapter::Start_Video() {
  if (_FireBaseVideoPool.Get_VideoPoolStarted() != true) {
#if defined(LOG_WRITE_MODE)
    G_LogD->Logging("Func", "into Start_Video Function");
#endif
    // for External Video Callback.
    _FireBaseVideoPool.TExternalVideoCallback = _FireBaseAdapter_ExternalVideoCallback;
    // Activate Video.
    _FireBaseVideoPool.Start_VideoPool();
  }
}

void FireBaseAdapter::Stop_Video() {
  if (_FireBaseVideoPool.Get_VideoPoolStarted() == true) {
#if defined(LOG_WRITE_MODE)
    G_LogD->Logging("Func", "into Stop_Video Function");
#endif
    _FireBaseVideoPool.TExternalVideoCallback = NULL;
    _FireBaseVideoPool.Stop_VideoPool();
    // Camera Safe Deactivate.
    while (_FireBaseVideoPool.Get_VideoPoolStarted() == true) m_sleep (33);
  }
}

void FireBaseAdapter::Start_Streaming() {
  if (_ExternalStreamViewer.Get_ExternalStreamViewerStarted() != true) {
#if defined(LOG_WRITE_MODE)
    G_LogD->Logging("Func", "into Start_Streaming Function");
#endif
    _ExternalStreamViewer.Start_ExternalStreamViewer();
  }
}

void FireBaseAdapter::Stop_Streaming() {
  if (_ExternalStreamViewer.Get_ExternalStreamViewerStarted() == true) {
#if defined(LOG_WRITE_MODE)
    G_LogD->Logging("Func", "into Stop_Streaming Function");
#endif
    _ExternalStreamViewer.Stop_ExternalStreamViewer();
    while (_ExternalStreamViewer.Get_ExternalStreamViewerStarted() == true) m_sleep (33);
  }
}

void FireBaseAdapter::Start_Engine() {
  if (_AdapterStarted == true
      && _EngineStarted != true) {
#if defined(LOG_WRITE_MODE)
    G_LogD->Logging("Func", "into Start_Engine Function");
#endif
    _EngineStarted = true;

    // for Core Engine Callbacks.
    _FireBaseEngine->TExternalFeatureSearchingResultPassingCallback = _FireBaseAdapter_FeatureSearchedResult;
    _FireBaseEngine->TExternalLaneSearchingResultPassingCallback = _FireBaseAdapter_LaneSearchedResult;
    _FireBaseEngine->TExternalForceAutofocusCallback = _FireBaseAdapter_AutoFocus;

    _VideoPassingThroughProcessingThread.StartThread(_FireBaseAdapter_VideoPassingThroughProcessingThread, this);

    Start_Video();
    Start_Streaming();

    // Activate Engine Thread.
    //_FireBaseEngine->Start_FireBaseEngine();
  }
}

void FireBaseAdapter::Stop_Engine() {
  if (_AdapterStarted == true
      && _EngineStarted == true) {
#if defined(LOG_WRITE_MODE)
    G_LogD->Logging("Func", "into Stop_Engine Function");
#endif
    //_FireBaseEngine->Stop_FireBaseEngine();

    Stop_Streaming();
    Stop_Video();

    _EngineStarted = false;
  }
}

void FireBaseAdapter::Start_GetCommand() {
#if defined(LOG_WRITE_MODE)
  G_LogD->SetCallback(_AllLogCallback);
#endif
  if (_AdapterStarted != true) {
#if defined(LOG_WRITE_MODE)
    G_LogD->Logging("Func", "into Start_GetCommand Function");
#endif
    _AdapterStarted = true;

    // Activate Command Parser Threads.
    // Must be cached ParseCommandThread.
    _ParseCommandThread.AttacheMode = true;
    _ParseCommandThread.StartThread(_FireBaseAdapter_ParseCommandThread, this);
    _ParseCommandThread.JoinThread();
  }
}

void FireBaseAdapter::Stop_GetCommand() {
  if (_AdapterStarted == true) {
#if defined(LOG_WRITE_MODE)
    G_LogD->Logging("Func", "into Stop_GetCommand Function");
#endif
    _AdapterStarted = false;
  }
}

#if defined(LOG_WRITE_MODE)
void FireBaseAdapter::_AllLogCallback(const char *__CallValueString, const char *__Messages) {
#if defined(WINDOWS_SYS)
  //G_Adapter->_ExConsoleAllCallback.cprintf("%s -> %s", __CallValueString, __Messages);

  if (strcmp(__CallValueString, "Func") == 0) {
    //G_Adapter->_ExConsoleFuncCallback.cprintf(ExConsoleLoggerEx::COLOR_WHITE, "%s", __Messages);
  }
  else if (strcmp(__CallValueString, "Time") == 0) {
    //G_Adapter->_ExConsoleTimeCallback.cprintf(ExConsoleLoggerEx::COLOR_WHITE, "%s", __Messages);
  }
  else if (strcmp(__CallValueString, "Data") == 0) {
    G_Adapter->_ExConsoleDataCallback.cprintf(ExConsoleLoggerEx::COLOR_WHITE, "%s", __Messages);
  }
  else if (strcmp(__CallValueString, "State") == 0) {
    //G_Adapter->_ExConsoleDataCallback.cprintf(ExConsoleLoggerEx::COLOR_WHITE, "%s", __Messages);
  }
#else
  printf("%s -> %s", __CallValueString, __Messages);
#endif
}
#endif // LOG_WRITE_MODE

#endif // MAKE_TEST