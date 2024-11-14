#include <iostream>
#include "een.h"
een::een(std::string config){
  mosquitto_lib_init();
  mosq_ = mosquitto_new(NULL, true, NULL);
  if (!mosq_) {
      std::cerr << "Error creating mosquitto handler"<<std::endl;
  }

  mosquitto_username_pw_set(mosq_, "admin", "changeme");
  if (mosquitto_connect(mosq_,
                        mqtt_host_name_.c_str(),
                        mqtt_host_port_,
                        mqtt_host_keepalive_)) {
      std::cerr << "Unable to connect."<<std::endl;
  }
}
void een::nbirth_send(){
  /** TODO :  */
}
void een::ndeath_send(){
  /** TODO :  */
}
void een::ncmd_rec(){
  /** TODO :  */
}
void een::dcmd_rec(){
  /** TODO :  */
}
void een::ndata__send(){
  /** TODO :  */
}
void een::rec_local_data_msg(std::string& msg){
  /** TODO :  */
}
void een::rec_local_config_msg(std::string& msg){
  try {
    nlohmann::json j = nlohmann::json::parse(msg);
    if(j.contains("name")){
      std::string name = j["name"];
      if(device_map_.find(name) != device_map_.end()){
        // TODO : should we reset config here?
        device_map_[name]->update(j);
      }
      else{
        device_map_[name] = std::make_shared<device_client>(group_id_,
                                                            edge_node_id_,
                                                            j);
      }

    }
  }
  catch (nlohmann::json::exception& e) {
    std::cerr << "nlohmann::json::exception : " << e.what() << std::endl;
  }
}

bool een::is_stable(){
  return stable_;
}

void een::service_mqtt(){
  mosquitto_loop(mosq_, 0, 1);
}
void een::service_iot_hub(){
  IoTHubModuleClient_LL_DoWork(iot_handle_);
}