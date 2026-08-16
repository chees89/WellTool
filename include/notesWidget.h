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

    QPlainTextEdit* textEdit;
    QString currentFilepath;

    QWidget* highlightPopup;
    QPushButton* highlightButton;
};