#include "server.hpp"
#include "httpm.hpp"
#include "jsonm.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

json all_user_info;
std::ifstream all_user_info_f;

Server::Server(){
    gethostname(this->server_host_name, sizeof(this->server_host_name));
    this->port = PORT_NUMBER;

    // initialize the file descriptor for socket tcp connection
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->fd < 0) ERR_MSG("socket"); 

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(this->port);

    int tmp = 1;
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) 
        ERR_MSG("setsockopt");
    if (bind(this->fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
        ERR_MSG("bind");
    if (listen(this->fd, 1024) < 0) 
        ERR_MSG("listen");

    fprintf(stderr, "\nServer starting on host: %.80s; port: %d; fd: %d...\n", this->server_host_name, this->port, this->fd);
}

int Server::accept_client(Client &cli){
    cli.fd = accept(this->fd, (struct sockaddr*)&cli.cliaddr, &cli.clilen);
    // https://jasonblog.github.io/note/linux_network/ntohs,_ntohl,_htons,htonlde_bi_jiao.html
    fprintf(stderr, "Server recieved a new client request,  client IP: %.80s; port: %d; fd: %d...\n", inet_ntoa((cli.cliaddr.sin_addr)), cli.cliaddr.sin_port, cli.fd);
    return cli.fd;
}

void Server::add_child_pid(pid_t pid){
    this->child_pid.insert(pid);
    fprintf(stderr, "New child process: %d\n", pid);
}

void Server::check_child_termination(){
    int status;
    pid_t pid;
    if(waitpid(0, &status, WNOHANG) > 0){
        child_pid.erase(pid);
    }
}

namespace utils{
// in the protocl for login packets, uname:password is the format
std::pair<std::string, std::string> parse_uname_password(std::string &content){
    size_t dli = content.find(HTTPM_REQUEST_LOGIN_DELIMITER);
    if(dli == std::string::npos){
        std::cerr << "Login Message Input Format Error\n";
        return std::pair<std::string, std::string>("", "");
    }

    std::string uname = content.substr(0, dli);
    std::string password = content.substr(dli+1, content.length()-dli-1);

    if(uname.length() == 0 || password.length() == 0){
        std::cerr << "Login Message Uname or Password Empty Error\n";
        return std::pair<std::string, std::string>("", "");
    }

    return std::pair<std::string, std::string>(uname, password);
}

bool uname_password_exists(std::string &uname, std::string &password){
    for(json::iterator it = all_user_info.begin(); it != all_user_info.end(); it++){
        if(uname.compare((*it)["username"]) == 0 && password.compare((*it)["password"]) == 0)
            return true;
    }
    return false;
}

std::tuple<std::string, std::string, std::string> parse_uname_password_email(std::string &content){
    size_t dli1 = content.find(HTTPM_REQUEST_SIGNUP_DELIMITER);
    if(dli1 == std::string::npos){
        std::cerr << "SignUp Message Input Format Error\n";
        return std::tuple<std::string, std::string, std::string>("", "", "");
    }

    std::string uname = content.substr(0, dli1);
    size_t dli2 = content.find(HTTPM_REQUEST_SIGNUP_DELIMITER, dli1+1);
    if(dli2 == std::string::npos){
        std::cerr << "SignUp Message Input Format Error\n";
        return std::tuple<std::string, std::string, std::string>("", "", "");
    }

    std::string password = content.substr(dli1+1, dli2-dli1-1);
    std::string email = content.substr(dli2+1, content.length()-dli2-1);

    if(uname.length() == 0 || password.length() == 0 || email.length() == 0){
        std::cerr << "SignUp Message Uname, Password or Email Empty Error\n";
        return std::tuple<std::string, std::string, std::string>("", "", "");
    }

    return std::tuple<std::string, std::string, std::string>(uname, password, email);
}

bool uname_exists(std::string &uname){
    for(json::iterator it = all_user_info.begin(); it != all_user_info.end(); it++){
        if(uname.compare((*it)["username"]) == 0)
            return true;
    }
    return false;
}

bool tab_exists(std::string &tab){
    // reference: https://www.geeksforgeeks.org/cpp-program-to-get-the-list-of-files-in-a-directory/
    // base directory path
    std::string dir_path = DATA_DIR_PATH;
    for(const auto& entry : std::filesystem::directory_iterator(dir_path)) {
        // get the path
        std::filesystem::path jsonfile = entry.path();
        std::string jsonfile_str = jsonfile.string();
        if(jsonfile_str.compare(tab) == 0){
            return true;
        }
    }
    return false;
}

std::tuple<std::string, int, std::string, std::string> parse_tab_id_uname_comment(std::string &content){
    size_t dli1 = content.find(HTTPM_REQUEST_ENTER_COMMENT_DELIMITER);
    if(dli1 == std::string::npos){
        std::cerr << "Enter Comment Message Input Format Error\n";
        return std::tuple<std::string, int, std::string, std::string>("", -1, "", "");
    }

    std::string tabname = content.substr(0, dli1);
    size_t dli2 = content.find(HTTPM_REQUEST_ENTER_COMMENT_DELIMITER, dli1+4);
    if(dli2 == std::string::npos){
        std::cerr << "Enter Comment Message Input Format Error\n";
        return std::tuple<std::string, int, std::string, std::string>("", -1, "", "");
    }

    std::string id_s = content.substr(dli1+4, dli2-dli1-4);
    int id = std::stoi(id_s);

    size_t dli3 = content.find(HTTPM_REQUEST_ENTER_COMMENT_DELIMITER, dli2+4);
    if(dli3 == std::string::npos){
        std::cerr << "Enter Comment Message Input Format Error\n";
        return std::tuple<std::string, int, std::string, std::string>("", -1, "", "");
    }

    std::string uname = content.substr(dli2+4, dli3-dli2-4);
    std::string comment = content.substr(dli3+4, content.length()-dli3-4);

    if(tabname.length() == 0 || id == -1 || uname.length() == 0 || comment.length() == 0){
        std::cerr << "Enter Comment Message Input Format Error\n";
        return std::tuple<std::string, int, std::string, std::string>("", -1, "", "");
    }

    return std::tuple<std::string, int, std::string, std::string>(tabname, id, uname, comment);
}

// check for a comment, does the tab, id, and user name exists
bool tab_uname_exists(std::string &tab, std::string &uname){
    if(!tab_exists(tab)) return false;
    if(!uname_exists(uname)) return false;
    return true;
}

}

void Client::http_communication_procedure(){
    char *buf = (char*)malloc(8192*sizeof(char));

    // Login Stage / signup
    while(1){
        read(this->fd, buf, 8192);
        fprintf(stderr, "==========Incoming Http Request==========\n%s\n=========================================\n", buf);
        int httpm_status_num;
        httpmRequest req = httpmRequest(std::string(buf));
        if(req.method != HTTPM_METHOD_POST)
            std::cerr << "Unexpected Httpm Method Type in Login/SignUp Stage\n";

        if(req.func == HTTPM_REQUEST_LOGIN){
            std::pair<std::string, std::string> uname_pair = utils::parse_uname_password(req.body);
            httpm_status_num = (utils::uname_password_exists(uname_pair.first, uname_pair.second))? HTTPM_STATUS_OK:HTTPM_STATUS_BAD_REQUEST;
        }else if(req.func == HTTPM_REQUEST_SIGNUP){
            std::tuple<std::string, std::string, std::string> user_tuple = utils::parse_uname_password_email(req.body);
            // if username already exists, it's a bad request
            httpm_status_num = (utils::uname_exists(std::get<0>(user_tuple)))? HTTPM_STATUS_BAD_REQUEST:HTTPM_STATUS_OK;

            // if status ok, create new user
            if(httpm_status_num == HTTPM_STATUS_OK){
                json new_user = {
                    {USER_NAME_KEY, std::get<0>(user_tuple)},
                    {PASSWORD_KEY, std::get<1>(user_tuple)},
                    {EMAIL_KEY, std::get<2>(user_tuple)},
                };
                all_user_info.push_back(new_user);

                // wirte back new user data
                std::string all_user_info_buf = all_user_info.dump(4);
                std::ofstream all_user_info_o("./database/metadata/user.json");
                all_user_info_o.write(all_user_info_buf.c_str(), all_user_info_buf.length());
                all_user_info_o.close();
            }
        }else{
            httpm_status_num = HTTPM_STATUS_BAD_REQUEST;
            std::cerr << "Unexpected functionality in httpm request in login/signUp stage\n";
        }

        httpmResponse resp = httpmResponse(httpm_status_num, HTTPM_METHOD_POST, req.func, "");
        write(this->fd, resp.response_cstring(), resp.response_string_len());
        
        if(httpm_status_num == HTTPM_STATUS_OK) break;
    }

    // send all the data needed
    std::string response_msg_body;
    while(1){
        read(this->fd, buf, 8192);
        fprintf(stderr, "==========Incoming Http Request==========\n%s\n=========================================\n", buf);
        int httpm_status_num;
        httpmRequest req = httpmRequest(std::string(buf));
        if(req.method == HTTPM_METHOD_POST){
            if(req.func == HTTPM_REQUEST_ENTER_COMMENT){
                std::tuple<std::string, int, std::string, std::string> comment_tuple = utils::parse_tab_id_uname_comment(req.body);
                std::string tab_path = std::string(DATA_DIR_PATH) + std::get<0>(comment_tuple) + std::string(".json");

                if(utils::tab_uname_exists(tab_path, std::get<2>(comment_tuple))){
                    std::ifstream json_f(tab_path);
                    json json_tab = json::parse(json_f);

                    // check if the id is valid
                    if(json_tab.size() >= std::get<1>(comment_tuple) && std::get<1>(comment_tuple) > 0){
                        httpm_status_num = HTTPM_STATUS_OK;

                        // add the user comment pair to the file
                        json user_comment = {{std::get<2>(comment_tuple), std::get<3>(comment_tuple)}};
                        json_tab[std::get<1>(comment_tuple)-1]["comments"].push_back(user_comment);

                        // write back the new comment
                        std::string all_tab_buf = json_tab.dump(4);
                        std::ofstream all_tab_o(tab_path);
                        all_tab_o.write(all_tab_buf.c_str(), all_tab_buf.length());
                        all_tab_o.close();
                    }else{
                        httpm_status_num = HTTPM_STATUS_BAD_REQUEST;
                    }
                }else{
                    httpm_status_num = HTTPM_STATUS_BAD_REQUEST;
                }
            }else if(req.func == HTTPM_REQUEST_COMPOSE_ARTICLE){

            }else{
                httpm_status_num = HTTPM_STATUS_BAD_REQUEST;
                std::cerr << "Unexpected POST functionality in data transfer stage\n";
            }
        }else if(req.method == HTTPM_METHOD_GET){        
            if(req.func == HTTPM_REQUEST_WHOLE_TAB_ARTICLE){
                std::string tab_path = std::string(DATA_DIR_PATH) + req.body + std::string(".json");
                httpm_status_num = (utils::tab_exists(tab_path))? HTTPM_STATUS_OK:HTTPM_STATUS_BAD_REQUEST;
                
                // if status ok, create json data message body
                if(httpm_status_num = HTTPM_STATUS_OK){
                    std::ifstream json_f(tab_path);
                    json json_tab = json::parse(json_f);
                    response_msg_body = json_tab.dump();
                    json_f.close();
                }
            }else{
                httpm_status_num = HTTPM_STATUS_BAD_REQUEST;
                std::cerr << "Unexpected GET functionality in data transfer stage\n";
            }
        }else{
            httpm_status_num = HTTPM_STATUS_BAD_REQUEST;
            std::cerr << "Unexpected Httpm Method Type in Data Transfer Stage\n";
        }

        httpmResponse resp = httpmResponse(httpm_status_num, req.method, req.func, response_msg_body);
        response_msg_body.clear();
        write(this->fd, resp.response_cstring(), resp.response_string_len());
    }
}

int main(){
    Server srv = Server();
    Client cli = Client();
    all_user_info_f = std::ifstream("./database/metadata/user.json");
    all_user_info = json::parse(all_user_info_f);

    while(true){
        // check if there's a child terminated
        srv.check_child_termination();

        // connect a client
        int conn_fd = srv.accept_client(cli);
        if(conn_fd < 0){
            if (errno == EINTR || errno == EAGAIN) continue;  // try again
            ERR_MSG("Some Error can't be resolved while connecting client.");
        }

        // fork a new process to serve the client
        pid_t pid = fork();
        if(pid < 0){
            ERR_MSG("Fork Failure");
        }else if(pid == 0){
             // forked process serving the client
            cli.http_communication_procedure();
            return 0;
        }
        
        // if p > 0, server process keep going
        srv.add_child_pid(pid);
    }
    return 0;
}