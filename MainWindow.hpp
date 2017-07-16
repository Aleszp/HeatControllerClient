#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

#include <QtCore/QtGlobal>
#include <QtWidgets/QMainWindow>
#include <QtSerialPort/QSerialPort>

namespace Ui 
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

	public:
		explicit MainWindow(QWidget* parent = nullptr);
		~MainWindow();

	private:
		void setupRS(void);

	private:
		Ui::MainWindow *ui;
		QSerialPort *rs_;
		QWidget w_;
};

#endif
