import datetime as dt
import paho.mqtt.client as mqtt
import threading
from mqttSender import mqttSender
from time import sleep


class gogo(threading.Thread):
	def __init__(self):
		self.running = True
		threading.Thread.__init__(self) 
	
	def run(self):
		
		cnt = 0
		while self.running :
			client2 = mqtt.Client("ClientPublisher3")
			client2.connect("52.141.36.28")
			client2.publish("eslow/alarm", "2")
			cnt = cnt + 1
			client2.disconnect()
			sleep(1)
			
	def stop(self):
		self.running = False

go = gogo()
go.start()
input('wait')
go.stop()