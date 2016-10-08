/*********************************************************************

          File: FisherYatesShuffle.cpp

   Description: FisherYatesShuffle Source Part.

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

#include "FisherYatesShuffle.hpp"

#if defined(_FisherYatesShuffle_hpp_)

#include "SystemGlobalHeaders.hpp"

int FisherYatesShuffle::_PsudoRandomNumber(int __Range) {
  srand(time(NULL));
  return (int)(rand() % __Range);
}

int FisherYatesShuffle::_PsudoFisherYatesShuffle(int __Range) {
  if (__Range == 0)
    return 0;

  int _tCnt = _PsudoRandomNumber(__Range);
  int _tGetRandomNum = -1;

  // Array Initialize
  vector<int> _tShuffleDump = vector<int>();

  for (int i=0; i<__Range; ++i) {
    _tShuffleDump.push_back(i);
  }

  // STL Standard Shuffle
  random_shuffle(_tShuffleDump.begin(), _tShuffleDump.end());

  // Cnt번 만큼 땡기면 끝이다. 마치 러시안 룰렛 같다.
  while (_tCnt > -1) {
    // Fisher-Yates Shuffle
    int _tRandomNum = _PsudoRandomNumber(_tShuffleDump.size());
    _tGetRandomNum = _tShuffleDump[_tRandomNum];

    // 해당 인덱스와 맨 끝 요소를 Swap.
    iter_swap(_tShuffleDump.begin() + _tRandomNum, _tShuffleDump.end() - 1);
    // 맨 끝 요소는 버림.
    _tShuffleDump.pop_back();
    _tCnt--;
    // 더 짧게 끝내기 위해 급사 시킴.
    //if (_PsudoRandomNumber(10) > 4)
    //break;
  }

  return _tGetRandomNum;
}

int FisherYatesShuffle::Go_Shuffle(int Range) {
  return _PsudoFisherYatesShuffle(Range);
}

#endif