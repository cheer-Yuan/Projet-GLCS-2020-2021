cmake_minimum_required(VERSION 3.18)

project(GLCS2020_project CXX)

find_package(MPI REQUIRED COMPONENTS CXX)
add_subdirectory("vendor/mdspan/" "mdspan" EXCLUDE_FROM_ALL)

add_subdirectory(baselib)
add_subdirectory(heatlib)
add_subdirectory(simpleui)
add_subdirectory(programoptions)

add_executable(simpleheat
		simpleheat.cpp
)

set(CMAKE_CXX_FLAGS "-lboost_program_options")
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

target_link_libraries(simpleheat PRIVATE
		GLCS2020_project::baselib
		GLCS2020_project::heatlib
		GLCS2020_project::simpleui
		GLCS2020_project::programoptions
		GLCS2020_project::hdf5data
		MPI::MPI_CXX
		${Boost_LIBRARIES})
