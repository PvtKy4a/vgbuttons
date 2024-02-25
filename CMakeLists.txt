cmake_minimum_required(VERSION 3.27)

add_library(vgbuttons INTERFACE)

target_sources(vgbuttons INTERFACE
    src/vgbuttons.c
)

target_include_directories(vgbuttons INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vgbuttons
)
