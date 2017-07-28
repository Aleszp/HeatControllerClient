#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

#include <QtCore/QtGlobal>
#include <QtCore/QVector>
#include <QtSerialPort/QSerialPort>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QBoxLayout>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_point_data.h>

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
		bool wyslijRozkaz(const char* rozkaz);

	private:
		void setupRS(void);
		void setupOkno(void);
		void setupCzasTemperatura(void);		
		void setupTemperatura(void);
		void setupWyslij(void);
		void setupZatrzymajGrzanie(void);
		void setupReset(void);
		void setupWykresChwilowy(void);
		void setupWykresDlugookresowy(void);
		void setupRozklad(void);
		bool obsluzBladRS(QSerialPort::SerialPortError kod_bledu);

	private:
		Ui::GlowneOkno *ui;
		QWidget* okno_;
		QVBoxLayout* glownyRozmieszczacz_;
		QHBoxLayout* wiersze_;
		unsigned iloscWierszy_;
		
		QSerialPort *rs232_;

		QSpinBox* zadanaTemperatura_;
		QPushButton* wyslij_;
		QPushButton* zatrzymajGrzanie_;
		QPushButton* reset_;
		
		QVector <double> czasChwilowy_;
		QVector <double> temperaturaChwilowa_;
		QVector <double> czasDlugookresowy_;
		QVector <double> temperaturaDlugookresowa_;
		
		QwtPlot* wykresChwilowy_;
		QwtPlot* wykresDlugookresowy_;
		QwtPlotCurve* danePomiaroweWykresChwilowy_;
		QwtPlotCurve* danePomiaroweWykresDlugookresowy_;
		
	public slots:
		void ustawTemperature(void);
		void zatrzymajGrzanie(void);
		void odbierzDane(void);
		void zrestartujUrzadenie(void);
};

#endif
