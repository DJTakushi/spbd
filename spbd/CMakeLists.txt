################################################################################
### SPARKPLUG B DEMO MAIN
################################################################################

cmake_minimum_required(VERSION 3.2)

################################################################################
### PROJECT DESCRIPTION AND META INFO
################################################################################

set(SPBD_PROJECT_NAME        "sparkplugb_demo")
set(SPBD_PROJECT_DESCRIPTION "SparkPlugB Demo")
set(SPBD_AUTHOR_DOMAIN       "https://github.com/DJTakushi/spbd")
set(SPBD_VERSION_MAJOR       "0")
set(SPBD_VERSION_MINOR       "0")
set(SPBD_VERSION_PATCH       "1")

set(SPBD_VERSION
    "${SPBD_VERSION_MAJOR}.${SPBD_VERSION_MINOR}.${SPBD_VERSION_PATCH}")
set(SPBD_NAME_VERSION
    "${SPBD_PROJECT_NAME} v${SPBD_VERSION} (${SPBD_VERSION_REVISION})")

################################################################################
### DECLARE PROJECT
################################################################################

project(${SPBD_PROJECT_NAME})

set(SPBD_SRC_DIR ${PROJECT_SOURCE_DIR}/spbd)

add_subdirectory(spbd)