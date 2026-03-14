#ifndef REMINDER_H
#define REMINDER_H

#include "task.h"

class Reminder : public Task{
    private:
        int due_date_; //still working of TAD, probably i change it to a 4 different atributes year, month, day and hour
    public:
        Reminder();
        Reminder(const std::string& name, const std::string& description, const int due);
        const int GetDue() const;
        bool SetDue(const int due);
};



#endif