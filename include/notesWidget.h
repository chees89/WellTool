#pragma once
#include <QString>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

class QPlainTextEdit;
class QPlainTextEdit;
class QPushButton;

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

    QVBoxLayout buttonsLayout;
    QVBoxLayout notesToolbarLayout;
    QVBoxLayout notesEditorLayout;

    QHBoxLayout notesPrimaryToolbar;
    QHBoxLayout notesSecondaryToolbarLayout;
    QHBoxLayout notesHeaderWidgetLayout;
    QHBoxLayout notesStatusBarLayout;

    QPlainTextEdit* textEdit = nullptr;
    QString currentFilepath;
    QString sidecarPath(const QString& pdfPath) const;

    QWidget* notesStatusBarWidget = nullptr;
    QWidget* notesEditorWorkspaceWidget = nullptr;
    QWidget* notesSecondaryToolbarWidget = nullptr;
    QWidget* notesToolbarWidget = nullptr;
    QWidget* notesHeaderWidget = nullptr;
    QWidget* highlightPopup = nullptr;

    QPushButton* highlightButton;
    QPushButton* boldButton;
    QPushButton* italicButton;
};
