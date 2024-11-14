#include "attribute_host.h"

attribute_host::attribute_host(){
  // TODO :
}

void attribute_host::update_attribute(nlohmann::json& j, steady_tp time){
  if(j.contains("name") && j.contains("datatype")){
    std::string attr_name = j["name"];
    uint64_t attr_datatype = j["datatype"];
    if(attributes_.find(attr_name)==attributes_.end()){
      attributes_[attr_name] = std::make_shared<attribute>(attr_datatype);
    }
    else{
      // do nothing?
    }

    /** TODO : should timestamp be updated if it is included?
    assume all should be updated (for now[!])  */
    if(j.contains("value") && j.contains("timestamp")){
      if(j["timestamp"] != attributes_[attr_name]->reported_epoch_get()){
        attributes_[attr_name]->set_value_with_timetamp(j["value"],time);
        attributes_[attr_name]->reported_epoch_set(j["timestamp"]);
      }
    }
  }
}

void attribute_host::update_attributes_from_array(nlohmann::json& j){
  steady_tp now = std::chrono::steady_clock::now();
  if(j.is_array()){
    for(auto it = j.begin(); it != j.end(); ++it){
      update_attribute(*it,now);
    }
  }
}
void attribute_host::ddata_gen(org_eclipse_tahu_protobuf_Payload& payload){
  // TODO :
}