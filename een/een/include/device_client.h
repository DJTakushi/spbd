#pragma once
#include <map>
#include <string>
#include <mosquitto.h>
#include "attribute_host.h"
class device_client {
  std::string device_id;
  attribute_host attribute_host_;
 public:
  device_client(std::string& data);
  void dbirth_send(struct mosquitto* m);
  void ddeath_send(struct mosquitto* m);
  void ddata_send(struct mosquitto* m);
  void dcmd_pass(std::string command);
  void update(std::string& data);
};