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
            if (request.find("login:") == 0) // если запрос начинается с login:admin:12345
               {
                int pos_first = request.find(":", 6);
                int pos_second = request.find(':', pos_first + 1);
                string user_name = request.substr(6, pos_first - 6);
                string password = request.substr(pos_first + 1, pos_second - pos_first - 1);
                bool auth = manager.auth(user_name, password);
                cout << user_name <<endl << password;
                respns = auth ? "Auth_success" : "Auth_failed";
               }
            else  // если запрос начинается с access:username:resource
            {  if (request.find("access:") == 0)
			    {
				    int pos1 = request.find(':', 7);
				    string user_name = request.substr(7, pos1 - 7);
				    int pos2 = request.find(':', pos1 + 1);
				    string acc = request.substr(pos1 + 1, pos2 - 1 - pos1);
                    bool auth = manager.findUser_role(user_name, acc);
                    respns = auth ? "Access_granted" : "Access_denied";
			    }
                else 
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