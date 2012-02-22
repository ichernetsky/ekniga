set(EKNIGA_SOURCES ek_image.c
                   ek_basicops.c
                   ek_threshold.c
                   ek_io.c)
set(EKNIGA_CLI_SOURCES ${EKNIGA_SOURCES}
                       ekniga_cli.c)
set(EKNIGA_GUI_SOURCES ${EKNIGA_SOURCES}
                       ekniga.c)
set(EKNIGA_LIB_SOURCES ${EKNIGA_SOURCES})

add_executable(${CLIAPPNAME} ${EKNIGA_CLI_SOURCES})
add_executable(${APPNAME} ${EKNIGA_GUI_SOURCES})
add_library(${LIBNAME} SHARED ${EKNIGA_LIB_SOURCES})

target_link_libraries(${CLIAPPNAME} ${CLIAPPLIBS})
target_link_libraries(${APPNAME} ${GUIAPPLIBS})
