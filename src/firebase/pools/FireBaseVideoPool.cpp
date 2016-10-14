/*********************************************************************

          File: FireBaseVideoPool.cpp

   Description: FireBaseVideoPool Source Part.

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

#include "FireBaseVideoPool.hpp"

#if defined(_FireBaseVideoPool_hpp_)

#if defined(LOG_WRITE_MODE)
#include "LogD.hpp"
#endif

FireBaseVideoPool::FireBaseVideoPool() {
  _Initialize_Members();
}

FireBaseVideoPool::~FireBaseVideoPool() {
  _Initialize_Members();
}

void FireBaseVideoPool::_Initialize_Members() {
  _VideoPoolActivated = false;
  _VideoPoolStarted = false;

  _CamResWidth = CAM_RESOLUTION_X;
  _CamResHeight = CAM_RESOLUTION_Y;
  _CamPort = 1;
}

void *FireBaseVideoPool::_FireBaseVideoPool_VisionThread(void *Param) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into _FireBaseVideoPool_VisionThread Function");
#endif
  FireBaseVideoPool *_TVideoPool = (FireBaseVideoPool *)Param;
  _TVideoPool->_VideoPoolStarted = true;

#if defined(SET_TARGET_INTEL)
  VideoCapture _TCapture(_TVideoPool->_CamPort);
  _TCapture.set(CV_CAP_PROP_FRAME_WIDTH, _TVideoPool->_CamResWidth);
  _TCapture.set(CV_CAP_PROP_FRAME_HEIGHT, _TVideoPool->_CamResHeight);

  namedWindow("FireBase-Core_Test");

  if (_TCapture.isOpened() == false)
    return 0;
#else
  // for mmal.
  mmalAdapter _TCapture;
  _TCapture.open(_TVideoPool->_CamResWidth, _TVideoPool->_CamResHeight, true);
#endif
  __MUTEXINIT(_TVideoPool->_Mutex_View);

  while (_TVideoPool->_VideoPoolActivated == true) {
#if defined(SET_TARGET_INTEL)
    // 영상을 줌.
    _TCapture >> _TVideoPool->_ViewMat;
#else
    // for mmal.
    _TVideoPool->_ViewMat = _TCapture.grab();
#endif
    // 반드시 Clone 해서 주어야 함.
    _TVideoPool->TExternalVideoCallback(_TVideoPool->_ViewMat.clone());

#if defined(SET_TARGET_INTEL)
    // 정보를 그려 보여준다.
    //_TAdapter->_SetInfomation(_TAdapter->_ViewMat);
    //imshow("FireBase-Core_Test", _TVideoPool->_ViewMat);

    //cv::waitKey(0);
#else
    // for mmal.
#endif
  }

  __MUTEXDESTROY(_TVideoPool->_Mutex_View);
  _TVideoPool->_VideoPoolStarted = false;

  return 0;
}

void FireBaseVideoPool::Start_VideoPool() {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into Start_VideoPool Function");
#endif
  if (_VideoPoolActivated != true) {
    _VideoPoolActivated = true;
    // Activate Vision Thread.
    _VisionThread.StartThread(_FireBaseVideoPool_VisionThread, this);
  }
}

void FireBaseVideoPool::Stop_VideoPool() {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into Stop_VideoPool Function");
#endif
  if (_VideoPoolActivated == true) {
    _VideoPoolActivated = false;
  }
}

void FireBaseVideoPool::Set_Resolution(int __Width, int __Height) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into Set_Resolution Function");
#endif
  _CamResWidth = __Width;
  _CamResHeight = __Height;
}

void FireBaseVideoPool::Set_CameraPort(int __Port) {
#if defined(LOG_WRITE_MODE)
  G_LogD->Logging("Func", "into Set_CameraPort Function");
#endif
  _CamPort = __Port;
}

#endif