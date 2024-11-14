#include <iostream>
#include "device_client.h"

device_client::device_client(nlohmann::json& config) : config_(config){
  update(config);
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
void device_client::update(nlohmann::json& j){
  if(j.contains("attributes")){
    attribute_host_.update_attributes(j["attributes"]);
  }
}