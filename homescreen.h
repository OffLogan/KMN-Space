#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QDialog>

namespace Ui {
class homeScreen;
}

class homeScreen : public QDialog
{
    Q_OBJECT

public:
    explicit homeScreen(QWidget *parent = nullptr);
    ~homeScreen();

private:
    Ui::homeScreen *ui;
};

#endif // HOMESCREEN_H
