#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QDialog>
#include <QShowEvent>
#include <QListWidgetItem>

#include "reminderGestor.h"
#include "taskGestor.h"

class notes;
class schedule;

namespace Ui {
class homeScreen;
}

class homeScreen : public QDialog
{
    Q_OBJECT

public:
    explicit homeScreen(QWidget *parent = nullptr);
    ~homeScreen();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void addTask();
    void addReminder();
    void removeTaskItem(QListWidgetItem *item);
    void removeReminderItem(QListWidgetItem *item);
    void openSettings();
    void openNotes();
    void openSchedule();

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
    notes *notesWindow_;
    schedule *scheduleWindow_;
};

#endif // HOMESCREEN_H
