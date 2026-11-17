#include "mainPanel.h"

mainPanel::mainPanel(QWidget* parent) : QWidget(parent) {
	const int collapsedWidth = 20;
	const int collapsedHeight = 50;

	currentTheme = readChanges();

	setWindowFlags(
			Qt::FramelessWindowHint |
			Qt::WindowStaysOnTopHint
		);

	resize(collapsedWidth, collapsedHeight);

	QSize screen = screenDefinition();
	int width = screen.width();
	int height = screen.height();

	move(width - collapsedWidth, height - collapsedHeight);

	opacityEffect = new QGraphicsOpacityEffect();

	tabs = new QTabWidget(this);
	tabs->setGraphicsEffect(opacityEffect);
	tabs->hide();

	scrollArea = new QScrollArea();

	QPushButton* eraserButton = new QPushButton("Eraser");
	QPushButton* chooseColorButton = new QPushButton("Choose Color");
	QPushButton* saveArt = new QPushButton("Save");

	eraserButton->setCheckable(true);

	CalculatorGui* calculator = new CalculatorGui();
	canvas = new drawingCanvas();
	QWidget* canvasTabContent = new QWidget();
	notesWidget* notes = new notesWidget();
	QWidget* themes = new QWidget();

	scrollArea->setWidget(canvas);
	canvas->resize(580, 1000);

	scrollArea->setWidgetResizable(false);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QLabel* label = new QLabel("Enable light theme");
	QCheckBox* checkBox = new QCheckBox();
	QHBoxLayout* boxLayout = new QHBoxLayout();
	QHBoxLayout* buttonsLayout = new QHBoxLayout();

	boxLayout->setSpacing(10);

	boxLayout->addWidget(checkBox);
	boxLayout->addWidget(label);

	buttonsLayout->addWidget(eraserButton);
	buttonsLayout->addWidget(chooseColorButton);
	buttonsLayout->addWidget(saveArt);

	themes->setLayout(boxLayout);

	if(currentTheme == Dark) checkBox->setChecked(false);
	else checkBox->setChecked(true);

	connect(checkBox, &QCheckBox::toggled, [this](bool checked) {
    			if(checked) applyTheme(Light);
    			else applyTheme(Dark);
		});

	connect(chooseColorButton, &QPushButton::clicked, [=] {
		canvas->setColor();
	});

	connect(saveArt, &QPushButton::clicked, [=] {
		canvas->saveAsImage();
	});

	tabs->addTab(notes, "Notes");
	tabs->addTab(canvasTabContent, "Canvas");
	tabs->addTab(calculator, "Calculator");
	tabs->addTab(themes, "Themes");

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(tabs);

	QVBoxLayout* canvasLayout = new QVBoxLayout();
	canvasLayout->addLayout(buttonsLayout);
	canvasLayout->addWidget(scrollArea);

	canvasTabContent->setLayout(canvasLayout);

	connect(eraserButton, &QPushButton::clicked, [this]() {
			canvas->toggleEraserMode();
		});

	layout->setSizeConstraint(QLayout::SetNoConstraint);

	this->setLayout(layout);
}

void mainPanel::resizeEvent(QResizeEvent* event) {
	if(!canvas || !scrollArea) return;

	auto scrollAreaVitalWidth = scrollArea->viewport()->width();
	auto currentHeight = std::max(scrollArea->viewport()->height(), canvas->height());

	canvas->resize(scrollAreaVitalWidth, currentHeight);

}

void mainPanel::mousePressEvent(QMouseEvent* pressing) {
	if(pressing->pos().x() < 10 &&
	   pressing->pos().y() > this->height() - 10 &&
	   isExpanded) {

		isResizing = true;
		mousePosResize = QCursor::pos();
		startSizeResize = this->size();
	   }
	else {
		mousePos = QCursor::pos();
		startGeometry = this->geometry();
		isDragging = false;
	}
}

void mainPanel::applyTheme(Colors theme) {
	currentTheme = theme;
	QByteArray path;
	QApplication *app = static_cast<QApplication*>(QApplication::instance());

	saveChanges(theme);

	if(theme == Dark) path = ":/dark.qss";
	else path = ":/light.qss";

	QByteArray arr = openThemes(path);

	app->setStyleSheet(arr);

}

void mainPanel::mouseMoveEvent(QMouseEvent* event) {
	if(isResizing) {
		QPoint globalMousePos = QCursor::pos();
		QPoint differenceInPos = globalMousePos - mousePosResize;

		int newWidth = startSizeResize.width() - differenceInPos.x();
		int newHeight = startSizeResize.height() + differenceInPos.y();

		const int clampedWidth = std::clamp(newWidth, 200, 800);
		const int clampedHeight = std::clamp(newHeight, 200, 650);

		auto new_x = screenDefinition().width() - clampedWidth;

		setGeometry(new_x, this->y(), clampedWidth, clampedHeight);
	}

	else {
	const int collapsedWidth = 20;

		isDragging = true;
		QPoint globalMousePos = QCursor::pos();
		QPoint differenceInPos = globalMousePos - mousePos;

		int mouseY = differenceInPos.y();
		int geometryY = startGeometry.y();
		int targetY = mouseY + geometryY;

		auto screen = screenDefinition();

		auto screenHeight = screen.height();
		auto windowHeight = this->height();

		auto maximum =  screenHeight - windowHeight;
		auto clampedY = std::clamp(targetY, 0, maximum);

		move(screen.width() - this->width(), clampedY);
	}
}

void mainPanel::mouseReleaseEvent(QMouseEvent* release) {
	if(isResizing) {
		isResizing = false;
		return;
	}

	if(!isDragging) {
		const int collapsedWidth = 20;
		const int collapsedHeight = 50;
		const int halfHeight = currentPanelHeight / 2;
		const int halfCollapsedHeight = collapsedHeight / 2;

		auto geometry = this->geometry();

		isExpanded = !isExpanded;

		QSize screen = screenDefinition();
		const int width = screen.width();
		const int height = screen.height();

		QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
		animation->setDuration(300);

		animation->setStartValue(geometry);

		QPoint center = geometry.center();
		auto centerY = center.y();
		auto newYExpanded = centerY - halfHeight;
		auto newYFolded = centerY - halfCollapsedHeight;

		auto limitationExpanded = std::clamp(newYExpanded, 0, height - currentPanelHeight);
		auto limitationFolded = std::clamp(newYFolded, 0, height - collapsedHeight);

		if(isExpanded == true) {
			QRect targetGeometryExpanded = QRect(width-currentPanelWidth, limitationExpanded, currentPanelWidth, currentPanelHeight);
			animation->setEndValue(targetGeometryExpanded);
			QPropertyAnimation* fadeAnimation = new QPropertyAnimation(opacityEffect, "opacity");

			connect(animation, &QPropertyAnimation::finished, [=] {
					fadeAnimation->setDuration(150);
					fadeAnimation->setStartValue(0.0);
					fadeAnimation->setEndValue(1.0);

					tabs->show();

					fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
					});

			animation->start(QAbstractAnimation::DeleteWhenStopped);

		}

		else {
			QPropertyAnimation* fadeAnimation = new QPropertyAnimation(opacityEffect, "opacity");

			fadeAnimation->setDuration(150);
			fadeAnimation->setStartValue(1.0);
			fadeAnimation->setEndValue(0.0);

			currentPanelWidth = this->width();
			currentPanelHeight = this->height();

			connect(fadeAnimation, &QPropertyAnimation::finished, [=] {
						QRect targetGeometryFolded(width-collapsedWidth, limitationFolded, collapsedWidth, collapsedHeight);
						animation->setEndValue(targetGeometryFolded);

						animation->start(QAbstractAnimation::DeleteWhenStopped);
					});

			fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);

		}
	}
}
