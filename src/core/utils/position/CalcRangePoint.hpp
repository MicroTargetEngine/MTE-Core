/*********************************************************************

          File: CalcRangePoint.hpp

   Description: CalcRangePoint Header Part.

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

#include "modinclude.hpp"

#ifndef _CalcRangePoint_hpp_
#define _CalcRangePoint_hpp_

#include "SystemCommon.hpp"

#include "GPS.hpp"

class CalcRangePoint {	
private:
public:
	bool Get_IsOutOfRangePointToPoint(int Radius1, double Point1X, double Point1Y, int Radius2, double Point2X, double Point2Y);

#if defined(SET_COMMON_MODULE_POSITION)
	bool Get_IsOutOfRangeGPSPointToGPSPoint(double Point1X, double Point1Y, double Point2X, double Point2Y, int Radius1, int Radius2, char UnitMode);
	//double Get_GPSToDistance(double Lat1, double Lon1, double Lat2, double Lon2, char UnitMode);
#endif
};

#endif // _CalcRangePoint_hpp_