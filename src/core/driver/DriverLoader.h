#pragma once

#include <QObject>
#include <windows.h>

/**
 * 内核驱动的安装, 卸载, 管理
 */
class DriverLoader : public QObject
{
	Q_OBJECT

public:
	DriverLoader();
	DriverLoader(const QString& binaryPath);
	DriverLoader(const QString& display, const QString& binaryPath);
	~DriverLoader();

	// 设置驱动文件的路径
	void SetDriverBinaryPath(const QString& path);

	BOOLEAN Install();
	BOOLEAN Uninstall();
	BOOLEAN Start();
	BOOLEAN Stop();

private:
	BOOLEAN openServiceMgr(SC_HANDLE& handle);
	BOOLEAN openService(SC_HANDLE& mgrHandle, SC_HANDLE& schandle);

private:
	QString   _ServiceName;
	QString   _DriverDisplayName;
	QString   _DriverBinaryPath;
	DWORD     _LastError;
};
