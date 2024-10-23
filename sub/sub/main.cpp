#include <chrono>
#include <iostream>
#include <thread>

#include <tahu.h>
#include <mosquitto.h>

/*******************************************************************************
 * SparkPlug B Subscriber
 * Based on tahu/c/examples/udt_example/example.c
 ******************************************************************************/
enum alias_map {
    Next_Server = 0,
    Rebirth = 1,
    Reboot = 2,
    Dataset = 3,
    Node_Metric0 = 4,
    Node_Metric1 = 5,
    Node_Metric2 = 6,
    Device_Metric0 = 7,
    Device_Metric1 = 8,
    Device_Metric2 = 9,
    Device_Metric3 = 10,
    My_Custom_Motor = 11
};

/* Mosquitto Callbacks */
void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void my_connect_callback(struct mosquitto *mosq, void *userdata, int result);
void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);
void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str);

int main(int argc, char* argv[]) {
  // MQTT Parameters - copied from tahu/c/examples/udt_example
  char host[40] = "localhost";
  char* mq_host = std::getenv("MQ_HOST");
  if (mq_host != NULL){
    strcpy(host,mq_host);
  }
  int port = 1883;
  int keepalive = 60;
  bool clean_session = true;
  struct mosquitto *mosq = NULL;

  // MQTT Setup
  srand(time(NULL));
  mosquitto_lib_init();
  mosq = mosquitto_new(NULL, clean_session, NULL);
  if (!mosq) {
      fprintf(stderr, "Error: Out of memory.\n");
      return 1;
  }
  #ifdef SPARKPLUG_DEBUG
    mosquitto_log_callback_set(mosq, my_log_callback);
  #endif
  mosquitto_connect_callback_set(mosq, my_connect_callback);
  mosquitto_message_callback_set(mosq, my_message_callback);
  mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);
  mosquitto_username_pw_set(mosq, "admin2", "changeme");
  mosquitto_will_set(mosq, "spBv1.0/Sparkplug B Devices/NDEATH/C Edge Node 2", 0, NULL, 0, false);

  // MQTT Connect
  if (mosquitto_connect(mosq, host, port, keepalive)) {
      fprintf(stderr, "Unable to connect.\n");
      return 1;
  }

  int counter = 0;
  while(true) {
    for (int j = 0; j < 50; j++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        mosquitto_loop(mosq, 0, 1);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
}

/*
 * Callback for incoming MQTT messages. Since this is a Sparkplug implementation these will be NCMD and DCMD messages
 */
void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    // Decode the payload
    org_eclipse_tahu_protobuf_Payload inbound_payload = org_eclipse_tahu_protobuf_Payload_init_zero;

    long decode_ret = decode_payload(&inbound_payload,
                                    (uint8_t*)(message->payload),
                                    message->payloadlen);
    if (decode_ret) {
      std::cout << "Failed to decode the payload : decode_ret : "<<decode_ret<<std::endl;
    }

    // Get the number of metrics in the payload and iterate over them handling them as needed
    int i;
    int limit = inbound_payload.metrics_count;
    for (i = 0; i < limit; i++) {
      if (inbound_payload.metrics[i].name != NULL) {  // alias 0 to 2
        std::string name = inbound_payload.metrics[i].name;
        if (name == "engine_speed") {
          std::cout <<"engine_speed value : "<< inbound_payload.metrics[i].value.double_value;
        }
      }
      else {
        std::cout << "alias : "  <<inbound_payload.metrics[i].alias;
      }
      std::cout << std::endl;
    }

}

/*
 * Callback for successful or unsuccessful MQTT connect.  Upon successful connect, subscribe to our Sparkplug NCMD and DCMD messages.
 * A production application should handle MQTT connect failures and reattempt as necessary.
 */
void my_connect_callback(struct mosquitto *mosq, void *userdata, int result) {
    if (!result) {
        // Subscribe to commands
        mosquitto_subscribe(mosq, NULL, "spBv1.0/Sparkplug B Devices/NCMD/C Edge Node 2/#", 0);
        mosquitto_subscribe(mosq, NULL, "spBv1.0/Sparkplug B Devices/DCMD/C Edge Node 2/#", 0);
        mosquitto_subscribe(mosq, NULL, "spBv1.0/Sparkplug B Devices/DDATA/C Edge Node 1/Emulated Device", 0);
    } else {
        fprintf(stderr, "MQTT Connect failed\n");
    }
}

/*
 * Callback for successful MQTT subscriptions.
 */
void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos) {
    int i;

    fprintf(stdout, "Subscribed (mid: %d): %d", mid, granted_qos[0]);
    for (i = 1; i < qos_count; i++) {
        fprintf(stdout, ", %d", granted_qos[i]);
    }
    fprintf(stdout, "\n");
}

/*
 * MQTT logger callback
 */
void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str) {
    // Print all log messages regardless of level.
    fprintf(stdout, "%s\n", str);
}
