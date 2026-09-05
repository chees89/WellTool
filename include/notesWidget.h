#pragma once
#include <QPlainTextEdit>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QColorDialog>
#include <QPrinter>

class notesWidget : public QWidget {
    Q_OBJECT

public:
    notesWidget(QWidget* parent = nullptr);

private slots:
    void onSelectionChanged();

private:
    void saveNote();
    void saveNoteAs();
    void openNote();
    void saveWithDialog();
    void writeToFile(const QString& path);
    void applyHighlight();
    void applyBold();
    void applyItalic();

    QPlainTextEdit* textEdit;
    QString currentFilepath;
    QString sidecarPath(const QString& pdfPath) const;

    QWidget* highlightPopup;
    QPushButton* highlightButton;
    QPushButton* boldButton;
    QPushButton* italicButton;
};
