################################################################################
### SparkPlug B Demo
################################################################################

set(TARGET spbd)

add_executable(${TARGET} main.cpp)

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET} libtahu.a libmosquitto.so)