#include <memory>

#include "connection_azure_routes.h"
#include "connection_mqtt.h"

enum connection_type {
  kAzureIot,
  kMqtt
};

class connection_factory {
 public:
  static std::shared_ptr<connection_i> create(connection_type type);
};