#include "../include/homescreen.h"
#include "../include/settings.h"
#include "ui_homescreen.h"

#include <QListWidget>
#include <QDate>
#include <QString>
#include <QStringList>
#include <Qt>

namespace {
QString normalizedField(const QString &value, const QString &defaultLabel)
{
    const QString trimmedValue = value.trimmed();
    return trimmedValue.compare(defaultLabel, Qt::CaseInsensitive) == 0 ? QString() : trimmedValue;
}

QString taskTextFromTask(const Task& task)
{
    if (task.GetDescription().empty()) {
        return QString::fromStdString(task.GetName());
    }

    return QString("%1 | %2")
        .arg(QString::fromStdString(task.GetName()),
             QString::fromStdString(task.GetDescription()));
}

QString reminderTextFromReminder(const Reminder& reminder)
{
    QStringList parts;
    parts << QString::fromStdString(reminder.GetName());

    if (!reminder.GetDescription().empty()) {
        parts << QString::fromStdString(reminder.GetDescription());
    }

    parts << QString("Due: %1").arg(reminder.GetDue());
    return parts.join(" | ");
}
}

homeScreen::homeScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::homeScreen)
    , taskData_()
    , reminderData_()
{
    ui->setupUi(this);

    taskData_.Load();
    reminderData_.Load();
    loadStoredData();

    connect(ui->pushButton, &QPushButton::clicked, this, &homeScreen::addTask);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &homeScreen::addReminder);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &homeScreen::openSettings);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &homeScreen::removeTaskItem);
    connect(ui->listWidget_2, &QListWidget::itemDoubleClicked, this, &homeScreen::removeReminderItem);
    ui->lineEdit->setPlaceholderText("Name");
    ui->textEdit->setPlaceholderText("Description");
    ui->lineEdit_3->setPlaceholderText("Name");
    ui->textEdit_2->setPlaceholderText("Description");
    ui->lineEdit_5->setPlaceholderText("Day/Month/Year");
    ui->lineEdit->setStyleSheet("QLineEdit { color: white; }");
    ui->lineEdit_3->setStyleSheet("QLineEdit { color: white; }");
    ui->lineEdit_5->setStyleSheet("QLineEdit { color: white; }");
    ui->textEdit->setStyleSheet("QTextEdit { color: white; }");
    ui->textEdit_2->setStyleSheet("QTextEdit { color: white; }");
}

homeScreen::~homeScreen()
{
    delete ui;
}

void homeScreen::loadStoredData()
{
    ui->listWidget->clear();
    ui->listWidget_2->clear();

    for (const Task& task : taskData_.Data()) {
        appendTaskItem(task);
    }

    for (const Reminder& reminder : reminderData_.Data()) {
        appendReminderItem(reminder);
    }
}

void homeScreen::appendTaskItem(const Task& task)
{
    QListWidgetItem* item = new QListWidgetItem(taskTextFromTask(task), ui->listWidget);
    item->setData(Qt::UserRole, task.GetTaskId());
}

void homeScreen::appendReminderItem(const Reminder& reminder)
{
    QListWidgetItem* item = new QListWidgetItem(reminderTextFromReminder(reminder), ui->listWidget_2);
    item->setData(Qt::UserRole, reminder.GetTaskId());
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
    const QString name = normalizedField(ui->lineEdit->text(), "Name");
    const QString description = normalizedField(ui->textEdit->toPlainText(), "Description");
    const QString taskText = buildTaskText();

    if (taskText.isEmpty() || name.isEmpty()) {
        return;
    }

    Task task(name.toStdString(), description.toStdString());
    if (!task.SetId(taskData_.NextId())) {
        return;
    }

    if (!taskData_.AddTask(task)) {
        return;
    }

    appendTaskItem(task);
    clearTaskInputs();
}

void homeScreen::addReminder()
{
    const QString name = normalizedField(ui->lineEdit_3->text(), "Name");
    const QString description = normalizedField(ui->textEdit_2->toPlainText(), "Description");
    const QString dueText = normalizedField(ui->lineEdit_5->text(), "Due").trimmed();
    const QString reminderText = buildReminderText();

    if (reminderText.isEmpty() || name.isEmpty() || dueText.isEmpty()) {
        return;
    }

    const QDate dueDate = QDate::fromString(dueText, "dd/MM/yy");
    if (!dueDate.isValid()) {
        return;
    }

    Reminder reminder(name.toStdString(), description.toStdString(), dueDate.toString("dd/MM/yy").toStdString());
    if (!reminder.SetId(reminderData_.NextId())) {
        return;
    }

    if (!reminderData_.AddReminder(reminder)) {
        return;
    }

    appendReminderItem(reminder);
    clearReminderInputs();
}

void homeScreen::removeTaskItem(QListWidgetItem *item)
{
    const int id = item->data(Qt::UserRole).toInt();
    if (!taskData_.RemoveTask(id)) {
        return;
    }

    delete ui->listWidget->takeItem(ui->listWidget->row(item));
}

void homeScreen::removeReminderItem(QListWidgetItem *item)
{
    const int id = item->data(Qt::UserRole).toInt();
    if (!reminderData_.RemoveReminder(id)) {
        return;
    }

    delete ui->listWidget_2->takeItem(ui->listWidget_2->row(item));
}

void homeScreen::openSettings()
{
    settings settingsDialog(this);
    settingsDialog.exec();
}
