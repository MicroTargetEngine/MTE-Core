/*********************************************************************

          File: StringListIter.hpp

   Description: StringListIter Header Part.

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

#include "modinclude.hpp"

#if defined(SET_COMMON_MODULE_STRINGTOOLS)

#ifndef _StringListIter_hpp_
#define _StringListIter_hpp_

#include "SystemCommon.hpp"

template <typename T>
class StringListIter {
private:
	list<T> *_StringList;
	typedef typename list<T>::iterator ISI;
	ISI _StringListIter;
	
	bool _IsInitializedStringList;
	bool _IsInitialize;
	int _EndPoint;

	// Area of Private Methods.
#pragma region Private Methods
	void _Initialize_Members() {
		_IsInitializedStringList = false;
		_IsInitialize = false;
	}

	void _Initialize() { }
	void _Deinitialize() {
		if (_IsInitialize == true)
			_Release_Pointer();
	}

	void _Initialize_StringList(list<T> *__StringList) {
		_StringList = new list<T>();

		_StringList = __StringList;
		_StringListIter = _StringList->begin();
		_EndPoint = __StringList->size();
		_IsInitialize = true;
		_IsInitializedStringList = true;
	}
	void _Release_Pointer() {
		if (_StringList != NULL && _IsInitializedStringList == true)
			delete _StringList;

		_IsInitialize = false;
	}
#pragma endregion Private Methods
public:
#pragma region Constructor & Destructor
	StringListIter() {
		_Initialize_Members();
		_Initialize();
	}
	StringListIter(list<T> *StringList) {
		_Initialize_Members();
		_Initialize();
		_Initialize_StringList(StringList);
	}
	~StringListIter() {
		_Deinitialize();
		_Initialize_Members();
	}
#pragma endregion Constructor & Destructor
	
	// Area of Public Methods.
#pragma region Public Methods
	IMPLEMENT_GET(typename list<T>::iterator, NowStringIter, _StringListIter); // thus, list<T>::iterator Get_NowStringIter();
	bool Move_PrevStringIter() {
		if (_IsInitialize == true && Is_FirstStringIter() != true) {
			_StringListIter--;
			return true;
		}
		return false;
	}
	bool Move_NextStringIter() {
		if (_IsInitialize == true && Is_LastStringIter() != true) {
			_StringListIter++;
			return true;
		}
		return false;
	}
	bool Is_FirstStringIter() {
		typename list<T>::iterator _TIter = _StringListIter;
		_TIter--;
		if (_IsInitialize == true && _StringList->begin() != _TIter) {
			return false;
		}
		return true;
	}
	bool Is_LastStringIter() {
		typename list<T>::iterator _TIter = _StringListIter;
		_TIter++;
		if (_IsInitialize == true && _StringList->end() != _TIter) {
			return false;
		}
		return true;
	}
#pragma endregion Public Methods
};

#endif // _StringListIter_hpp_

#endif // SET_COMMON_MODULE_STRINGTOOLS