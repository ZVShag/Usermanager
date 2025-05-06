#pragma once
#include <iostream>
#include <winsqlite/winsqlite3.h>
#include <vector>
#include<memory>
#include "User.h"
#include "UserManager.h"
using namespace std;

class DatabaseManager
{
	sqlite3* db;
public:
	DatabaseManager(const string& pth)
	{
		if (sqlite3_open(pth.c_str(), &db) != SQLITE_OK)
			cout << "Can't open database!\n" << endl;
		else
			Create_table();
	}
	~DatabaseManager() { sqlite3_close(db); }
	void Create_table()
	{
		const char* crt = "CREATE TABLE IF NOT EXISTS users ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT,"
			"username TEXT UNIQUE NOT NULL,"
			"password TEXT  NOT NULL,"
			"role TEXT NOT NULL);";
		char* ems = nullptr;
		if (sqlite3_exec(db, crt, nullptr, nullptr, &ems) != SQLITE_OK)
			cout << "Table create problem!" << endl;
	}
	bool saveUser(const string& username, const string& password, const string& role)
	{
		const char* rspns = "INSERT INTO users(username,password,role) Values (?,?,?);";
		sqlite3_stmt* stmt;
		if (sqlite3_prepare_v2(db, rspns, -1, &stmt, nullptr) != SQLITE_OK)
			cout << "Failed statement" << endl;
		sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_TRANSIENT);
		bool flag = (sqlite3_step(stmt) == SQLITE_DONE);
		sqlite3_finalize(stmt);
		return flag;
	}
	vector<User> load_from_database()
	{
		vector<User> users;
		const char* sql = "Select username,password,role from users";
		sqlite3_stmt* stmt;
		if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
		{
			cout << "Failed statement" << endl;
			return users;
		}
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
			string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
			users.push_back(User(username, password, role));
		}
		return users;

	}

};



