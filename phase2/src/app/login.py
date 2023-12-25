import sys, os
import TermTk as ttk
from httpm import *
from time import sleep

signUpRequired = False

def signUpProcedure(root=None, socket=None):
    signUpWin = ttk.TTkFrame(parent=root , pos=(1, 1), size=(80, 17))
    signUpWin.setLayout(ttk.TTkGridLayout())

    unameFrame = ttk.TTkFrame(parent=signUpWin, border=True)
    unameFrame.setLayout(ttk.TTkVBoxLayout())
    ttk.TTkLabel(parent=unameFrame, text="User Name:")
    unameInput = ttk.TTkLineEdit(parent=unameFrame, text="")

    pwdFrame = ttk.TTkFrame(parent=signUpWin, border=True)
    pwdFrame.setLayout(ttk.TTkVBoxLayout())
    ttk.TTkLabel(parent=pwdFrame, text="Password:")
    pwdInput = ttk.TTkLineEdit(parent=pwdFrame, text="")

    emailFrame = ttk.TTkFrame(parent=signUpWin, border=True)
    emailFrame.setLayout(ttk.TTkVBoxLayout())
    ttk.TTkLabel(parent = emailFrame, text="Email:")
    emailInput = ttk.TTkLineEdit(parent=emailFrame, text="")

    warningFrame = ttk.TTkFrame(parent=signUpWin, border=False)
    warningText = ttk.TTkLabel(parent=warningFrame, text="")

    @ttk.pyTTkSlot()
    def signUpButton_callback():
        # first check if the inputs are vliad
        ui_, pi_, ei_ = str(unameInput._text), str(pwdInput._text), str(emailInput._text)
        if " " in ui_ or ":" in ui_ \
            or " " in pi_ or ":" in pi_ \
            or " " in ei_ or ":" in ei_:
            warningText.setText("\nDon't include space or colon")
        else:
            # send the request and see if it works, 
            msg_body = signUpRequestMessage(unameInput._text, pwdInput._text, emailInput._text)
            msg = httpmRequest("POST", socket.getsockname()[0], socket.getpeername()[1], signUp=msg_body)
            socket.sendall(str(msg).encode())
            recv = httpmResponse(socket.recv(8192).decode())
            # if no, then maybe there's username already exists
            if not recv.success():
                warningText.setText("\nUsername already exists, try again")
            else:
                ttk.TTkHelper.quit()
        
    signUpButton = ttk.TTkButton(text="Sign Up", border=True, maxHeight=3)
    signUpButton.clicked.connect(signUpButton_callback)

    signUpWin.layout().addWidget(unameFrame, 0, 0, 1, 10)
    signUpWin.layout().addWidget(pwdFrame, 1, 0, 1, 10)
    signUpWin.layout().addWidget(emailFrame, 2, 0, 1, 10)
    signUpWin.layout().addWidget(signUpButton, 3, 0, 1, 1)
    signUpWin.layout().addWidget(warningFrame, 3, 1, 1, 9)

    return signUpWin, unameInput, pwdInput

def loginProcedure(root=None, socket=None):
    loginWin = ttk.TTkFrame(parent=root , pos=(1, 1), size=(80, 13))
    loginWin.setLayout(ttk.TTkGridLayout())

    unameFrame = ttk.TTkFrame(parent=loginWin, border=True)
    unameFrame.setLayout(ttk.TTkVBoxLayout())
    ttk.TTkLabel(parent=unameFrame, text="User Name:")
    unameInput = ttk.TTkLineEdit(parent=unameFrame, text="")

    pwdFrame = ttk.TTkFrame(parent=loginWin, border=True)
    pwdFrame.setLayout(ttk.TTkVBoxLayout())
    ttk.TTkLabel(parent=pwdFrame, text="Password:")
    pwdInput = ttk.TTkLineEdit(parent=pwdFrame, text="")

    warningFrame = ttk.TTkFrame(parent=loginWin, border=False)
    warningText = ttk.TTkLabel(parent=warningFrame, text="")

    @ttk.pyTTkSlot()
    def loginButton_callback():
        msg_body = loginRequestMessage(unameInput._text, pwdInput._text)
        msg = httpmRequest("POST", socket.getsockname()[0], socket.getpeername()[1], login=msg_body)
        socket.sendall(str(msg).encode())
        recv = httpmResponse(socket.recv(8192).decode())
        if not recv.success():
            warningText.setText("\nIncorrect name or password / Maybe Sign-up?")
        else:
            ttk.TTkHelper.quit()

    loginButton = ttk.TTkButton(text="Login", border=True, maxHeight=3)
    loginButton.clicked.connect(loginButton_callback)

    @ttk.pyTTkSlot()
    def signUpButton_callback():
        global signUpRequired 
        signUpRequired = True
        ttk.TTkHelper.quit()
        
    signUpButton = ttk.TTkButton(text="Sign Up", border=True, maxHeight=3)
    signUpButton.clicked.connect(signUpButton_callback)

    loginWin.layout().addWidget(unameFrame, 0, 0, 1, 10)
    loginWin.layout().addWidget(pwdFrame, 1, 0, 1, 10)
    loginWin.layout().addWidget(loginButton, 2, 0, 1, 1)
    loginWin.layout().addWidget(signUpButton, 2, 9, 1, 1)
    loginWin.layout().addWidget(warningFrame, 2, 1, 1, 8)

    return loginWin, unameInput, pwdInput

def loginFullProcedure(socket=None):
    root = ttk.TTk(title="login")
    _, unameInput, pwdInput = loginProcedure(root, socket)
    root.mainloop()

    if signUpRequired:
        sleep(1)
        root = ttk.TTk(title="sign up")
        _, unameInput, pwdInput = signUpProcedure(root, socket)
        root.mainloop()

    return unameInput._text

def main():
    loginFullProcedure()

if __name__ == "__main__":
    main()