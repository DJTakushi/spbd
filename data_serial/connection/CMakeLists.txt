cmake_minimum_required(VERSION 3.28.3)

project(connections)

file(GLOB_RECURSE SOURCE
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_library(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE
  "${PROJECT_SOURCE_DIR}/include"
)
