# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/<$<$CONFIG:DEBUG$>:${OUTPUT_DEBUG}$><$<$CONFIG:RELEASE$>:${OUTPUT_REL}$>")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG})
# Output directory for libraries in Release configuration
#sset(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${OUTPUT_REL})
#set(CMAKE_TOOLCHAIN_FILE C:/Users/svens/vcpkg/scripts/buildsystems/vcpkg.cmake
# CACHE STRING "Vcpkg toolchain file")
#set(VCPKG_TARGET_TRIPLET x64-windows)

#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
project (Intel_SighPP_CMake VERSION 0.1)
find_package(OpenCV CONFIG REQUIRED)
find_package(realsense2 CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(websocketpp CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(spdlog 1.6 CONFIG REQUIRED)
find_package(InferenceEngine REQUIRED)
find_package(ngraph REQUIRED)

add_subdirectory ("Intel_SighPP_CMake")

#find_package(GTest CONFIG REQUIRED PATHS C:/Users/svens/vcpkg/installed/x64-windows/share/gtest NO_DEFAULT_PATH)
#find_package(OpenCV CONFIG REQUIRED PATHS C:/Users/svens/vcpkg/installed/x64-windows/share/opencv3 NO_DEFAULT_PATH)
#find_package(realsense2 CONFIG REQUIRED PATHS C:/Users/svens/vcpkg/installed/x64-windows/share/realsense2 NO_DEFAULT_PATH)
#find_package(TBB CONFIG REQUIRED PATHS C:/Users/svens/vcpkg/installed/x64-windows/share/tbb NO_DEFAULT_PATH)