################################################################################
### SPARKPLUG B LISTENER MAIN
################################################################################

cmake_minimum_required(VERSION 3.2)

################################################################################
### PROJECT DESCRIPTION AND META INFO
################################################################################

set(SPBDL_PROJECT_NAME        "sparkplugb_listener")
set(SPBDL_PROJECT_DESCRIPTION "SparkPlugB listener")
set(SPBDL_AUTHOR_DOMAIN       "https://github.com/DJTakushi/spbd")
set(SPBDL_VERSION_MAJOR       "0")
set(SPBDL_VERSION_MINOR       "0")
set(SPBDL_VERSION_PATCH       "1")

set(SPBDL_VERSION
    "${SPBDL_VERSION_MAJOR}.${SPBDL_VERSION_MINOR}.${SPBDL_VERSION_PATCH}")
set(SPBDL_NAME_VERSION
    "${SPBDL_PROJECT_NAME} v${SPBDL_VERSION} (${SPBDL_VERSION_REVISION})")

################################################################################
### DECLARE PROJECT
################################################################################

project(${SPBDL_PROJECT_NAME})


################################################################################
### SparkPlug B Listener
################################################################################

set(TARGET spbd_listener)

add_executable(${TARGET} main.cpp)

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET} libtahu.a libmosquitto.so)