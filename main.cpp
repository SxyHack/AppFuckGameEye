#include "MainWindow.h"

#include <stdio.h>
#include <Windows.h>
#include <winternl.h>

#include <QApplication>
#include "MainWindow.h"
#include "DarkStyle.h"
#include "framelesswindow.h"
#include "log/CustomLogMessageHandler.h"

#pragma comment(lib, "ntdll.lib")
// link: https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-adts/0a1ecc7f-1e7b-4f2c-9fdd-232c3c029338
// Specifies the privilege to manage auditing and the security log.
#define SE_SECURITY_PRIVILEGE 0x00000008

// Specifies the privilege to take ownership of objects. 
// Possession of this privilege overrides the DACL on an object and gives the possessor implicit RIGHT_WRITE_OWNER access.
#define SE_TAKE_OWNERSHIP_PRIVILEGE 0x00000009

// Specifies the privilege to restore objects.
#define SE_RESTORE_PRIVILEGE 0x00000012

// Specifies the privilege to debug the system.
#define SE_DEBUG_PRIVILEGE 0x00000014

// Specifies the privilege to enable accounts to be trusted for delegation.
#define SE_ENABLE_DELEGATION_PRIVILEGE 0x0000001B

EXTERN_C NTSTATUS NTAPI RtlAdjustPrivilege(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	CustomLogMessageHandler::Instance();
	qInstallMessageHandler(CustomLogMessageHandler::handle);
	
	BOOLEAN bl;
	NTSTATUS status = RtlAdjustPrivilege(SE_DEBUG_PRIVILEGE, TRUE, FALSE, &bl);

	if (NT_SUCCESS(status))
	{
		qDebug() << "提升App权限...OK";
	}

	// style our application with custom dark style
	QApplication::setStyle(new DarkStyle);
	//QApplication::setPalette(QApplication::style()->standardPalette());

	// create frameless window (and set windowState or title)
	FramelessWindow framelessWindow;
	framelessWindow.setWindowTitle("Fuck Game Eye");
	//framelessWindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));

	// create our mainwindow instance
	MainWindow mainWindow;

	// add the mainwindow to our custom frameless window
	framelessWindow.setContent(&mainWindow);
	framelessWindow.show();

	return a.exec();
}
