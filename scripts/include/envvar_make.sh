#!/bin/bash
#===============================================================================
#
#          FILE: envvar_make.sh
# 
#         USAGE: This script have only functions.
# 
#   DESCRIPTION: Put bring in your application.
# 
#        AUTHOR: Doohoan Kim (Gabriel Kim, invi.dh.kim@gmail.com) 
#
#       CREATED: 2016/09/20
#      REVISION: 0.1.0
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

ExecPwd=$('pwd')
FindLevel=3
DotRootSrc=""
GlobalEnvFileName="GlobalEnv.env"

function findDotRootFileUp()
{
	local projname=$1
	local searchpwd=$2
	local level=$3
	
	echo "$projname, $searchpwd, $level"
	if [ $level -eq 0 ]; then
		return 1
	else
		local checkrt=0
		
		if [ ! -f $searchpwd/.root ]; then
			for i in $(ls -d */ 2>/dev/null)
			do
				local returnVal=-1
				cd ${i}
				findDotRootFileUp $projname $searchpwd/${i%%/} $((level-1))
				returnVal=$?
				cd ..

				if [ $returnVal -eq 2 ]; then
					checkrt=1
					break
				fi
			done

			if [ $checkrt -eq 1 ]; then
				return 2
			fi
		else
			if [[ $searchpwd == *"$projname" ]]; then
				DotRootSrc=$searchpwd
				return 2
			fi
		fi
	fi
}

function findDotRootFileDown()
{
	local projname=$1
	local searchpwd=$2
	local level=$3

	for ((i=0; i<level; i++))
	do
		cd ..
	done

	local downnowpwd=$('pwd')
	local checkrt=0
	echo "$downnowpwd"
	if [ ! -f .root ]; then
		for i in $(ls -d */ 2>/dev/null)
		do
			local returnVal=-1
			cd ${i}
			findDotRootFileUp $projname $downnowpwd/${i%%/} $((FindLevel+level))
			returnVal=$?
			cd ..

			if [ $returnVal -eq 2 ]; then
				checkrt=1
				break
			fi
		done
	else
		if [[ $downnowpwd == *"$projname" ]]; then
			DotRootSrc=$downnowpwd
			return 2
		else
			cd $searchpwd
		fi
	fi
}

function checkEnvFile()
{
	if [ -f $DotRootSrc/$GlobalEnvFileName ]; then
		return 1
	fi
}

function makeEnvFile()
{
	touch $GlobalEnvFileName
}

function makeEnvFileInner()
{
echo "[Path]
ProjectRoot=$DotRootSrc
ProjectCMakeDirName=cmake
DevEnvDirName=devenv
ScriptsDirName=scripts
SrcDirName=src
ToolDirName=tools
3rdpartyDirName=3rdparty

[ToolNames]
CrossToolName=CrossTool2NG
PkgConfigToolName=PkgConf" > $GlobalEnvFileName
}

function setEnvFileInit()
{
	local projectName=$1
	local isFoundedProjRoot=-1

	#findDotRootFileUp "$projectName" $ExecPwd 3
	findDotRootFileDown "$projectName" $ExecPwd 1
	isFoundedProjRoot=$?
	if [ $isFoundedProjRoot -eq 2 ]; then
		local isFindEnvFile=-1
	
		echo "Project found."
		echo "\"$projectName\" Project root is \"$DotRootSrc\""
		
		cd $DotRootSrc

		checkEnvFile
		isCheckEnvFile=$?

		if [ $isCheckEnvFile -eq 0 ]; then
			makeEnvFile
		fi

		makeEnvFileInner
	else
		echo "Project not found."
	fi
}

#setEnvFileInit "FireBase-Core"
