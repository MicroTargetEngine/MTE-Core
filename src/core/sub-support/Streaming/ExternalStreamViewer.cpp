/*********************************************************************

          File: ExternalStreamViewer.cpp

   Description: ExternalStreamViewer Source Part.

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

// Reffered by http://www.imgportal.net/home/wp-content/uploads/maris-script1.cpp
// see this, http://www.imgportal.net/home/?page_id=2595
// and, https://www.youtube.com/watch?v=2_yubW1qElA

#include "ExternalStreamViewer.hpp"

#if defined(_ExternalStreamViewer_hpp_)

//#include "Thread.hpp"

ExternalStreamViewer *G_ExternalStreamViewer;

void ExternalStreamViewer::_Initialize_Members() {
  _ExternalStreamViewerActivated = false;
  _LoopCount = 0;
  _GstApp = &s_app;
}

void ExternalStreamViewer::_Deinitialize_Members() {
  _ExternalStreamViewerActivated = false;
  _LoopCount = 0;
}

void ExternalStreamViewer::_Initialize() {
  _Initialize_Members();
  G_ExternalStreamViewer = this;
}

void ExternalStreamViewer::_Deinitialize() {
  _Deinitialize_Members();
  G_ExternalStreamViewer = NULL;
}

void ExternalStreamViewer::_SettingExternalStreamViewer() {
  GstFlowReturn _TGstFlowReturn;

  gst_init(NULL, NULL);

  _G_Loop = g_main_loop_new(NULL, TRUE);

  _Pipeline = gst_pipeline_new("gstreamer-encoder");
  if (!_Pipeline) { g_print("Error creating Pipeline, exiting..."); }

  _GstApp->videosrc = gst_element_factory_make("appsrc", "videosrc");
  if (!_GstApp->videosrc) { g_print("Error creating source element, exiting..."); }

  _GstApp->queue = gst_element_factory_make("queue", "queue");
  if (!_GstApp->queue) { g_print("Error creating queue element, exiting..."); }

  _GstApp->filter = gst_element_factory_make("capsfilter", "filter");
  if (!_GstApp->filter) { g_print("Error creating filter, exiting..."); }

  _GstApp->videoscale = gst_element_factory_make("videoscale", "videoscale");
  if (!_GstApp->videoscale) { g_print("Error creating videoscale, exiting..."); }

  _GstApp->videoenc = gst_element_factory_make("x264enc", "videoenc");
  if (!_GstApp->videoenc) { std::cout << "Error creating encoder, exiting..."; }

  _GstApp->rtppay = gst_element_factory_make("rtph264pay", "rtppay");
  if (!_GstApp->rtppay) { std::cout << "Error creating rtppay, exiting..."; }

  _GstApp->gdppay = gst_element_factory_make("gdppay", "gdppay");
  if (!_GstApp->gdppay) { std::cout << "Error creating gdppay, exiting..."; }

  _GstApp->ffmpeg = gst_element_factory_make("videoconvert", "ffmpeg");  //!!!!!! ffenc_mpeg2video
  if (!_GstApp->ffmpeg) { g_print("Error creating ffmpegcolorspace, exiting..."); }

  _GstApp->ffmpeg2 = gst_element_factory_make("videoconvert", "ffmpeg2");  //!!!!!! ffenc_mpeg2video
  if (!_GstApp->ffmpeg) { g_print("Error creating ffmpegcolorspace, exiting..."); }

  _GstApp->sink = gst_element_factory_make("tcpserversink", "sink");
  if (!_GstApp->sink) { g_print("Error creating sink, exiting..."); }

  g_print("Elements are created\n");

  g_object_set(G_OBJECT (_GstApp->sink), "host", VIEWER_IP, NULL); //
  g_object_set(G_OBJECT (_GstApp->sink), "port", _Port, NULL);
  g_object_set(G_OBJECT (_GstApp->sink), "sync", FALSE, NULL);

  g_object_set(G_OBJECT (_GstApp->videoenc), "bitrate", 256, NULL);
  g_object_set(G_OBJECT (_GstApp->videoenc), "noise-reduction", 10000, NULL);
  gst_util_set_object_arg(G_OBJECT (_GstApp->videoenc), "tune", "zerolatency");
  gst_util_set_object_arg(G_OBJECT (_GstApp->videoenc), "pass", "qual");
  g_object_set(G_OBJECT (_GstApp->rtppay), "config-interval", 1, NULL);

  g_print("end of settings\n");
}

void ExternalStreamViewer::_OpenExternalStreamViewer() {
//proprietà da associare al filtro della _TPipeline(bisogna convertire in yuv):
  _GstCapsYUV = gst_caps_new_simple("video/x-raw",
                                    "format", G_TYPE_STRING, "I420",
                                    "width", G_TYPE_INT, _Res_X,
                                    "height", G_TYPE_INT, _Res_Y,
      //"width", G_TYPE_INT, 320,
      //"height", G_TYPE_INT, 240,
                                    "framerate", GST_TYPE_FRACTION, 15, 1,
                                    "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
                                    NULL);

  g_object_set(G_OBJECT (_GstApp->filter), "caps", _GstCapsYUV, NULL);

  _GstCapsRGB = gst_caps_new_simple("video/x-raw",
                                    "format", G_TYPE_STRING, "RGB",
                                    "width", G_TYPE_INT, _Res_X,
                                    "height", G_TYPE_INT, _Res_Y,
      //"width", G_TYPE_INT, 320,
      //"height", G_TYPE_INT, 240,
                                    "framerate", GST_TYPE_FRACTION, 15, 1,
                                    "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
                                    NULL);

  gst_app_src_set_caps(GST_APP_SRC(_GstApp->videosrc), _GstCapsRGB);

  g_object_set(G_OBJECT (_GstApp->videosrc), "stream-type", 0, "format", GST_FORMAT_TIME, NULL);

  _GstBus = gst_pipeline_get_bus(GST_PIPELINE (_Pipeline));
  g_assert(_GstBus);
  gst_bus_add_watch(_GstBus, (GstBusFunc) _BusCall_Callback, _GstApp);

  gst_bin_add_many(GST_BIN (_Pipeline), _GstApp->videosrc, _GstApp->ffmpeg, _GstApp->filter, _GstApp->videoenc, _GstApp->rtppay, _GstApp->gdppay,
                   _GstApp->sink, NULL);

  g_print("Added all the Elements into the _TPipeline\n");

  int _TOk = 0;
  _TOk = gst_element_link_many(_GstApp->videosrc, _GstApp->ffmpeg, _GstApp->filter, _GstApp->videoenc, _GstApp->rtppay, _GstApp->gdppay,
                               _GstApp->sink, NULL);

  if (_TOk) g_print("Linked all the Elements together\n");
  else g_print("*** Linking error ***\n");

  g_assert(_GstApp->videosrc);
  g_assert(GST_IS_APP_SRC(_GstApp->videosrc));

  g_signal_connect (_GstApp->videosrc, "need-data", G_CALLBACK(_StartFeed_Callback), _GstApp);
  g_signal_connect (_GstApp->videosrc, "enough-data", G_CALLBACK(_StopFeed_Callback), _GstApp);
}

void ExternalStreamViewer::_PlayExternalStreamViewer() {
  g_print("Playing the video\n");
  gst_element_set_state(_Pipeline, GST_STATE_PLAYING);
}

void ExternalStreamViewer::_CloseExternalStreamViewer() {
  g_print ("Returned, stopping playback\n");
  gst_element_set_state (_Pipeline, GST_STATE_NULL);
  gst_object_unref ( _GstBus);
  g_main_loop_unref (_G_Loop);
  g_print ("Deleting pipeline\n");
}

gboolean ExternalStreamViewer::_NeedData_Callback(App *__App) {
  static GstClockTime timestamp = 0;
  GstBuffer *_TGstBuffer;
  guint _TGstBuffersize;
  GstFlowReturn _TGstFlowReturn;
  GstMapInfo _TGstMapInfo;

  G_ExternalStreamViewer->_LoopCount++;

  // lock queue.
  __MUTEXLOCK(G_ExternalStreamViewer->_Mutex_ImageQueue);

  Mat _TMat = G_ExternalStreamViewer->_ImageQueue.front();

  _TGstBuffersize = _TMat.cols * _TMat.rows * _TMat.channels();
  _TGstBuffer = gst_buffer_new_and_alloc(_TGstBuffersize);

  uchar *IMG_data = _TMat.data;

  // unlock queue.
  __MUTEXUNLOCK(G_ExternalStreamViewer->_Mutex_ImageQueue);

  // copy at buffer
  if (gst_buffer_map(_TGstBuffer, &_TGstMapInfo, (GstMapFlags) GST_MAP_WRITE)) {
    memcpy(_TGstMapInfo.data, IMG_data, _TGstBuffersize);
    gst_buffer_unmap(_TGstBuffer, &_TGstMapInfo);
  }
  else g_print("Buffer Copy Error.");

  //segnalo che ho abbastanza dati da fornire ad appsrc:
  g_signal_emit_by_name(__App->videosrc, "push-buffer", _TGstBuffer, &_TGstFlowReturn);

  //GST_DEBUG ("everything allright in cb_need_data");

  //nel caso di errore esce dal loop:
  if (_TGstFlowReturn != GST_FLOW_OK) {
    g_print("Error\n");
    //GST_DEBUG ("something wrong in cb_need_data");
    g_main_loop_quit(G_ExternalStreamViewer->_G_Loop);
  }

  gst_buffer_unref(_TGstBuffer);

  return TRUE;
}

void ExternalStreamViewer::_StartFeed_Callback(GstElement *__Pipeline, guint __Size, App *__App) {
  if (__App->sourceid == 0) {
    //GST_DEBUG ("start feeding");
    //esegue all'infinito cb_need_data (si ferma qui):
    __App->sourceid = g_timeout_add(67, (GSourceFunc) _NeedData_Callback, __App);
    //app->sourceid = g_timeout_add (1, (GSourceFunc) cb_need_data, app);
    //app->sourceid = g_idle_add ((GSourceFunc) cb_need_data, app);
  }
}

void ExternalStreamViewer::_StopFeed_Callback(GstElement *__Pipeline, App *__App) {
  if (__App->sourceid != 0) {
    //GST_DEBUG ("stop feeding");
    g_source_remove(__App->sourceid);
    __App->sourceid = 0;
  }
}

gboolean ExternalStreamViewer::_BusCall_Callback(GstBus *__Bus, GstMessage *__Message, gpointer __Data) {
  GError *_TGError = NULL;
  gchar *_TDbgInfo = NULL;
  //GST_DEBUG ("got message %s", gst_message_type_get_name(GST_MESSAGE_TYPE(__Message)));

  switch (GST_MESSAGE_TYPE (__Message)) {
    case GST_MESSAGE_ERROR: {
      gst_message_parse_error(__Message, &_TGError, &_TDbgInfo);
      g_printerr("ERROR from element %s: %s\n",
                 GST_OBJECT_NAME (__Message->src), _TGError->message);
      g_printerr("Debugging info: %s\n", (_TDbgInfo) ? _TDbgInfo : "none");
      g_error_free(_TGError);
      g_free(_TDbgInfo);
      g_main_loop_quit(G_ExternalStreamViewer->_G_Loop);
      break;
    }
    case GST_MESSAGE_EOS:
      g_main_loop_quit(G_ExternalStreamViewer->_G_Loop);
      break;
    default:
      break;
  }
  return TRUE;
}

void ExternalStreamViewer::_CheckLogHandler_Callback(const gchar *const __LogDomain,
                                                     const GLogLevelFlags __LogLevel, const gchar *const __Message,
                                                     gpointer const __UserData) {
  GstDebugLevel _TLevel;
  switch (__LogLevel & G_LOG_LEVEL_MASK) {
    case G_LOG_LEVEL_ERROR:
      _TLevel = GST_LEVEL_ERROR;
      break;
    case G_LOG_LEVEL_CRITICAL:
      _TLevel = GST_LEVEL_WARNING;
      break;
    case G_LOG_LEVEL_WARNING:
      _TLevel = GST_LEVEL_WARNING;
      break;
    case G_LOG_LEVEL_MESSAGE:
      _TLevel = GST_LEVEL_INFO;
      break;
    case G_LOG_LEVEL_INFO:
      _TLevel = GST_LEVEL_INFO;
      break;
    case G_LOG_LEVEL_DEBUG:
      _TLevel = GST_LEVEL_DEBUG;
      break;
    default:
      _TLevel = GST_LEVEL_LOG;
      break;
  }
  //gst_debug_log(GST_CAT_DEFAULT, _TLevel, "?", "?", 0, NULL, "%s", __Message);
}

void *ExternalStreamViewer::_ExternalStreamViewer_WorkerThread(void *Param) {
  ExternalStreamViewer *_TStreamViewer = (ExternalStreamViewer *) Param;

  // Setting StreamViewer.
  _TStreamViewer->_SettingExternalStreamViewer();

  _TStreamViewer->_OpenExternalStreamViewer();
  _TStreamViewer->_PlayExternalStreamViewer();

  g_print ("Running...\n");
  g_main_loop_run (_TStreamViewer->_G_Loop);

  _TStreamViewer->_CloseExternalStreamViewer();

  return 0;
}

void ExternalStreamViewer::Start_ExternalStreamViewer() {
  if (_ExternalStreamViewerActivated != true) {
    _ExternalStreamViewerActivated = true;
    // Searcher Thread Start.
    _Thread.StartThread(_ExternalStreamViewer_WorkerThread, this);
  }
}

void ExternalStreamViewer::Stop_ExternalStreamViewer() {
  if (_ExternalStreamViewerActivated == true) {
    _ExternalStreamViewerActivated = false;
    // quit infinite loop.
    g_main_loop_quit(_G_Loop);
  }
}

void ExternalStreamViewer::Push_ImageQueue(Mat __Image) {
  if (_ExternalStreamViewerActivated == true) {
    __MUTEXLOCK(_Mutex_ImageQueue);
    _ImageQueue.push(__Image);
    __MUTEXUNLOCK(_Mutex_ImageQueue);
  }
}

void ExternalStreamViewer::Set_Resolution(int __Res_X, int __Res_Y) {
  if (_ExternalStreamViewerActivated != true) {
    _Res_X = __Res_X;
    _Res_Y = __Res_Y;
  }
}

#endif