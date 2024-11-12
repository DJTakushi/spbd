#include <mosquitto.h>
#include "connection_base.h"

class connection_mqtt : public connection_base {
  struct mosquitto* mosq;
  std::string host_;
  int port_;
 public:
  connection_mqtt(std::string host,int port,size_t max_messages);
  bool initialize();
  void start_loop();
  void publish(std::string topic, std::string msg);
};