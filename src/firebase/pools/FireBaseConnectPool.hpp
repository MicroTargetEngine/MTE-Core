/*********************************************************************

          File: FireBaseConnectPool.hpp

   Description: FireBaseConnectPool Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/10/11

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

#if defined(SET_DEVICE_SERIAL) || defined(SET_COMMON_MODULE_ETHERNET)

#ifndef _FireBaseConnectPool_hpp_
#define _FireBaseConnectPool_hpp_

#include "FireBaseConnectVariable.hpp"

#if defined(SET_COMMON_MODULE_ETHERNET)
#include "Telepathy.hpp"
#endif
#if defined(SET_DEVICE_SERIAL)
#include "SerialAdapter.hpp"
#endif

class FireBaseConnectPool {
private:
  Thread _InputCLICommandThread, _InputCommandQueueProcessingThread;
  bool _ConnectPoolStarted;

  vector<ConnectInformation> _Connections;

  queue<MessageInformations> _RecvMsgQueue;
  ThreadMutex _Mutex_RecvMsgQueue;
  SyncSignal _SyncSignal_RecvMsgQueue;

  SerialAdapter _Serial;
  Telepathy::Server *_Ethernet;

  bool _Initialize();
  void _Deinitialize();
  void _Initialize_Members();

  void _Initialize_CLI();

  void _PutAuthor();
  bool _FindConnection(ConnectInformation &__Connection);
  string _SplitIDAndCommand(string &__Msg);
  string _GetCLICommandStr();

  void _SendToCommandLine(const char *Str, ...);
  template <typename T> bool _IsEmptyQueue(queue<T> __Queue, ThreadMutex __Mutex);

  // for Serial(Callback)
  static void _FireBaseConnectPool_SerialReceiveResult(string __Msg);
  // for Ethernet(Callback)
  static void _FireBaseConnectPool_EthernetServerReceiveResult(char *__Msg, int __Sock);
  static void _FireBaseConnectPool_EthernetAnyConnentionNotifier(int __Sock);
  static void _FireBaseConnectPool_EthernetAnyDisonnentionNotifier(int __Sock);

  static void *_FireBaseConnectPool_InputCLICommandThread(void *Param);
#if defined(SET_COMMON_MODULE_ETHERNET) || defined(SET_DEVICE_SERIAL)
  static void *_FireBaseConnectPool_RecvMsgQueueProcessingThread(void *Param);
#endif
public:
  FireBaseConnectPool();
  ~FireBaseConnectPool();

  // External for Callback
  typedef void(*_T_RECVMESSAGECALLBACK)(MessageInformations __Msg);
  _T_RECVMESSAGECALLBACK TRecvMessageCallback;

  void Start_ConnectPool();
  void Stop_ConnectPool();

  void Send_Message(MessageInformations __Msg);

  IMPLEMENT_GET(vector<ConnectInformation> ,Connections, _Connections)
};

#endif //_FireBaseConnectPool_hpp_

#endif // defined(SET_DEVICE_SERIAL) || defined(SET_COMMON_MODULE_ETHERNET)