#===============================================================================
#
#          File: intel_OSX_x86_64.toolchain.cmake
#
#         Uasge: cmake -DCMAKE_TOOLCHAIN_FILE=<this toolchain settings file>
#                <CMakeFile src>
#
#   Description: FireBase-Core Project for CLion using CMake.
#
#        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)
#
#       Created: 2016/09/05
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

set(CMAKE_C_COMPILER "gcc" CACHE STRING "C Compiler" FORCE)
set(CMAKE_CXX_COMPILER "g++" CACHE STRING "C++ Compiler" FORCE)
set(CMAKE_AR "ar" CACHE STRING "ar" FORCE)
set(CMAKE_LINKER "ld" CACHE STRING "linker" FORCE)
set(CMAKE_LINKER "gcc" CACHE STRING "linker" FORCE)
set(CMAKE_STRIP "strip" CACHE STRING "strip" FORCE)
set(CMAKE_RANLIB "ranlib" CACHE STRING "ranlib" FORCE)
set(CMAKE_NM "nm" CACHE STRING "nm" FORCE)