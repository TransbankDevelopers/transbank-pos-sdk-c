CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
#set(CMAKE_VERBOSE_MAKEFILE ON)
project (TransbankSDK VERSION 1.0.0 DESCRIPTION "Transbank C Library for POS")
include(GNUInstallDirs)
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

set(ASSEMBLY_NAME Transbank.POS.Utils)
if(UNIX)
  if(NOT APPLE)
    set(RID linux-x64)
    set(SWIG_LIB .so)
  else()
    set(RID osx-x64)
    set(SWIG_LIB .dylib)
  endif()
else()
  set(RID win-x64)
  set(SWIG_LIB .dll)
endif()
set(RUNTIME_BAR runtime.${RID}.${ASSEMBLY_NAME})

include_directories(./src/)
set_property(SOURCE transbank.i PROPERTY CPLUSPLUS ON)
list(APPEND CMAKE_SWIG_FLAGS "-namespace;${ASSEMBLY_NAME}")

set(UseSWIG_TARGET_NAME_PREFERENCE LEGACY)
swig_add_library(${RUNTIME_BAR}
	TYPE SHARED
	LANGUAGE csharp
    SOURCES	./src/transbank.i)

add_library(TransbankSDK SHARED
    ../src/transbank.c
    ../src/transbank_serial_utils.c)

find_library (
    LSP
    NAMES serialport libserialport)

message(STATUS "LSP: [${LSP}]")

target_link_libraries(TransbankSDK ${LSP})
