#include <QtSerialPort/QSerialPortInfo>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtCore/QDebug> 

#include <cstdlib>
#include <iostream>
#include <cstdlib>

#include "MainWindow.hpp"
#include "CustomDialog.hpp"

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent)
{
	setupRS();
	
	setupOkno();
    setupTemperatura();
    setupPrzycisk();
    
    okno_->show();
}
    
MainWindow::~MainWindow()
{
    rs232_->close();
    delete rs232_;
    delete zadanaTemperatura_;
    delete wyslij_;
    delete okno_;
}

void MainWindow::setupOkno(void)
{
	okno_=new QWidget();
	okno_->resize(800,600);
}

void MainWindow::setupRS(void)
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
	CustomDialog dialog(itemList);
	if (dialog.exec() == QDialog::Accepted)
	{
		rs232_->setPortName(dialog.comboBox()->currentText());
	}
	
	rs232_->open (QIODevice::ReadWrite);
	rs232_->setBaudRate (QSerialPort::Baud57600);
	rs232_->setDataBits (QSerialPort::Data8);
	rs232_->setStopBits (QSerialPort::OneStop);
	rs232_->setParity (QSerialPort::NoParity);
	rs232_->setFlowControl (QSerialPort::NoFlowControl);
    std::cerr<<rs232_->error()<<std::endl;
}

void MainWindow::setupTemperatura(void)
{
	zadanaTemperatura_=new QSpinBox(okno_);
    zadanaTemperatura_->setRange(0, 999);
    zadanaTemperatura_->setSingleStep(1);
    zadanaTemperatura_->setSuffix(" ℃");   
}

void MainWindow::setupPrzycisk(void)
{
	wyslij_=new QPushButton("Ustaw",okno_);
}

#include "MainWindow.moc"
