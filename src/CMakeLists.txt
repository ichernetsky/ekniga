set(EKNIGA_SOURCES ekniga.c)
add_executable(${APPNAME} ${EKNIGA_SOURCES})

target_link_libraries(${APPNAME} ${APPLIBS})
