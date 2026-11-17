#include "notesWidget.h"
#include <QFile>
#include <QFileDialog>
#include <QColorDialog>
#include <QPrinter>
#include <QShortcut>
#include <QPushButton>
#include <QTextStream>
#include <QTextCharFormat>
#include <QPlainTextEdit>

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

    boldButton = new QPushButton("B", highlightPopup);
    italicButton = new QPushButton("I", highlightPopup);

    popupLayout->addWidget(boldButton);
    popupLayout->addWidget(italicButton);

    connect(boldButton, &QPushButton::clicked, [=]() { applyBold(); });
    connect(italicButton, &QPushButton::clicked, [=]() { applyItalic(); });

    connect(textEdit, &QPlainTextEdit::selectionChanged,
        this, &notesWidget::onSelectionChanged);

    QShortcut* boldShortcut = new QShortcut(QKeySequence::Bold, textEdit);
    QShortcut* italicShortcut = new QShortcut(QKeySequence::Italic, textEdit);

    connect(boldShortcut, &QShortcut::activated, this, &notesWidget::applyBold);
    connect(italicShortcut, &QShortcut::activated, this, &notesWidget::applyItalic);

    boldShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    italicShortcut->setContext(Qt::WidgetWithChildrenShortcut);
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

void notesWidget::applyBold() {
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat format;

    if(!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }

    if (!cursor.hasSelection()) {
        bool isBold = textEdit->currentCharFormat().fontWeight() == QFont::Bold;
        format.setFontWeight(isBold ? QFont::Normal : QFont::Bold);
        textEdit->mergeCurrentCharFormat(format);
        return;
    }

    bool isBold = cursor.charFormat().fontWeight() == QFont::Bold;

    format.setFontWeight(isBold ? QFont::Normal : QFont::Bold);
    cursor.mergeCharFormat(format);

    cursor.clearSelection();
    textEdit->setTextCursor(cursor);
    highlightPopup->hide();
}

void notesWidget::applyItalic() {
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat format;

    if(!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }

    if(!cursor.hasSelection()) {
        bool isItalic = textEdit->currentCharFormat().fontItalic();
        format.setFontItalic(!isItalic);
        textEdit->mergeCurrentCharFormat(format);
        return;
    }

    bool isItalic = cursor.charFormat().fontItalic();

    format.setFontItalic(!isItalic);
    cursor.mergeCharFormat(format);

    cursor.clearSelection();
    textEdit->setTextCursor(cursor);
    highlightPopup->hide();
}

void notesWidget::writeToFile(const QString& path) {
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path);

    textEdit->document()->print(&printer);

    QFile sidecar(sidecarPath(path));
    if(sidecar.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&sidecar);
        out << textEdit->document()->toHtml();
        sidecar.close();
    }
}

void notesWidget::saveWithDialog() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "PDF Files (*.pdf)");
    if(fileName.isEmpty()) return;
    if(!fileName.endsWith(".pdf", Qt::CaseInsensitive)) fileName += ".pdf";

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

QString notesWidget::sidecarPath(const QString& pdfPath) const {
    return pdfPath + ".html";
}

void notesWidget::openNote() {
QString openFile = QFileDialog::getOpenFileName(this, "Open File", "", "PDF Files (*.pdf)");
    if(openFile.isEmpty()) return;

    QFile file(sidecarPath(openFile));
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        textEdit->document()->setHtml(in.readAll());
        file.close();
    }

    currentFilepath = openFile;
}
