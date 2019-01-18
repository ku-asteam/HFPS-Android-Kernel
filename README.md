# HFPS-Android-Kernel
Android Kernel Module for Highly Flexible Profiling System

## Introduction
Android Kernel Module for Highly Flexible Profiling System (`HFPS-Android-Kernel`) is an implementation of [*a*STEAM Project](https://asteam.korea.ac.kr)'s Highly Flexible Profiling System (HFPS) compatible with [Android Kernel](https://android.googlesource.com/kernel/). This module makes a proc file, extracts resource usage data from the kernel, and writes the data to the proc file.

## Build Requirements
* [Android Kernel (*msm-wahoo-4.4-oreo-mr1*)](https://android.googlesource.com/kernel/msm/+/android-msm-wahoo-4.4-oreo-mr1-preview2): Follow the [instructions of the docs](https://source.android.com/setup/build/building-kernels)
* pyenv
* Cross Compiler (*aarch64-linux-android-4.9*)

## Build Instructions
1. Get the original kernel code
2. Replace original files with new files in this repository
3. Modify `build.sh` file
4. Execute `build.sh` file

## Install Requirements
* TWRP recovery
* Magisk

## Install Instructions
* twrp install kernel/flashable/taimen_6d9954c3.zip

## How to use
* Root the device with Magisk
* `/proc/chromium`