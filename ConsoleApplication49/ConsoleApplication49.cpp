#include <iostream>
#include "Usermanager.h"
#include <boost/asio.hpp>
#include <string>
using namespace std;
using boost::asio::ip::tcp;

class Session {
    tcp::socket socket;
    UserManager& manager;
    boost::asio::streambuf stream_buffer; 

public:
    Session(tcp::socket sckt, UserManager& usmng) :
        socket(move(sckt)), manager(usmng) {
    }

    static void Start(Session* self) {
        self->readRequest();
    }

    ~Session() {
    }

private:
    void readRequest() {
        auto self = this;
        
        boost::asio::async_read_until(socket, stream_buffer, '\n',
            [this, self](boost::system::error_code ec,  size_t length) {
                if (!ec) {
                    
                     istream is(&stream_buffer);
                     string request;
                     getline(is, request); 

                    Proc_reqst(request);
                  
                }
                else {
                    delete this;
                }
            });
    }

    void Proc_reqst(const  string& request) {
        string respns;

        try {
            if (request.starts_with("login:")) {
                size_t user_start = 6;
                size_t pass_delim = request.find(':', user_start);
                size_t end_delim = request.find(':', pass_delim + 1);

                if (pass_delim == string::npos || end_delim == string::npos) {
                    throw invalid_argument("Invalid login format");
                }

                string user_name = request.substr(user_start, pass_delim - user_start);
                string password = request.substr(pass_delim + 1, end_delim - (pass_delim + 1));

                bool auth = manager.auth(user_name, password);
                respns = auth ? "Auth_success\n" : "Auth_failed\n";
            }
            else if (request.starts_with("access:")) {
                size_t user_start = 7;
                size_t res_delim = request.find(':', user_start);
                size_t end_delim = request.find(':', res_delim + 1);

                if (res_delim == string::npos || end_delim == string::npos) {
                    throw invalid_argument("Invalid access format");
                }

                string user_name = request.substr(user_start, res_delim - user_start);
                string resource = request.substr(res_delim + 1, end_delim - (res_delim + 1));

                bool auth = manager.findUser_role(user_name, resource);
                respns = auth ? "Access_granted\n" : "Access_denied\n";
            }
            else if (request.starts_with("registration:")) {
                size_t user_start = 13;
                size_t role_delim = request.find(':', user_start);
                size_t end_delim = request.find(':', role_delim + 1);

                if (role_delim == string::npos || end_delim == string::npos) {
                    throw invalid_argument("Invalid registration format");
                }

                string user_name = request.substr(user_start, role_delim - user_start);
                string role = request.substr(role_delim + 1, end_delim - (role_delim + 1));

                manager.addUser(RegularUser(user_name, role));
                respns = "User reg\n";
            }
            else {
                respns = "INVALID_REQUEST\n";
            }
        }
        catch (const exception& e) {
            respns = "ERROR: " + string(e.what()) + "\n";
        }

        sendResponse(respns);
    }

    void sendResponse(const  string& response) {
        auto self = this;
        boost::asio::async_write(socket, boost::asio::buffer(response),
            [this, self](boost::system::error_code ec,  size_t) {
                if (ec) {
                    delete this;
                }
                readRequest();
            });
    }
};

class Server {
    tcp::acceptor acept;
    UserManager& manager;

public:
    Server(boost::asio::io_context& io_context, int port, UserManager& usmanager) :
        acept(io_context, tcp::endpoint(tcp::v4(), port)), manager(usmanager) {
        acceptor();
    }

    void acceptor() {
        acept.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                Session* session = new Session(move(socket), this->manager);
                Session::Start(session);
            }
            acceptor();
            });
    }
};

int main() {
    UserManager user_list;
    Admin admin("admin", "123");
    user_list.addUser(admin);
    RegularUser user("Tom", "password");
    user_list.addUser(user);

    boost::asio::io_context io_context;
    Server serv(io_context, 1234, user_list);
    io_context.run();
    //login:admin:123
    return 0;
}