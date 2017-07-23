#include <QtSerialPort/QSerialPortInfo>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtCore/QDebug> 
#include <qwt/qwt_plot.h>

#include <cstdlib>
#include <iostream>
#include <cstdlib>

#include "GlowneOkno.hpp"
#include "WyborPortu.hpp"

GlowneOkno::GlowneOkno(QWidget* parent):QMainWindow(parent)
{
	setupRS();
	
	setupOkno();
    setupTemperatura();
    setupPrzycisk();
    setupWykres();
    
    okno_->show();
}
    
GlowneOkno::~GlowneOkno()
{
    rs232_->close();
    delete rs232_;
    delete zadanaTemperatura_;
    delete wyslij_;
    delete okno_;
}

void GlowneOkno::setupOkno(void)
{
	okno_=new QWidget();
	okno_->resize(800,600);
	okno_->setWindowTitle("Kontroler temperatury");
}

void GlowneOkno::setupRS(void)
{
	rs232_=new QSerialPort(this);
	QStringList itemList;
	
	do
	{
		itemList.clear();
		Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) 
		{
			itemList<<port.portName();
			qDebug() << port.portName() << port.vendorIdentifier() << port.productIdentifier() << port.hasProductIdentifier() << port.hasVendorIdentifier() << port.isBusy();
		}
		if(itemList.isEmpty())
		{
			int wybor;
			
			QMessageBox pytanie(QMessageBox::Question, "Brak portu szeregowego!", "Nie wykryto żadnego portu szeregowego! \nSprawdź czy port jest dostępny i podejmij ponowną próbę konfiguracji lub zakończ program.", QMessageBox::Abort|QMessageBox::Retry);
			wybor=pytanie.exec();
			
			if (wybor==QMessageBox::Abort) 
			{
				QMessageBox(QMessageBox::Critical, "Brak portu szeregowego!", "Koniec programu.", QMessageBox::Ok).exec();
				exit(1);
			}
		}
	}
	while(itemList.isEmpty());
	WyborPortu dialog(itemList);
	if (dialog.exec() == QDialog::Accepted)
	{
		rs232_->setPortName(dialog.poleKombi()->currentText());
	}
	
	rs232_->open (QIODevice::ReadWrite);
	rs232_->setBaudRate (QSerialPort::Baud57600);
	rs232_->setDataBits (QSerialPort::Data8);
	rs232_->setStopBits (QSerialPort::OneStop);
	rs232_->setParity (QSerialPort::NoParity);
	rs232_->setFlowControl (QSerialPort::NoFlowControl);
    std::cerr<<rs232_->error()<<std::endl;
}

void GlowneOkno::setupTemperatura(void)
{
	zadanaTemperatura_=new QSpinBox(okno_);
    zadanaTemperatura_->setRange(0, 999);
    zadanaTemperatura_->setSingleStep(1);
    zadanaTemperatura_->setSuffix(" ℃");   
}

void GlowneOkno::setupPrzycisk(void)
{
	wyslij_=new QPushButton("Ustaw",okno_);
}

void GlowneOkno::setupWykres(void)
{
	QwtPlot wykres (okno_);
	wykres.setTitle ("Temperatura");
	wykres.setAxisTitle (QwtPlot::xBottom, "Czas /s");
	wykres.setAxisTitle (QwtPlot::yLeft, "Temperatura /℃");
	wykres.setFixedSize (700, 500);
	wykres.setCanvasBackground(QBrush (QColor (0xff,0xfa, 0x6b)));
	wykres.setAxisScale (QwtPlot::xBottom, 0, 100);
	wykres.setAxisScale (QwtPlot::yLeft, 0, 800);
}

#include "GlowneOkno.moc"
