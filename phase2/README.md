# Computer Network Project Phase 2
written by Chih-Han Yang B10902069

## Introduction
This is phase 2 of the Computer Network Final Project introducing a comprehensive system comprising front-end and back-end programs. This setup enables functionalities like article browsing, commenting, video streaming, and audio playback.

The back-end is designed to manage user information (including cookies), handle article and comment data, and facilitate login and sign-up processes. Additionally, it utilizes the nlohmann/json C++ library for efficient JSON manipulation.

The front-end operates as a Text User Interface (TUI), delivering the aforementioned features seamlessly. While the back-end is predominantly coded in C++, the front-end relies heavily on Python, leveraging the advantages of object-oriented programming in both realms.

To support video streaming and audio playback, the implementation incorporates various packages such as youtube-dl, timg, and playsound. The compilation process employs CMake to handle the complexities of the program's structure.

## Direcotry Structure
```
phase2
├── CMakeCache.txt
├── CMakeFiles
├── cmake_install.cmake
├── CMakeLists.txt
├── database
│   ├── data
│   │   ├── art.json
│   │   ├── education.json
│   │   ├── finance.json
│   │   ├── medicine.json
│   │   ├── politics.json
│   │   ├── religion.json
│   │   ├── science.json
│   │   ├── sports.json
│   │   └── technology.json
│   ├── media
│   │   ├── oh_my_gah.mp3
│   │   └── oh_my_gah.mp4
│   └── metadata
│       └── user.json
├── demo.webm
├── download.sh
├── include
│   ├── httpm.hpp
│   ├── jsonm.hpp
│   ├── nlohmann
│   │   └── json.hpp
│   └── server.hpp
├── libphase2.so
├── Makefile
├── oh_my_gah.jpg
├── README.md
├── run.sh
├── server
└── src
    ├── app
    │   ├── audio.py
    │   ├── comment.py
    │   ├── demo.py
    │   ├── httpm.py
    │   ├── login.py
    │   ├── playVideo.py
    │   ├── showcase
    │   │   ├── graph.py
    │   │   ├── _showcasehelper.py
    │   │   ├── textedit.ANSI.txt
    │   │   ├── textedit.py
    │   │   └── windows.py
    │   └── video.py
    └── server
        ├── httpm.cpp
        └── server.cpp
```
1. ```CmakeLists.txt```: For cmake to create a specified makefile.
2. ```database```: Contains all the datas including the articles, the comments, and user information.
3. ```demo.webm```: The demonstration video.
4. ```download.sh```: Download all the required packges or modules.
5. ```include```: The include header files for server program.
6. ```Makefile```: Build the executable.
7. ```oh_my_gah.jpg```: A picture of Osaka saying oh my gah.
8. ```README.md```: Specifies all the details and provides execution steps.
9. ```run.sh```: Run the client program.
10. ```server```: The server executable

## Environment Requirement
1. CMake: For complex dependency management and compilation.
2. pyTermTk: For text user interface.
3. nlohmann/json library: For json manipulation in C++. This should be contained in the downloaded package.
4. playsound: For playing the mp3 file.
5. timg: For playing video in terminal.
6. youtube-dl: For downloading a youtube video in streaming.
7. ffmpeg: For youtube-dl to convert any type of audio to mp3 format.

## Execution
Note that all the following commands are executed in the root direcotry of the package in a Linux environment.
### Prerequisites
```
chmod u+x download.sh
chmod u+x run.sh
./download.sh
```
ps: download.sh would download all the packages and modules needed, excluding cmake. CMake requires a more complex installation. But as soon as you have the executable ```server``` in the downloaded package, you don't have to worry about that.

### Compile Server
```
cmake .
make
```
ps: Please make sure you have cmake to execute.

### Execute Server
```
./server <port number>
```
ps: Please specify the port number for the server.

### Execute Client
Open another terminal and execute:
```
./run.sh <host ip> <port number>
```
ps: If server is running on a local ip, it would be: 127.0.0.1. However, you should figure out the IP address of the machine that runs the ```server``` executable. Also, please make sure that the port number here is the same as the server's.

## Useful Links
These are the links of tools used and for debugging.
1. pyTermTk:
    - [Official Document](https://ceccopierangiolieugenio.github.io/pyTermTk/index.html)
    - [Github Repo](https://github.com/ceccopierangiolieugenio/pyTermTk)
2. About the server:
    - [Check Directory in C++](https://www.geeksforgeeks.org/cpp-program-to-get-the-list-of-files-in-a-directory/)
3. C++ Json Library:
    - [nlohmann/json Github Repo](https://github.com/nlohmann/json)
    - [nlohmann/json Releases](https://json.nlohmann.me/home/releases/)
        - ps. just download the json.hpp from the newest version
4. CMake:
    - [Official Document](https://cmake.org/cmake/help/latest/index.html)
5. playsound:
    - [Tutorial](https://www.geeksforgeeks.org/play-sound-in-python/)
6. timg:
    - [Github Repo](https://github.com/hzeller/timg)
7. youtube-dl:
    - [Github Repo](https://github.com/ytdl-org/youtube-dl)
    - [Debug: Fix Error Message about incorrect syntax](https://askubuntu.com/questions/1457872/how-do-i-fix-youtube-dl-running-it-gives-error-message-about-incorrect-syntax)
    - [Download mp3 only](https://askubuntu.com/questions/423508/can-i-directly-download-audio-using-youtube-dl)
    - [Download mp3 track](https://askubuntu.com/questions/178481/how-to-download-an-mp3-track-from-a-youtube-video)
    - [Download Thumbnails](https://stackoverflow.com/questions/39606419/how-can-i-download-just-thumbnails-using-youtube-dl)
8. Threading
    - [Concurrent Execution](https://medium.com/@shridharatram555/parallel-execution-in-python-running-multiple-functions-concurrently-with-multi-threading-and-a-cf90454aaf63)
9. ffmpeg
    - [Package Description](https://phoenixnap.com/kb/install-ffmpeg-ubuntu)
10. Other Links
    - [Wathc Video in Terminal](https://askubuntu.com/questions/1258409/can-one-watch-video-in-terminal-command-line)
    - [gnome termianl](https://stackoverflow.com/questions/19308415/execute-terminal-command-from-python-in-new-terminal-window)
    - [Hiding console output](https://stackoverflow.com/questions/33985863/hiding-console-output-produced-by-os-system)

## Source Code Hints
1. Self-implemented protocol HTTP/M (Http myself). Please refer to ```httpm.hpp```, ```httpm.cpp```, ```httpm.py```.
2. Multiple implemented HTTP/M functionalities. Please refer to ```httpm.hpp```, ```httpm.cpp```, ```httpm.py``` and ```server.cpp```.
3. The whole front end is implemented by ```pyTermTk``` - a TUI (text user interface) package. Please refer to ```demo.py```, ```login.py```, ```comment.py```, ```video.py``` and ```audio.py```.
4. The contents of the articles and comments, and the user informations (cookie), is saved in json file format. ```nlohmann/json``` JSON library is used for handling in C++.
5. Video Streaming uses ```youtube-dl```, ```timg``` and ```playsound``` packages. It takes a youtube url and play the video in terminal. Note that the audio prefetch entirely first, then start streaming the video. No pause feature is added. Please refer to ```video.py``` and ```playVideo.py```.
6. Audio uses ```youtube-dl``` and ```playsound``` packages. It takes a youtube url and play it in background. No pause feature is added. Please refer to ```audio.py```.