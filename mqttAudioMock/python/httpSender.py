import requests 

class httpSender:
	def __init__(self):
		pass
	def send_image(self, url, fileName):
	 	f = open(fileName, "rb")
	 	upload = {'file':f}
	 	self.post(url, upload)

	# def send_image(self, url, data):
	# 	upload = {'file':data}
	# 	self.post(url, upload)

	def post(self, url, data):
		response = requests.post(url, files=data)
