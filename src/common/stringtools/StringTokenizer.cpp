/*********************************************************************

          File: StringTokenizer.cpp

   Description: StringTokenizer Source Part.

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

#include "StringTokenizer.hpp"

#if defined(_StringTokenizer_hpp_)

StringTokenizer::StringTokenizer() {
	_Initialize_StringTokenizer();
}

StringTokenizer::~StringTokenizer() {
	_Deinitialize_StringTokenizer();
}

// 초기에 초기화 할 것에 대한 모든 것을 담는다.
void StringTokenizer::_Initialize_StringTokenizer() {
	Clear_StringToken();
	_StringList = new list<string>();
}

// 마지막에 없에야 할 모든것을 담는다.
void StringTokenizer::_Deinitialize_StringTokenizer() {
	delete _StringList;
}

void StringTokenizer::Clear_StringList() {
	// String List Clear.
	_StringList->clear();
}

void StringTokenizer::Clear_StringToken() {
	// Token List Clear.
	_Token = "";
}

list<char *> *StringTokenizer::Get_TokenedCharListArrays() {
	list<char *> *_TempTokenedCharList = new list<char *>();

	for_IterToEndC(list, string, _StringList, i) {
		char *_TCharArray = (char *)(_i->c_str());
		_TempTokenedCharList->push_back(_TCharArray);
	}
	
	return _TempTokenedCharList;
}

// String Input Functions.
void StringTokenizer::Set_InputCharString(const char* Str) {
	_InternalInputString.append(Str);
}

// String Token의 종류.
void StringTokenizer::Set_SingleToken(const char *SingleToken) {
	string _TempTokenString(SingleToken);
	_Token = _TempTokenString;
}

// Implement me.
/*
void StringTokenizer::Set_MultiToken(const char *, ...) {

}
*/

bool StringTokenizer::Go_StringToken() {
	string _TempString = _InternalInputString;
	int _TTokenPointer = 0;
	// list<string>::iterator _StringIter;

	// if, return Key or Empty that return false.
	if (_TempString.c_str() == "\r" || _TempString.c_str() == "" || _TempString.empty())
		return false;

	// Go Tokening.
	while ((_TTokenPointer = _TempString.find_first_of(_Token)) != _TempString.npos) {
		if (_TTokenPointer > 0)
			_StringList->push_back(_TempString.substr(0, _TTokenPointer));
		_TempString = _TempString.substr(_TTokenPointer + 1);
	}

	// 마지막 String 넣기.
	if (_TempString.length() > 0)
		_StringList->push_back(_TempString.substr(0, _TTokenPointer));

	return true;
}

#endif // _StringTokenizer_hpp_