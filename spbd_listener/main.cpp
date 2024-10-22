#include <chrono>
#include <iostream>
#include <thread>

#include <tahu.h>
#include <mosquitto.h>

/*******************************************************************************
 * SparkPlug B Listener
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
  mosquitto_log_callback_set(mosq, my_log_callback);
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
    if (message->payloadlen) {
        fprintf(stdout, "%s :: %d\n", message->topic, message->payloadlen);
    } else {
        fprintf(stdout, "%s (null)\n", message->topic);
    }
    fflush(stdout);

    // Decode the payload
    org_eclipse_tahu_protobuf_Payload inbound_payload = org_eclipse_tahu_protobuf_Payload_init_zero;

    // takushi
    const uint8_t* my_payload = static_cast<uint8_t *>(message->payload);


    if (decode_payload(&inbound_payload, my_payload, message->payloadlen)) {
    } else {
        fprintf(stderr, "Failed to decode the payload\n");
    }

    // Get the number of metrics in the payload and iterate over them handling them as needed
    int i;
    for (i = 0; i < inbound_payload.metrics_count; i++) {

        if (inbound_payload.metrics[i].name != NULL) {
            // Handle the incoming message as necessary - start with the 'Node Control' metrics
            if (strcmp(inbound_payload.metrics[i].name, "Node Control/Next Server") == 0) {
                // 'Node Control/Next Server' is an NCMD used to tell the device/client application to
                // disconnect from the current MQTT server and connect to the next MQTT server in the
                // list of available servers.  This is used for clients that have a pool of MQTT servers
                // to connect to.
                fprintf(stderr, "'Node Control/Next Server' is not implemented in this example\n");
            } else if (strcmp(inbound_payload.metrics[i].name, "output/Device Metric2") == 0) {
                // This is a metric we declared in our DBIRTH message and we're emulating an output.
                // So, on incoming 'writes' to the output we must publish a DDATA with the new output
                // value.  If this were a real output we'd write to the output and then read it back
                // before publishing a DDATA message.

                // We know this is an Int16 because of how we declated it in the DBIRTH
                uint32_t new_value = inbound_payload.metrics[i].value.int_value;
                fprintf(stdout, "CMD message for output/Device Metric2 - New Value: %d\n", new_value);

                // Create the DDATA payload
                org_eclipse_tahu_protobuf_Payload ddata_payload;
                get_next_payload(&ddata_payload);
                // Note the Metric name 'output/Device Metric2' is not needed because we're using aliases
                add_simple_metric(&ddata_payload, NULL, true, Device_Metric2, METRIC_DATA_TYPE_INT16, false, false, &new_value, sizeof(new_value));

                // Encode the payload into a binary format so it can be published in the MQTT message.
                // The binary_buffer must be large enough to hold the contents of the binary payload
                size_t buffer_length = 128;
                uint8_t *binary_buffer = (uint8_t *)malloc(buffer_length * sizeof(uint8_t));
                size_t message_length = encode_payload(binary_buffer, buffer_length, &ddata_payload);

                // Publish the DDATA on the appropriate topic
                mosquitto_publish(mosq, NULL, "spBv1.0/Sparkplug B Devices/DDATA/C Edge Node 2/Emulated Device", message_length, binary_buffer, 0, false);

                // Free the memory
                free(binary_buffer);
                free_payload(&ddata_payload);
            } else if (strcmp(inbound_payload.metrics[i].name, "output/Device Metric3") == 0) {
                // This is a metric we declared in our DBIRTH message and we're emulating an output.
                // So, on incoming 'writes' to the output we must publish a DDATA with the new output
                // value.  If this were a real output we'd write to the output and then read it back
                // before publishing a DDATA message.

                // We know this is an Boolean because of how we declated it in the DBIRTH
                bool new_value = inbound_payload.metrics[i].value.boolean_value;
                fprintf(stdout, "CMD message for output/Device Metric3 - New Value: %s\n", new_value ? "true" : "false");

                // Create the DDATA payload
                org_eclipse_tahu_protobuf_Payload ddata_payload;
                get_next_payload(&ddata_payload);
                // Note the Metric name 'output/Device Metric3' is not needed because we're using aliases
                add_simple_metric(&ddata_payload, NULL, true, Device_Metric3, METRIC_DATA_TYPE_BOOLEAN, false, false, &new_value, sizeof(new_value));

                // Encode the payload into a binary format so it can be published in the MQTT message.
                // The binary_buffer must be large enough to hold the contents of the binary payload
                size_t buffer_length = 128;
                uint8_t *binary_buffer = (uint8_t *)malloc(buffer_length * sizeof(uint8_t));
                size_t message_length = encode_payload(binary_buffer, buffer_length, &ddata_payload);

                // Publish the DDATA on the appropriate topic
                mosquitto_publish(mosq, NULL, "spBv1.0/Sparkplug B Devices/DDATA/C Edge Node 2/Emulated Device", message_length, binary_buffer, 0, false);

                // Free the memory
                free(binary_buffer);
                free_payload(&ddata_payload);
            } else {
                fprintf(stderr, "Unknown CMD: %s\n", inbound_payload.metrics[i].name);
            }
        } else if (inbound_payload.metrics[i].has_alias) {
            // Handle the incoming message as necessary - start with the 'Node Control' metrics
            if (inbound_payload.metrics[i].alias == Next_Server) {
                // 'Node Control/Next Server' is an NCMD used to tell the device/client application to
                // disconnect from the current MQTT server and connect to the next MQTT server in the
                // list of available servers.  This is used for clients that have a pool of MQTT servers
                // to connect to.
                fprintf(stderr, "'Node Control/Next Server' is not implemented in this example\n");
            } else if (inbound_payload.metrics[i].alias == Device_Metric2) {
                // This is a metric we declared in our DBIRTH message and we're emulating an output.
                // So, on incoming 'writes' to the output we must publish a DDATA with the new output
                // value.  If this were a real output we'd write to the output and then read it back
                // before publishing a DDATA message.

                // We know this is an Int16 because of how we declated it in the DBIRTH
                uint32_t new_value = inbound_payload.metrics[i].value.int_value;
                fprintf(stdout, "CMD message for output/Device Metric2 - New Value: %d\n", new_value);

                // Create the DDATA payload
                org_eclipse_tahu_protobuf_Payload ddata_payload;
                get_next_payload(&ddata_payload);
                // Note the Metric name 'output/Device Metric2' is not needed because we're using aliases
                add_simple_metric(&ddata_payload, NULL, true, Device_Metric2, METRIC_DATA_TYPE_INT16, false, false, &new_value, sizeof(new_value));

                // Encode the payload into a binary format so it can be published in the MQTT message.
                // The binary_buffer must be large enough to hold the contents of the binary payload
                size_t buffer_length = 128;
                uint8_t *binary_buffer = (uint8_t *)malloc(buffer_length * sizeof(uint8_t));
                size_t message_length = encode_payload(binary_buffer, buffer_length, &ddata_payload);

                // Publish the DDATA on the appropriate topic
                mosquitto_publish(mosq, NULL, "spBv1.0/Sparkplug B Devices/DDATA/C Edge Node 2/Emulated Device", message_length, binary_buffer, 0, false);

                // Free the memory
                free(binary_buffer);
                free_payload(&ddata_payload);
            } else if (inbound_payload.metrics[i].alias == Device_Metric3) {
                // This is a metric we declared in our DBIRTH message and we're emulating an output.
                // So, on incoming 'writes' to the output we must publish a DDATA with the new output
                // value.  If this were a real output we'd write to the output and then read it back
                // before publishing a DDATA message.

                // We know this is an Boolean because of how we declated it in the DBIRTH
                bool new_value = inbound_payload.metrics[i].value.boolean_value;
                fprintf(stdout, "CMD message for output/Device Metric3 - New Value: %s\n", new_value ? "true" : "false");

                // Create the DDATA payload
                org_eclipse_tahu_protobuf_Payload ddata_payload;
                get_next_payload(&ddata_payload);
                // Note the Metric name 'output/Device Metric3' is not needed because we're using aliases
                add_simple_metric(&ddata_payload, NULL, true, Device_Metric3, METRIC_DATA_TYPE_BOOLEAN, false, false, &new_value, sizeof(new_value));

                // Encode the payload into a binary format so it can be published in the MQTT message.
                // The binary_buffer must be large enough to hold the contents of the binary payload
                size_t buffer_length = 128;
                uint8_t *binary_buffer = (uint8_t *)malloc(buffer_length * sizeof(uint8_t));
                size_t message_length = encode_payload(binary_buffer, buffer_length, &ddata_payload);

                // Publish the DDATA on the appropriate topic
                mosquitto_publish(mosq, NULL, "spBv1.0/Sparkplug B Devices/DDATA/C Edge Node 2/Emulated Device", message_length, binary_buffer, 0, false);

                // Free the memory
                free(binary_buffer);
                free_payload(&ddata_payload);
            } else {
                fprintf(stderr, "Unknown CMD: %ld\n", inbound_payload.metrics[i].alias);
            }
        } else {
            fprintf(stdout, "Not a metric name or alias??\n");
        }
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
