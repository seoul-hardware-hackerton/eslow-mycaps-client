import pyaudio
from mqttSender import mqttSender
from time import sleep
import wave
import threading

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
CHUNK = 1024
RECORD_SECONDS = 10
WAVE_OUTPUT_FILENAME = "file.wav"

class audioworker (threading.Thread):
	def __init__(self):
		self.__isRun = True
		self.frames = []
		threading.Thread.__init__(self) 
	
	def save(self, audio):
	    waveFile = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
	    waveFile.setnchannels(CHANNELS)
	    waveFile.setsampwidth(audio.get_sample_size(FORMAT))
	    waveFile.setframerate(RATE)
	    waveFile.writeframes(b''.join(self.frames))
	
	def stop(self):
		self.__isRun = False

	def run(self):

		
		while self.__isRun:
			sender = mqttSender("Client1001")
			sender.connect("52.141.36.28")
			print ("recording...")
			audio = pyaudio.PyAudio()
			stream = audio.open(format=pyaudio.paInt16, 
		                channels=CHANNELS, 
		                rate=RATE, 
		                input=True, 
		                input_device_index=0,
		                frames_per_buffer=CHUNK)

			self.frames.clear()
			print ("clear")
			for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
			    data = stream.read(CHUNK)
			    self.frames.append(data)
			self.save(audio)
			print ("saved")
			f = open(WAVE_OUTPUT_FILENAME, "rb")
			imagestring = f.read()
			f.close()
			byteArray = bytearray(imagestring)
			print ("sent")
			sender.publish("eslow/audio", byteArray)
			print ("finished recording")
			sleep(1)
			sender.close()
			stream.stop_stream()
			stream.close()
			audio.terminate()
