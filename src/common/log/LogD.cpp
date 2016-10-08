/*********************************************************************

          File: LogD.cpp

   Description: LogD Source Part.

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

#include "LogD.hpp"

#if defined(_LogD_hpp_)

#if !defined(NO_LOG_MODE)
void LogD::Logging(const char *CallValueString, const char *Message, ...) {
#if defined(LOG_WRITE_MODE)
	va_list _TArgument_List;
	char _TStr[BUFFER_MAX_4096] = { 0, };
	string _TString;
	va_start(_TArgument_List, Message);
	vsprintf(_TStr, std::string(Message).append("\n").c_str(), _TArgument_List);
	va_end(_TArgument_List);

	TLogCallback(CallValueString, _TStr);
#endif
}
#endif

void LogD::SetCallback(void (*TCallbackFunc) (const char *, const char *)) {
#if defined(LOG_WRITE_MODE)
	TLogCallback = TCallbackFunc;
#endif
}
#endif // _LogD_hpp_