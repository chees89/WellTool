#include "mainPanel.h"
#include "themeLoader.h"
#include "saveTheme.h"
#include <QApplication>
#include <QWidget>
#include <QScreen>
#include <QStyleFactory>
#include <QIcon>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/icons/logo.ico"));
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	mainPanel panel;
	
	Colors changes = readChanges();

	if(changes == Dark) {
		QByteArray arr = openThemes(":/dark.qss");
		app.setStyleSheet(arr);
	}
	else {
		QByteArray arr = openThemes(":/light.qss");
		app.setStyleSheet(arr);
	}

	panel.show();
	return app.exec();
}
