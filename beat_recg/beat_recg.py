import time
from rcaudio import SimpleRecorder,VolumeAnalyzer

SR = SimpleRecorder()
VA = VolumeAnalyzer(rec_time = 1)
SR.register(VA)
SR.start()
while True:
    print("VOLUME : ",VA.get_volume())
    time.sleep(1)