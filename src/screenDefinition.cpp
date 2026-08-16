#include "screenDefinition.h"
#include <QScreen>
#include <QDebug>

QSize screenDefinition() {
       QScreen* screen = QGuiApplication::primaryScreen();
       QSize screenSize = screen->size();

       return screenSize;
}
