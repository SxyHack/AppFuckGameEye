#pragma once

#include <QObject>
#include <windows.h>

class DriverControl : public QObject
{
	Q_OBJECT

public:
	DriverControl(const QString& devName);
	~DriverControl();

	BOOL Connect();
	BOOL SetupHooks();

private:
	QString _DeviceName;
	HANDLE  _DeviceHandle;
};
