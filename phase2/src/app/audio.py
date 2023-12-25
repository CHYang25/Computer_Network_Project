import os
import TermTk as ttk
import time
from playsound import playsound as ps
import threading

play_text = "▶"
pause_text = "◼ "
playing = False
url = ""

def playsound_procedure():
    os.system(f"youtube-dl --extract-audio --audio-format mp3 -o ./database/media/tmp_audio.m4a '{url}' > /dev/null 2>&1")
    time.sleep(2)
    ps("./database/media/tmp_audio.mp3")
    os.remove("./database/media/tmp_audio.mp3")

def showAudio(root=None):
    global play_text, pause_text, playing, url

    searchFrame = ttk.TTkFrame(border=True)
    searchFrame.setLayout(ttk.TTkHBoxLayout())
    inputText = ttk.TTkLineEdit(text="")
    searchButton = ttk.TTkButton(text="Search")
    searchFrame.layout().addWidget(inputText, 0, 0, 1, 20)
    searchFrame.layout().addWidget(searchButton, 0, 20, 1, 3)

    statusText = ttk.TTkLabel(text="Enter a youtube url, it takes a few seconds to search, please wait patiently")

    audioScreen = ttk.TTkLabel(text="", border=True)
    playButton = ttk.TTkButton(text=play_text, border=True)

    # the col span should be long enough to fit the image,
    # or it'll just exit
    root.layout().addWidget(searchFrame, 0, 0, 1, 200)
    root.layout().addWidget(statusText, 1, 0, 1, 200)
    root.layout().addWidget(audioScreen, 2, 0, 100, 220)
    root.layout().addWidget(playButton, 0, 200, 2, 20)

    # the play button callback function
    @ttk.pyTTkSlot()
    def playButton_callback():
        global play_text, pause_text, playing, url
        if playing:
            playButton.setText(play_text)
        else:
            playButton.setText(pause_text)
        playing = not playing

        th = threading.Thread(target=playsound_procedure())
        th.start()

        if playing:
            playButton.setText(play_text)
        else:
            playButton.setText(pause_text)
        playing = not playing
        
    playButton.clicked.connect(playButton_callback)

    # the search button callback function
    @ttk.pyTTkSlot()
    def searchButton_callback():
        global url
        url = str(inputText._text)

    searchButton.clicked.connect(searchButton_callback)