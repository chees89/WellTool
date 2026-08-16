#include "drawingCanvas.h"
#include <QWidget>
#include <QMouseEvent>
#include <QVector>
#include <QColor>
#include <QPainter>
#include <QMenu>
#include <QColorDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QSlider>
#include <QDebug>
#include <cmath>
#include <QPushButton>
#include <QKeyEvent>

drawingCanvas::drawingCanvas(QWidget* parent) : QWidget(parent), 
    penColor(Qt::black),
    penWidth(2),
    isLeftButtonPressed(false),
    isErasing(false) {
        setFocusPolicy(Qt::StrongFocus);
}

void drawingCanvas::mousePressEvent(QMouseEvent* event) {    
    if(event->button() == Qt::LeftButton) {
        if(isErasing) {
            isLeftButtonPressed = true;

            UndoAction action;
            action.type = ActionType::Erase;
            action.previousStrokes = strokes;
            undoStack.push_back(action);

            eraseNearPoint(event->pos());
        }
        else {    
        
            isLeftButtonPressed = true;

            UndoAction action;
            action.type = ActionType::Draw;
            undoStack.push_back(action);
            
            Stroke newStroke;
            newStroke.color = penColor;
            newStroke.width = penWidth;
            newStroke.points.push_back(event->pos());

            strokes.push_back(newStroke);
            update();
            }
        }
        if(event->button() == Qt::RightButton) {
            if(widthDialog != nullptr) {
                widthDialog->raise();
                widthDialog->activateWindow();
            }

            else{            
                QDialog* dialog = new QDialog(this);
                QSlider* slider = new QSlider(Qt::Vertical, dialog);

                slider->setMinimum(1);
                slider->setMaximum(20);
                slider->setValue(penWidth);

                connect(slider, &QSlider::valueChanged, [=] (int value) {
                    penWidth = value;
                });

                connect(dialog, &QObject::destroyed, [this]() { widthDialog = nullptr; });

                QVBoxLayout* layout = new QVBoxLayout(dialog);
                layout->addWidget(slider);

                widthDialog = dialog;

                dialog->show();
                dialog->setAttribute(Qt::WA_DeleteOnClose);
            }
    }
}

void drawingCanvas::mouseMoveEvent(QMouseEvent* event) {
    if(isErasing) {
        if(isLeftButtonPressed) {
            eraseNearPoint(event->pos());
        }
    }

    else {
    
        if(isLeftButtonPressed) {
            strokes.back().points.push_back(event->pos());
            auto yPos = event->pos().y();

            if(this->height() - yPos < 100) {
                resize(this->width(), this->height() + 300);
            }

            update();
        }
    }
}

void drawingCanvas::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        isLeftButtonPressed = false;
    }
}

void drawingCanvas::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    renderStrokes(painter);
}

void drawingCanvas::renderStrokes(QPainter& painter) const {
    for(const auto& stroke : strokes) {
        QPen pen(stroke.color, stroke.width);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);

        if(stroke.points.size() == 1) {
            painter.drawPoint(stroke.points[0]);
        }
        else {
            for(int i = 0; i < stroke.points.size() - 1; i++) {
                    painter.drawLine(stroke.points[i], stroke.points[i+1]);
            }    
        }
    }
}

void drawingCanvas::toggleEraserMode() {
    isErasing = !isErasing;
}

void drawingCanvas::setColor() {
    QColor color = QColorDialog::getColor();
    if(color.isValid()) {
        penColor = color;
    }
}

void drawingCanvas::saveAsImage() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Drawing"), QString(), tr("PNG Image (*.png)"));

    if(fileName.isEmpty()) {
        return;
    }

    if(!fileName.endsWith(".png", Qt::CaseInsensitive)) {
        fileName += ".png";
    }

    QImage image(this->size(), QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    renderStrokes(painter);
    painter.end();

    if(!image.save(fileName, "PNG")) {
        qWarning() << "Failed to save image to" << fileName;
    }
}

void drawingCanvas::eraseNearPoint(QPoint pos) {
    int eraseRadius = penWidth;

    QVector<Stroke> newStrokes;

    for(const auto& stroke : strokes) {
        Stroke currentPiece;
        currentPiece.color = stroke.color;
        currentPiece.width = stroke.width;

        for(const auto& point : stroke.points) {
            auto euclideanDistance = std::sqrt(std::pow(point.x() - pos.x(), 2) + std::pow(point.y() - pos.y(), 2));

            if(euclideanDistance > eraseRadius) {
                currentPiece.points.push_back(point);
            }

            if(euclideanDistance <= eraseRadius && !currentPiece.points.empty()) {
                newStrokes.push_back(currentPiece);
                currentPiece.points.clear();
            }
        }

        if(!currentPiece.points.empty()) {
            newStrokes.push_back(currentPiece); 
        }
    }
    strokes = newStrokes;
    update();
}

void drawingCanvas::keyPressEvent(QKeyEvent* event) {
    if(event->matches(QKeySequence::Undo)) {
        if(!undoStack.isEmpty()) {
            auto lastAction = undoStack.last();
            undoStack.pop_back();

            if(lastAction.type == ActionType::Draw) {
                strokes.pop_back();
            }

            if(lastAction.type == ActionType::Erase) {
                strokes = lastAction.previousStrokes;
            }
        }
    }
    
    update();
}