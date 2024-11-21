################################################################################
### Data A
################################################################################

set(TARGET data_a)

find_package(CURL REQUIRED)

add_executable(${TARGET} main.cpp)

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET}
  libprov_device_ll_client.a
  libiothub_client.a
  libiothub_client_mqtt_transport.a
  libprov_mqtt_ws_transport.a 
  libumqtt.a
  libaziotsharedutil.a
  libparson.a
  libserializer.a
  libprov_auth_client.a
  libcurl.so
  libssl.a
  libcrypto.a
  libhsm_security_client.a
  libutpm.a
)