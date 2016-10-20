/*********************************************************************

          File: Telepathy.hpp

   Description: Telepathy Header Part.

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

#if defined(SET_COMMON_MODULE_ETHERNET)

#ifndef _Telepathy_hpp_
#define _Telepathy_hpp_

#include "SystemCommon.hpp"

#include "SystemUtil.hpp"

#if defined(WINDOWS_SYS)
//	#ifdef _AFXDLL
//#include <afxwin.h>
//	#else
#	if defined(MINGW_USING)
#include <winsock.h>
//#include <winsock2.h>
#	else
//#		if !defined(USING_MARIA_DB)
#include <windows.h>
//#		endif
#include <process.h>
#	endif
#include <tchar.h>
//#endif
// ws2_32.lib 링크
#pragma comment(lib, "ws2_32.lib")
#elif defined(POSIX_SYS)
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

// support poll.
#include <sys/poll.h>
#endif

typedef struct _ClientsList {
#if defined(WINDOWS_SYS)
	SOCKADDR_IN ClientAddress;
	SOCKET ClientSocket;
#elif defined(POSIX_SYS)
  sockaddr_in ClientAddress;
  int ClientSocket;
#endif
	int ClientNumber;
	// 이름 및 type의 최대 길이는 32까지.
	char *ClientType;
	char *ClientName;

	void ClientsListInitialize() {
		ClientNumber = -1;
		ClientType = new char[BUFFER_MAX_64];
		ClientName = new char[BUFFER_MAX_64];
	}

	void ClientsListDeinitialize() {
		ClientNumber = -1;
		delete ClientType;
		delete ClientName;
	}

  _ClientsList() { ClientsListInitialize(); }
  //~_ClientsList() { ClientsListDeinitialize(); }
} ClientsList;

typedef struct pollfd TCPPollFd;

class Telepathy {
private:

public:
	// Server Class
	class Server {

	private:
		Thread _Thread;
#if defined(WINDOWS_SYS)
		WSADATA _WSAData;
		SOCKET _ServerSocket;
		SOCKADDR_IN _ServerAddress;
#elif defined(POSIX_SYS)
    sockaddr_in _ServerAddress;
    int _ServerSocket;

    TCPPollFd _Fds[POLL_MAX_CONNECTIONS];
    int _ConnectionNumber;

    int _AddClient();
    bool _Receive(
#if defined(WINDOWS_SYS)
        SOCKET
#elif defined(POSIX_SYS)
        int
#endif
    ClientSocket);
#endif

	public:
		Server();
		~Server();

		//list<SOCKET> ConnectorsSocketList;
		vector<ClientsList> ConnectedClientList;

		bool IsInitializeServer;
		bool IsServerStarted;

		// server Callback
		typedef void (* _T_SERVERRECEIVEDCALLBACK)(char *Buffer,
#if defined(WINDOWS_SYS)
                                               SOCKET
#elif defined(POSIX_SYS)
                                               int
#endif
                                               ClientSocket);
		typedef void (* _T_ANYCONNECTIONNOTIFIER)(
#if defined(WINDOWS_SYS)
                                              SOCKET
#elif defined(POSIX_SYS)
                                              int
#endif
                                              ClientSocket);
    typedef void (* _T_ANYDISCONNECTIONNOTIFIER)(
#if defined(WINDOWS_SYS)
                                                 SOCKET
#elif defined(POSIX_SYS)
                                                 int
#endif
                                                 ClientSocket);
		// Server Receive Callback Pointer.
		_T_SERVERRECEIVEDCALLBACK TServerReceivedCallback;
		_T_ANYCONNECTIONNOTIFIER TAnyConnectionNotifier;
    _T_ANYDISCONNECTIONNOTIFIER TAnyDisconnectionNotifier;

		bool Initialize_Server();
		bool Start_Server();
		void Close_Server();

		bool SendDataToOne(char *Str,
#if defined(WINDOWS_SYS)
                       SOCKET
#elif defined(POSIX_SYS)
                       int
#endif
                       ClientSocket);
		void SendDataToAll(char *Str);

		// using pthread
		static void *Server_ConnectionThread(void *Param);
    /*
		// using pthread
		static void *Server_ReceivingThread(void *Param);
     */
	};

	// Client Class
	class Client {
	private:
		Thread _Thread;
		unsigned int _Address;
#if defined(WINDOWS_SYS)
		HOSTENT *_HostEntry;
		WSADATA _WSAData;
		SOCKET _ClientSocket;
		SOCKADDR_IN _ClientAddress;
#elif defined(POSIX_SYS)
    hostent *_HostEntry;
    int _ClientSocket;
    sockaddr_in _ClientAddress;
#endif		
	public:
		Client();
		~Client();

		bool IsInitializeClient;
		bool IsConnectedClient;

		// client Callback
		typedef void (* _T_CLIENTRECEIVEDCALLBACK)(char *Buffer);
		typedef void (* _T_CLIENTDISCONNECTEDCALLBACK)();

		// Client Receive Callback Pointer.
		_T_CLIENTRECEIVEDCALLBACK TClientReceivedCallback;
		_T_CLIENTDISCONNECTEDCALLBACK TClientDisconnectedCallback;

		bool ClientInitialize();
		void ClientReceiveStart();
		void ClientClose();
		bool ClientReceiving();

		bool ClientConnect();
		void ClientDisconnect();
		//bool ClientReConnect();

		bool SendData(char *Str);

		// using pthread
		static void *Client_ReceivingThread(void *Param);
	};
};
#endif // _Telepathy_hpp_

#endif // SET_COMMON_MODULE_ETHERNET