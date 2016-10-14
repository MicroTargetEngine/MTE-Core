/*********************************************************************

          File: FireBaseConnectPool.cpp

   Description: FireBaseConnectPool Source Part.

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

#include "FireBaseConnectPool.hpp"

#if defined(_FireBaseConnectPool_hpp_)

#include "RandomID.hpp"
#include "StringTokenizer.hpp"
#include "StringListIter.hpp"

#if defined(LOG_WRITE_MODE)
#include "LogD.hpp"
#endif

FireBaseConnectPool *G_ConnectPool;

FireBaseConnectPool::FireBaseConnectPool() {
  G_ConnectPool = this;
  //_Initialize();
  _Initialize_Members();
}

FireBaseConnectPool::~FireBaseConnectPool() {
  G_ConnectPool = NULL;
  _Initialize_Members();
}

bool FireBaseConnectPool::_Initialize() {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _Initialize Function");
#endif
  // CLI initialize.
  _Initialize_CLI();

  // Put Author.
  _PutAuthor();

  RandomID _TRandomID;

  // Push CLI Connection.
  ConnectInformation _TConnectInformation;
  _TConnectInformation.Types = 0;
  _TConnectInformation.UserID = _TRandomID.Make_RandomID();
  _Connections.push_back(_TConnectInformation);

  // Serial Initialize.
#if defined(SET_DEVICE_SERIAL)
  #if defined(SET_TARGET_ARM)
  _Serial.Initialize_Serial("/dev/TtyUSB0");
  #else
  _Serial.Initialize_Serial("/dev/tty.USB-Serial0.0");
  #endif
#endif
  if (_Serial.Connect_Serial() == true) {
    _Serial.TReceiveCallback = _FireBaseConnectPool_SerialReceiveResult;

    _TConnectInformation.Types = 2;
    _TConnectInformation.UserID = _TRandomID.Make_RandomID();

    _Connections.push_back(_TConnectInformation);
  }

  // Ethernet Initialize.
  _Ethernet = new Telepathy::Server();

  if (_Ethernet->Initialize_Server() != true)
    return false;

  _Ethernet->TServerReceivedCallback = _FireBaseConnectPool_EthernetServerReceiveResult;
  _Ethernet->TAnyConnectionNotifier = _FireBaseConnectPool_EthernetAnyConnentionNotifier;
  _Ethernet->TAnyDisconnectionNotifier = _FireBaseConnectPool_EthernetAnyDisonnentionNotifier;

  _Ethernet->Start_Server();

  // Activate Command Threads.
  _InputCLICommandThread.StartThread(_FireBaseConnectPool_InputCLICommandThread, this);
#if defined(SET_COMMON_MODULE_ETHERNET) || defined(SET_DEVICE_SERIAL)
  _InputCommandQueueProcessingThread.StartThread(_FireBaseConnectPool_RecvMsgQueueProcessingThread, this);
#endif

  return true;
}

void FireBaseConnectPool::_Deinitialize() {
  _Serial.Disconnect_Serial();
  delete _Ethernet;
}

void FireBaseConnectPool::_Initialize_Members() {
  _ConnectPoolStarted = false;
}

void FireBaseConnectPool::_Initialize_CLI() {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
}

void FireBaseConnectPool::_PutAuthor() {
  _SendToCommandLine("FireBase-Core Engine TEST Version %s.", ENGINE_EXEC_VER);
  _SendToCommandLine("Project \"FireBase\", Team \"RobotSarang\", Open Robot Marathon, IRC.\nCopyright (c) 2016 All right reserved.");
  _SendToCommandLine("Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)");
}

bool FireBaseConnectPool::_FindConnection(ConnectInformation &__Connection) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FindConnection Function");
#endif
  for_IterToEnd(vector, ConnectInformation, G_ConnectPool->_Connections, i) {
    if ((_i->Types == __Connection.Types)) {
      ConnectInformation _TConnectInformation;
      if (_i->UserID == __Connection.UserID) {
        if (__Connection.Types == 1) {
          _TConnectInformation.Socket = __Connection.Socket;
        }
        _TConnectInformation.UserID = _i->UserID;
        _TConnectInformation.Types = __Connection.Types;

        __Connection = _TConnectInformation;

        return true;
      }
      else if (__Connection.Types == 0 && __Connection.UserID == "") {
        // CLI는 ID를 파악하지 않아도 되는 유일한 경우이다.
        _TConnectInformation.UserID = _i->UserID;
        _TConnectInformation.Types = __Connection.Types;

        __Connection = _TConnectInformation;
        return true;
      }
    }
  }
  return false;
}

string FireBaseConnectPool::_SplitIDAndCommand(string &__Msg) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _SplitIDAndCommand Function");
#endif
  string _TMsgString = "", _TID = "";

  StringTokenizer *_TStringTokenizer = new StringTokenizer();

  _TStringTokenizer->Set_InputCharString(__Msg.c_str());
  _TStringTokenizer->Set_SingleToken(" ");

  if (_TStringTokenizer->Go_StringToken() == false) {
    delete _TStringTokenizer;
    return string("");
  }

  for_IterToEndC(list, string, _TStringTokenizer->Get_TokenedStringList(), i) {
    StringTokenizer *_TAtomicValues = new StringTokenizer();

    _TAtomicValues->Set_InputCharString((const char *)_i->c_str());
    _TAtomicValues->Set_SingleToken("=");

    if (_TAtomicValues->Go_StringToken() != true) {
      // Database String이 잘못 되었을 때.
      delete _TAtomicValues; // 중간에 나가기 때문에 반드시 delete 해준다.
      return string("");
    }

    StringListIter<char *> *_TIterationSeeker = new StringListIter<char *>(_TAtomicValues->Get_TokenedCharListArrays());

    if(strcmp((const char *)*_TIterationSeeker->Get_NowStringIter(), "ID") == 0) {
      _TIterationSeeker->Move_NextStringIter();
      _TID = (const char *)*_TIterationSeeker->Get_NowStringIter();
    }
    else {
      _TMsgString.append(*_i);
      if (_TStringTokenizer->Get_TokenedStringList()->end() != _i) {
        _TMsgString.append(" ");
      }
    }

    delete _TAtomicValues;
  }
  delete _TStringTokenizer;

  return _TID;
}

string FireBaseConnectPool::_GetCLICommandStr() {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _GetCLICommandStr Function");
#endif
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

void FireBaseConnectPool::_SendToCommandLine(const char *Str, ...) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _SendToCommandLine Function");
#endif
  va_list _TArgument_List;
  char _Str[BUFFER_MAX_4096];

  va_start(_TArgument_List, Str);
  vsprintf(_Str, Str, _TArgument_List);
  va_end(_TArgument_List);

  fprintf(stdout, "%s\n", _Str);
}

template <typename T>
bool FireBaseConnectPool::_IsEmptyQueue(queue<T> __Queue, ThreadMutex &__Mutex) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _IsEmptyQueue Function");
#endif
  bool _TResult = false;

  __MUTEXLOCK(__Mutex);
  _TResult = __Queue.empty();
  __MUTEXUNLOCK(__Mutex);

  return _TResult;
}

/* Start Communication Callbacks */
void FireBaseConnectPool::_FireBaseConnectPool_SerialReceiveResult(string __Msg) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseConnectPool_SerialReceiveResult Function");
#endif
  ConnectInformation _TConnectInformation;
  string _TMsgString = "", _TID = "";

  if ((_TID = G_ConnectPool->_SplitIDAndCommand(__Msg)) != "") {
    _TMsgString = __Msg;

    _TConnectInformation.UserID = _TID;
    _TConnectInformation.Types = 2;

    // is vaild connection?
    if (G_ConnectPool->_FindConnection(_TConnectInformation) == true) {
      MessageInformations _TMessageInformations;

      _TMessageInformations.UserInformation = _TConnectInformation;
      _TMessageInformations.RecvMessage = _TMsgString;

      G_ConnectPool->_RecvMsgQueue.push(_TMessageInformations);

      G_ConnectPool->_SyncSignal_RecvMsgQueue.Signal();
    }
  }
}

void FireBaseConnectPool::_FireBaseConnectPool_EthernetServerReceiveResult(char *__Msg, int __Sock) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseConnectPool_EthernetServerReceiveResult Function");
#endif
  ConnectInformation _TConnectInformation;
  string _TMsgString = "", _TID = "", _TMsg = __Msg;

  if ((_TID = G_ConnectPool->_SplitIDAndCommand(_TMsg)) != "") {
    _TMsgString = __Msg;

    _TConnectInformation.UserID = _TID;
    _TConnectInformation.Types = 1;
    _TConnectInformation.Socket = __Sock;

    // is vaild connection?
    if (G_ConnectPool->_FindConnection(_TConnectInformation) == true) {
      MessageInformations _TMessageInformations;

      _TMessageInformations.UserInformation = _TConnectInformation;
      _TMessageInformations.RecvMessage = _TMsgString;

      G_ConnectPool->_RecvMsgQueue.push(_TMessageInformations);

      G_ConnectPool->_SyncSignal_RecvMsgQueue.Signal();
    }
  }
}

void FireBaseConnectPool::_FireBaseConnectPool_EthernetAnyConnentionNotifier(int __Sock) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseConnectPool_EthernetAnyConnentionNotifier Function");
#endif
  // 누군가 들어오면 ConnectInformation에 추가.
  // 단, Client List에 ClientName을 일단 주고 시작한다.
  for_IterToEnd(vector, ClientsList, G_ConnectPool->_Ethernet->ConnectedClientList, i) {
    if (_i->ClientSocket == __Sock) {
      RandomID _TRandomID;
      _i->ClientName = (char *)_TRandomID.Make_RandomID().c_str();

      ConnectInformation _TConnectInformation;
      _TConnectInformation.Socket = __Sock;
      _TConnectInformation.Types = 1;
      _TConnectInformation.UserID = _i->ClientName;
      // Push ConnectPool
      G_ConnectPool->_Connections.push_back(_TConnectInformation);
      break;
    }
  }
}

void FireBaseConnectPool::_FireBaseConnectPool_EthernetAnyDisonnentionNotifier(int __Sock) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseConnectPool_EthernetAnyDisonnentionNotifier Function");
#endif
  // 끊기면 ConnectInformation에서 삭제
  for_IterToEnd(vector, ConnectInformation, G_ConnectPool->_Connections, i) {
    if (_i->Socket == __Sock) {
      G_ConnectPool->_Connections.erase(_i);
      break;
    }
  }
}
/* End Communication Callbacks */

/* Start CommandThread */
void *FireBaseConnectPool::_FireBaseConnectPool_InputCLICommandThread(void *Param) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseConnectPool_InputCLICommandThread Function");
#endif
  // 이 쓰레드는 명령을 받으면 무조건 큐에 집어 넣는다.
  // CLI는 대기하다가 리턴하는 방식이기 때문에 콜백으로 하는것이 아니라 별도의 Thread가 존재하여야 한다.
  FireBaseConnectPool *_TAdapter = (FireBaseConnectPool *)Param;

  while (_TAdapter->_ConnectPoolStarted == true) {
    string _TCommandStr = _TAdapter->_GetCLICommandStr();

    ConnectInformation _TConnectInformation;
    _TConnectInformation.Types = 0;

    if (_TAdapter->_FindConnection(_TConnectInformation) == true){
      // CLI도 마찬가지로 Connection을 찾는다.
      // 좀 복잡스럽지만, 이렇게 해야 구조가 완전히 통일 된다.
      MessageInformations _TMessageInformations;

      _TMessageInformations.UserInformation = _TConnectInformation;
      _TMessageInformations.RecvMessage = _TCommandStr;

      _TAdapter->_RecvMsgQueue.push(_TMessageInformations);

      _TAdapter->_SyncSignal_RecvMsgQueue.Signal();
    }
  }

  return 0;
}
/* End CommandThread */

#if defined(SET_COMMON_MODULE_ETHERNET) || defined(SET_DEVICE_SERIAL)
void *FireBaseConnectPool::_FireBaseConnectPool_RecvMsgQueueProcessingThread(void *Param) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseConnectPool_RecvMsgQueueProcessingThread Function");
#endif
  FireBaseConnectPool *_TAdapter = (FireBaseConnectPool *)Param;

  while (_TAdapter->_ConnectPoolStarted == true) {
    if (_TAdapter->_IsEmptyQueue(_TAdapter->_RecvMsgQueue, _TAdapter->_Mutex_RecvMsgQueue) != true) {
      MessageInformations _TMsgInfo = _TAdapter->_RecvMsgQueue.front();

      _TAdapter->TRecvMessageCallback(_TMsgInfo);
      _TAdapter->_RecvMsgQueue.pop();
    }
    else
      _TAdapter->_SyncSignal_RecvMsgQueue.Wait();

  }
  return 0;
}
#endif


void FireBaseConnectPool::Start_ConnectPool() {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into Start_ConnectPool Function");
#endif
  if (_ConnectPoolStarted != true) {
    _ConnectPoolStarted = true;

    _Initialize();
  }
}

void FireBaseConnectPool::Stop_ConnectPool() {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into Stop_ConnectPool Function");
#endif
  if (_ConnectPoolStarted == true) {
    _ConnectPoolStarted = false;

    _Deinitialize();
  }
}

void FireBaseConnectPool::Send_Message(MessageInformations __Msg) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into Send_Message Function");
#endif
  switch (__Msg.UserInformation.Types) {
    case 0 : // CLI
      break;
    case 1 : // Ethernet
      _Ethernet->SendDataToOne((char *)__Msg.SendMessage.c_str(), __Msg.UserInformation.Socket);
      break;
    case 2 : // Serial
      _Serial.Send(__Msg.SendMessage);
      break;
  }
}

#endif