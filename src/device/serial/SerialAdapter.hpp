/*********************************************************************

          File: SerialAdapter.hpp

   Description: SerialAdapter Header Part.

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

#include "modinclude.hpp"

#if defined(SET_DEVICE_SERIAL)

#ifndef _SerialAdapter_hpp_
#define _SerialAdapter_hpp_

#include "Thread.hpp"

#include "SerialSupport.hpp"
#include "SystemUtil.hpp"

#define MAX_SERIAL_CONNECTION 1

namespace SerialEnum {
  enum ConnectionSpeed {
    S_0 = B0, S_50 = B50, S_75 = B75, S_110 = B110, S_134 = B134,
    S_150 = B150, S_200 = B200, S_300 = B300, S_600 = B600, S_1200 = B1200,
    S_1800 = B1800, S_2400 = B2400, S_4800 = B4800, S_9600 = B9600, S_19200 = B19200,
    S_38400 = B38400,
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
    S_7200 = B7200, S_14400 = B14400, S_57600 = B57600, S_76800 = B76800, S_115200 = B115200,
    S_230400 = B230400
#endif
  };
}

typedef pollfd InternalPollfd;
typedef termios TerminalOpt;

class SerialAdapter {
private:
  InternalPollfd _SerialPool[MAX_SERIAL_CONNECTION];

  string _DevicePath;
  int _BaudRate;
  TerminalOpt _TerminalOpt;

  bool _SerialAdapterStarted;

  Thread _Thread;

  // Don't touch this function.
  void _SetDefaultOptions(TerminalOpt &__GetTerminalOpt);

  void _Initialize();

  int _OpenSerial();
  bool _CloseSerial();

  void _StartSerial();
  void _StopSerial();

  static void *_SerialAdapter_WorkerThread(void *Param);

public:
  SerialAdapter();
  SerialAdapter(const char *DevPath, int BaudRate = SerialEnum::S_9600);
  SerialAdapter(const char *DevPath, int BaudRate, TerminalOpt Opt);

  ~SerialAdapter();

  typedef void(*_T_RECEIVECALLBACK)(string CommandStr);
  _T_RECEIVECALLBACK TReceiveCallback;

  //bool Initialize_Serial(const char *DevPath);
  bool Initialize_Serial(const char *DevPath, int BaudRate = SerialEnum::S_9600);
  bool Initialize_Serial(const char *DevPath, int BaudRate, TerminalOpt Opt);
  bool Connect_Serial();
  void Disconnect_Serial();

  void Send(string CommandStr);

  TerminalOpt Get_SerialDeviceOption();
  void Set_SerialDeviceOption(TerminalOpt Options);

  IMPLEMENT_GET_SET(string, DevicePath, _DevicePath)
  IMPLEMENT_GET_SET(int, BaudRate, _BaudRate)
  IMPLEMENT_GET_SET(TerminalOpt, SerialOption, _TerminalOpt)
};

#endif // _SerialAdapter_hpp_

#endif