cmake_minimum_required(VERSION 3.0)
project(libWinTF8)
if(NOT MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O3")
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /Ox")
endif()
aux_source_directory(src SRC_SOURCES)
add_library(wintf8 STATIC ${SRC_SOURCES})
install(TARGETS wintf8
        RUNTIME DESTINATION "bin"
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib"
)
install(DIRECTORY src/
        DESTINATION "include/libwintf8"
        FILES_MATCHING PATTERN "*.h"
        PATTERN "libwintf8.h" EXCLUDE
        PATTERN "utils.h" EXCLUDE
)
install(FILES src/libwintf8.h
        DESTINATION "include"
)
