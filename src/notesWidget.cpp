#include "notesWidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QTextCharFormat>

notesWidget::notesWidget(QWidget* parent) : QWidget(parent),
    currentFilepath() {
    textEdit = new QPlainTextEdit(this);

    QPushButton* save = new QPushButton("Save Note");
    QPushButton* open = new QPushButton("Open Note");
    QPushButton* saveAs = new QPushButton("Save Note As");

    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* notesLayout = new QHBoxLayout();

    notesLayout->addWidget(save);
    notesLayout->addWidget(saveAs);
    notesLayout->addWidget(open);

    layout->addLayout(notesLayout);
    layout->addWidget(textEdit);

    this->setLayout(layout);
    
    connect(save, &QPushButton::clicked, [=]() {
        saveNote();
    });

    connect(saveAs, &QPushButton::clicked, [=]() {
        saveNoteAs();
    });

    connect(open, &QPushButton::clicked, [=]() {
        openNote();
    });

    highlightPopup = new QWidget(this, Qt::Tool | Qt::FramelessWindowHint);
    highlightButton = new QPushButton("Highlight", highlightPopup);

    QVBoxLayout* popupLayout = new QVBoxLayout(highlightPopup);
    popupLayout->setContentsMargins(4, 4, 4, 4);
    popupLayout->addWidget(highlightButton);

    highlightPopup->hide();

    connect(highlightButton, &QPushButton::clicked, [=]() {
        applyHighlight();
    });

    connect(textEdit, &QPlainTextEdit::selectionChanged,
        this, &notesWidget::onSelectionChanged);
}

void notesWidget::onSelectionChanged() {
    QTextCursor cursor = textEdit->textCursor();

    if(!cursor.hasSelection()) {
        highlightPopup->hide();
        return;
    }

    QRect cursorRect = textEdit->cursorRect(cursor);
    QPoint viewportPoint(cursorRect.center().x(), cursorRect.top());
    QPoint globalPoint = textEdit->viewport()->mapToGlobal(viewportPoint);

    highlightPopup->adjustSize();
    highlightPopup->move(globalPoint.x() - highlightPopup->width() / 2,
                          globalPoint.y() - highlightPopup->height() - 5);
    highlightPopup->show();
}

void notesWidget::applyHighlight() {
    QTextCursor cursor = textEdit->textCursor();

    if(!cursor.hasSelection()) {
        highlightPopup->hide();
        return;
    }

    QColor color = QColorDialog::getColor(Qt::yellow, this, "Choose Highlight Color");

    if(!color.isValid()) {
        return;
    }

    QTextCharFormat format;
    format.setBackground(color);
    cursor.mergeCharFormat(format);

    cursor.clearSelection();
    textEdit->setTextCursor(cursor);
    highlightPopup->hide();
}

void notesWidget::writeToFile(const QString& path) {
    QFile file(path);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        out << textEdit->document()->toHtml();
        file.close();
    }
}

void notesWidget::saveWithDialog() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Markdown Files (*.md)");

    if(fileName.isEmpty()) {
        return ;
    }

    writeToFile(fileName);

    currentFilepath = fileName;
}

void notesWidget::saveNote() {
    if(currentFilepath.isEmpty()) {
        saveWithDialog();
    }
    else {
        writeToFile(currentFilepath);
    }
}

void notesWidget::saveNoteAs() {
    saveWithDialog();
}

void notesWidget::openNote() {
    QString openFile = QFileDialog::getOpenFileName(this, "Open File", "", "Markdown Files (*.md)");

    if(openFile.isEmpty()) {
        return;
    }

    QFile file(openFile);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        QString content = in.readAll();

        textEdit->document()->setHtml(content);
        file.close();
    }

    currentFilepath = openFile;
}