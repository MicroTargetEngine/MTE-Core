/*********************************************************************

          File: RandomID.cpp

   Description: RandomID Source Part.

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

#include "RandomID.hpp"

#if defined(_RandomID_hpp_)

int RandomID::_Get_RandomNumStr(int __Range){
  FisherYatesShuffle _TFisherYatesShuffle;
  return _TFisherYatesShuffle.Go_Shuffle(__Range);
}

string RandomID::_Get_HashID(){
  string _TStr = "";

  for (int i = 0; i < SEED_STR_LENGTH; i++) {
    ostringstream _TOStr;
    _TOStr << _Get_RandomNumStr(29);
    _TStr.append(_TOStr.str().c_str());
  }

  return _TStr;
}

string RandomID::Make_RandomID() {
  md5 _TMD5;
  string _TRStr = _Get_HashID();

  md5_state_t _TState;
  md5_byte_t _TDigest[16];
  char _THexOutput[16*2 + 1];
  int di;

  _TMD5.md5_init(&_TState);
  _TMD5.md5_append(&_TState, (const md5_byte_t *)_TRStr.c_str(), _TRStr.length());
  _TMD5.md5_finish(&_TState, _TDigest);

  for (di = 0; di < 16; ++di)
    sprintf(_THexOutput + di * 2, "%02x", _TDigest[di]);

  return string(_THexOutput);
}

#endif // _RandomID_hpp_