#include "themeLoader.h"
#include <QFile>
#include <QDebug>

QByteArray openThemes(const QByteArray& path){
	QFile file(path);
	
	if(file.open(QIODevice::ReadOnly)) {
		QByteArray styleSheet = file.readAll();
		file.close();
		return styleSheet;
	}

	else {
		qDebug() << "Failed to open stylesheet " << path;
		return {};
	}
}
