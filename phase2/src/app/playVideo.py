import os
from playsound import playsound as ps
import threading
import argparse
import time

url = ""

def timg_playVideo():
    global url
    os.system(f"youtube-dl -o - '{url}' 2>/dev/null | timg -V -")

def ps_playSound():
    time.sleep(1.5)
    ps("./database/media/tmp_audio.mp3")
    os.remove("./database/media/tmp_audio.mp3")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-u", type=str, default=None)
    args = parser.parse_args()
    url = args.u

    os.system(f"youtube-dl --extract-audio --audio-format mp3 -o ./database/media/tmp_audio.m4a '{url}'")
   
    t1 = threading.Thread(target=timg_playVideo)
    t2 = threading.Thread(target=ps_playSound)
    t1.start()
    t2.start()
