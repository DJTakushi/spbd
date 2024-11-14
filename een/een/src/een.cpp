#include <iostream>
#include "een.h"
een::een(std::string config){
  /** TODO :  */
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
