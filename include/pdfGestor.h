#ifndef PDFGESTOR_H
#define PDFGESTOR_H

#include <QString>

class pdfGestor
{
public:
    static bool exportNoteToText(const QString& filePath,
                                 const QString& title,
                                 const QString& content,
                                 QString* errorMessage = nullptr);

    static bool exportNoteToPdf(const QString& filePath,
                                const QString& title,
                                const QString& content,
                                QString* errorMessage = nullptr);
};

#endif // PDFGESTOR_H
