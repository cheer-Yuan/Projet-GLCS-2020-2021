cmake_minimum_required(VERSION 3.18)

project(GLCS2020_project C CXX)

find_package(MPI REQUIRED COMPONENTS CXX)
add_subdirectory("vendor/mdspan/" "mdspan" EXCLUDE_FROM_ALL)

set(HDF5_PREFER_PARALLEL TRUE)
find_package(HDF5 REQUIRED COMPONENTS C)

add_subdirectory(baselib)
add_subdirectory(heatlib)
add_subdirectory(simpleui)
add_subdirectory(programoptions)
#add_subdirectory(hdf5data)

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
		#GLCS2020_project::hdf5data
 
		MPI::MPI_CXX
		hdf5::hdf5
		${Boost_LIBRARIES})
