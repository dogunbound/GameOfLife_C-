cmake_minimum_required(VERSION 3.24.2)

project(Conway_GOL VERSION 0.1)

find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

set(CMAKE_CXX_FALGS "${CMAKE_CXX_FLAGS} -pthread")

SET(BASEPATH "${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${BASEPATH}")



set(headers
  "aspect_ratio.h"
)

set(cpp
  "main.cpp"
)

add_executable(${PROJECT_NAME} ${headers} ${cpp})

target_link_libraries(${PROJECT_NAME} sfml-graphics)
target_link_libraries(${PROJECT_NAME} sfml-window)
target_link_libraries(${PROJECT_NAME} sfml-system)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
