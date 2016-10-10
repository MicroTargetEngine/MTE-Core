/*********************************************************************

          File: ImageStitcher.cpp

   Description: ImageStitcher Source Part.

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

#include "ImageStitcher.hpp"

#if defined(_ImageStitcher_hpp_)

Mat ImageStitcher::Do_StitchingHorizontallyFromTwoImage(Mat __Image_Left, Mat __Image_Right) {
  if (__Image_Left.channels() != __Image_Right.channels())
    return Mat();
  if (__Image_Left.depth() != __Image_Right.depth())
    return Mat();
  if(__Image_Left.type() != __Image_Right.type())
    return Mat();

  int _TResultImageWidth = __Image_Left.cols + __Image_Right.cols;
  int _TResultImageHeight = (__Image_Left.rows > __Image_Right.rows) ? __Image_Left.rows : __Image_Right.rows;

  Mat _TDst(_TResultImageWidth, _TResultImageHeight, __Image_Left.type(), __Image_Left.channels());
  Rect _TRectROISrc1(0, 0, __Image_Left.cols, __Image_Left.rows);
  Mat _TROISrc1(_TDst, _TRectROISrc1);
  __Image_Left.copyTo(_TROISrc1);

  Rect _TRectROISrc2(__Image_Left.cols, 0, __Image_Right.cols, __Image_Right.rows);
  Mat _TROISrc2(_TDst, _TRectROISrc2);
  __Image_Right.copyTo(_TROISrc2);

  return _TDst;
}

Mat ImageStitcher::Do_StitchingVerticallyFromTwoImage(Mat __Image_Up, Mat __Image_Down) {
  if (__Image_Up.channels() != __Image_Down.channels())
    return Mat();
  if (__Image_Up.depth() != __Image_Down.depth())
    return Mat();
  if(__Image_Up.type() != __Image_Down.type())
    return Mat();

  int _TResultImageWidth = (__Image_Up.cols > __Image_Down.cols) ? __Image_Up.cols : __Image_Down.cols;
  int _TResultImageHeight = __Image_Up.rows + __Image_Down.rows;

  Mat _TDst(_TResultImageWidth, _TResultImageHeight, __Image_Up.type(), __Image_Up.channels());
  Rect _TRectROISrc1(0, 0, __Image_Up.cols, __Image_Up.rows);
  Mat _TROISrc1(_TDst, _TRectROISrc1);
  __Image_Up.copyTo(_TROISrc1);

  Rect _TRectROISrc2(0, __Image_Up.rows, __Image_Down.cols, __Image_Down.rows);
  Mat _TROISrc2(_TDst, _TRectROISrc2);
  __Image_Down.copyTo(_TROISrc2);

  return _TDst;
}

#endif