#include <iostream>
#include"Usermanager.h"
#include <boost/asio.hpp>
#include <string>
using namespace std;
using boost::asio::ip::tcp;
//class Session
//{
//	tcp::socket socket;
//	UserManager manager;
//	
//public:
//	enable_shared_from_this<Session>{
//	Session(tcp::socket sckt, UserManager & usmng) :
//		socket(move(sckt)), manager(usmng) {
//}
//void Start() { readRequest(); } 
//private:
//	void readRequest()
//	{
//		auto self{shared_from_this()}
//		socket.async_read_some(boost::asio::bufer(data,max_length),[this,self]
//		(boost::system::error_code er,size_t lngth)
//		{
//			if (!er)
//			{
//
//			}
//		});
//	}
//	
//	void Proc_reqst(int length,string request(data,length),string respns)
//	{
//		if (request.find("login:") == 0) // если запрос начинается с login:admin:12345
//		{
//			int pos_first = request.find(":", 6);
//			int pos_second = request.find(':', pos_first + 1);
//			string user_name = request.substr(6, pos_first - 1);
//			string password = request.substr(pos_first + 1, pos_second - pos_first - 1);
//		}
//
// }
//
//
//}
//};
int main()
{
	
	UserManager user_list;
	Admin admin("admin", "123");
	user_list.addUser(admin);
	RegularUser user("user", "password");
	user_list.addUser(user);
	user_list.users_info();
	string request;
	cin >> request;

	// login:admin:12345
	int pos1 = request.find(':', 6);
	string user_name = request.substr(6, pos1-6); // admin
	int pos2 = request.find(':', pos1 + 1);
	string password = request.substr(pos1 + 1, pos2 - 1 - pos1);//12345
	cout<<user_list.auth(user_name,password);

}

