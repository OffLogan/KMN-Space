#include "../include/homescreen.h"
#include "../include/settings.h"
#include "ui_homescreen.h"

#include <QString>
#include <QStringList>

namespace {
QString normalizedField(const QString &value, const QString &defaultLabel)
{
    const QString trimmedValue = value.trimmed();
    return trimmedValue.compare(defaultLabel, Qt::CaseInsensitive) == 0 ? QString() : trimmedValue;
}
}

homeScreen::homeScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::homeScreen)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &homeScreen::addTask);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &homeScreen::addReminder);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &homeScreen::openSettings);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &homeScreen::removeTaskItem);
    connect(ui->listWidget_2, &QListWidget::itemDoubleClicked, this, &homeScreen::removeReminderItem);
}

homeScreen::~homeScreen()
{
    delete ui;
}

QString homeScreen::buildTaskText() const
{
    const QString name = normalizedField(ui->lineEdit->text(), "Name");
    const QString description = normalizedField(ui->textEdit->toPlainText(), "Description");

    if (description.isEmpty()) {
        return name;
    }

    return QString("%1 | %2").arg(name, description);
}

QString homeScreen::buildReminderText() const
{
    const QString name = normalizedField(ui->lineEdit_3->text(), "Name");
    const QString description = normalizedField(ui->textEdit_2->toPlainText(), "Description");
    const QString due = normalizedField(ui->lineEdit_5->text(), "Due");

    QStringList parts;
    parts << name;

    if (!description.isEmpty()) {
        parts << description;
    }

    if (!due.isEmpty()) {
        parts << QString("Due: %1").arg(due);
    }

    return parts.join(" | ");
}

void homeScreen::clearTaskInputs()
{
    ui->lineEdit->clear();
    ui->textEdit->clear();
}

void homeScreen::clearReminderInputs()
{
    ui->lineEdit_3->clear();
    ui->textEdit_2->clear();
    ui->lineEdit_5->clear();
}

void homeScreen::addTask()
{
    const QString taskText = buildTaskText();

    if (taskText.isEmpty()) {
        return;
    }

    ui->listWidget->addItem(taskText);
    clearTaskInputs();
}

void homeScreen::addReminder()
{
    const QString reminderText = buildReminderText();

    if (reminderText.isEmpty()) {
        return;
    }

    ui->listWidget_2->addItem(reminderText);
    clearReminderInputs();
}

void homeScreen::removeTaskItem(QListWidgetItem *item)
{
    delete ui->listWidget->takeItem(ui->listWidget->row(item));
}

void homeScreen::removeReminderItem(QListWidgetItem *item)
{
    delete ui->listWidget_2->takeItem(ui->listWidget_2->row(item));
}

void homeScreen::openSettings()
{
    settings settingsDialog(this);
    settingsDialog.exec();
}
