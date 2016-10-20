/*********************************************************************

          File: MathematicalFunc.cpp

   Description: MathematicalMacro Source Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

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

#include "MathematicalFunc.hpp"

#if defined(_MathematicalFunc_hpp_)

double MathematicalFunc::Get_AngleToPointByImagePosition(int __Dest_X, int __Dest_Y, int __Ref_X, int __Ref_Y) {
  double _Tdx = (double)__Ref_X - (double)__Dest_X;
  double _Tdy = (double)__Ref_Y - (double)__Dest_Y;
  double _TDeg = atan2(_Tdy, _Tdx) * (180.0f / M_PI);

  _TDeg += ((_TDeg < 0) ? 90 : -90);
  //return atan2(_Tdy, _Tdx) * (180.0f / M_PI);
  return _TDeg;
}

#endif