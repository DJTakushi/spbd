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
typedef std::map<std::string,std::shared_ptr<device_client>> device_map;

/** TODO: separate mosquitto and IotEdge content into inherited class */
class een{
  bool stable_{true}; // will be flagged false if problems detected
  std::string group_id_{"Sparkplug B Devices"};
  std::string edge_node_id_{"C Edge Node 1"};
  struct mosquitto *mosq_ {NULL};
  std::string mqtt_host_name_;
  uint mqtt_host_port_;
  uint mqtt_host_keepalive_;

  IOTHUB_MODULE_CLIENT_LL_HANDLE iot_handle_{NULL};
  device_map device_map_;

  void rec_local_data_msg(std::string& msg);
  void rec_local_config_msg(std::string& msg);

  void setup_mosquitto();
  void setup_iot_hub();
  static IOTHUBMESSAGE_DISPOSITION_RESULT input1_message_callback (
                                              IOTHUB_MESSAGE_HANDLE msg,
                                              void* userContextCallback);

 public:
  een(std::string config);
  void nbirth_send();
  void ndeath_send();
  void ncmd_rec();
  void dcmd_rec();
  void ndata__send();

  bool is_stable();
  void service_mqtt();
  void service_iot_hub();
};