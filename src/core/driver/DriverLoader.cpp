#include "DriverLoader.h"
#include <QtDebug>
#include <QFileInfo>

DriverLoader::DriverLoader(const QString& display, const QString& binaryPath)
	: QObject(nullptr)
	, _DriverDisplayName(display)
	, _DriverBinaryPath(binaryPath)
{
}

DriverLoader::DriverLoader(const QString& binaryPath)
	: QObject(nullptr)
	, _DriverBinaryPath(binaryPath)
{
	QFileInfo fileInfo(binaryPath);
	_DriverDisplayName = fileInfo.completeBaseName();
}

DriverLoader::DriverLoader()
	: QObject(nullptr)
{
}

DriverLoader::~DriverLoader()
{
}

void DriverLoader::SetDriverBinaryPath(const QString& path)
{
	QFileInfo fileInfo(path);
	_DriverBinaryPath = path;
	_DriverDisplayName = fileInfo.completeBaseName();
}

BOOLEAN DriverLoader::Install()
{
	qDebug() << "驱动安装...";

	SC_HANDLE hMgrHandle;
	SC_HANDLE hService;

	LPCTSTR lpDriverDisplayName = (LPCTSTR)_DriverDisplayName.utf16();
	LPCTSTR lpDriverBinaryPath = (LPCTSTR)_DriverBinaryPath.utf16();

	if (lpDriverDisplayName == NULL || lpDriverBinaryPath == NULL)
	{
		qCritical() << "驱动名字or服务名字为空, "
			<< "驱动名字:" << lpDriverDisplayName
			<< "服务名字:" << lpDriverBinaryPath;
		return FALSE;
	}

	if (openService(hMgrHandle, hService)) {
		qWarning() << "驱动已经安装, " << _DriverBinaryPath.toUtf8().constData();
		return TRUE;
	}

	hService = CreateService(
		hMgrHandle,             // handle of service control manager database
		lpDriverDisplayName,    // address of name of service to start
		lpDriverDisplayName,    // address of display name
		SERVICE_ALL_ACCESS,     // type of access to service
		SERVICE_KERNEL_DRIVER,  // type of service
		SERVICE_DEMAND_START,   // when to start service
		SERVICE_ERROR_NORMAL,   // severity if service fails to start
		lpDriverBinaryPath,     // address of name of binary file
		NULL,                   // service does not belong to a group
		NULL,                   // no tag requested
		NULL,                   // no dependency names
		NULL,                   // use LocalSystem account
		NULL                    // no password for service account
	);

	if (hService == NULL)
	{
		qCritical() << "驱动安装...FAIL, 错误代码:" << GetLastError();
		return FALSE;
	}

	qInfo() << "驱动安装...OK";
	return TRUE;
}

BOOLEAN DriverLoader::Uninstall()
{
	Stop();

	qDebug() << "驱动卸载...";

	SC_HANDLE hMgrHandle;
	SC_HANDLE hService;

	if (!openService(hMgrHandle, hService))
		return FALSE;

	BOOLEAN ret = DeleteService(hService);

	CloseServiceHandle(hService);
	CloseServiceHandle(hMgrHandle);

	if (!ret)
	{
		qCritical() << "驱动卸载...FAIL, 错误:" << GetLastError();
		return FALSE;
	}

	qInfo() << "驱动卸载...OK";

	return TRUE;
}

BOOLEAN DriverLoader::Start()
{
	qDebug() << "驱动启动...";

	SC_HANDLE hMgrHandle;
	SC_HANDLE hService;

	if (!openService(hMgrHandle, hService))
		return FALSE;

	BOOLEAN ret = StartService(hService, 0, NULL);

	// free handle
	CloseServiceHandle(hService);
	CloseServiceHandle(hMgrHandle);

	if (!ret)
	{
		DWORD err = GetLastError();
		if (err == ERROR_SERVICE_ALREADY_RUNNING)
		{
			qWarning() << "驱动已经启动!";
			return TRUE;
		}
		else
		{
			qCritical() << "驱动启动...FAIL 错误:" << err;
			return FALSE;
		}
	}

	qInfo() << "驱动启动...OK";
	return TRUE;
}

BOOLEAN DriverLoader::Stop()
{
	qDebug() << "驱动停止...";
	SC_HANDLE hMgrHandle;
	SC_HANDLE hService;
	SERVICE_STATUS status;

	if (!openService(hMgrHandle, hService))
		return FALSE;

	BOOLEAN ret = ControlService(hService, SERVICE_CONTROL_STOP, &status);

	// free handle
	CloseServiceHandle(hService);
	CloseServiceHandle(hMgrHandle);

	if (!ret)
	{
		qCritical() << "驱动停止...FAIL,错误" << GetLastError();
		return FALSE;
	}

	qInfo() << "驱动停止...OK";
	return TRUE;
}


BOOLEAN DriverLoader::openServiceMgr(SC_HANDLE& handle)
{
	handle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	return handle != NULL;
}

BOOLEAN DriverLoader::openService(SC_HANDLE& mgrHandle, SC_HANDLE& scHandle)
{
	LPCTSTR driverName = (LPCTSTR)_DriverDisplayName.utf16();
	if (driverName == NULL)
	{
		qCritical() << "驱动名字为空, 不能卸载!";
		return FALSE;
	}

	if (!openServiceMgr(mgrHandle))
	{
		qCritical() << "获取服务管理句柄失败";
		return FALSE;
	}

	scHandle = OpenService(mgrHandle, driverName, SERVICE_ALL_ACCESS);
	if (scHandle == NULL)
	{
		qCritical() << "OpenService Failed! Error=" << GetLastError();

		return FALSE;
	}

	return TRUE;
}
