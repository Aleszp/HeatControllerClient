#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

#include <QtCore/QtGlobal>
#include <QtSerialPort/QSerialPort>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QBoxLayout>

#include <qwt/qwt_plot.h>

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
		void wyslijRozkaz(const char* rozkaz);

	private:
		void setupRS(void);
		void setupOkno(void);
		void setupTemperatura(void);
		void setupWyslij(void);
		void setupZatrzymajGrzanie(void);
		void setupWykres(void);
		void setupRozklad(void);
		void obsluzBladRS(QSerialPort::SerialPortError blad);

	private:
		Ui::GlowneOkno *ui;
		QSerialPort *rs232_;
		QWidget* okno_;
		QSpinBox* zadanaTemperatura_;
		QPushButton* wyslij_;
		QPushButton* zatrzymajGrzanie_;
		QwtPlot* wykres_;
		QVBoxLayout* glownyRozmieszczacz_;
		
	public slots:
		void ustawTemperature(void);
		void odbierzDane(void);
};

#endif
