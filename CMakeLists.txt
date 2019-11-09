cmake_minimum_required(VERSION 3.15)

project(u8086 CXX)

include(ExternalProject)
find_package(Git REQUIRED)

set(CMAKE_CXX_STANDARD 20)

# Version
set(U8086_MAJOR_VERSION 0)
set(U8086_MINOR_VERSION 1)
set(U8086_PATCH_VERSION 0)
set(U8086_VERSION
  ${U8086_MAJOR_VERSION}.${U8086_MINOR_VERSION}.${U8086_PATCH_VERSION})

# GSL
list(APPEND GSL_CMAKE_ARGS
  -DGSL_TEST=OFF
  -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
)

# ChaiScript
list(APPEND CHAISCRIPT_CMAKE_ARGS
  -DDYNLOAD_ENABLED=OFF
  -DBUILD_MODULES=OFF
  -DBUILD_TESTING=OFF
  -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
)

ExternalProject_Add(
  gsl
  GIT_REPOSITORY https://github.com/microsoft/GSL.git
  GIT_SHALLOW    1
  CMAKE_ARGS     ${GSL_CMAKE_ARGS}
  PREFIX         ${CMAKE_BINARY_DIR}/external/gsl/prefix
  TMP_DIR        ${CMAKE_BINARY_DIR}/external/gsl/tmp
  STAMP_DIR      ${CMAKE_BINARY_DIR}/external/gsl/stamp
  DOWNLOAD_DIR   ${CMAKE_BINARY_DIR}/external/gsl/download
  SOURCE_DIR     ${CMAKE_BINARY_DIR}/external/gsl/src
  BINARY_DIR     ${CMAKE_BINARY_DIR}/external/gsl/build
)

ExternalProject_Add(
  ChaiScript
  GIT_REPOSITORY https://github.com/ChaiScript/ChaiScript.git
  GIT_SHALLOW    1
  GIT_TAG        v6.1.0
  CMAKE_ARGS     ${CHAISCRIPT_CMAKE_ARGS}
  PREFIX         ${CMAKE_BINARY_DIR}/external/chaiscript/prefix
  TMP_DIR        ${CMAKE_BINARY_DIR}/external/chaiscript/tmp
  STAMP_DIR      ${CMAKE_BINARY_DIR}/external/chaiscript/stamp
  DOWNLOAD_DIR   ${CMAKE_BINARY_DIR}/external/chaiscript/download
  SOURCE_DIR     ${CMAKE_BINARY_DIR}/external/chaiscript/src
  BINARY_DIR     ${CMAKE_BINARY_DIR}/external/chaiscript/build
)

# Source
add_subdirectory(src)