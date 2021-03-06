#include "DriverControlWindow.h"
#include "core/driver/DriverControl.h"
#include "settings/GlobalSettings.h"

#include <QMessageBox>
#include <QtDebug>
#include <QFileDialog>

DriverControlWindow::DriverControlWindow(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.selectDriverBtn, SIGNAL(clicked()), this, SLOT(onClickSelectFile()));
	connect(ui.installDriverBtn, SIGNAL(clicked()), this, SLOT(onClickInstallDriver()));
	connect(ui.startDriverBtn, SIGNAL(clicked()), this, SLOT(onClickStartDriver()));
	connect(ui.stopDriverBtn, SIGNAL(clicked()), this, SLOT(onClickStopDriver()));
	connect(ui.uninstallDriverBtn, SIGNAL(clicked()), this, SLOT(onClickUninstallDriver()));

	connect(ui.testHookBtn, SIGNAL(clicked()), this, SLOT(onClickTestHook()));
	connect(ui.testHook2Btn, SIGNAL(clicked()), this, SLOT(onClickTestHook2()));
}

DriverControlWindow::~DriverControlWindow()
{
	qDebug() << "~DriverControlWindow";
}

#include <string_view>
#include <string>

void DriverControlWindow::showEvent(QShowEvent* event)
{
	Q_UNUSED(event);
	QString driverFilePath;
	GSetting.GetDriverBinaryFilePath(driverFilePath);

	if (driverFilePath.isEmpty())
		return;

	_DriverLoader.SetDriverBinaryPath(driverFilePath);
	ui.selectDriverPath->setText(driverFilePath);
	onClickInstallDriver();
	onClickStartDriver();
}

void DriverControlWindow::onClickSelectFile()
{
	QString appPath = QApplication::applicationDirPath();
	QString selectedFilePath = QFileDialog::getOpenFileName(this, "选择驱动文件", appPath, "driver files(*.sys)");
	ui.selectDriverPath->setText(selectedFilePath);

	if (selectedFilePath.isEmpty())
		return;

	_DriverLoader.SetDriverBinaryPath(selectedFilePath);
	GSetting.SetDriverBinaryFilePath(selectedFilePath);

	onClickInstallDriver();
	onClickStartDriver();
}

void DriverControlWindow::onClickInstallDriver()
{
	if (_DriverLoader.Install())
	{
		ui.driverStatus->setText("安装成功");
		ui.driverStatus->setStyleSheet("QLabel { color: green }");
	}
	else
	{
		ui.driverStatus->setText("安装失败");
		ui.driverStatus->setStyleSheet("QLabel { color: red }");
	}
}

void DriverControlWindow::onClickUninstallDriver()
{
	if (_DriverLoader.Uninstall())
	{
		ui.driverStatus->setText("卸载成功");
		ui.driverStatus->setStyleSheet("QLabel { color: green }");
	}
	else
	{
		ui.driverStatus->setText("卸载失败");
		ui.driverStatus->setStyleSheet("QLabel { color: red }");
	}
}

void DriverControlWindow::onClickStartDriver()
{
	if (_DriverLoader.Start())
	{
		ui.driverStatus->setText("运行中...");
		ui.driverStatus->setStyleSheet("QLabel { color: green }");
	}
	else
	{
		ui.driverStatus->setText("无法运行");
		ui.driverStatus->setStyleSheet("QLabel { color: red }");
	}
}

void DriverControlWindow::onClickStopDriver()
{
	if (_DriverLoader.Stop())
	{
		ui.driverStatus->setText("已停止");
		ui.driverStatus->setStyleSheet("QLabel { color: green }");
	}
	else
	{
		ui.driverStatus->setText("无法停止");
		ui.driverStatus->setStyleSheet("QLabel { color: red }");
	}
}


void DriverControlWindow::onClickTestHook()
{
	DriverControl control("FuckGameEye");
	if (!control.Connect())
	{
		return;
	}

	if (!control.SetupSSDTHook())
	{
		QMessageBox::warning(this, "警告", "测试 SSDT HOOK 失败");
		return;
	}
}

void DriverControlWindow::onClickTestHook2()
{
	DriverControl control("FuckGameEye");
	if (!control.Connect())
	{
		return;
	}

	if (!control.SetupInfinityHook())
	{
		QMessageBox::warning(this, "警告", "测试 Infinity HOOK 失败");
		return;
	}

}
