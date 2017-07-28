#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug> 
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>

#include <cstdlib>
#include <iostream>

#include "GlowneOkno.hpp"
#include "WyborPortu.hpp"
#include "KodyWyjsciowe.hpp"

const char* bledy[14]=
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
	
	setupCzasTemperatura();
	
	setupOkno();
    setupRozklad();
    setupWyslij();
    setupReset();
    setupZatrzymajGrzanie();
    setupTemperatura();
    setupWykresChwilowy();
    setupWykresDlugookresowy();
    
    
    okno_->show();
}
    
GlowneOkno::~GlowneOkno()
{
	zadanaTemperatura_->setValue(0);
	ustawTemperature();
    rs232_->close();
    
    delete rs232_;
    delete zadanaTemperatura_;
    delete wyslij_;
    delete reset_;
    delete zatrzymajGrzanie_;
    delete wykresChwilowy_;
    delete wykresDlugookresowy_;
    delete danePomiaroweWykresChwilowy_;
    
    czasChwilowy_.clear();
    temperaturaChwilowa_.clear();
    czasDlugookresowy_.clear();
    temperaturaDlugookresowa_.clear();
    
    
    delete[] wiersze_;
    delete glownyRozmieszczacz_;
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
	iloscWierszy_=3;
	
	glownyRozmieszczacz_=new QVBoxLayout(okno_);
	okno_->setLayout(glownyRozmieszczacz_);

	wiersze_=new QHBoxLayout[iloscWierszy_];
	
	for(unsigned ii=0;ii<iloscWierszy_;ii++)
	{
		glownyRozmieszczacz_->addLayout(&wiersze_[ii]);
	}
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
			
			if(wybor==QMessageBox::Abort) 
			{
				QMessageBox(QMessageBox::Critical, "Brak portu szeregowego!", "Koniec programu.", QMessageBox::Ok).exec();
				exit(BRAK_PORTU);
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

void GlowneOkno::setupCzasTemperatura(void)
{
	czasChwilowy_.reserve(121);
	temperaturaChwilowa_.reserve(121);
	
	czasDlugookresowy_.reserve(65536);
	temperaturaDlugookresowa_.reserve(65536);
}

void GlowneOkno::setupTemperatura(void)
{
	zadanaTemperatura_=new QSpinBox;
	wiersze_[0].addWidget(zadanaTemperatura_);
    zadanaTemperatura_->setRange(0, 999);
    zadanaTemperatura_->setSingleStep(1);
    zadanaTemperatura_->setSuffix(" ℃");   
}

void GlowneOkno::setupWyslij(void)
{
	wyslij_=new QPushButton("Ustaw");
	wyslij_->setFixedSize(100,20);
	wiersze_[0].addWidget(wyslij_);
	
	QObject::connect(wyslij_, SIGNAL(clicked(bool)),this, SLOT(ustawTemperature()));
}

void GlowneOkno::setupZatrzymajGrzanie(void)
{
	zatrzymajGrzanie_=new QPushButton("ZatrzymajGrzanie");
	zatrzymajGrzanie_->setFixedSize(100,20);
	wiersze_[1].addWidget(zatrzymajGrzanie_);
	
	QObject::connect(zatrzymajGrzanie_, SIGNAL(clicked(bool)),this, SLOT(zatrzymajGrzanie()));
}

void GlowneOkno::setupReset(void)
{
	reset_=new QPushButton("Reset");
	reset_->setFixedSize(100,20);
	wiersze_[1].addWidget(reset_);
	
	QObject::connect(reset_, SIGNAL(clicked(bool)),this, SLOT(zrestartujUrzadenie()));
}

void GlowneOkno::setupWykresChwilowy(void)
{
	wykresChwilowy_=new QwtPlot;
	wiersze_[2].addWidget(wykresChwilowy_);
	wykresChwilowy_->setTitle ("Bieżąca temperatura próbki");
	wykresChwilowy_->setAxisTitle (QwtPlot::xBottom, "Czas /s");
	wykresChwilowy_->setAxisTitle (QwtPlot::yLeft, "Temperatura /℃");
	wykresChwilowy_->setCanvasBackground(QBrush (Qt::white));
	wykresChwilowy_->setAxisScale (QwtPlot::xBottom, 0, 120);
	wykresChwilowy_->setAxisScale (QwtPlot::yLeft, 0, 800);
	
	danePomiaroweWykresChwilowy_=new QwtPlotCurve;
	
	danePomiaroweWykresChwilowy_->setTitle("Temperatura");
	danePomiaroweWykresChwilowy_->setPen(QPen(Qt::blue, 3) ),
	danePomiaroweWykresChwilowy_->setRenderHint(QwtPlotItem::RenderAntialiased, true);	
}

void GlowneOkno::setupWykresDlugookresowy(void)
{
	wykresDlugookresowy_=new QwtPlot;
	wiersze_[2].addWidget(wykresDlugookresowy_);
	wykresDlugookresowy_->setTitle ("Temperatura próbki");
	wykresDlugookresowy_->setAxisTitle (QwtPlot::xBottom, "Czas /s");
	wykresDlugookresowy_->setAxisTitle (QwtPlot::yLeft, "Temperatura /℃");
	wykresDlugookresowy_->setCanvasBackground(QBrush (Qt::white));
	wykresDlugookresowy_->setAxisScale (QwtPlot::xBottom, 0, 120);
	wykresDlugookresowy_->setAxisScale (QwtPlot::yLeft, 0, 800);
	
	danePomiaroweWykresDlugookresowy_=new QwtPlotCurve;
	
	danePomiaroweWykresDlugookresowy_->setTitle("Temperatura");
	danePomiaroweWykresDlugookresowy_->setPen(QPen(Qt::blue, 1) ),
	danePomiaroweWykresDlugookresowy_->setRenderHint(QwtPlotItem::RenderAntialiased, true);	
}

bool GlowneOkno::wyslijRozkaz(const char* rozkaz)
{
	QSerialPort::SerialPortError error;
	bool stan=true;
	do
	{
		error=rs232_->error();
		if(error!=QSerialPort::NoError)
		{
			stan=obsluzBladRS(error);
		}
		else
		{
			stan=false;
		}
	}
	while(stan);
	
	if(error==QSerialPort::NoError)
	{
		rs232_->write(rozkaz);
		return OK;
	}
	return BLAD_PORTU;
}

void GlowneOkno::ustawTemperature(void)
{
	char tmp[4];
	int t=zadanaTemperatura_->value();
	sprintf(tmp,"T%03i",t);
	
	if(wyslijRozkaz(tmp)==OK)
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
	
	if((tmpTekst[10]==',')&&(tmpTekst[17]==','))
	{
		sscanf(tmpTekst,"%u,%u,%s",&(tmpCzas),&(tmpTemperatura),tmp);
		std::cout<<tmpCzas<<" "<<tmpTemperatura<<std::endl;
		
		czasChwilowy_.push_back((double)tmpCzas);
		temperaturaChwilowa_.push_back((double)tmpTemperatura);
		
		czasDlugookresowy_.push_back((double)tmpCzas);
		temperaturaDlugookresowa_.push_back((double)tmpTemperatura);
	
		while(czasChwilowy_.size()>120)
		{
			czasChwilowy_.removeFirst();
			temperaturaChwilowa_.removeFirst();
		}
	
		if(!czasChwilowy_.empty())
		{
			danePomiaroweWykresChwilowy_->setSamples(czasChwilowy_,temperaturaChwilowa_);
			danePomiaroweWykresChwilowy_->attach(wykresChwilowy_);
	
			if(czasChwilowy_.last()>120)
				wykresChwilowy_->setAxisScale (QwtPlot::xBottom, czasChwilowy_.last()-120, czasChwilowy_.last());
			else
				wykresChwilowy_->setAxisScale (QwtPlot::xBottom, 0, 120);
			wykresChwilowy_->replot();
		}
		
		while(czasDlugookresowy_.size()>65535)
		{
			czasDlugookresowy_.removeFirst();
			temperaturaDlugookresowa_.removeFirst();
		}	
		
		if(!czasDlugookresowy_.empty())
		{
			danePomiaroweWykresDlugookresowy_->setSamples(czasDlugookresowy_,temperaturaDlugookresowa_);
			danePomiaroweWykresDlugookresowy_->attach(wykresDlugookresowy_);
			if(czasDlugookresowy_.last()>120)
			{
				if(czasDlugookresowy_.last()>65535)
					wykresDlugookresowy_->setAxisScale (QwtPlot::xBottom, czasDlugookresowy_.last()-65535, czasDlugookresowy_.last());
				else
					wykresDlugookresowy_->setAxisScale (QwtPlot::xBottom, 0, czasDlugookresowy_.last());
			}
			else
				wykresDlugookresowy_->setAxisScale (QwtPlot::xBottom, 0, 120);
			wykresDlugookresowy_->replot();
		}
	}
	else
	{
		std::cout<<tmpTekst;
	}
}

bool GlowneOkno::obsluzBladRS(QSerialPort::SerialPortError kod_bledu)
{
	char opis_bledu[128];
	sprintf(opis_bledu,"Błąd portu szeregowego %i:\n%s.",kod_bledu,bledy[kod_bledu]);
	QMessageBox pytanie(QMessageBox::Warning, "Brak portu szeregowego!", opis_bledu, QMessageBox::Ignore|QMessageBox::Abort|QMessageBox::Retry);
	std::cerr<<opis_bledu<<std::endl;
	
	int wynik=pytanie.exec();
	
	if(wynik==QMessageBox::Abort) 
	{
		QMessageBox(QMessageBox::Critical, "Błąd portu szeregowego!", "Koniec programu.", QMessageBox::Ok).exec();
		exit(BLAD_PORTU);
		return false;
	}
	if(wynik==QMessageBox::Retry)
	{
		return true;
	}
	//jeśli ani abort ani retry, to ignore
	return false;
}

void GlowneOkno::zrestartujUrzadenie(void)
{
	if(wyslijRozkaz("R")==OK)
	{
		czasChwilowy_.clear();
		temperaturaChwilowa_.clear();
	}
}

void GlowneOkno::zatrzymajGrzanie(void)
{
	if(!wyslijRozkaz("T000")==OK)
	{
		std::cout<<"NIE MOŻNA ZATRZYMAĆ GRZANIA!"<<std::endl;
	}
	else
	std::cout<<"Zatrzymano grzanie (Ustawiono T=0)."<<std::endl;
}

#include "GlowneOkno.moc"
