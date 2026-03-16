#include "notesEditor.h"
#include "ui_noteseditor.h"

notesEditor::notesEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::notesEditor)
{
    ui->setupUi(this);
}

notesEditor::~notesEditor()
{
    delete ui;
}
