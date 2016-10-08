/*********************************************************************

          File: SQLiteSupport.hpp

   Description: SQLiteSupport Header Part.

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

#if defined(SET_LIB_SQLITE)

#ifndef _SQLiteSupport_hpp_
#define _SQLiteSupport_hpp_

// Common 부분이 굳이 필요 없을 것 같음.
// 추후 수정.
#include "SystemCommon.hpp"

#include "GPS.hpp"

#include <sqlite3.h>

#define SQLITE_ID 7

#define SQL_READ_ONLY SQLITE_OPEN_READONLY
#define SQL_READ_WRITE SQLITE_OPEN_READWRITE
#define SQL_CREATE SQLITE_OPEN_CREATE

//extern "C" {
static void distanceFunc(sqlite3_context *context, int argc, sqlite3_value **argv) {
	// check that we have four arguments (lat1, lon1, lat2, lon2)
	//assert(argc == 4);
	// check that all four arguments are non-null
	if (sqlite3_value_type(argv[0]) == SQLITE_NULL || sqlite3_value_type(argv[1]) == SQLITE_NULL || sqlite3_value_type(argv[2]) == SQLITE_NULL || sqlite3_value_type(argv[3]) == SQLITE_NULL) {
		sqlite3_result_null(context);
		return;
	}
	// get the four argument values
	double lat1 = sqlite3_value_double(argv[0]);
	double lon1 = sqlite3_value_double(argv[1]);
	double lat2 = sqlite3_value_double(argv[2]);
	double lon2 = sqlite3_value_double(argv[3]);
	
	GPS _TGPS;
	// apply the spherical law of cosines to our latitudes and longitudes, and set the result appropriately
	// 6378.1 is the approximate radius of the earth in kilometres
	//sqlite3_result_double(context, acos((sin(lat1rad) * sin(lat2rad)) + (cos(lat1rad) * cos(lat2rad) * cos(DEG2RAD(lon2) - DEG2RAD(lon1)))) * 6378.1);
	sqlite3_result_double(context, _TGPS.Get_GPSPointsToDistance(lat1, lon1, lat2, lon2, 'k'));
}
//};
#endif // _SQLiteSupport_hpp_

#endif // SET_LIB_SQLITE