//Settings.h
//Header file for settings class

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <string>


namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    ~settings();

private:
    Ui::settings *ui;
};



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




#endif // SETTINGS_H
