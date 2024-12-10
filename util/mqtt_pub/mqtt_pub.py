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

input_filepath = "config.json"

def on_connect(client, userdata, flags, rc):
  if rc == 0:
    print(f"Connected to {serverUrl} with result code "+str(rc))
  else:
    print("Failed to connect with result code "+str(rc))
    sys.exit()

  for sub in subscriptions:
    client.subscribe(sub)
    print(f"subscribed to {sub}")

# def on_message(client, userdata, msg):
#   print(f"{datetime.datetime.now()} : {msg.topic}")
#   # tokens = msg.topic.split("/")
#   print(msg.payload)


def on_publish(client, userdata, mid):
    try:
        userdata.remove(mid)
    except KeyError:
        print("on_publish() is called with a mid not present in unacked_publish")
        print("This is due to an unavoidable race-condition:")
        print("* publish() return the mid of the message sent.")
        print("* mid from publish() is added to unacked_publish by the main thread")
        print("* on_publish() is called by the loop_start thread")
        print("While unlikely (because on_publish() will be called after a network round-trip),")
        print(" this is a race-condition that COULD happen")
        print("")
        print("The best solution to avoid race-condition is using the msg_info from publish()")
        print("We could also try using a list of acknowledged mid rather than removing from pending list,")
        print("but remember that mid could be re-used !")

def main():
  content = ""
  with open(input_filepath, 'r') as f:
    content = f.read()

  # set up the MQTT client connection
  unacked_publish = set()
  client = mqtt.Client()
  client.user_data_set(unacked_publish)

  # client.on_connect = on_connect
  # client.on_message = on_message
  client.on_publish = on_publish
  client.username_pw_set(myUsername, myPassword)
  client.connect(serverUrl, mqtt_port, mqtt_keepalive)
  client.loop_start()

  try:
    msg_info = client.publish("data_serial_config", content, qos=1)
    unacked_publish.add(msg_info.mid)
    while len(unacked_publish):
      time.sleep(0.1)

    print("done!")
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
    elif i[0:2] == "- i":
      input_filepath=i[2:]
    else:
      print("unknown argument: \""+i+"\"")
  if len(subscriptions) == 0:
    print(f"applying default subscription : {default_subscription}")
    subscriptions.append(default_subscription)
  main()