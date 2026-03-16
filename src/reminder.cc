//Reminder.cc
//Implementation file for the reminder class

#include "../include/reminder.h"

#include <QDate>
#include <QString>

Reminder::Reminder()
    :      Task(), due_date_("")
    {
        //Must setup id
    }

Reminder::Reminder(const std::string& name, const std::string& description, const std::string& due)
    :      Task(name, description), due_date_(due)
    {
        //Must setup id
    }

const std::string Reminder::GetDue() const{return due_date_;}

bool Reminder::SetDue(const std::string& due){
    const QString dueDate = QString::fromStdString(due).trimmed();
    const QDate parsedDate = QDate::fromString(dueDate, "dd/MM/yy");

    if(!parsedDate.isValid()){
        return false;
    }

    due_date_ = parsedDate.toString("dd/MM/yy").toStdString();
    return true;
};
