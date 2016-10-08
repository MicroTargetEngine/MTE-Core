/*********************************************************************

          File: Thread.cpp

   Description: Thread Source Part.

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

#include "Thread.hpp"

#if defined(_Thread_hpp_)

SyncSignal::SyncSignal() {
	_Initialize();
}

SyncSignal::~SyncSignal() {
	_Deinitialize();
}

void SyncSignal::_Initialize() {
	__CONDINIT(_Cond);
#if defined(SET_LIB_PTHREAD)
	__MUTEXINIT(_Mutex);
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  __CSINIT(_CriticalSection);
#endif
	_IsWaiting = false;
}

void SyncSignal::_Deinitialize() {
	__CONDDESTROY(_Cond);
#if defined(SET_LIB_PTHREAD)
	__MUTEXDESTROY(_Mutex);
#endif
	_IsWaiting = false;
}

void SyncSignal::Wait() {
	_IsWaiting = true;
#if defined(SET_LIB_PTHREAD)
	__MUTEXLOCK(_Mutex);
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  __CSLOCK(_CriticalSection)
#endif
	__CONDWAIT(_Cond, _Mutex);
#if defined(SET_LIB_PTHREAD)
  __MUTEXUNLOCK(_Mutex);
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  __CSUNLOCK(_CriticalSection)
#endif
}

void SyncSignal::Signal() {
	_IsWaiting = false;
	__CONDSIGNAL(_Cond);
}

// Area of Constructor & Destructor.
#pragma region Constructor & Destructor
/*
#if defined(SET_PTHREAD)
Thread::Thread() {
	// 기본 Mode는 Detached, User Mode.
	AttacheMode = false;
	KernelMode = false;
}
#endif
*/
/*
// this function for pthread
#if defined(SET_PTHREAD)
Thread::Thread(bool ExcuteAttacheMode, bool ExcuteKernelMode) {
	AttacheMode = ExcuteAttacheMode;
	KernelMode = ExcuteKernelMode;
}
#endif
*/
Thread::~Thread() {
  _Deinitialize();
}
#pragma endregion Constructor & Destructor

// Area of Private Methods.
#pragma region Private Methods
void Thread::_Initialize() {
  _IsThreadStarted = false;
}

void Thread::_Deinitialize() {
#if defined(SET_LIB_PTHREAD)
  AttacheMode = false;
  KernelMode = false;
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  StackSize = 0;
  WaitingTime = (DWORD)0;
  InitFlag = (DWORD)0;
#endif
  _IsThreadStarted = false;
}
#pragma endregion Private Methods

// Area of Public Methods.
#pragma region Public Methods
void Thread::StartThread(void *(*_StartAddress) (void *), void *Argument) {
#if defined(SET_LIB_PTHREAD)
	int _TAttacheMode = -1;
	int _TKernelMode = -1;
	_TAttacheMode = (AttacheMode == false) ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE;
	_TKernelMode = (KernelMode == false) ? PTHREAD_SCOPE_SYSTEM : PTHREAD_SCOPE_PROCESS;

	// pthread attribute initialize.
	pthread_attr_init(&_ThreadAttr);
	// Detached thread.
	pthread_attr_setdetachstate(&_ThreadAttr, _TAttacheMode);
	// User space thread(linux do not support kernel mode thread, then don't use & set
	// 'PTHREAD_SCOPE_PROCESS' at pthread_attr_setscope function to scope parameter).
	pthread_attr_setscope(&_ThreadAttr, _TKernelMode);
	// Create thread.
  _IsThreadStarted = true;
	if (pthread_create(&_Thread, NULL, _StartAddress, (void *)Argument) < 0) {
		//if (ProgLog != NULL)
		//	ProgLog->Logging("Don't create thread.");
    _IsThreadStarted = false;
		return ;
	}
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  _Thread = (HANDLE)_beginthreadex(NULL, StackSize, _StartAddress, Argument, InitFlag, (unsigned *)&_ThreadID);
#endif
}

int Thread::JoinThread() {
	// _TResult가 -1이면 detatch mode이므로 실행 안됨을 나타냄.
	int _TRetult = -1;
#if defined(SET_LIB_PTHREAD)
	if (AttacheMode != false) {
		pthread_join(_Thread, (void **)&_TRetult);
	}
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  _TRetult = WaitForSingleObject(_Thread, WaitingTime);
#endif
	return _TRetult;
}

#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
void Thread::ResumeThread() {
  if(InitFlag == CREATE_SUSPENDED && _IsThreadStarted == true) {
    ResumeThread(_Thread);
  }
}
#endif
#pragma endregion Public Methods
#endif // _Thread_hpp_