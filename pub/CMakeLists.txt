################################################################################
### SPARKPLUG B DEMO PUB MAIN
################################################################################

cmake_minimum_required(VERSION 3.2)

################################################################################
### PROJECT DESCRIPTION AND META INFO
################################################################################

set(PUB_PROJECT_NAME        "sparkplugb_demo_pub")
set(PUB_PROJECT_DESCRIPTION "SparkPlugB Demo Publisher")
set(PUB_AUTHOR_DOMAIN       "https://github.com/DJTakushi/spbd")
set(PUB_VERSION_MAJOR       "0")
set(PUB_VERSION_MINOR       "0")
set(PUB_VERSION_PATCH       "1")

set(PUB_VERSION
    "${PUB_VERSION_MAJOR}.${PUB_VERSION_MINOR}.${PUB_VERSION_PATCH}")
set(PUB_NAME_VERSION
    "${PUB_PROJECT_NAME} v${PUB_VERSION} (${PUB_VERSION_REVISION})")

################################################################################
### DECLARE PROJECT
################################################################################

project(${PUB_PROJECT_NAME})

set(PUB_SRC_DIR ${PROJECT_SOURCE_DIR}/pub)

add_subdirectory(pub)