cmake_minimum_required(VERSION 3.1)
project(libWinTF8)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto -ffat-lto-objects")
        set(CMAKE_LD_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
        set(CMAKE_LD_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /LTCG")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
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
