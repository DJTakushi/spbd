#include "connection_i.h"

class connection_azure_routes : public connection_i {
 public:
  bool initialize();
  bool register_message_callback(std::string subscription,
                                        message_callback callback);
  void start_loop();
  void stop_loop();
};