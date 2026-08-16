#pragma once
#include <QSettings>

enum Colors {
	Dark,
	Light
};

void saveChanges(Colors theme);
Colors readChanges();
