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

def publishBirth():
    publishNodeBirth()
    publishDeviceBirth()

def publishNodeBirth():
    print( "Publishing Node Birth")

    # Create the node birth payload
    payload = sparkplug.getNodeBirthPayload()

    # Set up the Node Controls
    addMetric(payload, "Node Control/Next Server", None, MetricDataType.Boolean, False)
    addMetric(payload, "Node Control/Rebirth", None, MetricDataType.Boolean, False)
    addMetric(payload, "Node Control/Reboot", None, MetricDataType.Boolean, False)

    # Add some regular node metrics
    addMetric(payload, "Node Metric0", None, MetricDataType.String, "hello node")
    addMetric(payload, "Node Metric1", None, MetricDataType.Boolean, True)
    addNullMetric(payload, "Node Metric3", None, MetricDataType.Int32)

    # Create a DataSet (012 - 345) two rows with Int8, Int16, and Int32 contents and headers Int8s, Int16s, Int32s and add it to the payload
    columns = ["Int8s", "Int16s", "Int32s"]
    types = [DataSetDataType.Int8, DataSetDataType.Int16, DataSetDataType.Int32]
    dataset = initDatasetMetric(payload, "DataSet", None, columns, types)
    row = dataset.rows.add()
    element = row.elements.add();
    element.int_value = 0
    element = row.elements.add();
    element.int_value = 1
    element = row.elements.add();
    element.int_value = 2
    row = dataset.rows.add()
    element = row.elements.add();
    element.int_value = 3
    element = row.elements.add();
    element.int_value = 4
    element = row.elements.add();
    element.int_value = 5

    # Add a metric with a custom property
    metric = addMetric(payload, "Node Metric2", None, MetricDataType.Int16, 13)
    metric.properties.keys.extend(["engUnit"])
    propertyValue = metric.properties.values.add()
    propertyValue.type = ParameterDataType.String
    propertyValue.string_value = "MyCustomUnits"

    # Create the UDT definition value which includes two UDT members and a single parameter and add it to the payload
    template = initTemplateMetric(payload, "_types_/Custom_Motor", None, None)
    templateParameter = template.parameters.add()
    templateParameter.name = "Index"
    templateParameter.type = ParameterDataType.String
    templateParameter.string_value = "0"
    addMetric(template, "RPMs", None, MetricDataType.Int32, 0)
    addMetric(template, "AMPs", None, MetricDataType.Int32, 0)

    # Publish the node birth certificate
    byteArray = bytearray(payload.SerializeToString())
    client.publish("spBv1.0/" + myGroupId + "/NBIRTH/" + myNodeName, byteArray, 0, False)

def publishDeviceBirth():
    print( "Publishing Device Birth")

    # Get the payload
    payload = sparkplug.getDeviceBirthPayload()

    # Add some device metrics
    addMetric(payload, "input/Device Metric0", None, MetricDataType.String, "hello device")
    addMetric(payload, "input/Device Metric1", None, MetricDataType.Boolean, True)
    addMetric(payload, "output/Device Metric2", None, MetricDataType.Int16, 16)
    addMetric(payload, "output/Device Metric3", None, MetricDataType.Boolean, True)
    addMetric(payload, "DateTime Metric", None, MetricDataType.DateTime, int(time.time() * 1000))

    # Create the UDT definition value which includes two UDT members and a single parameter and add it to the payload
    template = initTemplateMetric(payload, "My_Custom_Motor", None, "Custom_Motor")
    templateParameter = template.parameters.add()
    templateParameter.name = "Index"
    templateParameter.type = ParameterDataType.String
    templateParameter.string_value = "1"
    addMetric(template, "RPMs", None, MetricDataType.Int32, 123)
    addMetric(template, "AMPs", None, MetricDataType.Int32, 456)

    # Publish the initial data with the Device BIRTH certificate
    totalByteArray = bytearray(payload.SerializeToString())
    client.publish("spBv1.0/" + myGroupId + "/DBIRTH/" + myNodeName + "/" + myDeviceName, totalByteArray, 0, False)


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
publishBirth()

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
