#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtCore/QStringList>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <iostream>

using namespace std;

class CustomDialog : public QDialog
{
	public:
    CustomDialog(const QStringList& items)
    {
        setLayout(new QHBoxLayout());

        box = new QComboBox;
        box->addItems(items);
        layout()->addWidget(box);
		setWindowTitle("Wybierz port");
		resize(300,50);
        QPushButton* ok = new QPushButton("ok");
        layout()->addWidget(ok);
        connect(ok, &QPushButton::clicked, this, [this]()
        {
           accept();
        });
    }

    QComboBox* comboBox() { return box; }

	private:
    QComboBox* box;
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSerialPort rs;
	QStringList itemList;
	Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) 
    {
        itemList<<port.portName();
        qDebug() << port.portName() << port.vendorIdentifier() << port.productIdentifier() << port.hasProductIdentifier() << port.hasVendorIdentifier() << port.isBusy();
    }
	CustomDialog dialog(itemList);
	if (dialog.exec() == QDialog::Accepted)
	{
		rs.setPortName(dialog.comboBox()->currentText());
	}
	
    
    QWidget w;
    w.resize(800,600);
    w.show();

    rs.open (QIODevice::ReadWrite);
	rs.setBaudRate (QSerialPort::Baud57600);
	rs.setDataBits (QSerialPort::Data8);
	rs.setStopBits (QSerialPort::OneStop);
	rs.setParity (QSerialPort::NoParity);
	rs.setFlowControl (QSerialPort::NoFlowControl);
    
    cout<<rs.error()<<endl;
    
    rs.close();
    return a.exec();
}
