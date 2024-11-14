#include <string>
#include <mosquitto.h>


#define USE_EDGE_MODULES
#include "azureiot/iothub_module_client_ll.h"
#include "azureiot/iothub_client_options.h"
#include "azureiot/iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "azureiot/iothubtransportmqtt.h"
#include "azureiot/iothub.h"

#include "device_client.h"

class een{
  std::string group_id_;
  std::string edge_node_id;
  struct mosquitto *mosq_ {NULL};
  IOTHUB_MODULE_CLIENT_LL_HANDLE handle_;
  std::map<std::string,device_client> device_map_;

  void rec_local_data_msg(std::string& msg);
 public:
  een(std::string config);
  void nbirth_send();
  void ndeath_send();
  void ncmd_rec();
  void dcmd_rec();
  void ndata__send();
};