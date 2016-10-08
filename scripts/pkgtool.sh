#!/bin/bash
#===============================================================================
#
#          FILE: selectpkg.sh
# 
#         USAGE: ./selectpkg.sh <option prefix> or <select pkg number>
# 
#   DESCRIPTION: package select(pkgconf) script.
# 
#        AUTHOR: Doohoan Kim (Gabriel Kim, invi.dh.kim@gmail.com) 
#
#       CREATED: 2016/09/14 20:54
#      REVISION: 0.1.0
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

# This variable must be your exported.
PkgListDir=$SD_PKGCONFIGS
NowPkgSelectedCacheFileName="NowPkgSelected.cache"

ChoosingPath=""

PkgListArr=()

function helpMeletedNowg()
{
	echo "help >>"
}

function parseOption()
{
	case "$1" in 
		"--list" )
			confirmPkgList
			exit
			;;
		"--help" )
			helpMsg
			exit
			;;
		"--now" )
			selectedNow
			exit
			;;
		[0-9] )
			selectPkg $1
			exit
			;;
		*)
			echo "Invaild input."
			helpMsg
	esac
}

function confirmPkgList()
{
	index=0
	while read -r line
	do
		TargetName=$line
		echo "$((index + 1)). $line"
		((index++))
	done < $PkgListDir/PackageList
}

function makePkgCache()
{
	if [ ! -f "$('pwd')/$NowPkgSelectedCacheFileName" ]; then
		touch $NowPkgSelectedCacheFileName
	fi
}

function writeNowPkg()
{
	if [ ! -f "$('pwd')/$NowPkgSelectedCacheFileName" ]; then
		makePkgCache
	fi

	echo "$1" > "$('pwd')/$NowPkgSelectedCacheFileName"
}

function selectedNow()
{
	if [ ! -f "$('pwd')/$NowPkgSelectedCacheFileName" ]; then
		echo "You're not select any package, please you should do.."
		echo "\"./pkgtool.sh --list\" after \"./pkgtool.sh <number>\""
	else
		NowPkg=""
		while read -r line
		do
			NowPkg=$line
		done < $NowPkgSelectedCacheFileName

		if [ "$NowPkg" == "" ]; then
			echo "You're not select the \"Package\"."
			echo "Command \"./pkglist.sh --list\"."
		else
			echo "$NowPkg"
		fi
	fi
}

function selectPkg()
{
	index=0
	while read -r line
	do
		PkgListArr[$index]=$line
		((index++))
	done < $PkgListDir/PackageList

	if [ ! "$1" -eq 0 ]; then
		xindex=$1
		((xindex-=1))
		((index-=1))

		if [ "$xindex" -le "$index" ] && [ "$xindex" -ge 0 ]; then
			echo "select ${PkgListArr[$xindex]}"
			
			writeNowPkg ${PkgListArr[$xindex]}		

			ChoosingPath=$PkgListDir/Pkgconf_${PkgListArr[$xindex]}/lib/pkgconfig
			echo "Set now pkgconf path \"$ChoosingPath\""
		else
			echo "Set wrong list number."
		fi
	fi
}

if [ ! "$*" == "" ]; then
	parseOption $*
else
	echo "Command \"./pkgtool.sh --help\""
fi
