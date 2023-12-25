#include "httpm.hpp"

httpmRequest::httpmRequest(std::string request_message){
    int st = 0, len = 0;
    std::string buf;
    while(request_message[len++] != ' ');
    buf = request_message.substr(st, len-1);
    if(buf.compare("POST") == 0)
        method = HTTPM_METHOD_POST;
    else if(buf.compare("GET") == 0)
        method = HTTPM_METHOD_GET;
    else
        method = (httpm_method_t) -1;
    st = len;

    while(request_message[len++] != ' ');
    url = request_message.substr(st, len-1-st);
    st = len;

    while(request_message[len++] != '\r');
    version = request_message.substr(st, len-1-st);
    assert(request_message[len++] == '\n');
    len += strlen("Host: ");
    st = len;

    while(request_message[len++] != '\r');
    host = request_message.substr(st, len-1-st);
    assert(request_message[len++] == '\n');
    len += strlen("Functionality: ");
    st = len;

    while(request_message[len++] != '\r');
    buf = request_message.substr(st, len-1-st);
    if(method == HTTPM_METHOD_POST){
        if(buf.compare(HTTPM_REQUEST_LOGIN_S) == 0)
            func = HTTPM_REQUEST_LOGIN;
        else if(buf.compare(HTTPM_REQUEST_SIGNUP_S) == 0)
            func = HTTPM_REQUEST_SIGNUP;
        else if(buf.compare(HTTPM_REQUEST_ENTER_COMMENT_S) == 0)
            func = HTTPM_REQUEST_ENTER_COMMENT;
        else if(buf.compare(HTTPM_REQUEST_COMPOSE_ARTICLE_S) == 0)
            func = HTTPM_REQUEST_COMPOSE_ARTICLE;
        else if(buf.compare(HTTPM_REQUEST_DEFAULT_S) == 0)
            func = HTTPM_REQUEST_DEFAULT;
        else
            func = (httpm_functionality_t) -1;
    }else if(method == HTTPM_METHOD_GET){
        if(buf.compare(HTTPM_REQUEST_WHOLE_TAB_ARTICLE_S) == 0)
            func = HTTPM_REQUEST_WHOLE_TAB_ARTICLE;
        else
            func = (httpm_functionality_t) -1;
    }else{
        func = (httpm_functionality_t) -1;
    }
    assert(request_message[len++] == '\n');
    assert(request_message[len++] == '\r');
    assert(request_message[len++] == '\n');
    st = len;

    while(request_message[len++] != '\r');
    body = request_message.substr(st, len-1-st);
    assert(request_message[len++] == '\n');
    assert(request_message[len++] == '\r');
    assert(request_message[len++] == '\n');
    // assert(request_message[len] == '\0');
}

httpmResponse::httpmResponse(httpm_status_t status_code, httpm_method_t client_method, httpm_functionality_t func, std::string body=""){
    std::stringstream ss;
    const char *msg;
    ss << status_code << "\r\n" << "Message:";
    if(client_method == HTTPM_METHOD_POST){
        switch (func){
            case HTTPM_REQUEST_LOGIN:
                msg = (status_code == HTTPM_STATUS_OK)? HTTPM_REQUEST_LOGIN_OK:HTTPM_REQUEST_LOGIN_ERROR;
                break;
            case HTTPM_REQUEST_SIGNUP:
                msg = (status_code == HTTPM_STATUS_OK)? HTTPM_REQUEST_SIGNUP_OK:HTTPM_REQUEST_SIGNUP_ERROR;
                break;
            case HTTPM_REQUEST_ENTER_COMMENT:
                msg = (status_code == HTTPM_STATUS_OK)? HTTPM_REQUEST_ENTER_COMMENT_OK:HTTPM_REQUEST_ENTER_COMMENT_ERROR;
                break;
            case HTTPM_REQUEST_COMPOSE_ARTICLE:
                msg = (status_code == HTTPM_STATUS_OK)? HTTPM_REQUEST_COMPOSE_ARTICLE_OK:HTTPM_REQUEST_COMPOSE_ARTICLE_ERROR;
                break;
            default:
                std::cerr << "undefined functionality for POST method\n";
                msg = (status_code == HTTPM_STATUS_OK)? HTTPM_REQUEST_DEFAULT_OK:HTTPM_REQUEST_DEFAULT_ERROR;
                break;
        }
        ss << msg << "\r\n\r\n";
    }else if(client_method == HTTPM_METHOD_GET){
        switch (func){
            case HTTPM_REQUEST_WHOLE_TAB_ARTICLE:
                msg = (status_code == HTTPM_STATUS_OK)? HTTPM_REQUEST_WHOLE_TAB_ARTICLE_OK:HTTPM_REQUEST_WHOLE_TAB_ARTICLE_ERROR;
                ss << msg << "\r\n\r\n";
                ss << body << "\r\n\r\n";
                break;            
            default:
                std::cerr << "undefined functionality for GET method\n";
                msg = (status_code == HTTPM_STATUS_OK)? HTTPM_REQUEST_DEFAULT_OK:HTTPM_REQUEST_DEFAULT_ERROR;
                ss << msg << "\r\n\r\n";
                break;
        }
    }else{
        std::cerr << "Error Http Method Type\n";
    }

    this->httpm_response_string = ss.str();
}

const char* httpmResponse::response_cstring(){
    return this->httpm_response_string.c_str();
}

size_t httpmResponse::response_string_len(){
    return this->httpm_response_string.length();
}