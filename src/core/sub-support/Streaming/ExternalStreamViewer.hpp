/*********************************************************************

          File: ExternalStreamViewer.hpp

   Description: ExternalStreamViewer Header Part.

 Origin Author: Irene M. Gironacci
      Modifier: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/10/10

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

#if defined(SET_LIB_GSTREAMER)

#ifndef _ExternalStreamViewer_hpp_
#define _ExternalStreamViewer_hpp_

#include "OpenCVSupport.hpp"
#include "GStreamerSupport.hpp"

#include "ExtendedBlackBox.hpp"
#include "Thread.hpp"

#include <queue>

#define STREAM_RESOLUTION_X 640
#define STREAM_RESOLUTION_Y 480

#define VIEWER_IP "127.0.0.1"
#define VIEWER_PORT 5002

class ExternalStreamViewer {
private:
  typedef struct _App {
    GstElement *ffmpeg;
    GstElement *ffmpeg2;
    GstElement *rtppay, *gdppay;
    GstElement *videoenc;
    GstElement *videosrc;
    GstElement *sink;
    GstElement *videoscale;
    GstElement *filter;
    guint sourceid;
    GstElement *queue;
    GTimer *timer;
  }App;
  App s_app;

  GMainLoop *_G_Loop;
  GstCaps *_GstCapsRGB;
  GstCaps *_GstCapsYUV;
  GstBus *_GstBus;
  GstElement *_Pipeline;
  App *_GstApp;

  std::queue<Mat> _ImageQueue;

  int _Res_X;
  int _Res_Y;
  int _Port;

  int _LoopCount;

  bool _ExternalStreamViewerActivated;
  bool _ExternalStreamViewerStarted;

  Thread _Thread;
  ThreadMutex _Mutex_ImageQueue;

  void _Initialize_Members();
  void _Deinitialize_Members();
  void _Initialize();
  void _Deinitialize();

  bool _IsEmptyImageQueue();

  void _SettingExternalStreamViewer();

  void _OpenExternalStreamViewer();
  void _PlayExternalStreamViewer();
  void _CloseExternalStreamViewer();

  // Callbacks.
  static gboolean _NeedData_Callback(App *__App);
  static void _StartFeed_Callback(GstElement *__Pipeline, guint __Size, App *__App);
  static void _StopFeed_Callback(GstElement *__Pipeline, App *__App);
  static gboolean _BusCall_Callback(GstBus *__Bus, GstMessage *__Message, gpointer __Data);
  static void _CheckLogHandler_Callback(const gchar *const __LogDomain,
                                        const GLogLevelFlags __LogLevel, const gchar *const __Message,
                                        gpointer const __UserData);
  // Thread.
  static void *_ExternalStreamViewer_WorkerThread(void *Param);

public:
  ExternalStreamViewer() :
      _Res_X(STREAM_RESOLUTION_X), _Res_Y(STREAM_RESOLUTION_Y), _Port(VIEWER_PORT) { _Initialize(); }
  ExternalStreamViewer(int __Res_X, int __Res_Y) :
      _Res_X(__Res_X), _Res_Y(__Res_Y), _Port(VIEWER_PORT) { _Initialize(); }
  ExternalStreamViewer(int __Res_X, int __Res_Y, int __PORT) :
      _Res_X(__Res_X), _Res_Y(__Res_Y), _Port(__PORT) { _Initialize(); }

  ~ExternalStreamViewer() { _Deinitialize(); }

  void Start_ExternalStreamViewer();
  void Stop_ExternalStreamViewer();

  void Push_ImageQueue(Mat __Image);
  void Set_Resolution(int __Res_X, int __Res_Y);

  IMPLEMENT_GET_SET(int, ResolutionX, _Res_X)
  IMPLEMENT_GET_SET(int, ResolutionY, _Res_Y)
  IMPLEMENT_GET_SET(int, Port, _Port)
  IMPLEMENT_GET(bool, ExternalStreamViewerStarted, _ExternalStreamViewerStarted);
};

#endif // _ExternalStreamViewer_hpp_

#endif