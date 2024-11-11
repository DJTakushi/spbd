#pragma once
#include <functional>
#include <string>

typedef std::function<void(std::string, void*)> message_callback;

class connection_i {
 public:
  virtual bool initialize() = 0;
  virtual bool register_message_callback(std::string subscription,
                                          message_callback callback) = 0;
  virtual void start_loop() = 0;
  virtual void stop_loop() = 0;
};