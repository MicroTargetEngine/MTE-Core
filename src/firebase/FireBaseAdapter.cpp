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
#include "RandomID.hpp"

#if defined(LOG_WRITE_MODE)
#include "LogD.hpp"
#endif

FireBaseAdapter *G_Adapter;

FireBaseAdapter::FireBaseAdapter() {
	G_Adapter = this;

  _Initialize_Members();
}

FireBaseAdapter::~FireBaseAdapter() {
  G_Adapter = NULL;

  _Initialize_Members();
}

void FireBaseAdapter::_Initialize_Members() {
  _CLIID = "";
  _SerialID = "";

  _CamResWidth = CV_CAP_PROP_FRAME_WIDTH;
  _CamResHeight = CV_CAP_PROP_FRAME_HEIGHT;
  _CamNum = 0;

  _AdapterStarted = false;
  _EngineStarted = false;
}

void FireBaseAdapter::_Initialize() {
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	_PutAuthor();

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

	_FireBaseEngine = new FireBaseEngine();
}

void FireBaseAdapter::_Deinitialize() {
	delete _FireBaseEngine;
}

void FireBaseAdapter::_PutAuthor() {
	_SendToCommandLine("FireBase-Core Engine TEST Version %s.", ENGINE_EXEC_VER);
	_SendToCommandLine("Project \"FireBase\", Team \"RobotSarang\", Open Robot Marathon, IRC.\nCopyright (c) 2016 All right reserved.");
	_SendToCommandLine("Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)");
}

string FireBaseAdapter::_GetCLICommandStr() {
  char *_TCommand = (char *)calloc(BUFFER_MAX_32767, sizeof(char));
  string _TCommandString = "";
  memset(_TCommand, 0, sizeof(_TCommand));

	if (fgets(_TCommand, BUFFER_MAX_32767, stdin) == NULL) {
		if (feof(stdin)) {
			return "";
		}
	}

	char *_StrPtr = strchr(_TCommand,'\n');

	if (_StrPtr != NULL)
		*_StrPtr = '\0';

  _TCommandString.append(_TCommand);
  memset(_TCommand, 0, sizeof(_TCommand));
  free(_TCommand);

  return _TCommandString;
}

void FireBaseAdapter::_ParseCommand(string __Command) {
	StringTokenizer *_TStringTokenizer = new StringTokenizer();

	_TStringTokenizer->Set_InputCharString(__Command.c_str());
	_TStringTokenizer->Set_SingleToken(" ");

	if (_TStringTokenizer->Go_StringToken() == false) {
		delete _TStringTokenizer;
		return ;
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
			Stop_Adapter();
		}
		else if (strcmp(_i->c_str(), "Set_Fall") == 0) {
      _FireBaseEngine->Pause_FireBaseEngine();
		}
		else if (strcmp(_i->c_str(), "Set_Raise") == 0) {
      _FireBaseEngine->Resume_FireBaseEngine();
		}
    else {
      _TAtomicValues->Set_InputCharString((const char *)_i->c_str());
      _TAtomicValues->Set_SingleToken(" ");

      if (_TAtomicValues->Go_StringToken() != true) {
        // Database String이 잘못 되었을 때.
        delete _TAtomicValues; // 중간에 나가기 때문에 반드시 delete 해준다.
        return ;
      }

      StringListIter<char *> *_TIterationSeeker = new StringListIter<char *>(_TAtomicValues->Get_TokenedCharListArrays());

      if (strcmp((const char *)*_TIterationSeeker->Get_NowStringIter(), "Set_CamResolution") == 0) {
        StringTokenizer *_TCamResValues = new StringTokenizer();

        _TIterationSeeker->Move_NextStringIter();

        _TCamResValues->Set_InputCharString((const char *)*_TIterationSeeker->Get_NowStringIter());
        _TCamResValues->Set_SingleToken(",");

        if (_TCamResValues->Go_StringToken() == false) {
          delete _TCamResValues;
          return ;
        }

        StringListIter<char *> *_TCamResValueSeeker = new StringListIter<char *>(_TCamResValues->Get_TokenedCharListArrays());

        _CamResWidth = atoi((const char *)*_TCamResValueSeeker->Get_NowStringIter());
        _TCamResValueSeeker->Move_NextStringIter();
        _CamResHeight = atoi((const char *)*_TCamResValueSeeker->Get_NowStringIter());

        delete _TCamResValueSeeker;
        delete _TCamResValues;

        // 엔진 일시 정지 및 Cam 잠시 정지.
        _FireBaseEngine->Pause_FireBaseEngine();

        // 다시 시작.
        _FireBaseEngine->Resume_FireBaseEngine();
      }
      else if(strcmp((const char *)*_TIterationSeeker->Get_NowStringIter(), "Set_CamNum") == 0) {
        _TIterationSeeker->Move_NextStringIter();
        _CamNum = atoi((const char *)*_TIterationSeeker->Get_NowStringIter());

        // 엔진 일시 정지 및 Cam 잠시 정지.
        _FireBaseEngine->Pause_FireBaseEngine();

        // 다시 시작.
        _FireBaseEngine->Resume_FireBaseEngine();
      }
    }

		delete _TAtomicValues;
	}
	delete _TStringTokenizer;
}

void FireBaseAdapter::_SendToCommandLine(const char *Str, ...) {
	va_list _TArgument_List;
	char _Str[BUFFER_MAX_4096];

	va_start(_TArgument_List, Str);
	vsprintf(_Str, Str, _TArgument_List);
	va_end(_TArgument_List);

	fprintf(stdout, "%s\n", _Str);
}

void FireBaseAdapter::_SetInfomation(Mat &__FrameData) {
	// Image에 정보를 그린다.
	// 추후에 Data가 다 나오고 나서.
	__MUTEXLOCK(_ViewPointMutex);
	for (int i = 0; i < _ViewPoint.size(); i++)
		cv::circle(__FrameData, Point(_ViewPoint.at(i).first, _ViewPoint.at(i).second), 10, Scalar(0,0,255), CV_FILLED);
	__MUTEXUNLOCK(_ViewPointMutex);
}

bool FireBaseAdapter::_IsEmptyCommandStrQueue() {
  bool _TResult = false;

  __MUTEXLOCK(_Mutex_CommandStrQueue);
  _TResult = _CommandStrQueue.empty();
  __MUTEXUNLOCK(_Mutex_CommandStrQueue);

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
	__MUTEXLOCK(G_Adapter->_ViewPointMutex);
	G_Adapter->_ViewPoint.clear();
	__MUTEXUNLOCK(G_Adapter->_ViewPointMutex);
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
  __MUTEXLOCK(G_Adapter->_ViewPointMutex);
  G_Adapter->_ViewPoint.clear();
  __MUTEXUNLOCK(G_Adapter->_ViewPointMutex);

}
/* end SearchedResult Callback */

/* Start CommandThreads */
void *FireBaseAdapter::_FireBaseAdapter_InputCLICommandThread(void *Param) {
	FireBaseAdapter *_TAdapter = (FireBaseAdapter *)Param;

	while (_TAdapter->_AdapterStarted == true) {
    string _TCommandStr = _TAdapter->_GetCLICommandStr();
    RecvInformations _TRecvInfo;

    _TRecvInfo.UserID = _TAdapter->_CLIID;
    _TRecvInfo.Types = 0;
    _TRecvInfo.Command = _TCommandStr;

    _TAdapter->_CommandStrQueue.push(_TRecvInfo);
    _TAdapter->_CommandStrQueueSyncSignal.Signal();
	}

	return 0;
}

#if defined(SET_COMMON_MODULE_ETHERNET)
void *FireBaseAdapter::_FireBaseAdapter_InputEthernetCommandThread(void *Param) {
  FireBaseAdapter *_TAdapter = (FireBaseAdapter *)Param;

  return 0;
}
#endif

#if defined(SET_DEVICE_SERIAL)
void *FireBaseAdapter::_FireBaseAdapter_InputSerialCommandThread(void *Param){
  FireBaseAdapter *_TAdapter = (FireBaseAdapter *)Param;

  while (_TAdapter->_AdapterStarted == true) {
    string _TCommandStr = _TAdapter->_GetCLICommandStr();
    RecvInformations _TRecvInfo;

    _TRecvInfo.UserID = _TAdapter->_CLIID;
    _TRecvInfo.Types = 2;
    _TRecvInfo.Command = _TCommandStr;

    _TAdapter->_CommandStrQueue.push(_TRecvInfo);
    _TAdapter->_CommandStrQueueSyncSignal.Signal();
  }

  return 0;
}
#endif

void *FireBaseAdapter::_FireBaseAdapter_ParseCommandThread(void *Param){
  FireBaseAdapter *_TAdapter = (FireBaseAdapter *)Param;

  while (_TAdapter->_AdapterStarted == true) {
    if (_TAdapter->_IsEmptyCommandStrQueue() != true) {
      RecvInformations _TRecvInfo = _TAdapter->_CommandStrQueue.front();

      _TAdapter->_ParseCommand(_TRecvInfo.Command);
      _TAdapter->_CommandStrQueue.pop();
    }
    else
      _TAdapter->_CommandStrQueueSyncSignal.Wait();
  }

  return 0;
}
/* End CommandThreads */

void *FireBaseAdapter::_FireBaseAdapter_VisionThread(void *Param) {
	FireBaseAdapter *_TAdapter = (FireBaseAdapter *)Param;

#if defined(SET_TARGET_INTEL)
	VideoCapture _TCapture(_TAdapter->_CamNum);
	_TCapture.set(CV_CAP_PROP_FRAME_WIDTH, _TAdapter->_CamResWidth);
	_TCapture.set(CV_CAP_PROP_FRAME_HEIGHT, _TAdapter->_CamResHeight);

	namedWindow("FireBase-Core_Test");

	if (_TCapture.isOpened() == false)
		return 0;
#else
  // for mmal.
  mmalAdapter _TCapture;
  _TCapture.open(_TAdapter->_CamResWidth, _TAdapter->_CamResHeight, true);
#endif
	__MUTEXINIT(_TAdapter->_ViewPointMutex);

	while (_TAdapter->_EngineStarted == true) {
#if defined(SET_TARGET_INTEL)
		// 영상을 줌.
		_TCapture >> _TAdapter->_ViewMat;
#else
    // for mmal.
    _TAdapter->_ViewMat = _TCapture.grab();
#endif
		if (_TAdapter->_FireBaseEngine->Get_EngineStart() == true) {
			// 반드시 Clone 해서 주어야 함.
			_TAdapter->_FireBaseEngine->Push_NowFrameImage(_TAdapter->_ViewMat.clone());
		}
#if defined(SET_TARGET_INTEL)
		// 정보를 그려 보여준다.
		//_TAdapter->_SetInfomation(_TAdapter->_ViewMat);
		imshow("FireBase-Core_Test", _TAdapter->_ViewMat);

		waitKey(33);
#else
    // for mmal.
#endif
	}

	__MUTEXDESTROY(_TAdapter->_ViewPointMutex);

	return 0;
}

void FireBaseAdapter::Start_Engine() {
	_EngineStarted = true;

  RandomID _TRandomID;
  _CLIID = _TRandomID.Make_RandomID();
  _SerialID = _TRandomID.Make_RandomID();
#if defined(SET_DEVICE_SERIAL)
  #if defined(SET_TARGET_ARM)
  _SerialAdapter.Initialize_Serial("/dev/TtyUSB0");
  _SerialAdapter.Connect_Serial();
  #else
  _SerialAdapter.Initialize_Serial("/dev/tty.USB-Serial0.0");
  #endif
#endif

	_FireBaseEngine->TExternalFeatureSearchingResultPassingCallback = _FireBaseAdapter_FeatureSearchedResult;
	_FireBaseEngine->TExternalLaneSearchingResultPassingCallback = _FireBaseAdapter_LaneSearchedResult;
	_FireBaseEngine->TExternalForceAutofocusCallback = _FireBaseAdapter_AutoFocus;

#if defined(LOG_WRITE_MODE)
	G_LogD->SetCallback(_AllLogCallback);
#endif

  // Activate Vision Thread.
	_VisionThread.StartThread(_FireBaseAdapter_VisionThread, this);

  // Activate Command Threads.
  _InputCLICommandThread.StartThread(_FireBaseAdapter_InputCLICommandThread, this);
#if defined(SET_COMMON_MODULE_ETHERNET)
  _InputEthernetCommandThread.StartThread(_FireBaseAdapter_InputEthernetCommandThread, this);
#endif
#if defined(SET_DEVICE_SERIAL)
  _InputSerialCommandThread.StartThread(_FireBaseAdapter_InputSerialCommandThread, this);
#endif

  // Activate Command Parser Threads.
  // Must be cached ParseCommandThread.
  _ParseCommandThread.StartThread(_FireBaseAdapter_ParseCommandThread, this);

  // Activate Engine Thread.
  _FireBaseEngine->Start_FireBaseEngine();
}

void FireBaseAdapter::Stop_Engine() {
	_EngineStarted = false;
#if defined(SET_TARGET_ARM)
  _SerialAdapter.Disconnect_Serial();
#endif
}

void FireBaseAdapter::Start_Adapter() {
	_AdapterStarted = true;
	_InputCLICommandThread.AttacheMode = true;

  _InputCLICommandThread.StartThread(_FireBaseAdapter_InputCLICommandThread, this);

	_InputCLICommandThread.JoinThread();
}

void FireBaseAdapter::Stop_Adapter() {
	_AdapterStarted = false;
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