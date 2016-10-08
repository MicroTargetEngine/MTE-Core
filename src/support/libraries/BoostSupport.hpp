/*********************************************************************

          File: BoostSupport.hpp

   Description: BoostSupport Header Part.

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

#include "LibrariesControl.hpp"

#if defined(SET_LIB_BOOST)

#ifndef _BoostSupport_hpp_
#define _BoostSupport_hpp_

//#include "SystemDependency.hpp"

#include <boost/serialization/serialization.hpp>

#include <boost/serialization/split_free.hpp>

#include <boost/serialization/vector.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>

#include <boost/iostreams/device/back_inserter.hpp>

#include <boost/iostreams/filtering_streambuf.hpp>

#include <boost/iostreams/filter/gzip.hpp>

namespace bio = boost::iostreams;
#endif // _BoostSupport_hpp_

#endif // SET_LIB_BOOST