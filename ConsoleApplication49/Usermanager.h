#pragma once
#include<iostream>
#include "User.h"
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>
#include "DatabaseManager.h"
using namespace std;
class UserManager
{
	vector<User> users;
	DatabaseManager dbmanager;

public:

	UserManager() :dbmanager("my_data_base.db")
	{
		load_from_db();
		
	}
	void users_info()
	{
		for (User& user : users)
		{
			cout << user.get_login() << '\t' << user.get_pass_hash() << '\t' << user.get_role() << endl;
		}
	}
	void load_from_db()
	{
		auto users_db = dbmanager.load_from_database();

		for ( User& user : users_db)
		{
			if (user.role == "Admin")
			{
				users.push_back(user);
			}
			else
				if (user.role == "User")
				{
				
					users.push_back(user);
				}
		}
	}
	void addUser(User& usr)
	{
		users.push_back(usr);
	}
	string findUser(const string& uname)
	{
		for (auto& us : users)
			if (uname == us.get_login())
				return us.salt;
		return "";
	}

	string findUser2(const string& uname)
	{
		for (auto& us : users)
			if (uname == us.get_login())
				return us.password;
		return "";
	}
	bool findUser_role(const string& uname,const string& resource)
	{
		for (auto& us : users)
			if (uname == us.get_login())
			{
				if (us.get_role() == "Admin")
					return Admin::hasAccess(resource);
				if (us.get_role() == "User")
					return RegularUser::hasAccess(resource);
				if (us.get_role() == "Guest")
					return Guest::hasAccess(resource);
			}
	}
	bool auth(const string& uname, const string& upass)
	{
		if (size(findUser(uname)) != 0)
		{
			if (User::sha256(upass, findUser(uname)) == findUser2(uname))
				return true;
		}
		return false;
	}
};



