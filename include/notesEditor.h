#ifndef NOTESEDITOR_H
#define NOTESEDITOR_H

#include <QMainWindow>

#include "noteGestor.h"

namespace Ui {
class notesEditor;
}

class notesEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit notesEditor(QWidget *parent = nullptr);
    ~notesEditor();

    void OpenNote(int noteId, NoteData *noteData);

signals:
    void noteSaved(int noteId);

private slots:
    void saveCurrentNote();
    void downloadCurrentNoteAsText();
    void downloadCurrentNoteAsPdf();

private:
    void loadCurrentNote();
    bool exportCurrentNote(bool exportPdf);

    Ui::notesEditor *ui;
    int currentNoteId_;
    NoteData *noteData_;
};

#endif // NOTESEDITOR_H
