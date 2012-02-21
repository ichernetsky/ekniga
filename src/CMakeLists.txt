set(EKNIGA_SOURCES ek_image.c
                   ek_io.c
                   ekniga.c)
add_executable(${APPNAME} ${EKNIGA_SOURCES})

target_link_libraries(${APPNAME} ${APPLIBS})
