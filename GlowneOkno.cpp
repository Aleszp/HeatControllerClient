#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug> 
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>

#include <cstdlib>
#include <iostream>
#include <cstdlib>

#include "GlowneOkno.hpp"
#include "WyborPortu.hpp"

GlowneOkno::GlowneOkno(QWidget* parent):QMainWindow(parent)
{
	setupRS();
	
	setupOkno();
    setupRozklad();
    setupTemperatura();
    setupWyslij();
    setupWykres();
    
    okno_->show();
}
    
GlowneOkno::~GlowneOkno()
{
    rs232_->close();
    delete rs232_;
    delete zadanaTemperatura_;
    delete wyslij_;
    delete wykres_;
    delete okno_;
}

void GlowneOkno::setupOkno(void)
{
	okno_=new QWidget();
	okno_->resize(800,600);
	okno_->setWindowTitle("Kontroler temperatury");
}

void GlowneOkno::setupRozklad(void)
{
	glownyRozmieszczacz_=new QVBoxLayout(okno_);
	okno_->setLayout(glownyRozmieszczacz_);
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
			
			QMessageBox pytanie(QMessageBox::Warning, "Brak portu szeregowego!", "Nie wykryto żadnego portu szeregowego! \nSprawdź czy port jest dostępny i podejmij ponowną próbę konfiguracji lub zakończ program.", QMessageBox::Abort|QMessageBox::Retry);
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
	
	int wynik;
	do
	{
		wynik=dialog.exec();
		rs232_->setPortName(dialog.poleKombi()->currentText());
		if(wynik==QDialog::Rejected)
		{
			QMessageBox(QMessageBox::Critical, "Nie wybrano portu szeregowego!", "Koniec programu.", QMessageBox::Ok).exec();
			exit(2);
		}
	}
	while(wynik != QDialog::Accepted);
	
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
	zadanaTemperatura_=new QSpinBox;
	glownyRozmieszczacz_->addWidget(zadanaTemperatura_);
    zadanaTemperatura_->setRange(0, 999);
    zadanaTemperatura_->setSingleStep(1);
    zadanaTemperatura_->setSuffix(" ℃");   
}

void GlowneOkno::setupWyslij(void)
{
	wyslij_=new QPushButton("Ustaw");
	wyslij_->setFixedSize(100,20);
	glownyRozmieszczacz_->addWidget(wyslij_);
}

void GlowneOkno::setupZatrzymajGrzanie(void)
{
	zatrzymajGrzanie_=new QPushButton("ZatrzymajGrzanie");
	zatrzymajGrzanie_->setFixedSize(100,20);
	glownyRozmieszczacz_->addWidget(zatrzymajGrzanie_);
}

void GlowneOkno::setupWykres(void)
{
	wykres_=new QwtPlot;
	glownyRozmieszczacz_->addWidget(wykres_);
	wykres_->setTitle ("Temperatura");
	wykres_->setAxisTitle (QwtPlot::xBottom, "Czas /s");
	wykres_->setAxisTitle (QwtPlot::yLeft, "Temperatura /℃");
	//wykres_->setSizePolicy();
	wykres_->setCanvasBackground(QBrush (Qt::white));
	wykres_->setAxisScale (QwtPlot::xBottom, 0, 120);
	wykres_->setAxisScale (QwtPlot::yLeft, 0, 800);
}

#include "GlowneOkno.moc"
