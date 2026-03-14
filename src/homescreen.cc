#include "../include/homescreen.h"
#include "../ui/homescreen.ui"

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
