#pragma once
#include <tahu.h>
#include <map>
#include <mutex>
#include <string>
#include "nlohmann/json.hpp"
#include "attribute.h"
class attribute_host{
  std::map<std::string,attribute*> attributes_;
 public:
  attribute_host();
  std::mutex attribute_mutex;
  void update_attributes(nlohmann::json& j);
  void ddata_gen(org_eclipse_tahu_protobuf_Payload& payload);
};
