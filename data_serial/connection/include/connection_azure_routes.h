#define USE_EDGE_MODULES
#include "azureiot/iothub_module_client_ll.h"
#include "azureiot/iothubtransportmqtt.h"
#include "connection_base.h"

class connection_azure_routes : public connection_base {
 private:
  IOTHUB_MODULE_CLIENT_LL_HANDLE handle;
 public:
  connection_azure_routes(size_t max_messages);
  bool initialize();
  void start_loop();
  void publish(std::string topic, std::string msg);
};
