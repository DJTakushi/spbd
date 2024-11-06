# based on tahu/python/examples/example_simple.py
# sudo docker build . -t pylisten && sudo docker run pylisten
import datetime
import sys
import time

import paho.mqtt.client as mqtt
import sparkplug_b as sparkplug

from sparkplug_b import *

# Application Variables
serverUrl = "192.168.130.51"
mqtt_port = 1883
mqtt_keepalive = 60
myGroupId = "Sparkplug B Devices"
myNodeName = "Python Edge Node 1"
myDeviceName = "Emulated Device"
myUsername = "admin"
myPassword = "changeme"

subscriptions = [
  f"spBv1.0/{myGroupId}/NCMD/{myNodeName}/#",
  f"spBv1.0/{myGroupId}/DCMD/{myNodeName}/#",
  f"spBv1.0/Sparkplug B Devices/DDATA/C Edge Node 1/Emulated Device"
]
nbirth_topic = f"spBv1.0/{myGroupId}/NBIRTH/{myNodeName}"
dbirth_topic = f"spBv1.0/{myGroupId}/DBIRTH/{myNodeName}/{myDeviceName}"
ndeath_topic = f"spBv1.0/{myGroupId}/NDEATH/{myNodeName}"

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
  print(f"{datetime.datetime.now()} : {msg.topic}")
  tokens = msg.topic.split("/")

  inboundPayload = sparkplug_b_pb2.Payload()
  inboundPayload.ParseFromString(msg.payload)
  print(inboundPayload)

def publishBirth(client):
  publishNodeBirth(client)
  publishDeviceBirth(client)

def publishNodeBirth(client):
  print( "Publishing Node Birth")
  nbirth_payload = sparkplug.getNodeBirthPayload()

  # Add some device metrics (removed by takushi)

  byteArray = bytearray(nbirth_payload.SerializeToString())
  client.publish(nbirth_topic, byteArray, 0, False)

def publishDeviceBirth(client):
  print( "Publishing Device Birth")
  dbirth_payload = sparkplug.getDeviceBirthPayload()

  # Add some device metrics (removed by takushi)

  totalByteArray = bytearray(dbirth_payload.SerializeToString())
  client.publish(dbirth_topic, totalByteArray, 0, False)

def setNodeDeath(client):
  deathPayload = sparkplug.getNodeDeathPayload()
  deathByteArray = bytearray(deathPayload.SerializeToString())
  client.will_set(ndeath_topic, deathByteArray, 0, False)


def main():
  # set up the MQTT client connection
  client = mqtt.Client()
  client.on_connect = on_connect
  client.on_message = on_message
  client.username_pw_set(myUsername, myPassword)
  setNodeDeath(client)
  client.connect(serverUrl, mqtt_port, mqtt_keepalive)

  while True: # Sit and wait for inbound or outbound events
    time.sleep(.1)
    client.loop()

if __name__=="__main__":
  main()