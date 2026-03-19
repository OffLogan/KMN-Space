#include "../include/pdfGestor.h"

#include <QFile>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>

namespace {
QString buildHtmlDocument(const QString& title, const QString& content)
{
    const QString safeTitle = title.toHtmlEscaped();
    const QString safeContent = content.toHtmlEscaped().replace("\n", "<br>");

    return QString(
               "<html><body style=\"font-family:'Helvetica'; font-size:12pt;\">"
               "<h1>%1</h1>"
               "<hr>"
               "<p>%2</p>"
               "</body></html>"
           ).arg(safeTitle, safeContent);
}
}

bool pdfGestor::exportNoteToText(const QString& filePath,
                                 const QString& title,
                                 const QString& content,
                                 QString* errorMessage)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        if (errorMessage != nullptr) {
            *errorMessage = "Could not create the TXT file.";
        }
        return false;
    }

    QTextStream stream(&file);
    stream << title << "\n\n" << content;
    file.close();

    return true;
}

bool pdfGestor::exportNoteToPdf(const QString& filePath,
                                const QString& title,
                                const QString& content,
                                QString* errorMessage)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setDocName(title);

    QTextDocument document;
    document.setHtml(buildHtmlDocument(title, content));
    document.print(&printer);

    QFile file(filePath);
    if (!file.exists() || file.size() <= 0) {
        if (errorMessage != nullptr) {
            *errorMessage = "Could not create the PDF file.";
        }
        return false;
    }

    return true;
}
