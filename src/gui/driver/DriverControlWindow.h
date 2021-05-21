#pragma once

#include <QWidget>
#include "ui_DriverControlWindow.h"

#include "core/driver/DriverLoader.h"

class DriverControlWindow : public QWidget
{
	Q_OBJECT

public:
	DriverControlWindow(QWidget* parent = Q_NULLPTR);
	~DriverControlWindow();

protected:
	void showEvent(QShowEvent* event) override;

private slots:
	void onClickSelectFile();
	void onClickInstallDriver();
	void onClickUninstallDriver();
	void onClickStartDriver();
	void onClickStopDriver();

	void onClickTestHook();
	void onClickTestHook2();

private:
	Ui::DriverControlWindow ui;
	DriverLoader _DriverLoader;
};
