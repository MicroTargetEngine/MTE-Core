/*********************************************************************

          File: mmalAdapter.cpp

   Description: mmalAdapter Source Part.

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

#include "mmalAdapter.hpp"

#if defined(_mmalAdapter_hpp_)

int fill_port_buffer(MMAL_PORT_T *port, MMAL_POOL_T *pool) {
  int q;
  int num = mmal_queue_length(pool->queue);

  for (q = 0; q < num; q++) {
    MMAL_BUFFER_HEADER_T *buffer = mmal_queue_get(pool->queue);
    if (!buffer) {
      fprintf(stderr, "Unable to get a required buffer %d from pool queue\n", q);
    }

    if (mmal_port_send_buffer(port, buffer) != MMAL_SUCCESS) {
      fprintf(stderr, "Unable to send a buffer to port (%d)\n", q);
    }
  }
}

int raspicamcontrol_set_rotation(MMAL_COMPONENT_T *camera, int rotation)
{
  int ret;
  int my_rotation = ((rotation % 360 ) / 90) * 90;

  ret = mmal_port_parameter_set_int32(camera->output[0], MMAL_PARAMETER_ROTATION, my_rotation);
  mmal_port_parameter_set_int32(camera->output[1], MMAL_PARAMETER_ROTATION, my_rotation);
  mmal_port_parameter_set_int32(camera->output[2], MMAL_PARAMETER_ROTATION, my_rotation);

  return ret;
}

int setup_camera(PORT_USERDATA *userdata) {
  MMAL_STATUS_T status;
  MMAL_COMPONENT_T *camera = 0;
  MMAL_ES_FORMAT_T *format;
  MMAL_PORT_T * camera_preview_port;
  MMAL_PORT_T * camera_video_port;
  MMAL_PORT_T * camera_still_port;
  MMAL_POOL_T * camera_video_port_pool;

  status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &camera);
  if (status != MMAL_SUCCESS) {
    fprintf(stderr, "Error: create camera %x\n", status);
    return -1;
  }
  userdata->camera = camera;
  userdata->camera_video_port = camera->output[MMAL_CAMERA_VIDEO_PORT];

  camera_preview_port = camera->output[MMAL_CAMERA_PREVIEW_PORT];
  camera_video_port = camera->output[MMAL_CAMERA_VIDEO_PORT];
  camera_still_port = camera->output[MMAL_CAMERA_CAPTURE_PORT];

  {
    MMAL_PARAMETER_CAMERA_CONFIG_T cam_config = {
        { MMAL_PARAMETER_CAMERA_CONFIG, sizeof (cam_config)},
        .max_stills_w = userdata->width,
        .max_stills_h = userdata->height,
        .stills_yuv422 = 0,
        .one_shot_stills = 1,
        .max_preview_video_w = userdata->width,
        .max_preview_video_h = userdata->height,
        .num_preview_video_frames = 3,
        .stills_capture_circular_buffer_height = 0,
        .fast_preview_resume = 0,
        .use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC
    };
    mmal_port_parameter_set(camera->control, &cam_config.hdr);
  }

  // Setup camera preview port format
  format = camera_preview_port->format;
  //format->encoding = MMAL_ENCODING_I420;
  format->encoding = MMAL_ENCODING_OPAQUE;
  format->encoding_variant = MMAL_ENCODING_I420;
  format->es->video.width = userdata->width;
  format->es->video.height = userdata->height;
  format->es->video.crop.x = 0;
  format->es->video.crop.y = 0;
  format->es->video.crop.width = userdata->width;
  format->es->video.crop.height = userdata->height;

  status = mmal_port_format_commit(camera_preview_port);

  if (status != MMAL_SUCCESS) {
    fprintf(stderr, "Error: camera viewfinder format couldn't be set\n");
    return -1;
  }

  // Setup camera video port format
  mmal_format_copy(camera_video_port->format, camera_preview_port->format);

  format = camera_video_port->format;
  format->encoding = MMAL_ENCODING_I420;
  format->encoding_variant = MMAL_ENCODING_I420;
  format->es->video.width = userdata->width;
  format->es->video.height = userdata->height;
  format->es->video.crop.x = 0;
  format->es->video.crop.y = 0;
  format->es->video.crop.width = userdata->width;
  format->es->video.crop.height = userdata->height;
  format->es->video.frame_rate.num = userdata->fps;
  format->es->video.frame_rate.den = 1;

  camera_video_port->buffer_num = 2;
  camera_video_port->buffer_size = (format->es->video.width * format->es->video.height * 12 / 8 ) * camera_video_port->buffer_num;

  fprintf(stderr, "camera video buffer_size = %d\n", camera_video_port->buffer_size);
  fprintf(stderr, "camera video buffer_num = %d\n", camera_video_port->buffer_num);

  status = mmal_port_format_commit(camera_video_port);
  if (status != MMAL_SUCCESS) {
    fprintf(stderr, "Error: unable to commit camera video port format (%u)\n", status);
    return -1;
  }

  camera_video_port_pool = (MMAL_POOL_T *) mmal_port_pool_create(camera_video_port, camera_video_port->buffer_num, camera_video_port->buffer_size);
  userdata->camera_video_port_pool = camera_video_port_pool;
  camera_video_port->userdata = (struct MMAL_PORT_USERDATA_T *) userdata;


  status = mmal_port_enable(camera_video_port, camera_video_buffer_callback);

  if (status != MMAL_SUCCESS) {
    fprintf(stderr, "Error: unable to enable camera video port (%u)\n", status);
    return -1;
  }

  status = mmal_component_enable(camera);
  if (status != MMAL_SUCCESS) {
    fprintf(stderr, "Error: unable to enable camera (%u)\n", status);
    return -1;
  }

  fill_port_buffer(userdata->camera_video_port, userdata->camera_video_port_pool);

  if (mmal_port_parameter_set_boolean(camera_video_port, MMAL_PARAMETER_CAPTURE, 1) != MMAL_SUCCESS) {
    printf("%s: Failed to start capture\n", __func__);
  }

  raspicamcontrol_set_rotation(camera, userdata->rotation);

  fprintf(stderr, "camera created\n");
  return 0;
}

int setup_encoder(PORT_USERDATA *userdata) {
  MMAL_STATUS_T status;
  MMAL_COMPONENT_T *encoder = 0;
  MMAL_PORT_T *preview_input_port = NULL;

  MMAL_PORT_T *encoder_input_port = NULL, *encoder_output_port = NULL;
  MMAL_POOL_T *encoder_input_port_pool;
  MMAL_POOL_T *encoder_output_port_pool;

  status = mmal_component_create(MMAL_COMPONENT_DEFAULT_VIDEO_ENCODER, &encoder);
  if (status != MMAL_SUCCESS) {
    fprintf(stderr, "Error: unable to create preview (%u)\n", status);
    return -1;
  }

  encoder_input_port = encoder->input[0];
  encoder_output_port = encoder->output[0];
  userdata->encoder_input_port = encoder_input_port;
  userdata->encoder_output_port = encoder_input_port;

  mmal_format_copy(encoder_input_port->format, userdata->camera_video_port->format);
  encoder_input_port->buffer_size = encoder_input_port->buffer_size_recommended;
  encoder_input_port->buffer_num = 2;

  mmal_format_copy(encoder_output_port->format, encoder_input_port->format);

  encoder_output_port->buffer_size = encoder_output_port->buffer_size_recommended;
  encoder_output_port->buffer_num = 2;
  // Commit the port changes to the input port
  status = mmal_port_format_commit(encoder_input_port);
  if (status != MMAL_SUCCESS) {
    fprintf(stderr, "Error: unable to commit encoder input port format (%u)\n", status);
    return -1;
  }

  // Only supporting H264 at the moment
  encoder_output_port->format->encoding = MMAL_ENCODING_H264;
  encoder_output_port->format->bitrate = 2000000;

  encoder_output_port->buffer_size = encoder_output_port->buffer_size_recommended;

  if (encoder_output_port->buffer_size < encoder_output_port->buffer_size_min) {
    encoder_output_port->buffer_size = encoder_output_port->buffer_size_min;
  }

  encoder_output_port->buffer_num = encoder_output_port->buffer_num_recommended;

  if (encoder_output_port->buffer_num < encoder_output_port->buffer_num_min) {
    encoder_output_port->buffer_num = encoder_output_port->buffer_num_min;
  }

  // Commit the port changes to the output port
  status = mmal_port_format_commit(encoder_output_port);
  if (status != MMAL_SUCCESS) {
    fprintf(stderr, "Error: unable to commit encoder output port format (%u)\n", status);
    return -1;
  }

  fprintf(stderr, "encoder input buffer_size = %d\n", encoder_input_port->buffer_size);
  fprintf(stderr, "encoder input buffer_num = %d\n", encoder_input_port->buffer_num);

  fprintf(stderr, "encoder output buffer_size = %d\n", encoder_output_port->buffer_size);
  fprintf(stderr, "encoder output buffer_num = %d\n", encoder_output_port->buffer_num);

  encoder_input_port_pool = (MMAL_POOL_T *) mmal_port_pool_create(encoder_input_port, encoder_input_port->buffer_num, encoder_input_port->buffer_size);
  userdata->encoder_input_pool = encoder_input_port_pool;
  encoder_input_port->userdata = (struct MMAL_PORT_USERDATA_T *) userdata;
  status = mmal_port_enable(encoder_input_port, encoder_input_buffer_callback);
  if (status != MMAL_SUCCESS) {
    fprintf(stderr, "Error: unable to enable encoder input port (%u)\n", status);
    return -1;
  }
  fprintf(stderr, "encoder input pool has been created\n");

  encoder_output_port_pool = (MMAL_POOL_T *) mmal_port_pool_create(encoder_output_port, encoder_output_port->buffer_num, encoder_output_port->buffer_size);
  userdata->encoder_output_pool = encoder_output_port_pool;
  encoder_output_port->userdata = (struct MMAL_PORT_USERDATA_T *) userdata;

  status = mmal_port_enable(encoder_output_port, encoder_output_buffer_callback);
  if (status != MMAL_SUCCESS) {
    fprintf(stderr, "Error: unable to enable encoder output port (%u)\n", status);
    return -1;
  }
  fprintf(stderr, "encoder output pool has been created\n");

  fill_port_buffer(encoder_output_port, encoder_output_port_pool);

  fprintf(stderr, "encoder has been created\n");
  return 0;
}

static void camera_video_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer) {
  PORT_USERDATA *userdata = (PORT_USERDATA *) port->userdata;

  static struct timespec t1;
  struct timespec t2;

  static int frame_count = 0;
  static int frame_post_count = 0;

  if (frame_count == 0) {
    clock_gettime(CLOCK_MONOTONIC, &t1);
  }
  frame_count++;

  //if(1){
  if( (CALC_FPS) && (frame_count % (userdata->fps*2) == 0) ){ //every 2 seconds
    // print framerate every n frame
    clock_gettime(CLOCK_MONOTONIC, &t2);
    float d = (t2.tv_sec + t2.tv_nsec / 1000000000.0) - (t1.tv_sec + t1.tv_nsec / 1000000000.0);
    float fps = 0.0;

    if (d > 0) {
      fps = frame_count / d;
    } else {
      fps = frame_count;
    }
    userdata->video_fps = fps;
    //fprintf(stderr, "  Frame = %d, Frame Post %d, Framerate = %.0f fps \n", frame_count, frame_post_count, fps);
  }

  //if(1){
  if(userdata->grabframe){
    mmal_buffer_header_mem_lock(buffer);

    //monkey with the imageData pointer, to avoid a memcpy
    char* oldImageData = userdata->stub->imageData;
    userdata->stub->imageData = buffer->data;
    cvResize(userdata->stub, userdata->small_image, CV_INTER_LINEAR);
    userdata->stub->imageData = oldImageData;

    mmal_buffer_header_mem_unlock(buffer);

    if (vcos_semaphore_trywait(&(userdata->complete_semaphore)) != VCOS_SUCCESS) {
      vcos_semaphore_post(&(userdata->complete_semaphore));
      frame_post_count++;
    }
  }

  //if(1){
  if( (userdata->stillfn) && (frame_count % (userdata->fps * still_interval) == 0) ){ //every 60 seconds
    mmal_buffer_header_mem_lock(buffer);

    fprintf(stderr, "WRITING STILL (%d)\n", frame_count);
/*
      //Just grab the Y and write it out ASAP
      //monkey with the imageData pointer, to avoid a memcpy
      char* oldImageData = userdata->stub->imageData;
      userdata->stub->imageData = buffer->data;
      //grab a still for export to www
      cvSaveImage("/home/pi/image.tmp.jpg", userdata->stub, 0);
      userdata->stub->imageData = oldImageData;
*/
/**/
    //TODO some of this can probably be collapsed down, but as we only do this once a minute I don't care so much....
    //so here we're going to attempt a new method to get full YUV
    unsigned char* pointer = (unsigned char *)(buffer -> data);
    //get Y U V as CvMat()s
    CvMat y = cvMat(userdata->height, userdata->width, CV_8UC1, pointer);
    pointer = pointer + (userdata->height*userdata->width);
    CvMat u = cvMat(userdata->height/2, userdata->width/2, CV_8UC1, pointer);
    pointer = pointer + (userdata->height*userdata->width/4);
    CvMat v = cvMat(userdata->height/2, userdata->width/2, CV_8UC1, pointer);
    //resize U and V and convert Y U and V into IplImages
    IplImage* uu = cvCreateImage(cvSize(userdata->width, userdata->height), IPL_DEPTH_8U, 1);
    cvResize(&u, uu, CV_INTER_LINEAR);
    IplImage* vv = cvCreateImage(cvSize(userdata->width, userdata->height), IPL_DEPTH_8U, 1);
    cvResize(&v, vv, CV_INTER_LINEAR);
    IplImage* yy = cvCreateImage(cvSize(userdata->width, userdata->height), IPL_DEPTH_8U, 1);
    cvResize(&y, yy, CV_INTER_LINEAR);
    //Create the final, 3 channel image
    IplImage* image = cvCreateImage(cvSize(userdata->width, userdata->height), IPL_DEPTH_8U, 3);
    CvArr * output[] = { image };
    //map Y to the 1st channel
    int from_to[] = {0, 0};
    const CvArr * inputy[] = { yy };
    cvMixChannels(inputy, 1, output, 1, from_to, 1);
    //map V to the 2nd channel
    from_to[1] = 1;
    const CvArr * inputv[] = { vv };
    cvMixChannels(inputv, 1, output, 1, from_to, 1);
    //map U to the 3rd channel
    from_to[1] = 2;
    const CvArr * inputu[] = { uu };
    cvMixChannels(inputu, 1, output, 1, from_to, 1);
    //convert the colour space
    cvCvtColor(image, image, CV_YCrCb2BGR);
    //save the image
    cvSaveImage(STILL_TMPFN, image, 0);
    //cleanup the images
    cvReleaseImage(&yy);
    cvReleaseImage(&vv);
    cvReleaseImage(&uu);
    cvReleaseImage(&image);
/**/

    mmal_buffer_header_mem_unlock(buffer);

    rename(STILL_TMPFN, userdata->stillfn);

    if (vcos_semaphore_trywait(&(userdata->complete_semaphore)) != VCOS_SUCCESS) {
      vcos_semaphore_post(&(userdata->complete_semaphore));
      frame_post_count++;
    }
  }

  //if(1){
  if(userdata->motion > 0){
    MMAL_BUFFER_HEADER_T *output_buffer = mmal_queue_get(userdata->encoder_input_pool->queue);
    if(output_buffer){
      mmal_buffer_header_mem_lock(buffer);
      memcpy(output_buffer->data, buffer->data, buffer->length);
      output_buffer->length = buffer->length;
      mmal_buffer_header_mem_unlock(buffer);

      if (mmal_port_send_buffer(userdata->encoder_input_port, output_buffer) != MMAL_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to send buffer \n");
      }
    }
    userdata->motion--;
  }
  mmal_buffer_header_release(buffer);

  // and send one back to the port (if still open)
  if (port->is_enabled) {
    MMAL_STATUS_T status;

    MMAL_BUFFER_HEADER_T *new_buffer;
    MMAL_POOL_T *pool = userdata->camera_video_port_pool;
    new_buffer = mmal_queue_get(pool->queue);

    if (new_buffer) {
      status = mmal_port_send_buffer(port, new_buffer);
    }

    if (!new_buffer || status != MMAL_SUCCESS) {
      fprintf(stderr, "[%s]Unable to return a buffer to the video port\n", __func__);
    }
  }
}


#endif