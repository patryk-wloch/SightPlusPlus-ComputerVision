# CMakeList.txt : CMake project for Intel_SighPP_CMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.

# add_library(mylib SHARED classification_result.hpp)
#add_definitions("-Wall" "-g")
#include_directories(${PROJECT_SOURCE_DIR}/src)
#file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(SRC_FILES 

	  ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/classification_result.hpp
    ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/setup_helper.hpp
	  ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/interface_controller/output_stream_controller.cpp    
	  ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/interface_controller/output_stream_controller.hpp
	  ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/interface_controller/api_controller.cpp
	  ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/interface_controller/api_controller.hpp
	  ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/interface_controller/api_impl_websocket.cpp
	  ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/interface_controller/api_user_interface.hpp
    
    
    "ml_lib/inference_controller.cpp"
    "ml_lib/inference_controller.hpp"
    ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/ ML_Lib/object_tracking.cpp
    ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/ML_Lib/object_tracking.hpp
    
    
  #  ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/priority_lib/smart_priority.cpp
   # ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/priority_lib/smart_priority.hpp
    #${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/priority_lib/covid_priority.cpp
   # ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/priority_lib/covid_priority.hpp
   # ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/priority_lib/prioritiser.cpp
   # ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/priority_lib/priority_module.hpp
   # ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/priority_lib/priority_module.cpp
    
   # ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/priority_lib/priority.hpp
   # ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/priority_lib/priority_clock.cpp
   # ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/priority_lib/priority_clock.hpp

	  ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/Intel_Sight_++.cpp
    ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/service_controller.hpp
    ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/src/service_controller.cpp
)



add_executable(intel_sightPP ${SRC_FILES} "ml_lib/types.hpp") #"Intel Sight ++.cpp" "service_controller.cpp") #${SRC_FILES})

    #
  #  ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/models/MobileNetSSD_deploy.caffemodel
    #${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/models/MobileNetSSD_deploy.prototxt
    #${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/models/MobileNetSSD_deploy-classnames.txt
    #${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/models/no_bn.caffemodel
    #${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/models/no_bn.prototxt
   # ${PROJECT_SOURCE_DIR}/Intel_SighPP_CMake/models/no_bn-classnames.txt

target_link_libraries(intel_sightPP ${OpenCV_LIBS})
target_include_directories(intel_sightPP PRIVATE ${OpenCV_DIR})
include_directories(include ${realsense_INCLUDE_DIR})
include_directories(include ${SPDLOG_INCLUDEDIR})
target_include_directories(intel_sightPP PRIVATE ${realsense_INCLUDE_DIR})
target_link_libraries(intel_sightPP ${realsense2_LIBRARY})
target_include_directories(intel_sightPP PRIVATE ${TBB_INCLUDE_DIRS})
target_compile_definitions(intel_sightPP PRIVATE ${TBB_DEFINITIONS})
target_link_libraries(intel_sightPP ${TBB_LIBRARIES})
target_link_libraries(intel_sightPP ${InferenceEngine_LIBRARIES})
target_link_libraries(intel_sightPP ${NGRAPH_LIBRARIES})
target_link_libraries(intel_sightPP ${TBB_IMPORTED_TARGETS})
target_link_libraries(intel_sightPP websocketpp::websocketpp)
target_link_libraries(intel_sightPP realsense2::realsense2 realsense2::realsense-file)
target_link_libraries(intel_sightPP spdlog::spdlog spdlog::spdlog_header_only)
# TODO: Add tests and install targets if needed.
