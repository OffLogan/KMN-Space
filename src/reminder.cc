//Reminder.cc
//Implementation file for the reminder class

#include "../include/reminder.h"


Reminder::Reminder()
    :      Task(), due_date_(0)
    {
        //Must setup id
    }

Reminder::Reminder(const std::string& name, const std::string& description, const int due)
    :      Task(name, description), due_date_(due)
    {
        //Must setup id
    }

const int Reminder::GetDue() const{return due_date_;}

bool Reminder::SetDue(const int due){
    if(due<=0){ //Checks if the due date is valid
        return false;
    }
    due_date_ = due;
    return true;
};
