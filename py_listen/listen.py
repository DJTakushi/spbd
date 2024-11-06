# based on tahu/python/examples/example_simple.py
import sys
import time

import paho.mqtt.client as mqtt
import sparkplug_b as sparkplug

from sparkplug_b import *

# Application Variables
serverUrl = "192.168.130.51"
myGroupId = "Sparkplug B Devices"
myNodeName = "Python Edge Node 1"
myDeviceName = "Emulated Device"
publishPeriod = 5000
myUsername = "admin"
myPassword = "changeme"

subscriptions = [
  "spBv1.0/" + myGroupId + "/NCMD/" + myNodeName + "/#",
  "spBv1.0/" + myGroupId + "/DCMD/" + myNodeName + "/#",
  "spBv1.0/Sparkplug B Devices/DDATA/C Edge Node 1/Emulated Device"
]

def on_connect(client, userdata, flags, rc):
  if rc == 0:
    print("Connected with result code "+str(rc))
  else:
    print("Failed to connect with result code "+str(rc))
    sys.exit()

  for sub in subscriptions:
    client.subscribe(sub)
    print(f"subscribed to {sub}")

  publishBirth(client) # Publish the birth certificates

def on_message(client, userdata, msg):
  print("Message arrived: " + msg.topic)
  tokens = msg.topic.split("/")

  inboundPayload = sparkplug_b_pb2.Payload()
  inboundPayload.ParseFromString(msg.payload)
  print(inboundPayload)

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
  nbirth_topic = f"spBv1.0/{myGroupId}/NBIRTH/{myNodeName}"
  client.publish(nbirth_topic, byteArray, 0, False)

def publishDeviceBirth(client):
  print( "Publishing Device Birth")

  # Get the payload
  payload = sparkplug.getDeviceBirthPayload()

  # Add some device metrics (removed by takushi)

  # Publish the initial data with the Device BIRTH certificate
  totalByteArray = bytearray(payload.SerializeToString())
  dbirth_topic = f"spBv1.0/{myGroupId}/DBIRTH/{myNodeName}/{myDeviceName}"
  client.publish(dbirth_topic, totalByteArray, 0, False)

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
  ndeath_topic=f"spBv1.0/{myGroupId}/NDEATH/{myNodeName}"
  client.will_set(ndeath_topic, deathByteArray, 0, False)
  client.connect(serverUrl, 1883, 60)

  while True:
    # Sit and wait for inbound or outbound events
    for _ in range(5):
      time.sleep(.1)
      client.loop()

if __name__=="__main__":
  main()