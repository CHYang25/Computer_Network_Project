pytermtk: https://ceccopierangiolieugenio.github.io/pyTermTk/index.html
    https://github.com/ceccopierangiolieugenio/pyTermTk

https://cool.ntu.edu.tw/courses/31601/assignments/196338

- about server
    - https://www.geeksforgeeks.org/cpp-program-to-get-the-list-of-files-in-a-directory/


use this library to handle json file: https://github.com/nlohmann/json
    (ps just include the json.hpp from: https://json.nlohmann.me/home/releases/ the newest version)

cmake: https://cmake.org/cmake/help/latest/index.html


- https://askubuntu.com/questions/1258409/can-one-watch-video-in-terminal-command-line

- playsound package: https://www.geeksforgeeks.org/play-sound-in-python/
timg video player https://github.com/hzeller/timg

- gnome terminal:https://stackoverflow.com/questions/19308415/execute-terminal-command-from-python-in-new-terminal-window

- youtube-dl https://github.com/ytdl-org/youtube-dl
    - issue: https://askubuntu.com/questions/1457872/how-do-i-fix-youtube-dl-running-it-gives-error-message-about-incorrect-syntax
        - https://askubuntu.com/questions/1457872/how-do-i-fix-youtube-dl-running-it-gives-error-message-about-incorrect-syntax
        - 
    - youtube-dl download mp3 with python: 
        - https://askubuntu.com/questions/423508/can-i-directly-download-audio-using-youtube-dl
        - should rename it to m4a first:
        ```
        [youtube] nlLhw1mtCFA: Downloading webpage
        [dashsegments] Total fragments: 1
        [download] Destination: ./database/media/tmp_audio.mp3
        [download] 100% of 1.06MiB in 00:00
        [ffmpeg] Correcting container in "./database/media/tmp_audio.mp3"
        [ffmpeg] Post-process file ./database/media/tmp_audio.mp3 exists, skipping
        ```
        ```
        youtube-dl --extract-audio --audio-format mp3 -o ./database/media/tmp_audio.m4a "https://www.youtube.com/watch?v=nlLhw1mtCFA"
        [youtube] nlLhw1mtCFA: Downloading webpage
        [dashsegments] Total fragments: 1
        [download] Destination: ./database/media/tmp_audio.m4a
        [download] 100% of 1.06MiB in 00:00
        [ffmpeg] Correcting container in "./database/media/tmp_audio.m4a"
        [ffmpeg] Destination: ./database/media/tmp_audio.mp3
        Deleting original file ./database/media/tmp_audio.m4a (pass -k to keep)
        ```
    - audio prefetch all first, video used streaming

    - https://stackoverflow.com/questions/39606419/how-can-i-download-just-thumbnails-using-youtube-dl
    - https://stackoverflow.com/questions/33985863/hiding-console-output-produced-by-os-system
    - https://superuser.com/questions/1487962/youtube-dl-record-live-stream-segmentation
    - https://askubuntu.com/questions/178481/how-to-download-an-mp3-track-from-a-youtube-video

- threading: https://medium.com/@shridharatram555/parallel-execution-in-python-running-multiple-functions-concurrently-with-multi-threading-and-a-cf90454aaf63


- ffmpeg: https://phoenixnap.com/kb/install-ffmpeg-ubuntu

required packages:
- ffmpeg
- youtube-dl
- timg
- pytermtk