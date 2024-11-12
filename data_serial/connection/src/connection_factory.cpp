#include "connection_factory.h"

std::shared_ptr<connection_i> connection_factory::create(connection_type type) {
  std::shared_ptr<connection_i> out;
  switch(type){
    case kAzureIot:
      out = std::make_shared<connection_azure_routes>(100);
      break;
    case kMqtt:
    default:
      out = std::make_shared<connection_mqtt>(100);
  }
  return out;
}