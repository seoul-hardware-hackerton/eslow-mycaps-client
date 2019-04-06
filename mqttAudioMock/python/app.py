from videoworker import videoworker
from audioworker import audioworker

video = videoworker()
video.set_max(100)

audio = audioworker()
audio.start()
video.start()
input('wait!!')
audio.stop()
video.stop()