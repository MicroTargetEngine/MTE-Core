/*********************************************************************

          File: Thread.hpp

   Description: Thread Header Part.

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

#if defined(SET_COMMON_MODULE_THREAD)

#ifndef _Thread_hpp_
#define _Thread_hpp_

#include "SystemCommon.hpp"

#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
#include <windows.h>
#include <process.h>
#else
#include "pthreadSupport.hpp"
#endif

//#include "LogD.hpp"

#if defined(SET_LIB_PTHREAD)
#define ThreadExit pthread_exit
#define ThreadSelf pthread_self

#define ThreadMutex pthread_mutex_t
#define ThreadCond pthread_cond_t
#define ThreadSemaphore sem_t

#define __SEMWAIT(__SemVar) sem_wait(&__SemVar)
#define __SEMPOST(__SemVar) sem_post(&__SemVar)

#define __MUTEXINIT(__MutexVar) { __MutexVar = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; \
                                  pthread_mutex_init(&__MutexVar, NULL); }
#define __MUTEXDESTROY(__MutexVar) pthread_mutex_destroy(&__MutexVar)
#define __MUTEXLOCK(__MutexVar) pthread_mutex_lock(&__MutexVar)
#define __MUTEXTRYLOCK(__MutexVar) pthread_mutex_trylock(&__MutexVar)
#define __MUTEXUNLOCK(__MutexVar) pthread_mutex_unlock(&__MutexVar)

#define __CONDINIT(__CondVar) { __CondVar = (pthread_cond_t)PTHREAD_COND_INITIALIZER; \
                                pthread_cond_init(&__CondVar, NULL); }
#define __CONDDESTROY(__CondVar) pthread_cond_destroy(&__CondVar)
#define __CONDWAIT(__CondVar, __MutexVar) pthread_cond_wait(&__CondVar, &__MutexVar)
#define __CONDTIMEWAIT(__CondVar, __MutexVar, __Time) pthread_cond_timedwait(&__CondVar, &__MutexVar, &__Time)
#define __CONDSIGNAL(__CondVar) pthread_cond_signal(&__CondVar)
#define __CONDBROADCAST(__CondVar) pthread_cond_broadcast(&__CondVar)

#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
#define ThreadMutex HANDLE
#define ThreadCond CONDITION_VARIABLE
#define ThreadCS CRITICAL_SECTION
//#define ThreadSemaphore sem_t

// #define __SEMWAIT(__SemVar) sem_wait(&__SemVar) //
// #define __SEMPOST(__SemVar) sem_post(&__SemVar) //

#define __MUTEXINIT(__MutexVar) { __MutexVar = CreateMutex(NULL, FALSE, NULL); }
#define __MUTEXDESTROY(__MutexVar) CloseHandle(&__MutexVar)
#define __MUTEXLOCK(__MutexVar) WaitForSingleObject(__MutexVar, INFINITE)
#define __MUTEXUNLOCK(__MutexVar) ReleaseMutex(__MutexVar)

// This part is critical section part(only windows).
#define __CSINIT(__CSVar) InitializeCriticalSection(&__CSVar)
#define __CSDESTROY(__CSVar) DeleteCriticalSection(&__CSVar)
#define __CSLOCK(__CSVar) EnterCriticalSection(&__CSVar)
#define __CSUNLOCK(__CSVar) LeaveCriticalSection(&__CSVar)

// This part is condition variable using critical section part(only windows).
#define __CONDINIT(__CondVar) InitializeConditionVariable(&__CondVar)
#define __CONDWAIT(__CondVar, __MutexVar) SleepConditionVariableCS(&__CondVar, &__MutexVar, INFINITE)
#define __CONDTIMEWAIT(__CondVar, __MutexVar, __Time) SleepConditionVariableCS(&__CondVar, &__MutexVar, (__Time * 1000))
#define __CONDSIGNAL(__CondVar) WakeConditionVariable(&__CondVar)
#define __CONDBROADCAST(__CondVar) WakeAllConditionVariable(&__CondVar)

#endif

// easy use Semaphore.
#define IMPLEMENT_SEMAPHORE_GET(__ReturnType, __FuncName, __ReturnValue, __SemaphoreVal) \
	__ReturnType _Get##__FuncName##WithSemaphore() \
	{ \
	__ReturnType _TVal; \
	__SEMWAIT(__SemaphoreVal); \
	_TVal = __ReturnValue; \
	__SEMPOST(__SemaphoreVal); \
	return _TVal; \
	}

#define IMPLEMENT_SEMAPHORE_SET(__Type, __FuncName, __DestValue, __SemaphoreVal) \
	void _Set##__FuncName##WithSemaphore(__Type __TxV) \
	{ \
	__SEMWAIT(__SemaphoreVal); \
	__DestValue = __TxV; \
	__SEMPOST(__SemaphoreVal); \
	}

#define IMPLEMENT_SEMAPHORE_GET_SET(__Type, __FuncName, __Value, __MutexVal) \
	IMPLEMENT_SEMAPHORE_GET(__Type, __FuncName, __Value, __SemaphoreVal) \
	IMPLEMENT_SEMAPHORE_SET(__Type, __FuncName, __Value, __SemaphoreVal)

// easy use mutex.
#define IMPLEMENT_MUTEX_GET(__ReturnType, __FuncName, __ReturnValue, __MutexVal) \
	__ReturnType _Get##__FuncName##WithMutex() \
	{ \
	__ReturnType _TVal; \
	__MUTEXLOCK(__MutexVal); \
	_TVal = __ReturnValue; \
	__MUTEXUNLOCK(__MutexVal); \
	return _TVal; \
	}

#define IMPLEMENT_MUTEX_SET(__Type, __FuncName, __DestValue, __MutexVal) \
	void _Set##__FuncName##WithMutex(__Type __TxV) \
	{ \
	__MUTEXLOCK(__MutexVal); \
	__DestValue = __TxV; \
	__MUTEXUNLOCK(__MutexVal); \
	}

#define IMPLEMENT_MUTEX_GET_SET(__Type, __FuncName, __Value, __MutexVal) \
	IMPLEMENT_MUTEX_GET(__Type, __FuncName, __Value, __MutexVal) \
	IMPLEMENT_MUTEX_SET(__Type, __FuncName, __Value, __MutexVal)

class SyncSignal {
private:
	ThreadMutex _Mutex;
#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  ThreadCS _CriticalSection;
#endif
	ThreadCond _Cond;

	bool _IsWaiting;

	void _Initialize();
	void _Deinitialize();

public:
	SyncSignal();
	~SyncSignal();

	void Wait();
	void Signal();

	IMPLEMENT_GET(bool, IsWaiting, _IsWaiting);
};

class Thread {
private:
#if defined(SET_LIB_PTHREAD)
	pthread_t _Thread;
	pthread_attr_t _ThreadAttr;
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  HANDLE _Thread;
  DWORD _ThreadID;
#endif
  bool _IsThreadStarted;

  void _Initialize();
  void _Deinitialize();
public:
	Thread()
#if defined(SET_LIB_PTHREAD)
      : AttacheMode(false), KernelMode(false)
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
      : StackSize(0), InitFlag(0), WaitingTime(INFINITE)
#endif
  { _Initialize(); }

#if defined(SET_LIB_PTHREAD)
	Thread(bool ExcuteAttacheMode, bool ExcuteKernelMode) : AttacheMode(ExcuteAttacheMode), KernelMode(ExcuteKernelMode)
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  // This initializer for windows thread
  Thread(int _StackSize = 0) : StackSize(_StackSize), WaitingTime(INFINITE)
  // it can be able to set "initflag"
  Thread(DWORD _InitFlag = 0) : StackSize(0), InitFlag(_InitFlag), WaitingTime(INFINITE)
  Thread(int _StackSize = 0, DWORD _WaitingTime = INFINITE) : StackSize(_StackSize), WaitingTime(_WaitingTime)
  Thread(int _StackSize = 0, DWORD _InitFlag = 0) : StackSize(_StackSize), InitFlag(_InitFlag)
  Thread(int _StackSize = 0, DWORD _InitFlag = 0, DWORD _WaitingTime = INFINITE) : StackSize(_StackSize), InitFlag(_InitFlag), WaitingTime(_WaitingTime)
#endif
  { _Initialize(); }

	~Thread();

	//LogD *ProgLog;
#if defined(SET_LIB_PTHREAD)
	// Thread 시작 및 Join 관련.
	bool AttacheMode;
	bool KernelMode;
#elif defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  int StackSize;
  DWORD WaitingTime;
  DWORD InitFlag;
#endif
	void StartThread(void *(*_StartAddress) (void *), void *Argument);
	int JoinThread();
#if defined(WINDOWS_SYS) && !defined(SET_LIB_PTHREAD)
  void ResumeThread();
#endif
};

#endif // _Thread_hpp_

#endif // SET_COMMON_MODULE_THREAD