#===============================================================================
#
#          File: envconfig.cmake
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

set(env_var
    "ProjectCMakeDirName;DevEnvDirName;ScriptsDirName;SrcDirName;ToolDirName;3rdpartyDirName;CrossToolName;PkgConfigToolName")

function(matching_global_env compare_name_matching)
  foreach(i ${env_var})
    if("${compare_name_matching}" MATCHES "${i}")
      set(return_compare_matches ${i} PARENT_SCOPE)
      break()
    endif()
  endforeach()
endfunction()

function(matching_env_var compare_name env_var_string)
  #if("${compare_name}" MATCHES "ProjectRoot")
  #  set(env_var_ProjectRoot ${env_var_string})
  if("${compare_name}" MATCHES "ProjectCMakeDirName")
    set(env_var_ProjectCMakeDirName "${env_var_string}")
  elseif("${compare_name}" MATCHES "DevEnvDirName")
    set(env_var_DevEnvDirName "${env_var_string}")
  elseif("${compare_name}" MATCHES "ScriptsDirName")
    set(env_var_ScriptsDirName "${env_var_string}")
  elseif("${compare_name}" MATCHES "SrcDirName")
    set(env_var_SrcDirName "${env_var_string}")
  elseif("${compare_name}" MATCHES "ToolDirName")
    set(env_var_ToolDirName "${env_var_string}")
  elseif("${compare_name}" MATCHES "3rdpartyDirName")
    set(env_var_3rdpartyDirName "${env_var_string}")
  elseif("${compare_name}" MATCHES "CrossToolName")
    set(env_var_CrossToolName "${env_var_string}")
  elseif("${compare_name}" MATCHES "PkgConfigToolName")
    set(env_var_PkgConfigToolName "${env_var_string}")
  endif()

  #set(env_var_ProjectRoot ${env_var_ProjectRoot} PARENT_SCOPE)
  set(env_var_ProjectCMakeDirName "${env_var_ProjectCMakeDirName}" PARENT_SCOPE)
  set(env_var_DevEnvDirName "${env_var_DevEnvDirName}" PARENT_SCOPE)
  set(env_var_ScriptsDirName "${env_var_ScriptsDirName}" PARENT_SCOPE)
  set(env_var_SrcDirName "${env_var_SrcDirName}" PARENT_SCOPE)
  set(env_var_ToolDirName "${env_var_ToolDirName}" PARENT_SCOPE)
  set(env_var_3rdpartyDirName "${env_var_3rdpartyDirName}" PARENT_SCOPE)
  set(env_var_CrossToolName "${env_var_CrossToolName}" PARENT_SCOPE)
  set(env_var_PkgConfigToolName "${env_var_PkgConfigToolName}" PARENT_SCOPE)
endfunction()

function(set_global_env proj_root)
  # it's global variable list
  file(STRINGS "${proj_root}/GlobalEnv.env" global_env)

  foreach(variables ${global_env})
    if(variables)
      if(variables MATCHES "[0-9a-zA-z]*=")
        set(loop_cnt 0)
        string(REGEX REPLACE "=" ";" list_str "${variables}")

        foreach(i ${list_str})
          if(loop_cnt MATCHES 0)
            matching_global_env(${i})
            set(store_env_var "${return_compare_matches}")
            set(loop_cnt 1)
          elseif(loop_cnt MATCHES 1)
            matching_env_var(${store_env_var} ${i})
          endif()
        endforeach()
      elseif(variables MATCHES "[[0-9a-zA-z]*]")
        unset(store_env_var)
        message(STATUS "Variables Header is ${variables} in GlobalEnv.env")
      endif()

      unset(store_env_var)
    endif()
  endforeach()

  set(env_var_ProjectRoot ${proj_root} PARENT_SCOPE)
  #set(env_var_ProjectRoot ${env_var_ProjectRoot} PARENT_SCOPE)
  set(env_var_ProjectCMakeDirName ${env_var_ProjectCMakeDirName} PARENT_SCOPE)
  set(env_var_DevEnvDirName ${env_var_DevEnvDirName} PARENT_SCOPE)
  set(env_var_ScriptsDirName ${env_var_ScriptsDirName} PARENT_SCOPE)
  set(env_var_SrcDirName ${env_var_SrcDirName} PARENT_SCOPE)
  set(env_var_ToolDirName ${env_var_ToolDirName} PARENT_SCOPE)
  set(env_var_3rdpartyDirName ${env_var_3rdpartyDirName} PARENT_SCOPE)
  set(env_var_CrossToolName ${env_var_CrossToolName} PARENT_SCOPE)
  set(env_var_PkgConfigToolName ${env_var_PkgConfigToolName} PARENT_SCOPE)

endfunction()

function(set_toolchain_env)
  set(_va_cmake_dir "${env_var_ProjectRoot}/${env_var_ProjectCMakeDirName}")
  set(_va_devenv_dir "${env_var_ProjectRoot}/${env_var_DevEnvDirName}")
  set(_va_scripts_dir "${env_var_ProjectRoot}/${env_var_ScriptsDirName}")
  set(_va_project_dir "${_va_devenv_dir}/CLion")
  set(_va_src_dir "${env_var_ProjectRoot}/${env_var_SrcDirName}")
  set(_va_tools_dir "${env_var_ProjectRoot}/${env_var_ToolDirName}")
  set(_va_3rdparty_dir "${env_var_ProjectRoot}/${env_var_3rdpartyDirName}")
  set(_va_toolchain_dir "${env_var_ProjectRoot}/${env_var_ToolDirName}/${env_var_CrossToolName}")
  set(_va_pkgconf_dir "${env_var_ProjectRoot}/${env_var_ToolDirName}/${env_var_PkgConfigToolName}")

  message(STATUS "** Project Root : ${env_var_ProjectRoot}")
  message(STATUS "** cmake dir : ${_va_cmake_dir}")
  message(STATUS "** dev dir : ${_va_devenv_dir}")
  message(STATUS "** scripts dir : ${_va_scripts_dir}")
  message(STATUS "** src dir : ${_va_src_dir}")
  message(STATUS "** tools dir : ${_va_tools_dir}")
  message(STATUS "** 3rdparty dir : ${_va_3rdparty_dir}")
  message(STATUS "** Toolchain dir : ${_va_toolchain_dir}")
  message(STATUS "** pkgconf dir : ${_va_pkgconf_dir}")

  message(STATUS "** project dir : ${_va_project_dir}")

  ## for target reference.
  #set(COMPILE_VA "arm")
  #set(COMPILE_VA "Intel")

  ## it's print a CMake all variables, watch error when a compile.
  if(_DEBUG MATCHES 1)
    if(_DEBUG_PROPERTY MATCHES 1)
      get_cmake_property(_variableNames VARIABLES)

      foreach(_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
      endforeach()
    endif()
  endif()

  if(NATIVE_TARGET_COMPILE MATCHES 1)
    set(_now_pkg_result "arm_RPi2")
    message(STATUS "** Now Selected Package : ${_now_pkg_result}")
  else()
    #set(Proj_src_Dir ${FireBase_Core_SOURCE_DIR})
    execute_process(
        COMMAND ./pkgtool.sh --now
        WORKING_DIRECTORY ${_va_scripts_dir}
        #RESULT_VARIABLE _now_pkg_result
        OUTPUT_VARIABLE _now_pkg_result
    )
    string(REGEX REPLACE "\n" "" _now_pkg_result "${_now_pkg_result}")
    message(STATUS "** Now Selected Package : ${_now_pkg_result}")

    ## Set all Complier Environments(when a compile time).
    ## I'll using gcc(g++) is 4.8.5, arm-linux-gnueabi-gcc(g++) is 4.9.3(made by crosstool-NG 1.22)
    message(STATUS "** Loading Toolchain File")
    set(CMAKE_TOOLCHAIN_FILE "${_va_project_dir}/cmake/platforms/${_now_pkg_result}.toolchain.cmake")

    include(${CMAKE_TOOLCHAIN_FILE})

    message(STATUS "** Now Selected Toolchain File : ${CMAKE_TOOLCHAIN_FILE}")
    #message(STATUS "** CMAKE_C_COMPILER : ${CMAKE_C_COMPILER}")
  endif()

  message(STATUS "** Loading Project \"CMAKE_CXX_FLAGS\"")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wpragmas -Winvalid-noreturn") # -std=c++11

  set(_va_cmake_dir ${_va_cmake_dir} PARENT_SCOPE)
  set(_va_devenv_dir ${_va_devenv_dir} PARENT_SCOPE)
  set(_va_scripts_dir ${_va_scripts_dir} PARENT_SCOPE)
  set(_va_project_dir ${_va_project_dir} PARENT_SCOPE)
  set(_va_src_dir ${_va_src_dir} PARENT_SCOPE)
  set(_va_tools_dir "${_va_tools_dir}" PARENT_SCOPE)
  set(_va_3rdparty_dir "${_va_3rdparty_dir}" PARENT_SCOPE)
  set(_va_toolchain_dir "${_va_toolchain_dir}" PARENT_SCOPE)
  set(_va_pkgconf_dir "${_va_pkgconf_dir}" PARENT_SCOPE)
  set(_va_pkg_name "${_now_pkg_result}" PARENT_SCOPE)
endfunction()

function(set_user_env)
  include(cmake/userdef/defines.cmake)
  include(cmake/userdef/libraries.cmake)
endfunction()