import paho.mqtt.client as mqtt
import wave
import pyaudio

audio = pyaudio.PyAudio()
frames = []

def save():
    FORMAT = pyaudio.paInt16
    CHANNELS = 1
    RATE = 16000
    CHUNK = 1024
    RECORD_SECONDS = 5
    WAVE_OUTPUT_FILENAME = "file.wav"
    waveFile = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    waveFile.setnchannels(CHANNELS)
    waveFile.setsampwidth(audio.get_sample_size(FORMAT))
    waveFile.setframerate(RATE)
    waveFile.writeframes(b''.join(frames))
    waveFile.close()

def on_message(client, userdata, message):
    frames.append(message.payload)
    if "end" == str(message.payload.decode("utf-8")):
        print("end")
        save()

client1 = mqtt.Client("ClientSubscriber")
client1.connect("127.0.0.1")
client1.subscribe("vds1/data")
client1.on_message = on_message
client1.loop_forever()
