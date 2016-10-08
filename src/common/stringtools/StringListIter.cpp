/*********************************************************************

          File: StringListIter.cpp

   Description: StringListIter Source Part.

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

#include "StringListIter.hpp"

#if defined(_StringListIter_hpp_)

// Area of Constructor & Destructor.
#pragma region Constructor & Destructor
/*
template <typename T>
IterationSeeker<T>::IterationSeeker() {
	_Initialize_Members();
	_Initialize();
}
*/
/*
template <typename T>
IterationSeeker<T>::IterationSeeker(list<T> *StringList) {
	_Initialize_Members();
	_Initialize();
	_Initialize_StringList(StringList);
}
*/
/*
template <typename T>
IterationSeeker<T>::~IterationSeeker() {
	_Deinitialize();
	_Initialize_Members();
}
*/
#pragma endregion Constructor & Destructor

// Area of Private Methods.
#pragma region Private Methods
/*
template <typename T>
void IterationSeeker<T>::_Initialize_Members() {
	_IsInitializedStringList = false;
	_IsInitialize = false;
}

template <typename T>
void IterationSeeker<T>::_Initialize() {

}

template <typename T>
void IterationSeeker<T>::_Deinitialize() {
	if (_IsInitialize == true)
		_Release_Pointer();
}
*/
/*
template <typename T>
void IterationSeeker<T>::_Initialize_StringList(list<T> *__StringList) {
	_StringList = new list<T>();

	_StringList = __StringList;
	_StringListIter = _StringList->begin();
	_EndPoint = __StringList->size();
	_IsInitialize = true;
	_IsInitializedStringList = true;
}

template <typename T>
void IterationSeeker<T>::_Release_Pointer() {
	if (_StringList != NULL && _IsInitializedStringList == true)
		delete _StringList;

	_IsInitialize = false;
}
*/
#pragma endregion Private Methods

// Area of Public Methods.
#pragma region Public Methods
/*
template <typename T>
bool IterationSeeker<T>::Move_PrevStringIter() {
	if (_IsInitialize == true && Is_FirstStringIter() != true) {
		_StringListIter--;
		return true;
	}
	return false;
}

template <typename T>
bool IterationSeeker<T>::Move_NextStringIter() {
	if (_IsInitialize == true && Is_LastStringIter() != true) {
		_StringListIter++;
		return true;
	}
	return false;
}

template <typename T>
bool IterationSeeker<T>::Is_FirstStringIter() {
	list<T>::iterator _TIter = _StringListIter;
	_TIter--;
	if (_IsInitialize == true && _StringList->begin() != _TIter) {
		return false;
	}
	return true;
}

template <typename T>
bool IterationSeeker<T>::Is_LastStringIter() {
	list<T>::iterator _TIter = _StringListIter;
	_TIter++;
	if (_IsInitialize == true && _StringList->end() != _TIter) {
		return false;
	}
	return true;
}
*/
#pragma endregion Public Methods

#endif // _StringListIter_hpp_