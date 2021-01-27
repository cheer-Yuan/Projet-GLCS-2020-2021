cmake_minimum_required(VERSION 3.18)

add_library(programoptions
        src/programoptionsconfig.cpp
        )
target_include_directories(programoptions PUBLIC include/)
target_link_libraries(programoptions PUBLIC GLCS2020_project::baselib)
add_library(GLCS2020_project::programoptions ALIAS programoptions)
