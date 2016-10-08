/*********************************************************************

          File: FireBaseAdapterVariable.hpp

   Description: FireBaseAdapterVariable Header Part.

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

#ifndef _FireBaseAdapterVariable_hpp_
#define _FireBaseAdapterVariable_hpp_

#include <string>

typedef struct _SendInformations {
  // 0 : CLI(for Test), 1 : Ethernet(for Command), 2 : Serial(between Embedded and Embedded)
  int Types;
  string UserID;
  string Result;

  _SendInformations() : Types(-1), UserID(""), Result("") { }

} SendInformations;

typedef struct _RecvInformations {
  // 0 : CLI(for Test), 1 : Ethernet(for Command), 2 : Serial(between Embedded and Embedded)
  int Types;
  string UserID;
  string Command;

  _RecvInformations() : Types(-1), UserID(""), Command("") { }

} RecvInformations;

#endif // _FireBaseAdapterVariable_hpp_