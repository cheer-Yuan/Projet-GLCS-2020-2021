cmake_minimum_required(VERSION 3.18)

add_library(hdf5data
        src/hdf5data.cpp
        )
target_include_directories(hdf5data PUBLIC include/)
target_link_libraries(hdf5data PUBLIC GLCS2020_project::baselib)
add_library(GLCS2020_project::hdf5data ALIAS hdf5data)
