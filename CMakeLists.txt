cmake_minimum_required(VERSION 2.6.2)
project(EKniga C)

set(APPNAME ekniga)
set(LIBNAME libekniga)
set(APPLIBS)

## pkg-config
find_package(PkgConfig REQUIRED)

## glib
pkg_check_modules(GLIB REQUIRED glib-2.0>=2.16)
include_directories(${GLIB_INCLUDE_DIRS})
set(APPLIBS ${APPLIBS} ${GLIB_LIBRARIES})

## libpng
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
set(APPLIBS ${APPLIBS} ${PNG_LIBRARIES})

## our source code
add_subdirectory(src)
