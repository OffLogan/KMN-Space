#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QDialog>
#include <QListWidgetItem>

#include "reminderGestor.h"
#include "taskGestor.h"

namespace Ui {
class homeScreen;
}

class homeScreen : public QDialog
{
    Q_OBJECT

public:
    explicit homeScreen(QWidget *parent = nullptr);
    ~homeScreen();

private slots:
    void addTask();
    void addReminder();
    void removeTaskItem(QListWidgetItem *item);
    void removeReminderItem(QListWidgetItem *item);
    void openSettings();

private:
    void loadStoredData();
    void appendTaskItem(const Task& task);
    void appendReminderItem(const Reminder& reminder);
    QString buildTaskText() const;
    QString buildReminderText() const;
    void clearTaskInputs();
    void clearReminderInputs();

    Ui::homeScreen *ui;
    TaskData taskData_;
    ReminderData reminderData_;
};

#endif // HOMESCREEN_H
