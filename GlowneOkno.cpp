#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug> 
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>

#include <cstdlib>
#include <iostream>

#include "GlowneOkno.hpp"
#include "WyborPortu.hpp"

const std::string bledy[14]=
{
	"Brak błędu",
	"Nie znaleziono urządzenia",
	"Brak uprawnień (być może port jest używany przez inny program)",
	"Port jest już otwarty",
	"Błąd parzystości",
	"Błąd ramki",
	"Błąd stanu przerwy (rodzaj błędu ramki)",
	"Ogólny błąd wysyłania",
	"Ogólny błąd odczytu",
	"Błąd zasobów (np. port został odłączony w czasie pracy programu)",
	"Niedozwolona operacja",
	"Nieznany błąd",
	"Upłynął limit czasu oczekiwania",
	"Port nie został prawidłowo otwarty"
};

GlowneOkno::GlowneOkno(QWidget* parent):QMainWindow(parent)
{
	setupRS();
	
	setupOkno();
    setupRozklad();
    setupWyslij();
    setupReset();
    setupTemperatura();
    setupWykres();
    
    okno_->show();
}
    
GlowneOkno::~GlowneOkno()
{
    rs232_->close();
    delete rs232_;
    delete zadanaTemperatura_;
    delete wyslij_;
    delete reset_;
    delete wykres_;
    delete danePomiaroweWykres_;
    
    czas_.clear();
    temperatura_.clear();
    
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
	int wybor=QMessageBox::Retry;
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
			QMessageBox pytanie(QMessageBox::Warning, "Brak portu szeregowego!", "Nie wykryto żadnego portu szeregowego! \nSprawdź czy port jest dostępny i podejmij ponowną próbę konfiguracji lub zakończ program.", QMessageBox::Ignore|QMessageBox::Abort|QMessageBox::Retry);
			wybor=pytanie.exec();
			
			if (wybor==QMessageBox::Abort) 
			{
				QMessageBox(QMessageBox::Critical, "Brak portu szeregowego!", "Koniec programu.", QMessageBox::Ok).exec();
				exit(1);
			}
		}
	}
	while(itemList.isEmpty()&&wybor!=QMessageBox::Ignore);
	
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
	rs232_->clear();
    std::cerr<<rs232_->error()<<std::endl;
    
    QObject::connect(rs232_, SIGNAL(readyRead()),this, SLOT(odbierzDane()));
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
	
	QObject::connect(wyslij_, SIGNAL(clicked(bool)),this, SLOT(ustawTemperature()));
}

void GlowneOkno::setupZatrzymajGrzanie(void)
{
	zatrzymajGrzanie_=new QPushButton("ZatrzymajGrzanie");
	zatrzymajGrzanie_->setFixedSize(100,20);
	glownyRozmieszczacz_->addWidget(zatrzymajGrzanie_);
}

void GlowneOkno::setupReset(void)
{
	reset_=new QPushButton("Reset");
	reset_->setFixedSize(100,20);
	glownyRozmieszczacz_->addWidget(reset_);
	
	QObject::connect(reset_, SIGNAL(clicked(bool)),this, SLOT(zrestartujUrzadenie()));
}

void GlowneOkno::setupWykres(void)
{
	wykres_=new QwtPlot;
	glownyRozmieszczacz_->addWidget(wykres_);
	wykres_->setTitle ("Bieżąca temperatura próbki");
	wykres_->setAxisTitle (QwtPlot::xBottom, "Czas /s");
	wykres_->setAxisTitle (QwtPlot::yLeft, "Temperatura /℃");
	wykres_->setCanvasBackground(QBrush (Qt::white));
	wykres_->setAxisScale (QwtPlot::xBottom, 0, 120);
	wykres_->setAxisScale (QwtPlot::yLeft, 0, 800);
	
	danePomiaroweWykres_=new QwtPlotCurve;
	
	danePomiaroweWykres_->setTitle("Temperatura");
	danePomiaroweWykres_->setPen(QPen(Qt::blue, 3) ),
	danePomiaroweWykres_->setRenderHint(QwtPlotItem::RenderAntialiased, true);	
}

void GlowneOkno::wyslijRozkaz(const char* rozkaz)
{
	QSerialPort::SerialPortError error=rs232_->error();
	if(error!=QSerialPort::NoError)
	{
		obsluzBladRS(error);
		return;
	}
	rs232_->write(rozkaz);
}

void GlowneOkno::ustawTemperature(void)
{
	char tmp[4];
	int t=zadanaTemperatura_->value();
	sprintf(tmp,"T%03i",t);
	wyslijRozkaz(tmp);
	std::cerr<<tmp<<std::endl;
}

void GlowneOkno::odbierzDane(void)
{
	char tmpTekst[1024];
	char tmp[256];
	uint32_t tmpCzas=0;
	uint32_t tmpTemperatura=0;
	
	rs232_->readLine(tmpTekst,1023);
	rs232_->clear();
	sscanf(tmpTekst,"%u,%u,%s",&(tmpCzas),&(tmpTemperatura),tmp);
	
	czas_.push_back((double)tmpCzas);
	temperatura_.push_back((double)tmpTemperatura);
	
	danePomiaroweWykres_->setSamples(czas_,temperatura_);
	danePomiaroweWykres_->attach(wykres_);
	
	if(czas_.last()>120)
		wykres_->setAxisScale (QwtPlot::xBottom, czas_.last()-120, czas_.last());
	else
		wykres_->setAxisScale (QwtPlot::xBottom, 0, 120);
		
	wykres_->replot();
	std::cout<<tmpCzas<<" "<<tmpTemperatura<<std::endl;
}

void GlowneOkno::obsluzBladRS(QSerialPort::SerialPortError kod_bledu)
{
	
	std::cerr<<"Błąd portu szeregowego "<<kod_bledu<<": "<<bledy[kod_bledu]<<"."<<std::endl;
}

void GlowneOkno::zrestartujUrzadenie(void)
{
	QSerialPort::SerialPortError error=rs232_->error();
	if(error!=QSerialPort::NoError)
	{
		obsluzBladRS(error);
		return;
	}
	rs232_->write("RRR");
	std::cout<<"R"<<std::endl;
	
	czas_.clear();
	temperatura_.clear();
}

#include "GlowneOkno.moc"
