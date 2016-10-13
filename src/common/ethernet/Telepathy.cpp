/*********************************************************************

          File: Telepathy.cpp

   Description: Telepathy Source Part.

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

#include "Telepathy.hpp"

#if defined(_Telepathy_hpp_)

#pragma region Server Class
// Telepathy Server Class Area.
Telepathy::Server *G_TelepathyServer;

#pragma region Constructor & Destructor
// constructor
Telepathy::Server::Server() {
	IsInitializeServer = false;
	IsServerStarted = false;
}

// destructor
Telepathy::Server::~Server() {
	if (IsInitializeServer == true) {
    Close_Server();
	}
}
#pragma endregion Constructor & Destructor

#pragma region Server Threads

void * Telepathy::Server::Server_ConnectionThread(void *Param) {
  Telepathy::Server *_TServer = (Telepathy::Server *) Param;

	while (_TServer->IsServerStarted == true) {
    bool _TCompressFds = false;
    int _TFds = poll(_TServer->_Fds, _TServer->_ConnectionNumber, -1);

    if (_TFds < 0) {
      // error.
    }

    int _TCurrentSize = _TServer->_ConnectionNumber;
    for (int i=0; i<_TCurrentSize; i++) {
      if (_TServer->_Fds[i].revents == 0)
        continue;

      if (_TServer->_Fds[i].fd == _TServer->_ServerSocket) {
        int _TSocket = G_TelepathyServer->_AddClient();
        if (_TSocket == -1) {
          // Socket Connect Fail.
          continue;
        }
        else {
          _TServer->_Fds[_TServer->_ConnectionNumber].fd = _TSocket;
          _TServer->_Fds[_TServer->_ConnectionNumber].events = POLLIN;
          _TServer->_ConnectionNumber++;
        }
      }
      else {
        if (G_TelepathyServer->_Receive(_TServer->_Fds[i].fd) == false) {
          close(_TServer->_Fds[i].fd);
          _TServer->_Fds[i].fd = -1;
          _TCompressFds = true;
        }
      }
    }

    if (_TCompressFds == true) {
      for (int i=0; i<_TServer->_ConnectionNumber; i++) {
        if (_TServer->_Fds[i].fd == -1) {
          for (int j=i; j<_TServer->_ConnectionNumber; j++)
            _TServer->_Fds[j].fd = _TServer->_Fds[j+1].fd;
          i--;
          _TServer->_ConnectionNumber--;
        }
      }
    }
  }

  // 서버 끝나면 전부 끊어버린다
  for (int i=0; i<_TServer->_ConnectionNumber; i++){
    if (_TServer->_Fds[i].fd >= 0) {
      close(_TServer->_Fds[i].fd);
    }
  }
#if defined(WINDOWS_SYS)
	_endthread();
#elif defined(POSIX_SYS)

#endif
	return 0;
}

/*
void *Telepathy::Server::Server_ReceivingThread(void *Param) {
#if defined(WINDOWS_SYS)
  SOCKET
#elif defined(POSIX_SYS)
  int
#endif
      _CTlientSocket = (
#if defined(WINDOWS_SYS)
      SOCKET
#elif defined(POSIX_SYS)
      int
#endif
      ) Param;

	while (1) {
		// 현재 Thread는 계속 받는다.
		if (G_TelepathyServer->_Receive(_CTlientSocket) == false)
			break;
	}
#if defined(WINDOWS_SYS)
	_endthread();
#elif defined(POSIX_SYS)

#endif
	return 0;
}*/
#pragma endregion Server Threads

// Server 초기화.
bool Telepathy::Server::Initialize_Server() {
	// 이 부분은 통째로 Windows용
	// 추후 다른 OS도 추가.

#if defined(WINDOWS_SYS)
	// WSAStartUp
	if (WSAStartup(0x101, &_WSAData) != 0)
		return false;
#endif
	// using IPv4
	_ServerAddress.sin_family = AF_INET;
	// 32bit IPv4 address
	_ServerAddress.sin_addr.s_addr =
#if defined(POSIX_SYS)
      htonl(
#endif
      INADDR_ANY
#if defined(POSIX_SYS)
      )
#endif
      ;
	//_M_ServerAddress.sin_addr.s_addr = inet_addr(IP_ADDR_LOCAL);
	// port 사용
	_ServerAddress.sin_port = htons((u_short)ART_TCP_PORT);

	// Socket Create.
	_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	int _TOptionValue = 1;
	// TCP No Delay Option.
	setsockopt(_ServerSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&_TOptionValue, sizeof(_TOptionValue));

	// Socket이 잘못 되었다면..
	if (_ServerSocket ==
#if defined(WINDOWS_SYS)
      INVALID_SOCKET
#elif defined(POSIX_SYS)
      -1
#endif
      )
		return false;

	// socket bind.
	if (bind(_ServerSocket, (sockaddr *)&_ServerAddress, sizeof(_ServerAddress)) != 0) {
    Close_Server();
		return false;
	}

	// socket listen.
	if (listen(_ServerSocket, TCP_LISTEN_QUEUE) != 0) {
    Close_Server();
		return false;
	}

	// 외부 Receive 함수용.
	G_TelepathyServer = this;
	IsInitializeServer = true;

  // poll init.
  _Fds[0].fd = _ServerSocket;
  _Fds[0].events = POLLIN;
  _ConnectionNumber = 1;

	return true;
}

// Server 기동.
bool Telepathy::Server::Start_Server() {
	bool _TIsStarted = false;
	if (IsInitializeServer != true) {
		// failed started server.
		_TIsStarted = false;
	}
	else {
		// Client 관리 Thread 시작.
		_Thread.StartThread(Server_ConnectionThread, this);
		_TIsStarted = IsServerStarted = true;
	}
	return _TIsStarted;
}

// Server 종료.
void Telepathy::Server::Close_Server() {
	if (_ServerSocket != NULL) {
#if defined(WINDOWS_SYS)
    closesocket(_ServerSocket);
		WSACleanup();
#elif defined(POSIX_SYS)
    close(_ServerSocket);
#endif
		IsInitializeServer = false;
		IsServerStarted = false;
	}
}

// Client가 Server 접속시 Socket List에 붙여 Listen하게 하는 과정.
// User의 접속을 위하여 필요한 과정.
int Telepathy::Server::_AddClient() {
	// Client 접속시, 접속 연결 기능.
#if defined(WINDOWS_SYS)
	SOCKET
#elif defined(POSIX_SYS)
  int
#endif
	_TSocket;
#if defined(WINDOWS_SYS)
	SOCKADDR_IN
#elif defined(POSIX_SYS)
  sockaddr_in
#endif
	_TClientAddress;
	int _TClientLength = sizeof(_TClientAddress);

	// Accept를 하여 Socket을 연결한다.
	_TSocket = accept(_ServerSocket, (struct sockaddr *)&_TClientAddress,
#if defined(POSIX_SYS)
                    (socklen_t *)
#endif
      &_TClientLength);

	int _TOptionValue = 1;
	// TCP No Delay Option.
	setsockopt(_TSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&_TOptionValue, sizeof(_TOptionValue));

	// Client Sockets를 넣어준다.
	if (_TSocket !=
#if defined(WINDOWS_SYS)
      INVALID_SOCKET
#elif defined(POSIX_SYS)
      -1
#endif
      ) {
		ClientsList _TClientList;
		// Clientlist Initialize.
		//_TClientList.ClientsListInitialize();
		// Client Address.
		_TClientList.ClientAddress = _TClientAddress;
		// Socket.
		_TClientList.ClientSocket = _TSocket;
		// Client Type.
		_TClientList.ClientType = "";
		// Client Name.
		_TClientList.ClientName = "";
		// push at lists back.
		ConnectedClientList.push_back(_TClientList);
		//ConnectorsSocketList.push_back(_TSocket);
		TAnyConnectionNotifier(_TSocket);
	}
	else
		return -1;

	// Thread Begin.
	//_Thread.StartThread(Server_ReceivingThread, (void *)_TSocket);
  return _TSocket;
}

// Server가 Client들에게 정보를 받는 과정.
bool Telepathy::Server::_Receive(
#if defined(WINDOWS_SYS)
    SOCKET
#elif defined(POSIX_SYS)
    int
#endif
ClientSocket) {
	char _TBuffer[BUFFER_MAX_32767];
#if defined(WINDOWS_SYS)
	int
#elif defined(POSIX_SYS)
  ssize_t
#endif
      _TReadBufferLength;

	memset(_TBuffer, NULL, sizeof(_TBuffer));

	_TReadBufferLength =
#if defined(WINDOWS_SYS)
		recv(ClientSocket, _TBuffer, BUFFER_MAX_32767, 0);
#elif defined(POSIX_SYS)
    recv(ClientSocket, _TBuffer, BUFFER_MAX_32767, 0);
#endif
	
	if (_TReadBufferLength <= 0) {
		// ClientsList로 인하여 구현이 바뀜.
		// User Spacific하게 바꿀 수 있음.

    for_IterToEnd(vector, ClientsList, ConnectedClientList, i) {
			if (_i->ClientSocket == ClientSocket) {
				// Iterator를 하나 더 만들어준다.
				vector<ClientsList>::iterator _TClientListIter = _i;
				// Iterator가 위치를 잃어버려도 어차피 return하기 때문에 상관 없음.
				// 다른 Iterator를 만들어 이것을 하나 증가시켜 현재 Iterator의 위치가 end인지 검사한다.
				// 아니라면 삭제.
				if ((++_TClientListIter) != ConnectedClientList.end())
					_i = ConnectedClientList.erase(_i);
				else {
          // List에서 pop_back.
          ConnectedClientList.pop_back();
          // break를 넣어주지 않으면 죽어버린다.
          break;
        }
			}
		}

    TAnyDisconnectionNotifier(ClientSocket);
		return false;
	}
	else {
		// 만약 있다면, 다른 곳의 외부 함수 호출.
		// 정확히 말하면, Server를 쓰는 곳에서 호출.
		TServerReceivedCallback(_TBuffer, ClientSocket);
	}
	return true;
}

bool Telepathy::Server::SendDataToOne(char *Str,
#if defined(WINDOWS_SYS)
                                      SOCKET
#elif defined(POSIX_SYS)
                                      int
#endif
                                      ClientSocket) {
	// LIt is List Iterator.
	//list<SOCKET>::iterator _TLIt;
	int _TSendStatus = 0;
  int _TIndex = 0;

	// 지정된 Client에게 보냄.
	// STL List의 특성상, remove할 때 문제가 있으며, erase를 할 때도 Iterator에서 다음을 인식 할 수 없는 것에 매우 주의 하였다.
	// 그러므로 List의 끝에 있는 접속자를(begin에서 end로 iterator가 돌 경우) 삭제 할 때,
	// Error가 날 수 있다는 것을 전제하여 다음과 같이 Code를 만들었다.
	for_IterToEnd(vector, ClientsList, ConnectedClientList, i) {
		// 일단 보낼 쪽의 Socket을 검사한다.
		if (_i->ClientSocket == ClientSocket) {
			_TSendStatus = send(_i->ClientSocket, Str, strlen(Str) + 1, 0);
			if (_TSendStatus == -1) {
        TAnyDisconnectionNotifier(ClientSocket);

        // 먼저 Poll Desc 삭제.
        close(_Fds[_TIndex+1].fd);
        _Fds[_TIndex+1].fd = -1;

				// Iterator를 하나 더 만들어준다.
				vector<ClientsList>::iterator _TClientListIter = _i;
				// Iterator가 위치를 잃어버려도 어차피 return하기 때문에 상관 없음.
				// 다른 Iterator를 만들어 이것을 하나 증가시켜 현재 Iterator의 위치가 end인지 검사한다.
				// 아니라면 삭제.
				if ((++_TClientListIter) != ConnectedClientList.end())
					_i = ConnectedClientList.erase(_i);
				else {
					// List에서 pop_back.
					ConnectedClientList.pop_back();
					// break를 넣어주지 않으면 죽어버린다.
					break;
				}
				return false;
			}
			return true;
		}
    _TIndex++;
	}
	return true;
}

void Telepathy::Server::SendDataToAll(char *Str) {
	// 모두에게 Message를 보낸다.
	// 이 함수는 어떤 Client의 Socket이 끊겼다면 해당 Client로의 전송이 실패할 수도 있기 때문에
	// 어떤 Client로의 Message 전송이 실패 할 수도 있다.
	// 즉, 전부 다 성공한다는 보장이 없다.
	int _TSendStatus = 0;
  int _TIndex = 0;

	for_IterToEnd(vector, ClientsList, ConnectedClientList, i) {
		_TSendStatus = send(_i->ClientSocket, Str, strlen(Str) + 1, 0);
		if (_TSendStatus == -1) {
      TAnyDisconnectionNotifier(_Fds[_TIndex+1].fd);

      // 먼저 Poll Desc 삭제.
      close(_Fds[_TIndex+1].fd);
      _Fds[_TIndex+1].fd = -1;

			// Iterator를 하나 더 만들어준다.
			vector<ClientsList>::iterator _TClientListIter = _i;
			// Iterator가 위치를 잃어버려도 어차피 return하기 때문에 상관 없음.
			// 다른 Iterator를 만들어 이것을 하나 증가시켜 현재 Iterator의 위치가 end인지 검사한다.
			// 아니라면 삭제.
			if ((++_TClientListIter) != ConnectedClientList.end())
				_i = ConnectedClientList.erase(_i);
			else {
				// List에서 pop_back.
				ConnectedClientList.pop_back();
				// break를 넣어주지 않으면 죽어버린다.
				break;
			}
		}
    _TIndex++;
	}
}
#pragma endregion Server Class

#pragma region Client Class
// Telepathy Client Class Area.
Telepathy::Client *G_TelepathyClient;

#pragma region Constructor & Destructor
// constructor
Telepathy::Client::Client() {
	IsInitializeClient = false;
	IsConnectedClient = false;

	G_TelepathyClient = this;
}

// deconstructor
Telepathy::Client::~Client() {
	if (IsConnectedClient == true)
		ClientClose();
	IsInitializeClient = false;
	G_TelepathyClient = NULL;
}
#pragma endregion Constructor & Destructor

#pragma region Client Threads
void *Telepathy::Client::Client_ReceivingThread(void *Param) {
	while (1) {
		if (G_TelepathyClient->ClientReceiving() == false) {
			G_TelepathyClient->ClientDisconnect();
			break;
		}
	}
	return 0;
}
#pragma endregion Client Threads

bool Telepathy::Client::ClientInitialize() {
//	int _TOptVal;

#if defined(WINDOWS_SYS)
  // WSAStartUp
	if (WSAStartup(0x101, &_WSAData) != 0)
		return false;
#endif
	// Socket Create.
	_ClientSocket = socket(AF_INET, SOCK_STREAM, 0);

	// Socket이 잘못 되었다면..
  if (_ClientSocket !=
      #if defined(WINDOWS_SYS)
      INVALID_SOCKET
      #elif defined(POSIX_SYS)
      -1
#endif
      )
		return false;

	int _TOptionValue = 1;
	// TCP No Delay Option.
	setsockopt(_ClientSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&_TOptionValue, sizeof(_TOptionValue));
	//if (setsockopt(_ClientSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&_TOptVal, sizeof(int)) == -1)
	//	return false;
	// Local IP Address.
	_Address = inet_addr(IP_ADDR_LOCAL);
	// get host entry.
	_HostEntry = gethostbyaddr((char *)&_Address, sizeof(_Address), AF_INET);

	if (_HostEntry == NULL) {
		ClientClose();
		return false;
	}

	// using IPv4
	_ClientAddress.sin_family = AF_INET;
	// 32bit IPv4 address
	_ClientAddress.sin_addr.s_addr = *((
#if defined(WINDOWS_SYS)
      unsigned long
#elif defined(POSIX_SYS)
      in_addr_t
#endif
      *)_HostEntry->h_addr);
	//_M_ServerAddress.sin_addr.s_addr = inet_addr(IP_ADDR_LOCAL);
	// port 사용
	_ClientAddress.sin_port = htons((u_short)ART_TCP_PORT);

	IsInitializeClient = true;
	return true;
}

void Telepathy::Client::ClientReceiveStart() {
	if (IsConnectedClient != true) {
		// failed started Client.
	}
	else {
		// Client 관리 Thread 시작.
		_Thread.StartThread(Client_ReceivingThread, NULL);
	}
}

void Telepathy::Client::ClientClose() {
	if (_ClientSocket != NULL) {
		shutdown(_ClientSocket, 0x02); // BOTH.
#if defined(WINDOWS_SYS)
		closesocket(_ClientSocket);
#elif defined(POSIX_SYS)
    close(_ClientSocket);
#endif
	}
	IsConnectedClient = false;
}

bool Telepathy::Client::ClientReceiving() {
	char _TBuffer[BUFFER_MAX_32767];
#if defined(WINDOWS_SYS)
	int
#elif defined(POSIX_SYS)
  ssize_t
#endif
      _TReadBufferLength;

	memset(_TBuffer, NULL, sizeof(_TBuffer));

	_TReadBufferLength =
#if defined(WINDOWS_SYS)
		recv(_ClientSocket, _TBuffer, BUFFER_MAX_32767, 0);
#elif defined(POSIX_SYS)
    recv(_ClientSocket, _TBuffer, BUFFER_MAX_32767, 0);
#endif

	if (_TReadBufferLength == -1) {
		return false;
	}
	else {
		// 만약 있다면, 다른 곳의 외부 함수 호출.
		// 정확히 말하면, Client를 쓰는 곳에서 호출.

		TClientReceivedCallback(_TBuffer);
	}
	return true;
}

bool Telepathy::Client::ClientConnect() {
	if (connect(_ClientSocket, (sockaddr *)&_ClientAddress, sizeof(_ClientAddress))) {
		ClientClose();
		return false;
	}

	// 외부 Receive 함수용.
	IsConnectedClient = true;

	return true;
}

void Telepathy::Client::ClientDisconnect() {
	ClientClose();
	TClientDisconnectedCallback();
	//IsConnectedClient = false;
}

bool Telepathy::Client::SendData(char *Str) {
	int _TSendStatus = 0;

	_TSendStatus = send(_ClientSocket, Str, strlen(Str)+1, 0);

	if (_TSendStatus == -1)
		return false;

	return true;
}
#pragma endregion Client Class

#endif // _Telepathy_hpp_