#=========================================================================
#
#          File: CMakeLists.txt
#
#         Usage: It's only push Compile button at CLion!!
#
#   Description: FireBase-Core Project for CLion using CMake.
#
#        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)
#
#       Created: 2016/09/05
#
# Copyright (c) 2016, Team "FireBase", Open Robot Marathon, IRC.
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
#=========================================================================

cmake_minimum_required(VERSION 3.6)
include(cmake/recursively_include_src.cmake)
include(cmake/envconfig.cmake)

## for script debug.
set(_DEBUG 0)
set(_DEBUG_PROPERTY 0)

add_definitions(-DLOG_WRITE_MODE)

## 1. set "Project Root"
set(clion_root_str ${CMAKE_SOURCE_DIR})
string(REGEX REPLACE "/devenv/CLion" "" project_root_str "${clion_root_str}")
message(STATUS "root : ${project_root_str}")

## 2. set "Global & Toolchain & User Environments"
## Read global environments after toolchain env.
## this sources contain from "cmake" directory.
### set "Global Environments"
set_global_env("${project_root_str}")
### set "Toolchain Environments"
set_toolchain_env()
### set "User Environments"
set_user_env()

## 3. include Project sources & directories
## Recursively searching and make src string for source and directories.
recursively_include_src(${_va_project_dir}/src ".c;.cpp;.h;.hpp")

## 4. set "Project Name"
project(FireBase_Core)

## 5. include dependent include path & lib directories
message(STATUS "pkgconf Include Directories : ${INCLUDE_DIRS}")
include_directories(${INCLUDE_DIRS})

message(STATUS "pkgconf Library Directories : ${LIBRARY_DIRS}")
link_directories(${LIBRARY_DIRS})

## 6. include directories & add exec or lib
## src_directories and src_files variables via recursively_include_src function inner variables.
include_directories(${src_directories})
add_executable(FireBase_Core ${src_files})

## 7. include dependent libs
message(STATUS "** Libraries Include : ${LIBRARY_MODULES}")
target_link_libraries(FireBase_Core pthread ${LIBRARY_MODULES})

#message(STATUS "** sources :${src_files}")
#message(STATUS "** dirs :${src_directories}")

## 8. install binary
install(TARGETS FireBase_Core
    ARCHIVE DESTINATION ${_va_project_dir}/bulid/lib
    LIBRARY DESTINATION ${_va_project_dir}/bulid/lib
    RUNTIME DESTINATION ${_va_project_dir}/bulid)