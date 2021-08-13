# CMakeList.txt : CMake project for Intel_SighPP_CMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Add source to this project's executable.
# TODO: Add tests and install targets if needed.

#file(GLOB MY_FILES 
	#${CMAKE_CURRENT_SOURCE_DIR}/models/*.caffemodel
	#${CMAKE_CURRENT_SOURCE_DIR}/models/*.prototxt
	#${CMAKE_CURRENT_SOURCE_DIR}/models/*.txt) 
#file(COPY ${MY_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/models)

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src/debug/models)
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src/models)
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models/ DESTINATION ${CMAKE_BINARY_DIR}/bin/models)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/models/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug/models/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/models/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/models/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/models/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/release/models/)
add_subdirectory("src")



#ADD_SUBDIRECTORY("tests")
#add_subdirectory("models")

