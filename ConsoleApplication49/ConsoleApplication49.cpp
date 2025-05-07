#include <iostream>
#include"Usermanager.h"
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;
class Session
{
	tcp::socket sock;
	UserManager manager;
	
public:
	enable_shared_from_this<Session>{
	Session(tcp::socket sckt, UserManager & usmng) :
		sock(move(sckt)), manager(usmng) {
}
void Start() { readRequest(); } 
private:
	void readRequest() {auto self{shared_from_this()}
	
};
int main()
{
	
	UserManager user_list;

	
	
}

