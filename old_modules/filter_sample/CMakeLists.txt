################################################################################
### SPARKPLUG B DEMO FILTER SAMPLE
################################################################################

cmake_minimum_required(VERSION 3.28.3)

################################################################################
### PROJECT DESCRIPTION AND META INFO
################################################################################

set(PROJECT_NAME        "sparkplugb_demo_filter_sample")
set(PROJECT_DESCRIPTION "SparkPlugB Demo Filter Sample")
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

set(SRC_DIR ${PROJECT_SOURCE_DIR}/filter_sample)

add_subdirectory(filter_sample)