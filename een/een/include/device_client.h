#pragma once
#include <map>
#include <string>
#include <mosquitto.h>
#include "attribute_host.h"
class device_client {
  std::string device_id_;
  std::string group_id_;
  std::string edge_node_id_;
  attribute_host attribute_host_;
  nlohmann::json config_;
  std::string topic_ddata_;
  void set_topics();
 public:
  device_client(std::string group_id,
                std::string node_id,
                nlohmann::json& config);
  void dbirth_send(struct mosquitto* m);
  void ddeath_send(struct mosquitto* m);
  void ddata_send(struct mosquitto* m);
  void dcmd_pass(std::string command);
  void update(nlohmann::json& j);
};