/*********************************************************************

          File: CalcCenter.cpp

   Description: CalcCenter Source Part.

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

#include "CalcCenter.hpp"

#if defined(_CalcCenter_hpp_)

void CalcCenter::Add_Point(double PointX, double PointY) {
	Coordinates _TCoordinates;
	_TCoordinates._XP = PointX;
	_TCoordinates._YP = PointY;
	_CoordinatesArray.push_back(_TCoordinates);
}

void CalcCenter::Clear_Point() {
	_CoordinatesArray.clear();
}

Coordinates CalcCenter::Get_CenterPoint() {
	if (!(_CenterPoint._XP == 0 && _CenterPoint._YP == 0))
		return _CenterPoint;

	double _TCenterX = 0, _TCenterY = 0;
	double _TArea = 0;

	Coordinates _TPointFirst, _TPointSecond;
	double _TFactor = 0.0;
	int _TISecond;

	for (int i = 0; i < _CoordinatesArray.size(); i++) {
		_TISecond = (i + 1) % _CoordinatesArray.size();

		_TPointFirst._XP = _CoordinatesArray.at(i)._XP;
		_TPointFirst._YP = _CoordinatesArray.at(i)._YP;
		_TPointSecond._XP = _CoordinatesArray.at(_TISecond)._XP;
		_TPointSecond._YP = _CoordinatesArray.at(_TISecond)._YP;

		_TFactor = (_TPointFirst._XP * _TPointSecond._YP) - (_TPointSecond._XP * _TPointFirst._YP);
		_TArea += _TFactor;

		_TCenterX += (_TPointFirst._XP * _TPointSecond._XP) * _TFactor;
		_TCenterY += (_TPointFirst._YP * _TPointSecond._YP) * _TFactor;
	}

	_TArea /= 2.0;
	_TArea *= 6.0;

	_TFactor = 1 / _TArea;

	_TCenterX *= _TFactor;
	_TCenterY *= _TFactor;

	_CenterPoint._XP = _TCenterX;
	_CenterPoint._YP = _TCenterY;

	return _CenterPoint;
}

#endif // _CalcCenter_hpp_