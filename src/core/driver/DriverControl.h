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
	BOOL SetupSSDTHook();
	BOOL SetupInfinityHook();

private:
	QString _DeviceName;
	HANDLE  _DeviceHandle;
};
