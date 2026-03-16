#ifndef NOTES_H
#define NOTES_H

#include <QMainWindow>

namespace Ui {
class notes;
}

class notes : public QMainWindow
{
    Q_OBJECT

public:
    explicit notes(QWidget *parent = nullptr);
    ~notes();

private slots:
    void goBackHome();

private:
    Ui::notes *ui;
};

#endif // NOTES_H
