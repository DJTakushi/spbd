#pragma once
#include <tahu.h>
#include <map>
#include <mutex>
#include <string>
#include "nlohmann/json.hpp"
#include "attribute.h"

typedef std::map<std::string,std::shared_ptr<attribute>> attribute_map;
class attribute_host{
  attribute_map attributes_;
 public:
  attribute_host();
  std::mutex attribute_mutex;
  void update_attribute(nlohmann::json& j, steady_tp time);
  void update_attributes_from_array(nlohmann::json& j);
  void ddata_gen(org_eclipse_tahu_protobuf_Payload& payload);
};
