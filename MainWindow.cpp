#include "MainWindow.h"
#include "framelesswindow.h"
#include "gui/driver/DriverControlWindow.h"

#include <QtDebug>
#include <QDialog>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//ui.menubar->setVisible(false);
	connect(ui.actionDriver, SIGNAL(triggered(bool)), this, SLOT(onMenuActionDriver(bool)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::onMenuActionDriver(bool trigged)
{
	Q_UNUSED(trigged);

	FramelessWindow* framelessWindow = new FramelessWindow;
	framelessWindow->setWindowTitle("驱动管理");

	DriverControlWindow* dc = new DriverControlWindow();

	framelessWindow->setContent(dc);
	framelessWindow->show();
}
