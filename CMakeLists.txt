# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project (Intel_SightPlusPlus VERSION 3.0)
find_package(OpenCV CONFIG REQUIRED)
find_package(realsense2 CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(websocketpp CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(spdlog 1.6 CONFIG REQUIRED)
find_package(InferenceEngine REQUIRED)
find_package(ngraph REQUIRED)

add_subdirectory ("SightPlusPlus_CMake")

