import pyaudio
from mqttSender import mqttSender

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
CHUNK = 1024
RECORD_SECONDS = 5

audio = pyaudio.PyAudio()
stream = audio.open(format=pyaudio.paInt16, 
                channels=CHANNELS, 
                rate=RATE, 
                input=True, 
                input_device_index=0,
                frames_per_buffer=CHUNK)

print ("recording...")
sender = mqttSender("ClientPublisher")
sender.connect("127.0.0.1")
for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
    data = stream.read(CHUNK)
    sender.publish("vds1/data", data)

sender.publish("vds1/data","end")
print ("finished recording")

stream.stop_stream()
stream.close()
audio.terminate()