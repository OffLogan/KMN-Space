#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QDialog>
#include <QListWidgetItem>

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
    QString buildTaskText() const;
    QString buildReminderText() const;
    void clearTaskInputs();
    void clearReminderInputs();

    Ui::homeScreen *ui;
};

#endif // HOMESCREEN_H
