cmake_minimum_required(VERSION 3.18)

project(GLCS2020_project C CXX)

find_package(MPI REQUIRED COMPONENTS CXX)
#find_package(HDF5 REQUIRED COMPONENTS C HL)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)


#set(HDF5_PREFER_PARALLEL TRUE)
include_directories(/usr/include/hdf5/openmpi)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES("D:/OSGEARTH/lib")

add_subdirectory("vendor/mdspan/" "mdspan" EXCLUDE_FROM_ALL)
add_subdirectory(baselib)
add_subdirectory(heatlib)
add_subdirectory(simpleui)
add_subdirectory(programoptions)
#add_subdirectory(hdf5data)

add_executable(simpleheat
		simpleheat.cpp
)

#set(CMAKE_CXX_FLAGS "-lboost_program_options" "-lhdf5")

target_link_libraries(simpleheat PRIVATE
		GLCS2020_project::baselib
		GLCS2020_project::heatlib
		GLCS2020_project::simpleui
		GLCS2020_project::programoptions
		#GLCS2020_project::hdf5data
 
		MPI::MPI_CXX
		#hdf5::hdf5
		${Boost_LIBRARIES}
		)

### Codes related to hdf5 are marked as annotation to avoid unresolved errors ###