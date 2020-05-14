# IPHub Client [![Build Status](https://travis-ci.com/Hun1eR/IPHub-Client.svg?branch=master)](https://travis-ci.com/Hun1eR/IPHub-Client) [![Download](https://badgen.net/github/release/Hun1eR/IPHub-Client)](https://github.com/Hun1eR/IPHub-Client/releases/latest) ![Platform](https://badgen.net/badge/platform/linux%20%7C%20windows/gray) [![License](https://img.shields.io/github/license/Hun1eR/IPHub-Client)](LICENSE)


A simple AMX Mod X module that provide API for sending request on [IPHub](https://iphub.info) service.  
See [IPHub API Documentation](https://iphub.info/api) for more details.


>**[iphubclient.inc](third_party/scripting/include/iphubclient.inc)**  
&nbsp;  
**[plugin_example1.sma](third_party/scripting/iphub_sample1.sma)**  
**[plugin_example2.sma](third_party/scripting/iphub_sample2.sma)**


## Installation
* Install the [AMXX Mod X](https://www.amxmodx.org) version 1.8.2 or later.
* Download archive [iphubclient.zip](https://github.com/Hun1eR/IPHub-Client/releases/latest)
* Unpack the `iphubclient_amxx.dll` (for Windows) or `iphubclient_amxx_i386.so` (for Linux) to `<hlds>/cstrike/addons/amxmodx/modules` directory.


## Building
#### Prerequisites:
Compiler with C++17 support ([Visual Studio](https://visualstudio.microsoft.com), [GCC](https://gcc.gnu.org) or [Intel C++ Compiler](https://software.intel.com/c-compilers)).  
[CMake](https://cmake.org) version 3.13.5 or later (for Linux).

#### Building on Windows:
&nbsp;&nbsp;&nbsp;&nbsp;Just open the solution in Visual Studio and compile.

#### Building on Linux:
&nbsp;&nbsp;&nbsp;Open the console and go to the directory with sources (where *CMakeLists.txt* is located).  
&nbsp;&nbsp;&nbsp;For compile with *GCC* run the following command:<br/>
```sh
  rm -rf build && CC=gcc CXX=g++ cmake -B build && cmake --build build --parallel
```
&nbsp;&nbsp;&nbsp;For compile with *Intel C++ Compiler* run the following command:<br/>
```sh
  rm -rf build && CC=icc CXX=icpc cmake -B build && cmake --build build --parallel
```

## Release History
* 1.0.1
    * Code refactoring.
    * Updated SDKs and HTTPRequest library.
* 1.0.0
    * First release.


## Credits
Elviss Strazdins ([HTTPRequest](https://github.com/elnormous/HTTPRequest))  
Niels Lohmann ([json](https://github.com/nlohmann/json))  
[Giferns](https://github.com/Giferns) for help in testing.


## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
