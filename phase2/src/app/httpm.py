# This is the file that defines the httpm (my http)
# there would be request and response messages

HTTPM_STATUS_OK = 200
HTTPM_STATUS_BAD_REQUEST = 400

HTTPM_REQUEST_LOGIN = "Http Request Login"
HTTPM_REQUEST_SIGNUP = "Http Request Sign-up"
HTTPM_REQUEST_ENTER_COMMENT = "Http Request Enter comment"
HTTPM_REQUEST_COMPOSE_ARTICLE = "Http Request Compose article"
HTTPM_REQUEST_WHOLE_TAB_ARTICLE = "Http Request Whole tab of article"
HTTPM_REQUEST_DEFAULT = "Http Request Default"

# this is the login request message
class loginRequestMessage:
    def __init__(self, uname, password) -> None:
        self.uname = uname
        self.password = password
    def __str__(self) -> str:
        return f"{self.uname}:{self.password}"

# this is the signUp request message
class signUpRequestMessage:
    def __init__(self, uname, password, email) -> None:
        self.uname = uname
        self.password = password
        self.email = email
    def __str__(self) -> str:
        return f"{self.uname}:{self.password}:{self.email}"

# this is the enter comment request message
class enterCommentRequestMessage:
    def __init__(self, uname, comment, tabname, id) -> None:
        self.uname = uname
        self.comment = comment
        self.tabname = tabname
        self.id = id
    def __str__(self) -> str:
        return f"{self.tabname}/;;/{self.id}/;;/{self.uname}/;;/{self.comment}"

# this is the compose article request message
class composeArticleRequestMessage:
    def __init__(self) -> None:
        pass
    def __str__(self) -> str:
        pass

# this is the get whole tab article request message
class wholeTabArticleRequestMessage:
    def __init__(self, tab_name) -> None:
        self.tab_name = tab_name
    def __str__(self) -> str:
        return f"{self.tab_name}"

class httpmRequest:
    METHOD = {"post": "POST", "get": "GET"}

    def __init__(self, method, srcHost, destHost, **kwargs) -> None:
        if method != httpmRequest.METHOD["post"] and method != httpmRequest.METHOD["get"]:
            raise ValueError("The method should be either POST or GET")
        self.method = method
        self.url = destHost
        self.version = "HTTP/M"
        self.host = srcHost

        # if the method is POST, then it has the following functionality
        # login, sign up, enter comment, compose an article
        func = ""
        if method == httpmRequest.METHOD["post"]:
            if "login" in kwargs: 
                # login message, the body would be a string converted from the loginRequestMessage
                func = HTTPM_REQUEST_LOGIN
                self.body = str(kwargs["login"])
            elif "signUp" in kwargs:
                # sign up message, this body would be a string converted from the signUpRequestMessage
                func = HTTPM_REQUEST_SIGNUP
                self.body = str(kwargs["signUp"])
            elif "enterComment" in kwargs:
                # enter-comment message, this body would be a string converted from the enterCommentRequestMessage
                func = HTTPM_REQUEST_ENTER_COMMENT
                self.body = str(kwargs["enterComment"])
            elif "composeArticle" in kwargs:
                # compose-article message, this body would be a string converted from the composeArticleRequestMessage
                func = HTTPM_REQUEST_COMPOSE_ARTICLE
                self.body = str(kwargs["composeArticle"])
            else:
                func = HTTPM_REQUEST_DEFAULT
                self.body = ""
        # if the method is GET, then it has the following functionality
        # get a whole tab of articles, 
        elif method == httpmRequest.METHOD["get"]:
            if "wholeTabArticles" in kwargs:
                # get-a-whole-tab-of-articles message, this body would be a string converted from the wholeTabArticleRequestMessage
                func = HTTPM_REQUEST_WHOLE_TAB_ARTICLE
                self.body = str(kwargs["wholeTabArticles"])
            else:
                func = HTTPM_REQUEST_DEFAULT
                self.body = ""

        self.httpm_request_string = f"{self.method} {self.url} {self.version}\r\nHost: {self.host}\r\nFunctionality: {func}\r\n\r\n{self.body}\r\n\r\n"
    def __str__(self) -> str:
        return self.httpm_request_string

class httpmResponse:
    # the constructor would take a string of response and parse it into a response
    def __init__(self, response):
        response_msg_segments = response.split("\r\n\r\n")
        response_header_segments = response_msg_segments[0].split("\r\n")
        self.status_code = int(response_header_segments[0])
        self.message = response_header_segments[1]
        self.body = response_msg_segments[1]

    def success(self):
        return self.status_code == HTTPM_STATUS_OK