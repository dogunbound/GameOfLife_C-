cmake_minimum_required(VERSION 3.24.2)

project(Conway_GOL VERSION 0.1)

find_package(SFML 2.5.1 COMPONENTS graphics window system REQUIRED)

set(CMAKE_CXX_FALGS "${CMAKE_CXX_FLAGS} -pthread")

SET(BASEPATH "${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${BASEPATH}")

set(headers
  "game_of_life.h"
  "computation_box.h"
  "cell.h"
  "ui.h"
  "ui_cell.h"
  "ui_constants.h"
)

set(cpp
  "ui.cpp"
  "game_of_life.cpp"
  "computation_box.cpp"
  "main.cpp"
)

add_executable(${PROJECT_NAME} ${headers} ${cpp})

target_link_libraries(${PROJECT_NAME} sfml-graphics)
target_link_libraries(${PROJECT_NAME} sfml-window)
target_link_libraries(${PROJECT_NAME} sfml-system)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)


if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME} INTERFACE
    -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
  )
endif()


message("compiler: ${CMAKE_CXX_COMPILER_ID}")
