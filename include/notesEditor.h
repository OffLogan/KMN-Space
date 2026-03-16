#ifndef NOTESEDITOR_H
#define NOTESEDITOR_H

#include <QMainWindow>

namespace Ui {
class notesEditor;
}

class notesEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit notesEditor(QWidget *parent = nullptr);
    ~notesEditor();

private:
    Ui::notesEditor *ui;
};

#endif // NOTESEDITOR_H
