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
//		socket.async_read_some(boost::asio::bufer(data,max_length),
//		[this,self]
//		(boost::system::error_code er,size_t lngth)
//		{
//			if (!er)
//			{
//				Proc_reqst();
//			}
//		});
//	}
//
//
//
//	void Proc_reqst(int length,string request(data,length),string respns)
//	{
//		if (request.find("login:") == 0) // если запрос начинается с login:admin:12345
//		{
//			int pos_first = request.find(":", 6);
//			int pos_second = request.find(':', pos_first + 1);
//			string user_name = request.substr(6, pos_first - 6);
//			string password = request.substr(pos_first + 1, pos_second - pos_first - 1);
//			bool auth = manager.auth(user_name, password);
//			respns = auth ? "Auth_success" : "Auth_failed";
//		}
//
//
//
//
//		else  // если запрос начинается с access:username:resource
//			if (request.find("access:") == 0)
//			{
//				int pos1 = request.find(':', 7);
//				string user_name = request.substr(7, pos1 - 7);
//				int pos2 = request.find(':', pos1 + 1);
//				string acc = request.substr(pos1 + 1, pos2 - 1 - pos1);
//			}
//
//	}
//	
//
//
//
//};
int main()
{
	
	UserManager user_list;
	Admin admin("admin", "123");
	user_list.addUser(admin);
	RegularUser user("Tom", "password");
	user_list.addUser(user);
	
	string request;
	cin >> request;

	string respns;
	if (request.find("login:") == 0) // если запрос начинается с login:admin:12345
	{
		int pos_first = request.find(":", 6);
		int pos_second = request.find(':', pos_first + 1);
		string user_name = request.substr(6, pos_first - 6);
		string password = request.substr(pos_first + 1, pos_second - pos_first - 1);
		bool auth = user_list.auth(user_name, password);
		respns = auth ? "Auth_success" : "Auth_failed";
		cout << respns;
	}
	else

	{
		if (request.find("access:") == 0)
		{          // access:Tom:profile
			int pos_first = request.find(":", 7);
			int pos_second = request.find(':', pos_first + 1);
			string user_name = request.substr(7, pos_first - 7);
			string resource = request.substr(pos_first + 1, pos_second - pos_first - 1);
			bool res = user_list.findUser_role(user_name, resource);
			cout << res;
		}
	}
	// registration:Tom:123&64ABC

}

