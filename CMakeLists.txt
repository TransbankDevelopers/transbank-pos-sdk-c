CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
project (TransbankSDK VERSION 1.0.0 DESCRIPTION "Transbank C Library for POS" LANGUAGES C)
include(GNUInstallDirs)
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

#Define extension depending on OS.
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
set(RUNTIME_BAR TransbankWrap.${RID})

include_directories(${CMAKE_SOURCE_DIR}/src)
set_property(SOURCE ./src/transbank.i PROPERTY CPLUSPLUS ON)
list(APPEND CMAKE_SWIG_FLAGS "-namespace;${ASSEMBLY_NAME}")

set(UseSWIG_TARGET_NAME_PREFERENCE LEGACY)
swig_add_library(${RUNTIME_BAR}
	TYPE SHARED
	LANGUAGE csharp
    SOURCES	./src/transbank.i ./src/transbank.h ./src/transbank_serial_utils.h)

find_library (
    LSP
    NAMES serialport libserialport)

target_link_libraries(${RUNTIME_BAR} ${LSP})
