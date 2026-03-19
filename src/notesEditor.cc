#include "notesEditor.h"
#include "ui_noteseditor.h"

#include "../include/pdfGestor.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>

namespace {
QString downloadsFilePath(const QString& baseName, const QString& extension)
{
    QString downloadsDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (downloadsDir.isEmpty()) {
        downloadsDir = QDir::homePath();
    }

    QDir directory(downloadsDir);
    const QString safeBaseName = baseName.trimmed().isEmpty() ? QString("note") : baseName;
    QString candidate = directory.filePath(QString("%1.%2").arg(safeBaseName, extension));
    int suffix = 1;

    while (QFileInfo::exists(candidate)) {
        candidate = directory.filePath(QString("%1_%2.%3").arg(safeBaseName).arg(suffix).arg(extension));
        ++suffix;
    }

    return candidate;
}
}

notesEditor::notesEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::notesEditor)
    , currentNoteId_(0)
    , noteData_(nullptr)
{
    ui->setupUi(this);
    ui->titleLineEdit->setPlaceholderText("New note");
    connect(ui->saveButton, &QPushButton::clicked, this, &notesEditor::saveCurrentNote);
    connect(ui->downloadTxtButton, &QPushButton::clicked, this, &notesEditor::downloadCurrentNoteAsText);
    connect(ui->downloadPdfButton, &QPushButton::clicked, this, &notesEditor::downloadCurrentNoteAsPdf);
    connect(ui->closeButton, &QPushButton::clicked, this, &notesEditor::hide);
}

notesEditor::~notesEditor()
{
    delete ui;
}

void notesEditor::OpenNote(int noteId, NoteData *noteData)
{
    currentNoteId_ = noteId;
    noteData_ = noteData;
    loadCurrentNote();
    show();
    raise();
    activateWindow();
}

void notesEditor::saveCurrentNote()
{
    if (noteData_ == nullptr || currentNoteId_ <= 0) {
        QMessageBox::warning(this, "Notes", "No note is selected.");
        return;
    }

    Note *note = noteData_->FindById(currentNoteId_);
    if (note == nullptr) {
        QMessageBox::warning(this, "Notes", "The selected note could not be found.");
        return;
    }

    const QString title = ui->titleLineEdit->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Notes", "The note title cannot be empty.");
        return;
    }

    note->SetTitle(title.toStdString());
    note->SetContent(ui->contentTextEdit->toPlainText().toStdString());

    if (!noteData_->UpdateNote(*note)) {
        QMessageBox::warning(this, "Notes", "The note could not be saved.");
        return;
    }

    emit noteSaved(currentNoteId_);
}

void notesEditor::loadCurrentNote()
{
    if (noteData_ == nullptr || currentNoteId_ <= 0) {
        ui->titleLineEdit->clear();
        ui->contentTextEdit->clear();
        return;
    }

    const Note *note = noteData_->FindById(currentNoteId_);
    if (note == nullptr) {
        ui->titleLineEdit->clear();
        ui->contentTextEdit->clear();
        return;
    }

    const QString title = QString::fromStdString(note->GetTitle()).trimmed();
    ui->titleLineEdit->setText(title.compare("New note", Qt::CaseInsensitive) == 0 ? QString() : title);
    ui->contentTextEdit->setPlainText(QString::fromStdString(note->GetContent()));
    ui->contentTextEdit->setEnabled(!note->IsFolder());
}

void notesEditor::downloadCurrentNoteAsText()
{
    exportCurrentNote(false);
}

void notesEditor::downloadCurrentNoteAsPdf()
{
    exportCurrentNote(true);
}

bool notesEditor::exportCurrentNote(bool exportPdf)
{
    if (noteData_ == nullptr || currentNoteId_ <= 0) {
        QMessageBox::warning(this, "Notes", "No note is selected.");
        return false;
    }

    const Note *note = noteData_->FindById(currentNoteId_);
    if (note == nullptr) {
        QMessageBox::warning(this, "Notes", "The selected note could not be found.");
        return false;
    }

    if (note->IsFolder()) {
        QMessageBox::warning(this, "Notes", "Folders cannot be exported.");
        return false;
    }

    const QString title = ui->titleLineEdit->text().trimmed().isEmpty()
        ? QString("note_%1").arg(currentNoteId_)
        : ui->titleLineEdit->text().trimmed();
    const QString content = ui->contentTextEdit->toPlainText();
    const QString filePath = downloadsFilePath(title, exportPdf ? "pdf" : "txt");

    QString errorMessage;
    const bool success = exportPdf
        ? pdfGestor::exportNoteToPdf(filePath, title, content, &errorMessage)
        : pdfGestor::exportNoteToText(filePath, title, content, &errorMessage);

    if (!success) {
        QMessageBox::warning(this, "Notes", errorMessage);
        return false;
    }

    QMessageBox::information(this,
                             "Notes",
                             QString("%1 file created in:\n%2")
                                 .arg(exportPdf ? "PDF" : "TXT", filePath));
    return true;
}
