/*********************************************************************

          File: StringConverter.cpp

   Description: StringConverter Source Part.

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

#include "StringConverter.hpp"

#if defined(_StringConverter_hpp_)

template<typename T>
string StringConverter::ToString(const T& Any) {
	return dynamic_cast<std::ostringstream &>((std::ostringstream() << std::dec << Any)).str();
}

// 만들어 놓고 뭔가(?) 이상함. 조치를 취해야 함.
const char *StringConverter::StringToConstCharPointer(string Str) {
	const char *_TConstStr = Str.c_str();
	char *_TStr = new char[BUFFER_MAX_32767];
	memset(_TStr, 0, sizeof(_TStr));
	strcpy(_TStr, _TConstStr);
	return ConstCharToChar(_TStr);
}

char *StringConverter::ConstCharToChar(const char *Str) {
	char *_TStr = const_cast<char *>(Str);
	return _TStr;
}

char *StringConverter::StringToChar(string Str) {
	return ConstCharToChar(StringToConstCharPointer(Str));
}

#endif // _StringTools_hpp_