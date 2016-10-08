/*********************************************************************

          File: mmalAdapter.hpp

   Description: mmalAdapter Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/30

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

//#if defined(SET_DEVICE_MMAL)

#ifndef _mmalAdapter_hpp_
#define _mmalAdapter_hpp_

#include "mmalSupport.hpp"

#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_CAPTURE_PORT 2

#define CALC_FPS 1

#define DEFAULT_VIDEO_FPS 30
#define DEFAULT_VIDEO_WIDTH 1280
#define DEFAULT_VIDEO_HEIGHT 720

typedef struct {
  int width;
  int height;
  int fps;

  MMAL_COMPONENT_T *camera;
  MMAL_COMPONENT_T *encoder;
  MMAL_COMPONENT_T *preview;

  MMAL_PORT_T *camera_video_port;
  MMAL_POOL_T *camera_video_port_pool;
  MMAL_PORT_T *encoder_input_port;
  MMAL_POOL_T *encoder_input_pool;
  MMAL_PORT_T *encoder_output_port;
  MMAL_POOL_T *encoder_output_pool;

  int opencv_width;
  int opencv_height;
  VCOS_SEMAPHORE_T complete_semaphore;

  signed int motion;
  int grabframe;

  float video_fps;
  float opencv_fps;

  IplImage* small_image; // resized image
  IplImage* stub; // stub

  char *stillfn; //place to write stills to
  int rotation;
} PORT_USERDATA;

#endif //_mmalAdapter_hpp_

//#endif