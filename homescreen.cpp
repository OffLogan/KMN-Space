#include "homescreen.h"
#include "ui_homescreen.h"

homeScreen::homeScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::homeScreen)
{
    ui->setupUi(this);
}

homeScreen::~homeScreen()
{
    delete ui;
}
