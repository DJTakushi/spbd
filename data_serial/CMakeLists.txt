cmake_minimum_required(VERSION 3.2)

set(PUB_PROJECT_NAME        "sparkplugb_data_serial")
set(PUB_PROJECT_DESCRIPTION "SparkPlugB Demo Data Serial")
set(PUB_AUTHOR_DOMAIN       "https://github.com/DJTakushi/spbd")
set(PUB_VERSION_MAJOR       "0")
set(PUB_VERSION_MINOR       "0")
set(PUB_VERSION_PATCH       "1")

set(PUB_VERSION
    "${PUB_VERSION_MAJOR}.${PUB_VERSION_MINOR}.${PUB_VERSION_PATCH}")
set(PUB_NAME_VERSION
    "${PUB_PROJECT_NAME} v${PUB_VERSION} (${PUB_VERSION_REVISION})")


project(${PUB_PROJECT_NAME})

set(PUB_SRC_DIR ${PROJECT_SOURCE_DIR}/pub)

add_subdirectory(data_serial)
add_subdirectory(connection)