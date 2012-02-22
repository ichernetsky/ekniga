cmake_minimum_required(VERSION 2.6.2)
project(EKniga C)

set(APPNAME ekniga)
set(CLIAPPNAME ekniga-cli)
set(LIBNAME libekniga)

set(COMMONLIBS)

## pkg-config
find_package(PkgConfig REQUIRED)

## libpng
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
set(COMMONLIBS ${COMMONLIBS} ${PNG_LIBRARIES})

set(CLIAPPLIBS ${COMMONLIBS})

## glib
pkg_check_modules(GLIB REQUIRED glib-2.0>=2.16)
include_directories(${GLIB_INCLUDE_DIRS})
set(CLIAPPLIBS ${CLIAPPLIBS} ${GLIB_LIBRARIES})

set(GUIAPPLIBS ${COMMONLIBS})

## gtk+
pkg_check_modules(GTK REQUIRED gtk+-3.0)
include_directories(${GTK_INCLUDE_DIRS})
set(GUIAPPLIBS ${GUIAPPLIBS} ${GTK_LIBRARIES})

## our source code
add_subdirectory(src)
