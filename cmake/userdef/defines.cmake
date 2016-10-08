#===============================================================================
#
#          File: defines.cmake
#
#         Uasge: Using function in any cmake sources.
#
#   Description: FireBase-Core Project for CLion using CMake.
#
#        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)
#
#       Created: 2016/09/22
#
# Copyright (c) 2016, FireBase Team, Open Robot Marathon, IRC.
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
#===============================================================================

## set for project will using "Toolchain Definitions"
## if you set the toolchain prefix is "arm-"..
if(_va_pkg_name MATCHES "arm_[^ ]*")
  ## if you add the "Target Machines" that write under "endif()"
  if(_va_pkg_name MATCHES "[^ ]*RPi2[^ ]*")
    message(STATUS "Target Machine using : Raspberry Pi")
    add_definitions(-DUSING_TARGET_ARM)
    add_definitions(-DUSING_DEVICE_MMAL -DUSING_DEVICE_SERIAL)
    set(TARGET_ARM 1)
    ## write here your "Target Machine dependant Libraries"
    ## if you use an "RPi1"..
    ## for "example" :
    ## elseif(_va_pkg_name MATCHES "[^ ]*RPi1[^ ]*")
  endif()
endif()

## set for project will using "Library Definitions"
## FireBase must have "Libraries" definition.
#add_definitions(-DUSING_LIB_PTHREAD -DUSING_LIB_OPENCV) # -DUSING_COMM

add_definitions(-DUSING_DEVICE_SERIAL -DUSING_DEVICE_MMAL)

## set for Project "Common Modules"
#add_definitions(-DUSING_COMMON_MODULE_DATABASE -DUSING_COMMON_MODULE_LOG -DUSING_COMMON_MODULE_MATHEMATICAL -DUSING_COMMON_MODULE_STRINGTOOLS -DUSING_COMMON_MODULE_THREAD -DUSING_COMMON_MODULE_TIME)
add_definitions(-DUSING_COMMON_MODULE_LOG -DUSING_COMMON_MODULE_MATHEMATICAL -DUSING_COMMON_MODULE_STRINGTOOLS -DUSING_COMMON_MODULE_HASH -DUSING_COMMON_MODULE_RANDOM)

## set for Project "Core Modules"
add_definitions(-DUSING_CORE_ENGINE_MODULE_IMAGESEARCHER -DUSING_CORE_ENGINE_MODULE_LANESEARCHER) #-DUSING_CORE_MODULE_DATAAUTOMATOR  -DUSING_CORE_ENGINE_MODULE_LANESEARCHER -DUSING_CORE_ENGINE_MODULE_STREAMING

## set for project will using "OpenCV Mode Definitions a.k.a ClientCoreDependency.hpp"
## It's default values.
add_definitions(-DUSING_MODE_FAST_DETECTOR -DUSING_MODE_ORB_EXTRACTOR -DUSING_MODE_KNN_METHOD -DUSING_MODE_BF_MATCHER)
#add_definitions(-DUSING_MODE_ONLY_DETECTION)


add_definitions(-DMAKE_TEST)