
#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>


class Settings{
    private:
        std::string theme_;
        std::string language_;
        int fontSize_;
        bool notificationsStatus_;
    public:
        //Constructors
        Settings(); //Define the default settings of the app
        //Observers
        const std::string GetTheme() const;
        const std::string GetLanguage() const;
        const int GetFontSize() const;
        const bool GetNotificationsStatus() const;
        //Functions
        void ChangeTheme(std::string &theme);
        void ChangeLanguage(std::string &language);
        void ChangeFontSize(int fontSize);
        void TurnOnNotifications();
        void TurnOffNotifications();
};




#endif