#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QMainWindow>

class QTableWidgetItem;

namespace Ui {
class schedule;
}

class schedule : public QMainWindow
{
    Q_OBJECT

public:
    explicit schedule(QWidget *parent = nullptr);
    ~schedule();
    void load();

private slots:
    void goBackHome();
    void addEntry();
    void deleteSelectedEntry();
    void saveSchedule();
    void clearSchedule();

private:
    bool writeSchedule(bool showSuccessMessage);
    void loadSchedule();
    void appendRow(const QString& day,
                   const QString& time,
                   const QString& subject,
                   const QString& location);
    void clearInputs();
    QString scheduleFilePath() const;

    Ui::schedule *ui;
};

#endif // SCHEDULE_H
