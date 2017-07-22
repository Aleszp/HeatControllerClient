#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

#include <QtCore/QtGlobal>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
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
		void setupOkno(void);
		void setupTemperatura(void);
		void setupPrzycisk(void);

	private:
		Ui::MainWindow *ui;
		QSerialPort *rs232_;
		QWidget* okno_;
		QSpinBox* zadanaTemperatura_;
		QPushButton* wyslij_;
};

#endif
