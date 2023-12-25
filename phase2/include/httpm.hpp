// This is the file that defines the httpm (my http)
// there would be request and response messages
// the response class here corresponds with the request class in httpm.py
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include <cstring>
#define HTTPM_METHOD_POST 0
#define HTTPM_METHOD_GET 1
#define httpm_method_t int
#define httpm_functionality_t int

#define HTTPM_STATUS_OK 200
#define HTTPM_STATUS_BAD_REQUEST 400
#define httpm_status_t int

enum httpm_request_post_functionality{
    HTTPM_REQUEST_LOGIN,
    HTTPM_REQUEST_SIGNUP,
    HTTPM_REQUEST_ENTER_COMMENT,
    HTTPM_REQUEST_COMPOSE_ARTICLE,
    HTTPM_REQUEST_DEFAULT,
};

enum httpm_request_get_functionality{
    HTTPM_REQUEST_WHOLE_TAB_ARTICLE,
};

#define HTTPM_REQUEST_LOGIN_S "Http Request Login"
#define HTTPM_REQUEST_SIGNUP_S "Http Request Sign-up"
#define HTTPM_REQUEST_ENTER_COMMENT_S "Http Request Enter comment"
#define HTTPM_REQUEST_COMPOSE_ARTICLE_S "Http Request Compose article"
#define HTTPM_REQUEST_WHOLE_TAB_ARTICLE_S "Http Request Whole tab of article"
#define HTTPM_REQUEST_DEFAULT_S "Http Request Default"

#define HTTPM_REQUEST_LOGIN_OK "Login Successful"
#define HTTPM_REQUEST_LOGIN_ERROR "Login Error: user not found"
#define HTTPM_REQUEST_SIGNUP_OK "Sign-up Successful"
#define HTTPM_REQUEST_SIGNUP_ERROR "Sign-up Error: user name already exists"
#define HTTPM_REQUEST_ENTER_COMMENT_OK "Enter comment Successful"
#define HTTPM_REQUEST_ENTER_COMMENT_ERROR "Enter comment Error: enter comment error, please resend comment"
#define HTTPM_REQUEST_COMPOSE_ARTICLE_OK "Compose article Successful"
#define HTTPM_REQUEST_COMPOSE_ARTICLE_ERROR "Compose article Error: compose article error, please resend article"
#define HTTPM_REQUEST_WHOLE_TAB_ARTICLE_OK "Get whole tab of article Successful"
#define HTTPM_REQUEST_WHOLE_TAB_ARTICLE_ERROR "Get whole tab of article Error"
#define HTTPM_REQUEST_DEFAULT_OK "Default Response OK"
#define HTTPM_REQUEST_DEFAULT_ERROR "Default Response Error"

#define HTTPM_REQUEST_LOGIN_DELIMITER ":"
#define HTTPM_REQUEST_SIGNUP_DELIMITER ":"
#define HTTPM_REQUEST_ENTER_COMMENT_DELIMITER "/;;/"

class httpmRequest{
    public:
        httpm_method_t method;
        std::string url;
        std::string version;
        std::string host;
        httpm_functionality_t func;
        std::string body;
        httpmRequest(std::string request_message);
};

class httpmResponse{
    public:
        httpm_status_t status_code;
        std::string httpm_response_string;
        httpmResponse(httpm_status_t status_code, httpm_method_t client_method, httpm_functionality_t func, std::string body);
        const char* response_cstring();
        size_t response_string_len();
};

