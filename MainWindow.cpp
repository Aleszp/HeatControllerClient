#include <QtSerialPort/QSerialPortInfo>
#include <QtWidgets/QMessageBox>

#include <cstdlib>
#include <iostream>

#include "MainWindow.hpp"
#include "CustomDialog.hpp"

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent)
{
    setupRS();
    
    w_.resize(800,600);
    w_.show();
}
    
MainWindow::~MainWindow()
{
    rs_->close();
    delete rs_;
}

void MainWindow::setupRS(void)
{
	rs_=new QSerialPort(this);
	QStringList itemList;
	Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) 
    {
        itemList<<port.portName();
        //qDebug() << port.portName() << port.vendorIdentifier() << port.productIdentifier() << port.hasProductIdentifier() << port.hasVendorIdentifier() << port.isBusy();
    }
    if(itemList.isEmpty())
    {
		if (QMessageBox::Ok == QMessageBox(QMessageBox::Critical, "Brak portu szeregowego!", "Nie wykryto portu szeregowego! \nKoniec programu.", QMessageBox::Ok).exec()) 
		{
			exit(1);
		}
	}
	CustomDialog dialog(itemList);
	if (dialog.exec() == QDialog::Accepted)
	{
		rs_->setPortName(dialog.comboBox()->currentText());
	}
	
	rs_->open (QIODevice::ReadWrite);
	rs_->setBaudRate (QSerialPort::Baud57600);
	rs_->setDataBits (QSerialPort::Data8);
	rs_->setStopBits (QSerialPort::OneStop);
	rs_->setParity (QSerialPort::NoParity);
	rs_->setFlowControl (QSerialPort::NoFlowControl);
    std::cerr<<rs_->error()<<std::endl;
}

#include "MainWindow.moc"
