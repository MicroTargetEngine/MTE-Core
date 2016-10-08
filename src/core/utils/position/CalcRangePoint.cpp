/*********************************************************************

          File: CalcRangePoint.cpp

   Description: CalcRangePoint Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18 13:34

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

#include "CalcRangePoint.hpp"

#if defined(LOG_WRITE_MODE)
#include "LogD.hpp"
#endif

// 두점과 두 점간의 반경이 있을 때, 비교하는 점의 반경이 원 반경을 넘었는지를 검사하는 함수.
// 1번은 이전 기준점, 2번은 현재 기준점.
bool CalcRangePoint::Get_IsOutOfRangePointToPoint(int Radius1, double Point1X, double Point1Y, int Radius2, double Point2X, double Point2Y) {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into Get_IsOutOfRangePointToPoint Function");
#endif
	double _TDistPointToPoint = sqrt((Point2X - Point1X)*(Point2X - Point1X) + (Point2X - Point1X)*(Point2X - Point1X));
	return (abs(Radius1 - _TDistPointToPoint) < Radius2) ? true : false;
}

#if defined(SET_COMMON_MODULE_POSITION)
// Radius1은 기준점에 대한 반경, Radius2는 자신으로부터의 탐색 반경.
bool CalcRangePoint::Get_IsOutOfRangeGPSPointToGPSPoint(double Point1X, double Point1Y, double Point2X, double Point2Y, int Radius1, int Radius2, char UnitMode) {
#if defined(LOG_WRITE_MODE)
	G_LogD->Logging("Func", "into Get_IsOutOfRangeGPSPointToGPSPoint Function");
#endif
	GPS _TGPS;
	double _TDist = _TGPS.Get_GPSPointsToDistance(Point1X, Point1Y, Point2X, Point2Y, UnitMode);
	return (abs(Radius1 - (int)(_TDist * 1000.0f)) < Radius2) ? true : false;
}
#endif