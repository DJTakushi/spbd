set(TARGET data_serial)

if (${AZURE_ROUTES})
  add_compile_definitions(AZURE_ROUTES=${AZURE_ROUTES})
endif()

add_executable(${TARGET} main.cpp)

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_include_directories(${TARGET} PRIVATE
  "../connection/include"
  "../serial_helpers/include"
)

target_link_libraries(${TARGET}
  libmosquitto.so
  connections
  serial_helpers
)