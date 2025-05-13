#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/sha.h>
#include <vector>


using namespace std;

class User
{
protected:
     string generateSalt() 
    {
        string slt;
        srand(time(0));
        for (int i = 0;i < 8;i++)
            slt += char(rand() % 10 + 48);
        for (int i = 0;i < 8;i++)
            slt += char(rand() % 26 + 65);
        
        return slt;
    }

public:
    string login;
    string password;
    string salt;
    string role;

    User(const string& lgn, const string& pswrd, const string& role)
        : login(lgn), salt(generateSalt()), role(role)
    {
        password = sha256(pswrd, salt);
    }

    virtual ~User() = default;

    static string sha256(const string& password, const string& salt)
    {
        string salted_password = password + salt;
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, salted_password.c_str(), salted_password.size());
        SHA256_Final(hash, &sha256);

        std::stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) 
        {
            ss << hex <<setw(2) <<setfill('0') << (int)hash[i];
        }

        return ss.str();
    }

    static bool hasAccess(const string& rl)
    {
        return false;
    }

    bool auth(const string& input_password)
    {
        return sha256(input_password, salt) == password;
    }

    string& get_login() { return login; }
    string& get_role() { return role; }
    string get_pass_hash() { return password; }
};

class Admin : public User
{
public:
    Admin(const string& lgn, const string& pswrd) : User(lgn, pswrd, "Admin") {}
    static bool hasAccess(const string& rl) 
    { 
        return true; 
    }
};

class RegularUser : public User
{
public:
    RegularUser(const string& lgn, const string& pswrd) : User(lgn, pswrd, "User") {}
    static bool hasAccess(const string& rl) 
    {
        return ((rl == "profile") || (rl == "public"));
    }
};

class Guest : public User
{
public:
    Guest() : User("Guest", "", "Guest")
    {
        salt = "";
        password = "";
    }

    static bool hasAccess(const string& rl) 
    {
        return (rl == "public");
    }
};