#===============================================================================
#
#          File: recursivity_include_src.cmake
#
#         Uasge: Using function in any cmake sources.
#
#   Description: FireBase-Core Project for CLion using CMake.
#
#        Author: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com)
#
#       Created: 2016/09/18
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

# reference : https://gist.github.com/Asher-/617872

# function processes each sub-directory and then adds each source file in directory
# each function should cascade back upward in setting variables so that the bottom directory
# adds its source first, and then the level above it adds its source and passes both up, and so on...

# Using examples.
# recursively_include_src(src ".c;.cpp;.h;.hpp")

function(recursively_include_src which_directory extension_list)
	# get list of all files for this directory
	file(GLOB this_directory_all_files "${which_directory}/*")

  # get rid of .srctree
  file(GLOB this_srctree "${which_directory}/.srctree")

  # get directories only
  if(this_directory_all_files)
    if(this_srctree)
      # remove .srctree from list of files to get list of directories
      list(REMOVE_ITEM this_directory_all_files ${this_srctree})

      # remove "BlockList" File from list
      file(READ ${CMAKE_SOURCE_DIR}/BlockList block_list)
      string(ASCII 27 Esc)
      string(REGEX REPLACE "\n" ";" block_list "${block_list}")

      foreach(block_files ${block_list})
        set(block_files_src ${which_directory}/${block_files})
        file(GLOB block_file_src ${block_files_src})
        if(block_file_src)
          list(REMOVE_ITEM this_directory_all_files ${block_file_src})
        endif()
      endforeach()

      #message(STATUS "return var a : ${extension_list}, dir : ${which_directory}")

      set(files_list "")
      # get list of source from this directory
      foreach(file_extension ${extension_list})
        set(file_extension_src ${which_directory}/*${file_extension})
        file(GLOB this_directory_src ${file_extension_src})

        if(this_directory_src)
          set(files_list "${files_list};${this_directory_src}")

          # remove source from list of files to get list of directories
          list(REMOVE_ITEM this_directory_all_files ${this_directory_src})
        endif()
      endforeach()

      set(this_directory_directories ${this_directory_all_files})

      # for each sub-directory, call self with sub-directory as arg
      foreach(this_sub_directory ${this_directory_directories})
        recursively_include_src(${this_sub_directory} "${extension_list}")
      endforeach()

      # add source files to ${src_files} in PARENT_SCOPE
      set(src_files ${src_files} ${files_list} PARENT_SCOPE)
      # add directories to ${src_directories} in PARENT_SCOPE
      set(src_directories ${src_directories} ${which_directory} PARENT_SCOPE)
    endif()
  endif()
	
endfunction()