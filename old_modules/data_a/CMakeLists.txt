################################################################################
### SPARKPLUG B DEMO DATA A
################################################################################

cmake_minimum_required(VERSION 3.28.3)

################################################################################
### PROJECT DESCRIPTION AND META INFO
################################################################################

set(PROJECT_NAME        "sparkplugb_demo_data_a")
set(PROJECT_DESCRIPTION "SparkPlugB Demo Data  A")
set(AUTHOR_DOMAIN       "https://github.com/DJTakushi/spbd")
set(VERSION_MAJOR       "0")
set(VERSION_MINOR       "0")
set(VERSION_PATCH       "1")

set(VERSION
    "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(NAME_VERSION
    "${PROJECT_NAME} v${VERSION} (${VERSION_REVISION})")

################################################################################
### DECLARE PROJECT
################################################################################

project(${PROJECT_NAME})

set(SRC_DIR ${PROJECT_SOURCE_DIR}/data_a)

add_subdirectory(data_a)