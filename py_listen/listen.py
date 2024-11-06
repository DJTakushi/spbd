# based on tahu/python/examples/example_simple.py
import sys
sys.path.insert(0, "../core/")

import paho.mqtt.client as mqtt
import sparkplug_b as sparkplug
import time
import random
import string

from sparkplug_b import *

# Application Variables
serverUrl = "192.168.130.51"
myGroupId = "Sparkplug B Devices"
myNodeName = "Python Edge Node 1"
myDeviceName = "Emulated Device"
publishPeriod = 5000
myUsername = "admin"
myPassword = "changeme"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected with result code "+str(rc))
    else:
        print("Failed to connect with result code "+str(rc))
        sys.exit()

    global myGroupId
    global myNodeName

    print("starting subscriptions...")
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("spBv1.0/" + myGroupId + "/NCMD/" + myNodeName + "/#")
    print("subscribed 1...")

    client.subscribe("spBv1.0/" + myGroupId + "/DCMD/" + myNodeName + "/#")
    print("subscribed 2...")

    # client.subscribe("spBv1.0/Sparkplug B Devices/DDATA/danny")
    # print("subscribed 3...")

    client.subscribe("spBv1.0/Sparkplug B Devices/DDATA/C Edge Node 1/Emulated Device")
    print("subscribed 4...")

    print("subscribed in on_connect()")

def on_message(client, userdata, msg):
    print("Message arrived: " + msg.topic)
    # print("Message payload : " + msg.payload.decode("utf-8"))
    tokens = msg.topic.split("/")

    if tokens[0] == "spBv1.0" and tokens[1] == myGroupId and (tokens[2] == "DDATA") and tokens[3] == "C Edge Node 1":
        inboundPayload = sparkplug_b_pb2.Payload()
        inboundPayload.ParseFromString(msg.payload)
        print(f"metrics size : {len(inboundPayload.metrics)}")
        for metric in inboundPayload.metrics:
            if metric.name == "engine_speed":
                print(f"engine_speed : {metric.double_value}")

def publishBirth(client):
    publishNodeBirth(client)
    publishDeviceBirth(client)

def publishNodeBirth(client):
    print( "Publishing Node Birth")

    # Create the node birth payload
    payload = sparkplug.getNodeBirthPayload()

    # Add some device metrics (removed by takushi)

    # Publish the node birth certificate
    byteArray = bytearray(payload.SerializeToString())
    client.publish("spBv1.0/" + myGroupId + "/NBIRTH/" + myNodeName, byteArray, 0, False)

def publishDeviceBirth(client):
    print( "Publishing Device Birth")

    # Get the payload
    payload = sparkplug.getDeviceBirthPayload()

    # Add some device metrics (removed by takushi)

    # Publish the initial data with the Device BIRTH certificate
    totalByteArray = bytearray(payload.SerializeToString())
    client.publish("spBv1.0/" + myGroupId + "/DBIRTH/" + myNodeName + "/" + myDeviceName, totalByteArray, 0, False)

def main():
  print("Starting main application")

  # Create the node death payload
  deathPayload = sparkplug.getNodeDeathPayload()

  # Start of main program - Set up the MQTT client connection
  client = mqtt.Client(serverUrl, 1883, 60)
  client.on_connect = on_connect
  client.on_message = on_message
  client.username_pw_set(myUsername, myPassword)
  deathByteArray = bytearray(deathPayload.SerializeToString())
  client.will_set("spBv1.0/" + myGroupId + "/NDEATH/" + myNodeName, deathByteArray, 0, False)
  client.connect(serverUrl, 1883, 60)

  # Short delay to allow connect callback to occur
  time.sleep(.1)
  client.loop()

  # Publish the birth certificates
  publishBirth(client)

  while True:
      # Periodically publish some new data
      payload = sparkplug.getDdataPayload()

      # Add some random data to the inputs
      addMetric(payload, None, None, MetricDataType.String, ''.join(random.choice(string.ascii_lowercase) for i in range(12)))

      # Note this data we're setting to STALE via the propertyset as an example
      metric = addMetric(payload, None, None, MetricDataType.Boolean, random.choice([True, False]))
      metric.properties.keys.extend(["Quality"])
      propertyValue = metric.properties.values.add()
      propertyValue.type = ParameterDataType.Int32
      propertyValue.int_value = 500

      # Publish a message data
      byteArray = bytearray(payload.SerializeToString())
      client.publish("spBv1.0/" + myGroupId + "/DDATA/" + myNodeName + "/" + myDeviceName, byteArray, 0, False)

      # Sit and wait for inbound or outbound events
      for _ in range(5):
          time.sleep(.1)
          client.loop()

if __name__=="__main__":
  main()