################################################################################
### Filter Sample
################################################################################

set(TARGET filter_sample)

add_executable(${TARGET} main.cpp)

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET}
  libiothub_client.a
  libiothub_client_mqtt_transport.a
  libumqtt.a
  libaziotsharedutil.a
  libparson.a
  libprov_auth_client.a
  libcurl.so
  libssl.so
  libcrypto.so
  libhsm_security_client.a
  libutpm.a
  libuuid.so
  libuhttp.a
)