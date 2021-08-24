# CMakeList.txt : CMake project for Intel_SighPP_CMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.17)

enable_testing()
include(GoogleTest)
#find_package(GTest CONFIG REQUIRED)
#include(GoogleTest)
add_executable(test_demo "test.cpp" "test_classification_result.cpp" "test_ml_controller.cpp" "mock_ml_model_impl.cpp" "test_prio.cpp")

target_link_libraries(test_demo GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_link_libraries(test_demo ${OpenCV_LIBS})
target_include_directories(test_demo PRIVATE ${OpenCV_DIR})
include_directories(include ${realsense_INCLUDE_DIR})
include_directories(include ${SPDLOG_INCLUDEDIR})
target_include_directories(test_demo PRIVATE ${realsense_INCLUDE_DIR})
target_link_libraries(test_demo ${realsense2_LIBRARY})
target_include_directories(test_demo PRIVATE ${TBB_INCLUDE_DIRS})
target_compile_definitions(test_demo PRIVATE ${TBB_DEFINITIONS})
target_link_libraries(test_demo ${TBB_LIBRARIES})
target_link_libraries(test_demo ${TBB_IMPORTED_TARGETS})
target_link_libraries(test_demo websocketpp::websocketpp)
target_link_libraries(test_demo realsense2::realsense2 realsense2::realsense-file)
target_link_libraries(test_demo spdlog::spdlog spdlog::spdlog_header_only)

#include_directories(${GTEST_INCLUDE_DIRS})
#target_link_libraries(tester PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
add_test(NAME test_demo COMMAND test_demo)
#add_subdirectory("models")

