#pragma once
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
#include <QKeyEvent>
#include <QImage>
#include <QFileDialog>
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

struct Stroke {
    QVector<QPoint> points;
    QColor color;
    int width;
};

enum class ActionType {
    Draw, Erase
};

struct UndoAction {
    ActionType type;
    QVector<Stroke> previousStrokes;
};

class drawingCanvas : public QWidget {
    Q_OBJECT

public:
    drawingCanvas(QWidget* parent = nullptr);
    void toggleEraserMode();
    void setColor();
    void saveAsImage();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    void eraseNearPoint(QPoint pos);
    void renderStrokes(QPainter& painter) const;

    QVector<Stroke> strokes;
    QVector<UndoAction> undoStack;
    QColor penColor;
    int penWidth;
    bool isLeftButtonPressed;
    bool isErasing;
    QDialog* widthDialog = nullptr;
};
