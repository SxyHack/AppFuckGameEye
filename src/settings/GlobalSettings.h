#pragma once

#include <QSettings>

class GlobalSettings
{
public:
	static GlobalSettings& Instance();

	void SetDriverBinaryFilePath(const QString& path);
	void GetDriverBinaryFilePath(QString& path);
protected:
	GlobalSettings();

private:
	QSettings* mSettings;
};


#define GSetting GlobalSettings::Instance()