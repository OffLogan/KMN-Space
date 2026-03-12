//User.cc
//Implementation file for the user's class

#include "../include/user.h"

//Constructors
User::User()
    :   id_(0), username_(""), password_(""), email_(""){}

User::User(const std::string& username, const std::string& password, const std::string& email)
    :   username_(username), password_(password), email_(email)
    {
        //id_ = ;
        //Didn't think how the user's id will be assigned to each user yet
    }


//Observers
const std::string User::GetUsername() const {return username_;}
const std::string User::GetPassword() const {return password_;}
const std::string User::GetEmail() const {return email_;}
const Settings User::GetSettings() const {return settings_;}

//Modifiers
bool User::SetUserName(const std::string& username){
    if(username!=""){
        username_ = username;
        return true;
    }
    return false;
};

bool User::SetPassword(const std::string& password){
    if(password!=""){
        password_ = password;
        return true;
    }
    return false;
};

bool User::SetEmail(const std::string& email){
        if(email!=""){
        email_ = email;
        return true;
    }
    return false;
};

bool User::SetSettings(const Settings& settings){
        settings_ = settings; //Must re evaluate this function, not sure if this will be how it works
        return true;
};
