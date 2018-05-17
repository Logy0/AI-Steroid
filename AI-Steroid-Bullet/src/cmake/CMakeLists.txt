
#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)


set (CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++11")
set (CMAKE_EXE_LINKER_FLAGS "-lsfml-graphics -lsfml-window -lsfml-system")


set(EXECUTABLE_OUTPUT_PATH ../../bin/${CMAKE_BUILD_TYPE})

project(AI-Steroid)

file( GLOB SOURCES ../sources/* )

# Define sources and executable
set(EXECUTABLE_NAME "AI-Steroid.exe")
add_executable(${EXECUTABLE_NAME} ${SOURCES})


# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
