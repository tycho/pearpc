#/bin/sh

DIR=release
EXT=.exe
LOG=release.log

BINS=0
VARS=5

function ppc_configure {

	./configure --enable-cpu=jitc_x86 CFLAGS="$1" CXXFLAGS="$1" >>$LOG 2>&1
}

function ppc_compile {

	echo "COMPILING PPC VARIANT: $1 ..."

	ppc_configure "$2"

	make clean all >>$LOG 2>&1

	if test -f src/ppc$EXT
	then
		cp src/ppc$EXT $DIR/ppc-$1$EXT
		strip $DIR/ppc-$1$EXT
		echo "HAVE BINARY ppc-$1$EXT"
		let BINS=BINS+1
	else
		echo "ERROR: NO BINARY FOUND!!!"
	fi
}

./autogen.sh

mkdir $DIR
rm -fr $DIR/*

rm -f $LOG

COMMON_CFLAGS="-funroll-loops -funroll-all-loops -fomit-frame-pointer -foptimize-register-move -fexpensive-optimizations -ffast-math -fno-defer-pop -fno-cprop-registers -fno-if-conversion2 -frerun-loop-opt -falign-labels"

ppc_compile "intel-sse3"  "-O3 -march=nocona   -mtune=nocona   -mmmx         -msse -msse2 -msse3 $COMMON_CFLAGS"
ppc_compile "intel-sse2"  "-O3 -march=pentium4 -mtune=pentium4 -mmmx         -msse -msse2        $COMMON_CFLAGS"
ppc_compile "amd-sse3"    "-O3 -march=athlon64 -mtune=athlon64 -mmmx -m3dnow -msse -msse2 -msse3 $COMMON_CFLAGS"
ppc_compile "amd-sse2"    "-O3 -march=athlonxp -mtune=athlonxp -mmmx -m3dnow -msse -msse2        $COMMON_CFLAGS"
ppc_compile "generic-mmx" "-O3 -march=i686     -mtune=i686     -mmmx                             $COMMON_CFLAGS"

if test "$BINS" -eq "$VARS"
then
	TS=`date.exe +%Y%m%d`
	echo "HAVE BINARIES. COMPRESSING TO $TS ..."
	rar a -m5 -mdg -s ../redscorp-release-$TS.rar $DIR
else
	echo "ERROR: ONLY $BINS FROM $VARS ARE BUILT!!!"
fi
