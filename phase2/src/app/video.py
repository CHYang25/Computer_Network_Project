import os
import TermTk as ttk
import time

play_text = "▶"
pause_text = "▐▐"
playing = False
url = ""

def showVideo(root=None):
    global play_text, pause_text, playing, url

    searchFrame = ttk.TTkFrame(border=True)
    searchFrame.setLayout(ttk.TTkHBoxLayout())
    inputText = ttk.TTkLineEdit(text="")
    searchButton = ttk.TTkButton(text="Search")
    searchFrame.layout().addWidget(inputText, 0, 0, 1, 20)
    searchFrame.layout().addWidget(searchButton, 0, 20, 1, 3)

    statusText = ttk.TTkLabel(text="Enter a youtube url, it takes a few seconds to search, please wait patiently")

    videoScreen = ttk.TTkLabel(text="", border=True)
    playButton = ttk.TTkButton(text=play_text, border=True)

    # the col span should be long enough to fit the image,
    # or it'll just exit
    root.layout().addWidget(searchFrame, 0, 0, 1, 200)
    root.layout().addWidget(statusText, 1, 0, 1, 200)
    root.layout().addWidget(videoScreen, 2, 0, 150, 220)
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
        # opens a new terminal to play the video
        os.system(f"gnome-terminal -- python3 /home/chyang/workspace/CN/Computer_Network_Project/phase2/src/app/playVideo.py -u {url}")

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

        # download the thumbnail of the video, ignoring the terminal output
        os.system(f"youtube-dl --write-thumbnail --skip-download -o ./database/media/tmp_thumbnail '{url}' >/dev/null 2>&1")
        time.sleep(4)
        os.system("timg ./database/media/tmp_thumbnail* > ./database/media/tmp_thumbnailt.txt")
        
        # so that the write could be ready, otherwise there's an exception
        time.sleep(4)
        with open("./database/media/tmp_thumbnailt.txt", "r") as f:
            videoScreen.setText("".join(f.readlines()))
        os.system("rm -f ./database/media/tmp*")

    searchButton.clicked.connect(searchButton_callback)