#!/bin/bash

export PYENV_ROOT="$HOME/.pyenv"

export PATH=$PATH:"<<PATH TO dtc tool>>"
export PATH=$PATH:"<<PATH TO kernel root directory>>"

eval "$(pyenv init -)"
eval "$(pyenv virtualenv-init -)"

pyenv shell build

RDIR=$(pwd)

export MODEL=taimen
export VERSION=6d9954c3
export ARCH=arm64

export BUILD_CROSS_COMPILE="<<PATH TO cross compiler>>"

export THREADS=`grep processor /proc/cpuinfo|wc -l`

OUTDIR=$RDIR/arch/$ARCH/boot
DTSDIR=$RDIR/arch/$ARCH/boot/dts
DTBDIR=$OUTDIR/dtb
INCDIR=$RDIR/include

IMAGEDIR=$RDIR/image
BUILDDIR=$RDIR/build
BOOTDIR=$RDIR/boot
FLASHABLEDIR=$RDIR/flashable
PREBUILTDIR=$RDIR/prebuilt

PAGE_SIZE=2048
DTB_PADDING=0

KERNEL_DEFCONFIG=gfx_wahoo_defconfig

FUNC_CLEAN_KERNEL()
{
	echo "Cleaning build..."

	rm -rf $BUILDDIR
	rm -rf $BOOTDIR
}

FUNC_BUILD_TAGS()
{
	make ARCH=$ARCH -C $RDIR O=$BUILDDIR CROSS_COMPILE=$BUILD_CROSS_COMPILE \
			$KERNEL_DEFCONFIG || ABORT "Failed to set up build"

	make ARCH=$ARCH -C $RDIR O=$BUILDDIR -j$THREADS CROSS_COMPILE=$BUILD_CROSS_COMPILE tags

	mv $BUILDDIR/tags $RDIR/tags
}

FUNC_SETUP_BUILD()
{
	echo "Creating kernel config for $KERNEL_DEFCONFIG..."

	mkdir -p $BUILDDIR

	make ARCH=$ARCH -C $RDIR O=$BUILDDIR CROSS_COMPILE=$BUILD_CROSS_COMPILE \
			$KERNEL_DEFCONFIG || ABORT "Failed to set up build"
}

FUNC_BUILD_KERNEL()
{
	echo ""
	echo "=============================================="
	echo "START : FUNC_BUILD_KERNEL"
	echo "=============================================="
	echo ""
	echo "build common config="$KERNEL_DEFCONFIG ""
	echo "build "$MODEL"-"$VERSION""

	while ! make -j$THREADS ARCH=$ARCH -C $RDIR O=$BUILDDIR CROSS_COMPILE=$BUILD_CROSS_COMPILE; do
		read -rp "Build failed. Retry? " do_retry
		case $do_retry in
			Y|y) continue ;;
			*) return 1 ;;
		esac
	done

	echo ""
	echo "================================="
	echo "END   : FUNC_BUILD_KERNEL"
	echo "================================="
	echo ""
}

FUNC_MAKE_BOOT()
{
	rm -rf $BOOTDIR
	mkdir -p $BOOTDIR

	cd $BOOTDIR
	cp $PREBUILTDIR/image/$VERSION/boot.img ./boot.img
	mkboot boot.img ramdisk

	cp $BUILDDIR/arch/arm64/boot/Image.lz4-dtb ./ramdisk/kernel
	#cp $BUILDDIR/arch/arm64/boot/Image.lz4 ./ramdisk/kernel
	echo SEANDROIDENFORCE >> ./ramdisk/kernel

	mkboot ramdisk $MODEL"_"$VERSION"_boot.img"

	cp $BUILDDIR/arch/arm64/boot/dtbo.img ./$MODEL"_"$VERSION"_dtbo.img"

	rm -rf boot.img ramdisk

	cd $RDIR
}

FUNC_MAKE_FLASHABLE()
{
	rm -rf $FLASHABLEDIR
	mkdir -p $FLASHABLEDIR

	cd $FLASHABLEDIR

	cp -r $PREBUILTDIR/flashable/sample ./

	cp $BUILDDIR/arch/arm64/boot/Image.lz4-dtb ./sample/
	cp $BUILDDIR/arch/arm64/boot/dtbo.img ./sample/dtbo

	7z a -mx9 -tzip $MODEL"_"$VERSION".zip" ./sample/*

	rm -rf ./sample

	cd $RDIR
}



if [ "$1" = "clean" ]; then
rm -rf ./build.log
(
	FUNC_CLEAN_KERNEL
)
fi

if [ "$1" = "make" ] ; then
rm -rf ./build.log
(
	START_TIME=`date +%s`

	FUNC_SETUP_BUILD && FUNC_BUILD_KERNEL && FUNC_MAKE_FLASHABLE

	END_TIME=`date +%s`
	
	ELAPSED_TIME=$END_TIME-$START_TIME
	echo "Total compile time was $ELAPSED_TIME seconds"

) 2>&1 | tee -a ./build.log

fi

if [ "$1" = "tags" ]; then
	FUNC_BUILD_TAGS
fi
