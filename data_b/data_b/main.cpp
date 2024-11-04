// based on azure-iot-sdk-c/iothub_client/samples/iothub_client_sample_module_sender/iothub_client_sample_module_sender.c

#include <stdio.h>
#include <stdlib.h>

#define USE_EDGE_MODULES
#include "azureiot/iothub_module_client_ll.h"
#include "azureiot/iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "azureiot/iothub_client_options.h"
#include "azureiot/iothubtransportmqtt.h"
#include "azureiot/iothub.h"

static char msgText[1024];
static char propText[1024];
#define MESSAGE_COUNT 500
#define DOWORK_LOOP_NUM 60


typedef struct EVENT_INSTANCE_TAG {
  IOTHUB_MESSAGE_HANDLE messageHandle;
  size_t messageTrackingId; // For tracking the messages in the user callback.
} EVENT_INSTANCE;

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result,
                                    void* userContextCallback) {
  EVENT_INSTANCE* eventInstance = (EVENT_INSTANCE*)userContextCallback;
  /* Some device specific action code goes here... */
  IoTHubMessage_Destroy(eventInstance->messageHandle);
}

int main(void) {
  EVENT_INSTANCE messages[MESSAGE_COUNT];

  srand((unsigned int)time(NULL));
  double avgWindSpeed = 10.0;
  double minTemperature = 20.0;
  double minHumidity = 60.0;

  if (IoTHub_Init() != 0) {
    (void)printf("Failed to initialize the platform.\r\n");
    return 1;
  }
  IOTHUB_MODULE_CLIENT_LL_HANDLE handle;
  /* Note: You must use MQTT_Protocol as the argument below.
  Using other protocols will result in undefined behavior.*/
  handle = IoTHubModuleClient_LL_CreateFromEnvironment(MQTT_Protocol);
  if (handle == NULL) {
    (void)printf("ERROR: handle is NULL!\r\n");
    return 1;
  }

  size_t iterator = 0;
  double engine_speed=0.0;
  do {
    double temperature = minTemperature + (rand() % 10);
    double humidity = minHumidity +  (rand() % 20);
    engine_speed+=0.01;
    sprintf_s(msgText, sizeof(msgText), "{\"deviceId\":\"myFirstDevice\",\"windSpeed\":%.2f,\"temperature\":%.2f,\"humidity\":%.2f,\"engine_speed\":%.2f}", avgWindSpeed + (rand() % 4 + 2), temperature, humidity, engine_speed);
    if ((messages[iterator].messageHandle = IoTHubMessage_CreateFromString(msgText)) == NULL) {
      (void)printf("ERROR: iotHubMessageHandle is NULL!\r\n");
    }
    else {
      (void)IoTHubMessage_SetMessageId(messages[iterator].messageHandle, "MSG_ID");
      (void)IoTHubMessage_SetCorrelationId(messages[iterator].messageHandle, "CORE_ID");

      messages[iterator].messageTrackingId = iterator;
      MAP_HANDLE propMap = IoTHubMessage_Properties(messages[iterator].messageHandle);
      (void)sprintf_s(propText, sizeof(propText), temperature > 28 ? "true" : "false");
      Map_AddOrUpdate(propMap, "temperatureAlert", propText);

      if (IoTHubModuleClient_LL_SendEventToOutputAsync(handle, messages[iterator].messageHandle, "temperatureOutput", SendConfirmationCallback, &messages[iterator]) != IOTHUB_CLIENT_OK) {
        (void)printf("ERROR: IoTHubModuleClient_LL_SendEventAsync..........FAILED!\r\n");
      }
      else {
        // (void)printf("IoTHubModuleClient_LL_SendEventAsync accepted message [%d] for transmission to IoT Hub.\r\n", (int)iterator);
      }
    }

    IoTHubModuleClient_LL_DoWork(handle);
    ThreadAPI_Sleep(1000);
    iterator++;
  } while (1);

  // Loop while we wait for messages to drain off.
  size_t index = 0;
  for (index = 0; index < DOWORK_LOOP_NUM; index++) {
    IoTHubModuleClient_LL_DoWork(handle);
    ThreadAPI_Sleep(100);
  }

  IoTHubModuleClient_LL_Destroy(handle);

  (void)printf("Finished executing\n");
  IoTHub_Deinit();
  return 0;
}
