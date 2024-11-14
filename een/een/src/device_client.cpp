#include <iostream>
#include "device_client.h"

device_client::device_client(std::string& data){
  update(data);
}
void device_client::dbirth_send(struct mosquitto* m){
  // TODO :
}
void device_client::ddeath_send(struct mosquitto* m){
  // TODO :
}
void device_client::ddata_send(struct mosquitto* m){
  // TODO :
}
void device_client::dcmd_pass(std::string command){
  // TODO :
}
void device_client::update(std::string& data){
  try {
    nlohmann::json j = nlohmann::json::parse(data);
    attribute_host_.update_attributes(j);
  }
  catch (nlohmann::json::exception& e) {
    std::cerr << "nlohmann::json::exception : " << e.what() << std::endl;
  }
}