import numpy as np
import cv2
from httpSender import httpSender
import threading, requests, time

class videoworker (threading.Thread):
	def __init__(self):
		self.__isRun = True
		self.__url = 'http://52.141.36.28:9000/attachments/file'
		threading.Thread.__init__(self)
		self.max = 3
	
	def set_max(self, max):
		self.max = max

	def run(self):
		url = self.__url
		cam = self.cam_resource()
		
		count = 0
		num = 0
		while(self.__isRun):
			count = count + 1
			img = self.capture(cam)
			if (count % 100 == 0) and (self.max > num):
				fileName = 'save.jpg'.format(count)
				self.save(fileName, img)
				self.send(url, fileName)
				num = num + 1
				print('sent img')
		
		self.release(cam)
	
	def save(self, fileName, img):
		cv2.imwrite(fileName, img)

	def capture(self, cam):
		ret, frame = cam.read()
		return cv2.cvtColor(frame, cv2.IMREAD_COLOR)

	def send(self, url, fileName):
		sender = httpSender()
		sender.send_image(url, fileName)
	
	def cam_resource(self):
		return cv2.VideoCapture(0)

	def release(self, cap):
		cap.release()
		cv2.destroyAllWindows()

	def stop(self):
		self.__isRun = False
