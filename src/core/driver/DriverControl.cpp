#include "DriverControl.h"
#include "ioctlcfg.h"

#include <QtDebug>

DriverControl::DriverControl(const QString& devName)
	: QObject(nullptr)
	, _DeviceName(devName)
	, _DeviceHandle(NULL)
{
}

DriverControl::~DriverControl()
{
	if (_DeviceHandle)
	{
		CloseHandle(_DeviceHandle);
	}
}

BOOL DriverControl::Connect()
{
	qDebug() << "链接内核驱动...";

	QString fullName = QString("\\\\.\\%1").arg(_DeviceName);
	LPCTSTR lpDevName = (LPCTSTR)fullName.utf16();

	_DeviceHandle = CreateFile(lpDevName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == _DeviceHandle)
	{
		DWORD error = GetLastError();
		if (ERROR_FILE_NOT_FOUND == error)
		{
			qCritical() << "CreateFile() Failed: " << error;
			return FALSE;
		}
	}

	qInfo() << "链接内核驱动...OK";

	return TRUE; 
}

BOOL DriverControl::SetupSSDTHook()
{
	QByteArray params("App: Setup Hook...");
	char results[256] = { 0 };
	DWORD bytesReturned = 0;

	BOOL ret = DeviceIoControl(_DeviceHandle,
		IOCTL_FGE_HOOK_SSDT,
		(LPVOID)params.constData(),
		params.length(),
		results,
		sizeof(results),
		&bytesReturned,
		NULL
	);

	if (!ret) 
	{
		qCritical() << "DeviceIoControl()发生错误, " << GetLastError();
		return FALSE;
	}

	qDebug() << "内核返回:" << results;

	return TRUE;
}

BOOL DriverControl::SetupInfinityHook()
{
	QByteArray params("App: Setup Hook...");
	char results[256] = { 0 };
	DWORD bytesReturned = 0;

	BOOL ret = DeviceIoControl(_DeviceHandle,
		IOCTL_FGE_HOOK_INF,
		NULL,
		0,
		results,
		sizeof(results),
		&bytesReturned,
		NULL
	);

	if (!ret)
	{
		qCritical() << "DeviceIoControl()发生错误, " << GetLastError();
		return FALSE;
	}

	qDebug() << "内核返回:" << results;
	return TRUE;
}
