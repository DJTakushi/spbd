################################################################################
### SPARKPLUG B SUBSCRIBER MAIN
################################################################################

cmake_minimum_required(VERSION 3.2)

################################################################################
### PROJECT DESCRIPTION AND META INFO
################################################################################

set(SUB_PROJECT_NAME        "sparkplugb_subscriber")
set(SUB_PROJECT_DESCRIPTION "SparkPlugB subscriber")
set(SUB_AUTHOR_DOMAIN       "https://github.com/DJTakushi/spbd")
set(SUB_VERSION_MAJOR       "0")
set(SUB_VERSION_MINOR       "0")
set(SUB_VERSION_PATCH       "1")

set(SUB_VERSION
    "${SUB_VERSION_MAJOR}.${SUB_VERSION_MINOR}.${SUB_VERSION_PATCH}")
set(SUB_NAME_VERSION
    "${SUB_PROJECT_NAME} v${SUB_VERSION} (${SUB_VERSION_REVISION})")

################################################################################
### DECLARE PROJECT
################################################################################

project(${SUB_PROJECT_NAME})

set(SUB_SRC_DIR ${PROJECT_SOURCE_DIR}/sub)

add_subdirectory(sub)