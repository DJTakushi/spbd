#include "attribute_host.h"

attribute_host::attribute_host(){
  // TODO :
}
void attribute_host::update_attributes(nlohmann::json& j){
  steady_tp now = std::chrono::steady_clock::now();
  if(j.is_array()){
    for(auto it = j.begin(); it != j.end(); ++it){
      if(it->contains("name") && it->contains("datatype")){
        std::string attr_name = (*it)["name"];
        uint64_t attr_datatype = (*it)["datatype"];
        if(attributes_.find(attr_name)==attributes_.end()){
          attributes_[attr_name] = std::make_shared<attribute>(attr_datatype);
        }
        else{
          // do nothing?
        }

        if(it->contains("value") && it->contains("timestamp")){
          if((*it)["timestamp"] != attributes_[attr_name]->reported_epoch_get()){
            attributes_[attr_name]->set_value_with_timetamp((*it)["value"],now);
            attributes_[attr_name]->reported_epoch_set((*it)["timestamp"]);
          }
        }

        //should timestamp be updated if it is included?
        // assume all should be updated (for now[!])

      }

    }
  }
}
void ddata_gen(org_eclipse_tahu_protobuf_Payload& payload){
  // TODO :
}