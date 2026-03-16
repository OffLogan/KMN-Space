#include "../include/homescreen.h"
#include "notes.h"
#include "ui_notes.h"

notes::notes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::notes)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &notes::goBackHome);
}

notes::~notes()
{
    delete ui;
}

void notes::goBackHome()
{
    if (parentWidget() != nullptr) {
        parentWidget()->show();
    }

    hide();
}
