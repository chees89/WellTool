#pragma once
#include <QWidget>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include "saveTheme.h"
#include "themeLoader.h"
#include "drawingCanvas.h"
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QScrollArea>
#include "screenDefinition.h"
#include "saveTheme.h"
#include "themeLoader.h"
#include "drawingCanvas.h"
#include "notesWidget.h"
#include "calculator.h"
#include <QWidget>
#include <QDebug>
#include <algorithm>
#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QScrollArea>
#include <QPushButton>
#include <QResizeEvent>

class mainPanel : public QWidget {
Q_OBJECT

public:
	mainPanel(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* pressing) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* release) override;
	void applyTheme(Colors theme);
	void resizeEvent(QResizeEvent* event) override;

	int currentPanelWidth = 600;
	int currentPanelHeight = 350;

	bool isExpanded = false;
	bool isDragging = false;
	bool isResizing = false;

	QPoint mousePosResize;
	QSize startSizeResize;

	QPoint mousePos;
	QRect startGeometry;
	QTabWidget* tabs;
	QGraphicsOpacityEffect* opacityEffect;

	drawingCanvas* canvas = nullptr;
	QScrollArea* scrollArea = nullptr;

	Colors currentTheme;
};
