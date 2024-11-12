#pragma once
#include <functional>
#include <string>

typedef std::function<void(std::string, void*)> message_callback;

class connection_i {
 public:
  virtual bool initialize() = 0;
  virtual void start_loop() = 0;
  virtual void stop_loop() = 0;
  virtual std::string get_received_message() = 0;
  virtual void publish(std::string topic, std::string msg) = 0;
};