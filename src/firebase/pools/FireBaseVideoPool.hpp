/*********************************************************************

          File: FireBaseVideoPool.hpp

   Description: FireBaseVideoPool Header Part.

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

#if defined(SET_LIB_OPENCV)

#ifndef _FireBaseVideoPool_hpp_
#define _FireBaseVideoPool_hpp_

#include "OpenCVSupport.hpp"
#include "Thread.hpp"

#if defined(SET_DEVICE_MMAL)
#include "mmalAdapter.hpp"
#endif

#define CAM_RESOLUTION_X 640
#define CAM_RESOLUTION_Y 480

class FireBaseVideoPool {
private:
  Thread _VisionThread;
  ThreadMutex _Mutex_View;

  Mat _ViewMat;

  bool _VideoPoolActivated;
  bool _VideoPoolStarted;

  int _CamResWidth;
  int _CamResHeight;
  int _CamPort;

  void _Initialize_Members();
  static void *_FireBaseVideoPool_VisionThread(void *Param);
public:
  FireBaseVideoPool();
  ~FireBaseVideoPool();

  typedef void(*_T_EXTERNALVIDEOCALLBACK)(Mat __View);
  _T_EXTERNALVIDEOCALLBACK TExternalVideoCallback;

  void Start_VideoPool();
  void Stop_VideoPool();

  void Set_Resolution(int __Width, int __Height);
  void Set_CameraPort(int __Port);

  IMPLEMENT_GET(bool, VideoPoolStarted, _VideoPoolStarted)
  IMPLEMENT_GET_SET(int, CamWidth, _CamResWidth)
  IMPLEMENT_GET_SET(int, CamHeight, _CamResHeight)
};

#endif // _FireBaseVideoPool_hpp_

#endif // SET_LIB_OPENCV