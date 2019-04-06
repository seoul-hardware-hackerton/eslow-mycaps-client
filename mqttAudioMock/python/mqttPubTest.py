import datetime as dt
import paho.mqtt.client as mqtt

client2 = mqtt.Client("ClientPublisher3")
client2.connect("52.141.36.28")
client2.publish("eslow/alarm", "hello world")
client2.disconnect()