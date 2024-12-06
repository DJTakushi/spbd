# sudo docker build . -t mqtt_listen && sudo docker run pylisten
import datetime
import os
import sys
import time

import paho.mqtt.client as mqtt

# Application Variables
serverUrl = "192.168.130.51"
mqtt_port = 1883
mqtt_keepalive = 60
myUsername = "admin"
myPassword = "changeme"

subscriptions = []
default_subscription = "een"

def on_connect(client, userdata, flags, rc):
  if rc == 0:
    print(f"Connected to {serverUrl} with result code "+str(rc))
  else:
    print("Failed to connect with result code "+str(rc))
    sys.exit()

  for sub in subscriptions:
    client.subscribe(sub)
    print(f"subscribed to {sub}")

def on_message(client, userdata, msg):
  print(f"{datetime.datetime.now()} : {msg.topic}")
  # tokens = msg.topic.split("/")
  print(msg.payload)

def main():
  # set up the MQTT client connection
  client = mqtt.Client()
  client.on_connect = on_connect
  client.on_message = on_message
  client.username_pw_set(myUsername, myPassword)
  client.connect(serverUrl, mqtt_port, mqtt_keepalive)

  try:
    while True: # Sit and wait for inbound or outbound events
      time.sleep(.1)
      client.loop()
  except KeyboardInterrupt:
    print('exiting...')
    try:
      sys.exit(0)
    except SystemExit:
      os._exit(0)

if __name__=="__main__":
  for i in sys.argv[1:]:
    if i[0:2] == "-a":
      serverUrl = i[2:]
    elif i[0:2] == "-s":
      subscriptions.append(i[2:])
    else:
      print("unknown argument: \""+i+"\"")
  if len(subscriptions) == 0:
    print(f"applying default subscription : {default_subscription}")
    subscriptions.append(default_subscription)
  main()