import pyaudio
from mqttSender import mqttSender
from time import sleep
import wave
import threading

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
CHUNK = 1024
RECORD_SECONDS = 5
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
		audio = pyaudio.PyAudio()
		stream = audio.open(format=pyaudio.paInt16, 
		                channels=CHANNELS, 
		                rate=RATE, 
		                input=True, 
		                input_device_index=0,
		                frames_per_buffer=CHUNK)

		print ("recording...")
		self.frames.clear()
		sender = mqttSender("Client1000")
		sender.connect("52.141.36.28")
		for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
		    data = stream.read(CHUNK)
		    self.frames.append(data)
				
		self.save(audio)

		with open(WAVE_OUTPUT_FILENAME, "rb") as output:
			file = output.read()
			byteArray2 = bytearray(file)
			#print(byteArray2)

			sender.publish("eslow/audi", byteArray2)
			#sender.close()
			print ("finished recording")

		stream.stop_stream()
		stream.close()
		audio.terminate()

worker = audioworker()
worker.start()
input('wait')
worker.stop()