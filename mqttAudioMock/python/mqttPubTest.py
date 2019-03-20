import datetime as dt
import paho.mqtt.client as mqtt

client2 = mqtt.Client("ClientPublisher")
client2.connect("127.0.0.1")
client2.publish("vds1/data", "hello world")
