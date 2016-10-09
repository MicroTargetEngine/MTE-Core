#===============================================================================
#
#          File: libraries.cmake
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

set(LIBRARY_MODULES "")
set(LIBRARY_DIRS "")
set(INCLUDE_DIRS "")

## set "PKG_CONFIG_EXECUTABLE" variable
set(PKG_CONFIG_EXECUTABLE "${_va_pkgconf_dir}/Pkgconf_${_va_pkg_name}/pkgconf")

## if using function is "pkg_check_modules" and "pkg_search_module"
## then including cmake default module is "FindPkgConfig.cmake"
include(FindPkgConfig)
message(STATUS "** pkgconf executable file path : ${PKG_CONFIG_EXECUTABLE}")

## Check dependant packages for project
## 1. OpenCV
pkg_check_modules(OPENCV opencv)
set(LIBRARY_MODULES ${LIBRARY_MODULES} ${OPENCV_LIBRARIES})
set(LIBRARY_DIRS ${LIBRARY_DIRS} ${OPENCV_LIBRARY_DIRS})
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${OPENCV_INCLUDE_DIRS})

## 2. GStreamer
## 2-1. GStreamer Modules & Dirs
pkg_check_modules(GSTREAMER_1_0 gstreamer-1.0)
set(LIBRARY_MODULES "${LIBRARY_MODULES};${GSTREAMER_1_0_LIBRARIES}")
set(LIBRARY_DIRS "${LIBRARY_DIRS};${GSTREAMER_1_0_LIBRARY_DIRS}")
set(INCLUDE_DIRS "${INCLUDE_DIRS};${GSTREAMER_1_0_INCLUDE_DIRS}")
## 2-2. GStreamer Dependent library(libpcre, libffi)
pkg_check_modules(LIBPCRE libpcre)
set(LIBRARY_MODULES "${LIBRARY_MODULES};${LIBPCRE_LIBRARIES}")
set(LIBRARY_DIRS "${LIBRARY_DIRS};${LIBPCRE_LIBRARY_DIRS}")
set(INCLUDE_DIRS "${INCLUDE_DIRS};${LIBPCRE_INCLUDE_DIRS}")
pkg_check_modules(LIBFFI libffi)
set(LIBRARY_MODULES "${LIBRARY_MODULES};${LIBFFI_LIBRARIES}")
set(LIBRARY_DIRS "${LIBRARY_DIRS};${LIBFFI_LIBRARY_DIRS}")
set(INCLUDE_DIRS "${INCLUDE_DIRS};${LIBFFI_INCLUDE_DIRS}")

if (TARGET_ARM MATCHES 1)
  pkg_check_modules(BCM_HOST bcm_host)
  set(LIBRARY_MODULES "${LIBRARY_MODULES};${BCM_HOST_LIBRARIES}")
  set(LIBRARY_DIRS "${LIBRARY_DIRS};${BCM_HOST_LIBRARY_DIRS}")
  set(INCLUDE_DIRS "${INCLUDE_DIRS};${BCM_HOST_INCLUDE_DIRS}")
  pkg_check_modules(MMAL mmal)
  set(LIBRARY_MODULES "${LIBRARY_MODULES};${MMAL_LIBRARIES}")
  set(LIBRARY_DIRS "${LIBRARY_DIRS};${MMAL_LIBRARY_DIRS}")
  set(INCLUDE_DIRS "${INCLUDE_DIRS};${MMAL_INCLUDE_DIRS}")
endif()

set(LIBRARY_MODULES "${LIBRARY_MODULES}" PARENT_SCOPE)
set(LIBRARY_DIRS "${LIBRARY_DIRS}" PARENT_SCOPE)
set(INCLUDE_DIRS "${INCLUDE_DIRS}" PARENT_SCOPE)