#!/usr/bin/env python3

# MIT License
#
# Copyright (c) 2021 Eugenio Parodi <ceccopierangiolieugenio AT googlemail DOT com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys, os, argparse
import re
import random

sys.path.append(os.path.join(sys.path[0],'../tmp'))
sys.path.append(os.path.join(sys.path[0],'..'))
import TermTk as ttk

from  showcase.graph         import demoGraph
from  showcase.windows       import demoWindows
from  showcase.textedit      import demoTextEdit


from comment import *
from login import loginProcedure, loginFullProcedure
from video import showVideo
from audio import showAudio
import socket

HOST = "127.0.0.1"
PORT = 8087

def demoShowcase(root=None, border=True, uname=None, sc=None):
    splitter = ttk.TTkSplitter()
    root.layout().addWidget(splitter, 0, 0)

    logInput = ttk.TTkKeyPressView(visible=False, maxHeight=3, minHeight=3)
    root.layout().addWidget(logInput, 1, 0)

    # domTree = ttk.TTkFrame(title="Tom Inspector", border=True, visible=False, layout=ttk.TTkGridLayout())
    # ttk.TTkTomInspector(parent=domTree)
    # root.layout().addWidget(domTree, 0, 1)

    leftFrame   = ttk.TTkFrame(parent=splitter, layout=ttk.TTkGridLayout(), border=False)

    themesFrame = ttk.TTkFrame(title="Theme", border=True, layout=ttk.TTkVBoxLayout(), maxHeight=5, minHeight=5)
    listMenu = ttk.TTkList(maxWidth=30, minWidth=10)
    logInputToggler = ttk.TTkCheckbox(text='ShowInput')
    logInputToggler.stateChanged.connect(lambda x: logInput.setVisible(x==ttk.TTkK.Checked))
    tomTreeToggler = ttk.TTkCheckbox(text='Tom View', enabled=False)
    # tomTreeToggler.stateChanged.connect(lambda x: domTree.setVisible(x==ttk.TTkK.Checked))
    mouseToggler = ttk.TTkCheckbox(text='Mouse 🐀', checked=True)
    mouseToggler.stateChanged.connect(lambda x: ttk.TTkTerm.push(ttk.TTkTerm.Mouse.ON if x==ttk.TTkK.Checked else ttk.TTkTerm.Mouse.OFF))
    quitButton = ttk.TTkButton(text="Quit", border=True, maxHeight=3)
    quitButton.clicked.connect(ttk.TTkHelper.quit)

    leftFrame.layout().addWidget(themesFrame,     0, 0)
    leftFrame.layout().addWidget(listMenu,        1, 0)
    leftFrame.layout().addWidget(mouseToggler,    2, 0)
    leftFrame.layout().addWidget(tomTreeToggler,  3, 0)
    leftFrame.layout().addWidget(logInputToggler, 4, 0)
    leftFrame.layout().addWidget(quitButton,      5, 0)

    mainFrame = ttk.TTkFrame(parent=splitter, layout=ttk.TTkGridLayout(), border=False)

    # Set the size of the left column (quite useless but required by my OCD)
    splitter.setSizes([15,root.width()-11])

    # Themes
    themesFrame.layout().addWidget(r1 := ttk.TTkRadioButton(text="ASCII", radiogroup="theme"))
    themesFrame.layout().addWidget(r2 := ttk.TTkRadioButton(text="UTF-8", radiogroup="theme", checked=True))
    themesFrame.layout().addWidget(r3 := ttk.TTkRadioButton(text="Nerd",  radiogroup="theme"))

    r1.clicked.connect( lambda : ttk.TTkTheme.loadTheme(ttk.TTkTheme.ASCII))
    r2.clicked.connect( lambda : ttk.TTkTheme.loadTheme(ttk.TTkTheme.UTF8 ))
    r3.clicked.connect( lambda : ttk.TTkTheme.loadTheme(ttk.TTkTheme.NERD ))

    # Menu
    listMenu.addItem(f"Logs")
    tabLogs = ttk.TTkTabWidget(parent=mainFrame, border=False, visible=False)
    tabLogs.addTab(ttk.TTkLogViewer(), " LogViewer ")
    
    listMenu.addItem(f"Widgets")
    tabWidgets = ttk.TTkTabWidget(parent=mainFrame, border=False, visible=False)
    tabWidgets.addTab(demoTextEdit(),    " Text Edit ", 'showcase/textedit.py')
    
    listMenu.addItem(f"Graphs")
    tabGraphs = ttk.TTkTabWidget(parent=mainFrame, border=False, visible=False)
    tabGraphs.addTab(demoGraph(),       " Graph Test ", 'showcase/graph.py')

    listMenu.addItem(f"License")
    tabWindows = ttk.TTkTabWidget(parent=mainFrame, border=False, visible=False)
    tabWindows.addTab(demoWindows(),     " Windows Test ",  'showcase/windows.py')

    listMenu.addItem(f"Comments")
    tabComment = ttk.TTkTabWidget(parent=mainFrame, border=False, visible=False)
    categorized_pages = ["sports", "politics", "finance", "technology", "science", "education", "religion", "art", "medicine"]
    for page in categorized_pages:
        tabComment.addTab(showComment(page, uname=uname, socket=sc), page.capitalize(), "include/comment.py")

    listMenu.addItem(f"Videos")
    tabVideos = ttk.TTkFrame(parent=mainFrame, border=False, visible=False)
    tabVideos.setLayout(ttk.TTkGridLayout())
    showVideo(root=tabVideos)

    listMenu.addItem(f"Audios")
    tabAudios = ttk.TTkFrame(parent=mainFrame, border=False, visible=False)
    tabAudios.setLayout(ttk.TTkGridLayout())
    showAudio(root=tabAudios)

    @ttk.pyTTkSlot(str)
    def _listCallback(label):
        widget = None
        if   label == "Logs":    widget = tabLogs
        elif label == "Widgets": widget = tabWidgets
        elif label == "Graphs":  widget = tabGraphs
        elif label == "License": widget = tabWindows
        elif label == "Comments": widget = tabComment
        elif label == "Videos": widget = tabVideos
        elif label == "Audios": widget = tabAudios

        if widget:
            if _listCallback.active:
                _listCallback.active.hide()
            widget.show()
            _listCallback.active = widget

    _listCallback.active = None
    listMenu.textClicked.connect(_listCallback)
    listMenu.setCurrentRow(0)

    return splitter

def main(uname, sc):
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', help='Full Screen (default)', action='store_true')
    parser.add_argument('-w', help='Windowed',    action='store_true')
    parser.add_argument('-t', help='Track Mouse', action='store_true')
    args = parser.parse_args()
    windowed = args.w
    mouseTrack = args.t

    root = ttk.TTk(title="Application", mouseTrack=mouseTrack)
    if windowed:
        winTab = ttk.TTkWindow(parent=root,pos=(0,0), size=(120,40), title="pyTermTk Showcase", border=True, layout=ttk.TTkGridLayout(), flags=ttk.TTkK.NONE)
        border = True
    else:
        root.setLayout(ttk.TTkGridLayout())
        winTab = root
        border = False

    demoShowcase(winTab, border, uname, sc)
    root.mainloop()

if __name__ == "__main__":
    # use socket to obtain tcp connection first
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    
    uname = loginFullProcedure(socket=s)
    main(uname, s)
