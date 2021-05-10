set(LIBNAME ${CMAKE_PROJECT_NAME}-dev)

set(SOURCES Parser.c include/Parser.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_library(${LIBNAME} STATIC ${SOURCES})

add_executable(${CMAKE_PROJECT_NAME} ParseMain.c)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC cvector ${CMAKE_PROJECT_NAME}-dev)
