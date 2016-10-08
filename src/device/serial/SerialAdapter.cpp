/*********************************************************************

          File: SerialAdapter.cpp

   Description: SerialAdapter Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/10/02

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

#include "SerialAdapter.hpp"

#if defined(_SerialAdapter_hpp_)

SerialAdapter::SerialAdapter() {
  _Initialize();
}

SerialAdapter::SerialAdapter(const char *DevPath, int BaudRate) {
  _Initialize();
  Initialize_Serial(DevPath, BaudRate);
}

SerialAdapter::SerialAdapter(const char *DevPath, int BaudRate, TerminalOpt Opt) {
  _Initialize();
  Initialize_Serial(DevPath, BaudRate, Opt);
}

SerialAdapter::~SerialAdapter() {
  _SerialAdapterStarted = false;
  Disconnect_Serial();
}

void SerialAdapter::_SetDefaultOptions(TerminalOpt &__GetTerminalOpt) {
  __GetTerminalOpt.c_cflag = _BaudRate | CLOCAL | CREAD;
  __GetTerminalOpt.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
  __GetTerminalOpt.c_cflag |= CS8;
  //__GetTerminalOpt.c_iflag &= ~(IXON | IXOFF | IXANY);
  //__GetTerminalOpt.c_oflag &= ~OPOST;
  __GetTerminalOpt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  //__GetTerminalOpt.c_lflag |= ICANON | ECHO | ECHOE | ISIG;
  //__GetTerminalOpt.c_lflag |= ICANON;
  //__GetTerminalOpt.c_cc[VMIN] = 0;
  //__GetTerminalOpt.c_cc[VTIME] = 0;
}

void SerialAdapter::_Initialize() {
  _SerialAdapterStarted = false;

  _SerialPool[0].fd = -1;
  _SerialPool[0].events = POLLIN | POLLERR;
  _SerialPool[0].revents = 0;
  _DevicePath = "";
  _BaudRate = 0;
}

int SerialAdapter::_OpenSerial() {
  if (_DevicePath != "") {
    _SerialPool[0].fd = open(_DevicePath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY); // | O_NONBLOCK

    if (_SerialPool[0].fd == -1)
      return -1;
  }

  return 0;
}

bool SerialAdapter::_CloseSerial() {
  if (_SerialPool[0].fd != -1) {
    int _TVal = close(_SerialPool[0].fd);

    if (_TVal == -1)
      return false;
  }

  return true;
}

void SerialAdapter::_StartSerial() {
  if (_SerialPool[0].fd != -1) {
    _SerialAdapterStarted = true;
    //_Thread.AttacheMode = true;
    _Thread.StartThread(_SerialAdapter_WorkerThread, this);
  }
  else
    printf("Not Started. Error.\n");
}

void SerialAdapter::_StopSerial() {
  if (_SerialPool[0].fd != -1) {
    _SerialAdapterStarted = false;
  }
  else
    printf("Not Started. Error.\n");
}

void *SerialAdapter::_SerialAdapter_WorkerThread(void *Param) {
  SerialAdapter *_TEngine = (SerialAdapter *)Param;
  string _TRecvStringBuffer = "";
  bool _TIsReturnChar = false;

  while (_TEngine->_SerialAdapterStarted == true) {
    int _TFds = poll(&_TEngine->_SerialPool[0], MAX_SERIAL_CONNECTION, -1);
    char _TBuffer[BUFFER_MAX_512] ={0,};

    memset(_TBuffer, 0, sizeof(_TBuffer));

    if (_TEngine->_SerialPool[0].revents & POLLIN) { //  | POLLERR
      if (_TFds == 0) {
        // Time out.
      }
      else {
        if(_TFds > 0) {
          //if( fds[0].revents & POLLIN ) {
          //string _TStringBuffer = "";
          int _TLength = read(_TEngine->_SerialPool[0].fd, _TBuffer, sizeof(_TBuffer));
          _TBuffer[_TLength] = 0;

          for (int i=0; i<_TLength; i++) {
            if (_TBuffer[i] == '\r')
              _TIsReturnChar = true;
            else
              _TRecvStringBuffer += _TBuffer[i];
          }
        }
        else { // _TFds == 1
          // poll error.
          //printf("No data within 5 seconds.\n");
          //_TEngine->_StopSerial();
        }

        if (_TIsReturnChar == true) {
          // callback.
          _TEngine->TReceiveCallback(_TRecvStringBuffer);
          _TRecvStringBuffer = "";
          _TIsReturnChar = false;
        }
      }
    }
    else {
      if (_TEngine->_SerialPool[0].revents & POLLHUP) {
        _TEngine->_StopSerial();
        printf("Serial Connection is broken. Connection Closed.");
        break;
      }
      else if (_TEngine->_SerialPool[0].revents & POLLNVAL) {
        _TEngine->_StopSerial();
        printf("File Desc invaild. Connection Closed.");
        break;
      }
      else if (_TEngine->_SerialPool[0].revents & POLLOUT) {

      }
    }
  }

  return 0;
}

bool SerialAdapter::Initialize_Serial(const char *DevPath, int BaudRate) {
  Set_DevicePath(DevPath);

  // Serial Open
  _OpenSerial();

  if (_SerialPool[0].fd != -1) {
    Set_BaudRate(BaudRate);

    // Set Flag
    TerminalOpt _TOpt = Get_SerialDeviceOption();
    _SetDefaultOptions(_TOpt);
    Set_SerialDeviceOption(_TOpt);

    // store Serial Options.
    Set_SerialOption(_TOpt);
  }
  else
    return false;

  return true;
}

bool SerialAdapter::Initialize_Serial(const char *DevPath, int BaudRate, TerminalOpt Opt) {
  Set_DevicePath(DevPath);

  // Serial Open
  _OpenSerial();

  if (_SerialPool[0].fd != -1) {
    Set_BaudRate(BaudRate);

    // 밀어넣기 함..
    Set_SerialDeviceOption(Opt);
    Set_SerialOption(Opt);
  }
  else
    return false;

  return true;
}

bool SerialAdapter::Connect_Serial() {
  if (_SerialPool[0].fd != -1 && _SerialAdapterStarted == false) {
    // _InputBaudRate();
    // _InputSerialOption();
    _StartSerial();
  }
  else {
    printf("Connection Failed.");
    return false;
  }
  return true;
}

void SerialAdapter::Disconnect_Serial() {
  if (_SerialPool[0].fd != -1) {
    if (_SerialAdapterStarted == true) {
      if (_CloseSerial() == true)
        _SerialPool[0].fd != -1;
    }
  }
  else
    printf("Not Disconnected. Error.\n");
}

void SerialAdapter::Send(string CommandStr) {
  if (_SerialPool[0].fd != -1) {
    if (_SerialAdapterStarted == true) {
      CommandStr += '\r';
      write(_SerialPool[0].fd, CommandStr.c_str(), CommandStr.length());
    }
  }
  else
    printf("Not Sended. Error.\n");
}

TerminalOpt SerialAdapter::Get_SerialDeviceOption() {
  TerminalOpt _TTerminalOpt;
  if (_SerialPool[0].fd != -1) {
    tcgetattr(_SerialPool[0].fd, &_TTerminalOpt);
  }
  return _TTerminalOpt;
}

void SerialAdapter::Set_SerialDeviceOption(TerminalOpt Options) {
  if (_SerialPool[0].fd != -1) {
    tcsetattr(_SerialPool[0].fd, TCSANOW, &Options);
  }
}

#endif