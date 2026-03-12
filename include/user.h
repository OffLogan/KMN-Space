//User.h
//Header file for the user's class

#ifndef USER_H
#define USER_H

#include "settings.h"
#include <string>

class User{
    private:
        long int id_;
        std::string username_;
        std::string password_;
        std::string email_;
        Settings settings_;
    public:
        //Constructors
        User();
        User(const std::string& username, const std::string& password, const std::string& email);
        //Observers
        const std::string GetUsername() const;
        const std::string GetPassword() const;
        const std::string GetEmail() const;
        const Settings GetSettings() const;
        //Modifiers
        bool SetUserName(const std::string& username);
        bool SetPassword(const std::string& password);
        bool SetEmail(const std::string& email);
        bool SetSettings(const Settings& settings);
};






#endif