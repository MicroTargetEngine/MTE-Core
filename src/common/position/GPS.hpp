/*********************************************************************

          File: GPS.hpp

   Description: GPS Header Part.

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

#if defined(SET_COMMON_MODULE_POSITION)

#ifndef _GPS_hpp_
#define _GPS_hpp_

#include "SystemCommon.hpp"

#include "MathematicalMacro.hpp"

#if defined(LOG_WRITE_MODE)
#include "LogD.hpp"
#endif

class GPS {
public:
	double Get_ConvertStatuteDistance(double Lat1, double Lon1, double Lat2, double Lon2) {
#if defined(LOG_WRITE_MODE)
		G_LogD->Logging("Func", "into Get_ConvertStatuteDistance Function");
#endif
		double _TTheta = Lon1 - Lon2;
		return RAD2DEG(acos((sin(DEG2RAD(Lat1)) * sin(DEG2RAD(Lat2)))
			+ (cos(DEG2RAD(Lat1)) * cos(DEG2RAD(Lat2)) * cos(DEG2RAD(_TTheta))))) * 60 * 1.1515;
	}

	// GPS의 Distance를 환산.
	double Get_GPSPointsToDistance(double Lat1, double Lon1, double Lat2, double Lon2, char UnitMode) {
#if defined(LOG_WRITE_MODE)
		G_LogD->Logging("Func", "into Get_GPSPointsToDistance Function");
#endif
		// Lat은 위도, Lon은 경도.
		// 1번은 자신의 위치, 2번은 대상의 위치
		// 1번과 2번이 바뀌어도 됨.
		double _TDist = Get_ConvertStatuteDistance(Lat1, Lon1, Lat2, Lon2);

		switch (UnitMode) {
			case 'M' :
				// 법령마일
				break;
			case 'K' :
				// 킬로미터(한국 기본)
				_TDist *= 1.609344;
				break;
			case 'N' :
				// 해상 마일
				_TDist *= 0.8684;
				break;
		}

		return _TDist;
	}
};
#endif // _GPS_hpp_

#endif // SET_COMMON_MODULE_POSITION