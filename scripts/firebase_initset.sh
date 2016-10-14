!#/bin/bash - 
#===============================================================================
#
#          FILE: firebase_proj_init.sh
# 
#         USAGE: ./firebase_proj_init.sh 
# 
#   DESCRIPTION: FireBase Project initialization script.
# 
#        AUTHOR: Doohoon Kim (Gabriel Kim, invi.dh.kim@gmail.com) 
#  ORGANIZATION: none
#       CREATED: 2016/09/14 23:40
#      REVISION: 0.1.0
#===============================================================================

ScriptFileName="firebase_proj_init.sh"

NowPwd=$('pwd')

echo "$ScriptFileName is now excuting \"$NowPwd\""

function forCLion()
{
	## for CLion
	cd ../devenv/CLion
	ln -s ../../src src
	ln -s ../../cmake cmake

	cd $NowPwd
}

function forDotSrcTree() {
	for dir in *;
	do
		if [ -d "$dir" ]; then
			(cd -- "$dir" && forDotSrcTree)
		fi
		touch .srctree
	done
}
																										
forCLion
(cd ../src; forDotSrcTree)
