#

## Dev

- git clone git://sigrok.org/libserialport (Also a Dependencie)
- Driver USB Serial Chipset Prolific: <http://www.prolific.com.tw/US/ShowProduct.aspx?p_id=229&pcid=41>

### macOS

This instructiosn asume you have [homebrew](https://brew.sh/) installed.

- brew install automake
- brew install autoconf
- brew install libtool
- brew install swig
- Driver USB Serial Chipset CH340: <https://blog.sengotta.net/signed-mac-os-driver-for-winchiphead-ch340-serial-bridge/>

### Windows 10

- Visual Studio 2017 + c++ utilities.
- USB Serial Chipset driver CH340: <http://www.wch.cn/download/CH341SER_EXE.html> (This depends on the serial adapter you have)
- [msys2 - mingw-w64](http://www.msys2.org/) follow the instructions in the web site.
    - instal mingw tolchain
        - 32bits: pacman -S mingw-w64-i686-toolchain
        - 64bits: pacman -S mingw-w64-x86_64-toolchain
- Swig (you can use [Chocolatey](https://chocolatey.org/))

### Unix

- build-esentials
- automake
- autoconf
- libtool
- swig


### Build

```bash
cc src/hello.c -o build/hello -lserialport
```

### RUN

./build/hello

### Install

cp buid/transbank.dylib /usr/local/lib
