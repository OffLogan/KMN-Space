#ifndef REMINDER_H
#define REMINDER_H

#include "task.h"
#include <string>

class Reminder : public Task{
    private:
        std::string due_date_;
    public:
        Reminder();
        Reminder(const std::string& name, const std::string& description, const std::string& due);
        const std::string GetDue() const;
        bool SetDue(const std::string& due);
};



#endif
