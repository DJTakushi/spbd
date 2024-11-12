#include <iostream>
#include "connection_mqtt.h"

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result) {
  if (!result) {
    // Subscribe to commands
    mosquitto_subscribe(mosq, NULL, "data_serial_input", 0);
  } else {
    std::cerr << "MQTT Connect failed" << std::endl;
  }
}

void my_message_callback(struct mosquitto *mosq,
                          void *userdata,
                          const struct mosquitto_message *message) {
  std::string payload((char *)(message->payload),message->payloadlen);
  connection_mqtt* connection = (connection_mqtt*)userdata;
  connection->add_message_to_queue(payload);
}

connection_mqtt::connection_mqtt(std::string host,
                                  int port,
                                  size_t max_messages)
    : connection_base(max_messages), host_(host), port_(port){
}

bool connection_mqtt::initialize(){
  bool ret = true;
  mosquitto_lib_init();
  bool clean_session = true;
  mosq = mosquitto_new(NULL, clean_session, this);
  if (mosq != NULL) {
    mosquitto_connect_callback_set(mosq, my_connect_callback);
    mosquitto_message_callback_set(mosq, my_message_callback);
    mosquitto_username_pw_set(mosq, "admin", "changeme");

    // MQTT Connect
    int keepalive = 60;
    if (mosquitto_connect(mosq, host_.c_str(), port_, keepalive)) {
      std::cerr<< "Unable to connect to MQTT."<<std::endl;
      ret = false;
    }
  }
  else {
    std::cerr <<  "Error: Out of memory."<<std::endl;
    ret = false;
  }
  return ret;
}

void connection_mqtt::start_loop(){
  active_=true;
  do_work_thread_ = std::thread([this](){
    while(this->active_){
      mosquitto_loop(mosq, 0, 1);
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  });
}

void connection_mqtt::publish(std::string topic, std::string msg){
  mosquitto_publish(mosq, NULL, topic.c_str(), msg.size(), msg.c_str(), 0, false);
}