#include "saveTheme.h"
#include <QSettings>

void saveChanges(Colors theme) {
	QSettings settings("NothingWellOrg", "WeelTool");
	int themeNumber = static_cast<int>(theme);

	settings.setValue("theme", themeNumber);
}

Colors readChanges() {
	QSettings settings("NothingWellOrg", "WeelTool");
	int read = settings.value("theme", static_cast<int>(Dark)).toInt();
	
	Colors result = static_cast<Colors>(read);
	return result;
}
