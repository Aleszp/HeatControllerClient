#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

#include <QtCore/QtGlobal>
#include <QtSerialPort/QSerialPort>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>


namespace Ui 
{
	class GlowneOkno;
}

class GlowneOkno : public QMainWindow
{
    Q_OBJECT

	public:
		explicit GlowneOkno(QWidget* parent = nullptr);
		~GlowneOkno();

	private:
		void setupRS(void);
		void setupOkno(void);
		void setupTemperatura(void);
		void setupPrzycisk(void);
		void setupWykres(void);

	private:
		Ui::GlowneOkno *ui;
		QSerialPort *rs232_;
		QWidget* okno_;
		QSpinBox* zadanaTemperatura_;
		QPushButton* wyslij_;
};

#endif
