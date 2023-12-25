import sys, os
import TermTk as ttk
import pandas as pd
from httpm import *

class TopicPage:
    def __init__(self, root, PageTitle, title, category, content, comments, uname, tabname, id, socket):
        self.uname = uname
        self.tabname = tabname
        self.id = id
        self.socket = socket
        self.page = ttk.TTkFrame(parent=root, border=True, visible=False, title=PageTitle)
        self.page.setLayout(ttk.TTkGridLayout())

        # close button
        self.closeButton = ttk.TTkButton(parent=self.page, text="Close Window", border=True, maxHeight=1)
        self.closeButton.clicked.connect(self._closeButtonCallback)

        # content area
        self.contentArea = ttk.TTkScrollArea(parent=self.page)
        self._contentFrame = ttk.TTkFrame(pos=(0,0) , size=(50,50), parent=self.contentArea.viewport(), border=True)
        self._contentFrame.setLayout(ttk.TTkGridLayout())
        self._contentFrame.layout().addWidget(ttk.TTkLabel(pos=(0,0), text=TCCFormatter(title, category, content)), 0, 0)

        # comment area
        self.commentArea = ttk.TTkScrollArea(parent=self.page)
        self._commentFrame = ttk.TTkFrame(pos=(0,0) , size=(50,50), parent=self.commentArea.viewport(), border=True)
        self._commentText = []
        for comment in comments:
            tmp_ls = [f"\033[1m{k}\033[0m: {v}\n" for k, v in comment.items()]
            self._commentText.append(textChangeLine(tmp_ls[0], 47))
        self._commentFrame.setLayout(ttk.TTkGridLayout())
        self._commentFrameLabel = ttk.TTkLabel(pos=(0,0), text="\n".join(self._commentText))
        self._commentFrame.layout().addWidget(self._commentFrameLabel, 0, 0)
        
        # type comment area
        self.inputFrame = ttk.TTkFrame(parent=self.page, border=True, title=f"Write down your Comments")
        self.inputFrame.setLayout(ttk.TTkGridLayout())
        self._inputText = ttk.TTkLineEdit(parent=self.inputFrame, text="")
        self._inputButton = ttk.TTkButton(parent=self.inputFrame, text="Enter")
        self.inputFrame.layout().addWidget(self._inputText, 0, 0, 1, 3)
        self.inputFrame.layout().addWidget(self._inputButton, 0, 3)
        self._inputButton.clicked.connect(self._sendButtonCallback)

        self.page.layout().addWidget(self.closeButton, 0, 0)
        self.page.layout().addWidget(self.contentArea, 1, 0, 2)
        self.page.layout().addWidget(self.commentArea, 3, 0)
        self.page.layout().addWidget(self.inputFrame, 4, 0)
        
    @ttk.pyTTkSlot()
    def _sendButtonCallback(self):
        # show the entered comment
        self._commentText.append(textChangeLine(f"\033[1m{self.uname}\033[0m: {str(self._inputText._text)}\n", 47))
        self._commentFrameLabel.setText("\n".join(self._commentText))

        # send the comment to the server
        recv = None
        while True:
            msg_body = enterCommentRequestMessage(self.uname, str(self._inputText._text), self.tabname, self.id)
            msg = httpmRequest("POST", self.socket.getsockname()[0], self.socket.getpeername()[1], enterComment=msg_body)
            self.socket.sendall(str(msg).encode())
            recv = httpmResponse(self.socket.recv(8192).decode())
            if recv.success():
                break

    @ttk.pyTTkSlot()
    def _closeButtonCallback(self):
        self.page.hide()

def textChangeLine(text:str, width:int):
    tokens = text.split(" ")
    lines = [""]
    acc_len = 0
    for word in tokens:
        acc_len += len(word)+1
        if acc_len > width: 
            lines.append("")
            acc_len = len(word)+1
        lines[-1] += word + " "
    return "\n".join(lines)

def TCCFormatter(title, category, content): # title category content formatter
    return f"\033[1m{title}\033[0m\n{category}\n\n{content}"

def requestWholeTabProcedure(tabname, socket) -> str:
    recv = None
    while True:
        msg_body = wholeTabArticleRequestMessage(tabname)
        msg = httpmRequest("GET", socket.getsockname()[0], socket.getpeername()[1], wholeTabArticles=msg_body)
        socket.sendall(str(msg).encode())
        recv = httpmResponse(socket.recv(8192).decode())
        if recv.success():
            break
    return recv.body
    

def showComment(tabname, root=None, uname="User", socket=None):
    frame = ttk.TTkFrame(parent=root, border=True, title="List of Topics")
    frame.setLayout(ttk.TTkHBoxLayout())
    
    topicWidgets = {}
    listTopics = ttk.TTkList(parent=frame, maxWidth=100, minWidth=0)
    
    # request the content from the server
    jsonstr = requestWholeTabProcedure(tabname, socket)
    context_df = pd.read_json(jsonstr)
    for i in range(len(context_df)):
        context_df["title"][i] = textChangeLine(context_df["title"][i], 47)
        context_df["content"][i] = textChangeLine(context_df["content"][i], 47)
    
    for i in range(len(context_df)):
        topic = f"{i+1}." + context_df["title"][i][:40] + " ..."
        listTopics.addItem(topic)
        topic_page = TopicPage(frame, topic, 
            title=context_df["title"][i], 
            category=context_df["category"][i], 
            content=context_df["content"][i], 
            comments=context_df["comments"][i],
            uname=uname,
            tabname = tabname,
            id=i+1,
            socket=socket
        )
        topicWidgets[topic] = topic_page


    @ttk.pyTTkSlot(str)
    def _listCallback(label):
        widget = topicWidgets[str(label)].page
       
        if widget:
            if _listCallback.active:
                _listCallback.active.hide()
            widget.show()
            _listCallback.active = widget

    _listCallback.active = None
    listTopics.textClicked.connect(_listCallback)
    listTopics.setCurrentRow(len(topicWidgets))

    return frame