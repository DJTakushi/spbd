################################################################################
### SPARKPLUG B DEMO EEN MAIN
################################################################################

cmake_minimum_required(VERSION 3.2)

################################################################################
### PROJECT DESCRIPTION AND META INFO
################################################################################

set(EEN_PROJECT_NAME        "sparkplugb_demo_een")
set(EEN_PROJECT_DESCRIPTION "SparkPlugB Demo Embedded Edge Node")
set(EEN_AUTHOR_DOMAIN       "https://github.com/DJTakushi/spbd")
set(EEN_VERSION_MAJOR       "0")
set(EEN_VERSION_MINOR       "1")
set(EEN_VERSION_PATCH       "12")

set(EEN_VERSION
    "${EEN_VERSION_MAJOR}.${EEN_VERSION_MINOR}.${EEN_VERSION_PATCH}")
set(EEN_NAME_VERSION
    "${EEN_PROJECT_NAME} v${EEN_VERSION} (${EEN_VERSION_REVISION})")

################################################################################
### DECLARE PROJECT
################################################################################

project(${EEN_PROJECT_NAME})

set(EEN_SRC_DIR ${PROJECT_SOURCE_DIR}/een)

add_subdirectory(een)
add_subdirectory(attributes)
# add_subdirectory(iothub_handler)