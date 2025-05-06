#include <iostream>
#include "User.h"
#include"Usermanager.h"
#include "DatabaseManager.h"
using namespace std;
int main()
{
 
	
	Admin a("dfdf", "dfsdfdsfd");
	UserManager user_list;
	user_list.addUser(a);
	DatabaseManager db("new.db");
	cout << user_list.findUser("dfdf");
	
}

