cmake_minimum_required(VERSION 3.0)
project(libWinTF8)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /GL")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

aux_source_directory(src SRC_SOURCES)
add_library(wintf8 STATIC ${SRC_SOURCES})

set_property(TARGET wintf8 PROPERTY CXX_STANDARD 11)
# Currently buggy, wait for CMake update
# target_compile_features(wintf8 PUBLIC cxx_deleted_functions cxx_explicit_conversions cxx_nullptr cxx_right_angle_brackets cxx_rvalue_references)
# target_compile_features(wintf8 PRIVATE cxx_generalized_initializers cxx_range_for)
# if(WIN32)
#     target_compile_features(wintf8 PRIVATE cxx_auto_type cxx_decltype)
# else()
#     target_compile_features(wintf8 PUBLIC cxx_inheriting_constructors)
# endif()

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
