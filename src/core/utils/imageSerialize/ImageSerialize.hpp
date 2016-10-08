/*********************************************************************

          File: DataSerialize.hpp

   Description: DataSerialize Header Part.

        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)

       Created: 2016/09/18 13:34

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

#if defined(SET_LIB_BOOST) && defined(SET_LIB_OPENCV)

#ifndef _DataSerialize_hpp_
#define _DataSerialize_hpp_

#include "OpenCVSupport.hpp"
#include "BoostSupport.hpp"

BOOST_SERIALIZATION_SPLIT_FREE(::cv::KeyPoint)
namespace boost {
    namespace serialization {
        /** Serialization support for cv::KeyPoint */
        template<class Archive>
        void save(Archive &ar, const ::cv::KeyPoint &_key, const unsigned int version) {
            ar & _key.pt.x;
            ar & _key.pt.y;
            ar & _key.size;
            ar & _key.angle;
            ar & _key.response;
            ar & _key.octave;
            ar & _key.class_id;
        }

        /** Serialization support for cv::KeyPoint */
        template<class Archive>
        void load(Archive &ar, ::cv::KeyPoint &_key, const unsigned int version) {
            ar & _key.pt.x;
            ar & _key.pt.y;
            ar & _key.size;
            ar & _key.angle;
            ar & _key.response;
            ar & _key.octave;
            ar & _key.class_id;
        }
    }
}

BOOST_SERIALIZATION_SPLIT_FREE(::cv::Mat)
namespace boost {
    namespace serialization {
        /** Serialization support for cv::Mat */
        template<class Archive>
        void save(Archive &ar, const ::cv::Mat &_mat, const unsigned int version) {
            size_t elem_size = _mat.elemSize();
            size_t elem_type = _mat.type();

            ar & _mat.cols;
            ar & _mat.rows;
            ar & elem_size;
            ar & elem_type;

            const size_t data_size = _mat.cols * _mat.rows * elem_size;
            ar & boost::serialization::make_array(_mat.ptr(), data_size);
        }

        /** Serialization support for cv::Mat */
        template<class Archive>
        void load(Archive &ar, ::cv::Mat &_mat, const unsigned int version) {
            int cols, rows;
            size_t elem_size, elem_type;

            ar & cols;
            ar & rows;
            ar & elem_size;
            ar & elem_type;

            _mat.create(rows, cols, elem_type);

            size_t data_size = _mat.cols * _mat.rows * elem_size;
            ar & boost::serialization::make_array(_mat.ptr(), data_size);
        }
    }
}

#endif // _DataSerialize_hpp_

#endif // SET_LIB_BOOST SET_LIB_OPENCV