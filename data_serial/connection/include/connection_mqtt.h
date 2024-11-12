#include "connection_base.h"

class connection_mqtt : public connection_base {
 public:
  connection_mqtt(size_t max_messages);
  bool initialize();
  void start_loop();
  void publish(std::string output_name, std::string msg);
};