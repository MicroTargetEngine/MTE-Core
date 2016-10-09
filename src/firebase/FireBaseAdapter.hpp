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

#include "FireBaseAdapterVariable.hpp"

#if defined(SET_DEVICE_MMAL)
#include "mmalAdapter.hpp"
#endif
#if defined(SET_DEVICE_SERIAL)
#include "SerialAdapter.hpp"
#endif

#if defined(WINDOWS_SYS)
#include "ExConsole/ConsoleLogger.hpp"
#endif

class FireBaseAdapter {
private:
	FireBaseEngine *_FireBaseEngine;

  SerialAdapter _SerialAdapter;

	Thread _InputCLICommandThread, _InputEthernetCommandThread, _InputSerialCommandThread, _ParseCommandThread;
  Thread _VisionThread;

  ThreadMutex _ViewPointMutex;
  ThreadMutex _Mutex_CommandStrQueue;

  SyncSignal _CommandStrQueueSyncSignal;

	vector<pair<int, int> > _ViewPoint;
	queue<RecvInformations> _CommandStrQueue;

	Mat _ViewMat;

	string _CLIID;
	string _SerialID;

  int _CamResWidth;
  int _CamResHeight;
  int _CamNum;

	//char *_Command;
	bool _AdapterStarted;
	bool _EngineStarted;

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

	void _PutAuthor();
	string _GetCLICommandStr();
	void _ParseCommand(string __Command);
	void _SendToCommandLine(const char *Str, ...);

	void _SetInfomation(Mat &__FrameData);

  bool _IsEmptyCommandStrQueue();

	static void _FireBaseAdapter_FeatureSearchedResult(
#if !defined(MODE_ONLY_DETECTION)
			vector<FeatureData>
#else
      FeatureSets
#endif
			__Data);
	//static void _FireBaseAdapter_DatabaseSearchedResult(vector<FeatureData> __SortedData);
	static void _FireBaseAdapter_AutoFocus(void);
  static void _FireBaseAdapter_LaneSearchedResult(LaneDetectData __Data);

#if defined(LOG_WRITE_MODE)
	static void _AllLogCallback(const char *__CallValueString, const char *__Messages);
#endif

	static void *_FireBaseAdapter_InputCLICommandThread(void *Param);
#if defined(SET_COMMON_MODULE_ETHERNET)
	static void *_FireBaseAdapter_InputEthernetCommandThread(void *Param);
#endif
#if defined(SET_DEVICE_SERIAL)
  static void *_FireBaseAdapter_InputSerialCommandThread(void *Param);
#endif
	static void *_FireBaseAdapter_ParseCommandThread(void *Param);

	static void *_FireBaseAdapter_VisionThread(void *Param);

public:
	FireBaseAdapter();
  ~FireBaseAdapter();

	void Start_Engine();
	void Stop_Engine();


	void Start_Adapter();
	void Stop_Adapter();
};

#endif

#endif
