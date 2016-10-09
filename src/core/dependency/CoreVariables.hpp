/*********************************************************************

          File: CoreVariables.hpp

   Description: CoreVariables Header Part.

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

#ifndef _CoreVariables_hpp_
#define _CoreVariables_hpp_

#include "CoreModuleControl.hpp"

#include "SystemCommon.hpp"

#include "OpenCVSupport.hpp"

#if defined(SET_CORE_ENGINE_MODULE_DATAAUTOMATOR) || defined(SET_CORE_ENGINE_MODULE_FEATURESEARCHER)
typedef struct _FeatureData {
	/*
	string hid;
	string comp_name;
	string comp_name_en;
	string comp_category;
	string comp_tel;
	string comp_addr;
	string default_Nation;
	string info_nation;
	string comp_expl;
	double favorite;
	double gps_x;
	double gps_y;
	double _Distance;
	*/
	vector<cv::KeyPoint> img_key;
	cv::Mat img_desc;
	// 정형화 해 준 거리

	int _DisplayPoint_X;
	int _DisplayPoint_Y;

	string Key;

  /*
	bool operator ()(_InnerFormalizaionData const &__Lhs, _InnerFormalizaionData const &__Rhs) {
		return __Lhs._Distance < __Rhs._Distance;
		//if (__Lhs._Distance < __Rhs._Distance) return true;
		//if (__Lhs._Distance > __Rhs._Distance) return false;
	}; */
} FeatureData;
#endif

#if defined(SET_CORE_ENGINE_MODULE_LANESEARCHER)
typedef struct _LaneDetectData {
  Mat ImageData;
  int VPoint_X; // Vanishing Point X
  int VPoint_Y; // Vanishing Point Y

  int RPoint_X;
  int RPoint_Y;

  _LaneDetectData() : VPoint_X(0), VPoint_Y(0), RPoint_X(0), RPoint_Y(0) {}
  ~_LaneDetectData() {
    VPoint_X = 0;
    VPoint_Y = 0;
    RPoint_X = 0;
    RPoint_Y = 0;
  }

	double Get_AngleToPointByImagePosition() {
    double _Tdx = (double)RPoint_X - (double)VPoint_X;
    double _Tdy = (double)RPoint_Y - (double)VPoint_Y;
    return (atan2(_Tdy, _Tdx) * 180.0f) / M_PI;
  }
} LaneDetectData;
#endif

#endif
