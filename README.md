# msocr

## Clone

- install [git-lfs](https://git-lfs.github.com)
- `git pull && git lfs pull`

## Build

- install [Android NDK](https://developer.android.com/ndk/guides/)
- build standalone toolchains for x86 and x86_64
 - `/opt/android-ndk/build/tools/make_standalone_toolchain.py --arch x86 --api 16 --install-dir=/opt/android-ndk-x86`
 - `/opt/android-ndk/build/tools/make_standalone_toolchain.py --arch x86_64 --api 21 --install-dir=/opt/android-ndk-x86_64`
- build anlog
 - `cd anlog && ndk-build`
- build msocrw
 - `cd msocrw && ndk-build`
- build nodeocr (requires standalone toolchains created above)
 - `cd nodeocr/jni && make`

## Run
- Extract android-x86_64 rootfs from http://www.android-x86.org/download
 - rpm2cpio - system.sfs - system.img - /{bin,xbin,lib,lib64,etc,usr}
 - `ln -s /system /`
 - create /proc, /sys, /dev, ...
- Copy this directory after compiling to /ocr under rootfs
- Enter rootfs
- `systemd-nspawn`
- `cd /ocr`
- `LD_PRELOAD=anlog/libs/x86_64/libanlog.so LD_LIBRARY_PATH=mslib/x86_64:msocrw/libs/x86_64:opencv/sdk/native/libs/x86_64:zxing/lib/x86_64 node/x86_64/bin/node`

## Usage

```javascript
const ocr = require("/ocr/nodeocr/libs/x86_64/nodeocr"); // node module path
console.log(ocr);
ocr.init('/ocr/res'); // MsOcrRes.orp directory
const fs = require('fs');
const buf = fs.readFileSync('/example.jpg');
const res = ocr.recognize_image(buf);
console.log(res);
```

## Credits

- [Microsoft Translator for Android](https://play.google.com/store/apps/details?id=com.microsoft.translator&hl=en_US)
- [nodejs for android](https://github.com/sjitech/nodejs-android-prebuilt-binaries/tree/master/nodejs-7.8.0-android-x64-full)
- [opencv for android](https://sourceforge.net/projects/opencvlibrary/files/opencv-android/3.4.3/opencv-3.4.3-android-sdk.zip/download)
- [zxing-cpp](https://github.com/nu-book/zxing-cpp/tree/master/wrappers/android)
- [IDA Pro](https://t.me/idapro)
- [JADX GUI](https://github.com/skylot/jadx)
- [Android-x86](http://www.android-x86.org/)