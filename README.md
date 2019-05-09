# Transbank POS - SDK in Clang

## Dev

Download and install **libserialport** dependency ([instructions](https://sigrok.org/wiki/Libserialport)) ```git clone git://sigrok.org/libserialport```

For collaborators, if do you use Visual Studio Code please use this extension with **default formatting enabled**:

[C/C++ IntelliSense by Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)

### Drivers (This depends on the serial adapter you have)
- Driver USB Serial **Chipset CH340** for Windows: <http://www.wch.cn/download/CH341SER_EXE.html>
- Driver USB Serial **Chipset CH340** for macOS: <https://blog.sengotta.net/signed-mac-os-driver-for-winchiphead-ch340-serial-bridge/>
- Driver USB Serial **Chipset Prolific** for macOS: <http://www.prolific.com.tw/US/ShowProduct.aspx?p_id=229&pcid=41>

### Windows 10

- Visual Studio 2017 + c++ utilities.

- [msys2 - mingw-w64](http://www.msys2.org/) follow the instructions in the web site.
    - Install mingw tolchain
        - 32bits: ```pacman -S mingw-w64-i686-toolchain```
        - 64bits: ```pacman -S mingw-w64-x86_64-toolchain```
    - Install mingw cmocka
        - 32bits: ```pacman -S mingw32/mingw-w64-i686-cmocka```
        - 64bits: ```pacman -S mingw64/mingw-w64-x86_64-cmocka```
- Swig (you can use [Chocolatey](https://chocolatey.org/))

### macOS

This instructions asume you have [homebrew](https://brew.sh/) installed.

- ```brew install automake```
- ```brew install autoconf```
- ```brew install libtool```
- ```brew install swig```
- ```brew install cmocka```

### Unix

- build-esentials
- automake
- autoconf
- libtool
- swig
- cmocka


## Run / Examples & Installation

#### Build

```bash
make construct example=main
```

#### Run

```bash
make run example=main
```

#### Debug

```bash
make debug example=main
```

#### Installation

```bash
cp build/transbank.dylib /usr/local/lib
```

#### Test
The project now uses [cmocka](https://cmocka.org) to run unit test.
Once you have **cmocka** installed on yor machine, you can simply do:

```bash
make cmocka-test
```