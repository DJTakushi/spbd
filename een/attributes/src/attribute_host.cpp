#include <iostream>
#include "attribute_host.h"
attribute_host::attribute_host(){
  // TODO :
}

void attribute_host::update_attribute(nlohmann::json& j, steady_tp time){
  if(j.contains("name") && j.contains("datatype")){
    std::string attr_name = j["name"];
    uint64_t attr_datatype = j["datatype"];
    if(attributes_.find(attr_name)==attributes_.end()){
      std::cout << "creating attribute : "<< attr_name <<"..."<<std::endl;
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
    else{
      std::cout << "missing 'value' and/or 'timestamp'" << std::endl;
    }
  }
  else{
    std::cout << "name or datatype not found in json : "<<j.dump()<<std::endl;
  }
}

void attribute_host::update_attributes_from_array(nlohmann::json& j){
  steady_tp now = std::chrono::steady_clock::now();
  if(j.is_array()){
    for(auto it = j.begin(); it != j.end(); ++it){
      update_attribute(*it,now);
    }
  }
  else{
    std::cout << "json not an array : " << j.dump()<<std::endl;
  }
}
void attribute_host::ddata_gen(org_eclipse_tahu_protobuf_Payload& payload){
  for(auto attr : attributes_){
    if(attr.second->is_recently_published()){
      size_t attr_size;
      switch (attr.second->get_datatype()){
        case METRIC_DATA_TYPE_INT64:
          attr_size = sizeof(uint64_t);
          break;
        case METRIC_DATA_TYPE_DOUBLE:
          attr_size = sizeof(double);
          break;
        case METRIC_DATA_TYPE_STRING:
          attr_size = sizeof(std::string);
          break;
      }

      add_simple_metric(&payload,
                        attr.first.c_str(),
                        true,
                        0, // alias; should maybe be removed
                        attr.second->get_datatype(),
                        false,
                        false,
                        attr.second->get_value(),
                        attr_size);
    }
  }
}