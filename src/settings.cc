//Settings.cc
//Implementation file for the settings class

#include "../include/settings.h"
#include "../ui/settings.ui"

settings::settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::settings)
{
    ui->setupUi(this);
}

settings::~settings()
{
    delete ui;
}

//Constructor

Settings::Settings() //Default system settings
    :   theme_("dark"), language_("english"), fontSize_(12), notificationsStatus_(false){}
