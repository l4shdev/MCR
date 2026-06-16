cmake_minimum_required(VERSION 3.25)

project(MCR)

find_package(Geode REQUIRED)

add_geode_mod(
    MCR
    SOURCES
        src/main.cpp
        src/MacroManager.cpp
        src/MCRLayer.cpp
    RESOURCES
        resources/logo.png
        resources/mcr_button.png
)

if(MSVC)
    target_compile_options(MCR PRIVATE /utf-8)
endif()
