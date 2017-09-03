//Nagłówki z katalogu QtSerialPort
#include <QtSerialPort/QSerialPortInfo>

//Nagłówki z katalogu QtCore
#include <QtCore/QDebug> 

//Nagłówki z katalogu QtWidgets
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>

//Nagłówki z katalogu QtGui
#include <QtGui/QPalette>

//Standardowe nagłówki C/C++
#include <cstdlib>
#include <iostream>

//Nagłówki z katalogu programu
#include "GlowneOkno.hpp"
#include "WyborPortu.hpp"
#include "TypyWyliczeniowe.hpp"
#include "WartosciStale.hpp"

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
	konsola_=false;
	
	setupRS();
	setupCzasTemperatura();
	setupOkno();
	setupRozklad();
	setupWykresChwilowy();
	setupWykresDlugookresowy();  
    
	manual_=new TrybManualny(this);
	wiersze_[1].addWidget(manual_);
    
	automat_=new TrybProgramowalny(this);
	wiersze_[1].addWidget(automat_);
	wiersze_[1].addStretch();
    
	okno_->show();
}
    
GlowneOkno::~GlowneOkno()
{
	manual_->setTemperaturaDocelowa(0);
	ustawTemperature(false);
    rs232_->close();
   
    delete rs232_;
    
    delete danePomiaroweWykresChwilowy_;
    delete danePomiaroweWykresDlugookresowy_;
    delete wykresChwilowy_;
    delete wykresDlugookresowy_;
    
    delete czasChwilowy_;
    delete temperaturaChwilowa_;
    delete czasDlugookresowy_;
    delete temperaturaDlugookresowa_;
    
    delete[] wiersze_;
    delete glownyRozmieszczacz_;
    
    delete automat_;
    delete manual_;
    delete okno_;
    
}

void GlowneOkno::setupOkno(void)
{
	okno_=new QWidget();
	okno_->resize(1024,768);
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
			exit(NIE_WYBRANO_PORTU);
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
	czasChwilowy_=new QVector <double>;
	temperaturaChwilowa_=new QVector <double>;
	czasDlugookresowy_=new QVector <double>;
	temperaturaDlugookresowa_=new QVector <double>;
	
	czasChwilowy_->reserve(121);
	temperaturaChwilowa_->reserve(121);
	czasDlugookresowy_->reserve(65536);
	temperaturaDlugookresowa_->reserve(65536);
}

void GlowneOkno::setupWykresChwilowy(void)
{
	wykresChwilowy_=new QwtPlot(okno_);
	wiersze_[0].addWidget(wykresChwilowy_);
	wykresChwilowy_->setTitle ("Bieżąca temperatura próbki");
	wykresChwilowy_->enableAxis(QwtPlot::yRight);
	wykresChwilowy_->enableAxis(QwtPlot::yLeft, false);
	wykresChwilowy_->setAxisScale (QwtPlot::xBottom, 0, 120);
	wykresChwilowy_->setAxisScale (QwtPlot::yRight, 0, 800);
	wykresChwilowy_->setAxisTitle (QwtPlot::xBottom, "Czas /s");
	wykresChwilowy_->setAxisTitle (QwtPlot::yRight, "Temperatura /℃");
	wykresChwilowy_->setCanvasBackground(QBrush (Qt::white));
	
	danePomiaroweWykresChwilowy_=new QwtPlotCurve;
	danePomiaroweWykresChwilowy_->setTitle("Temperatura");
	danePomiaroweWykresChwilowy_->setPen(QPen(Qt::black, 3) ),
	danePomiaroweWykresChwilowy_->setRenderHint(QwtPlotItem::RenderAntialiased, true);	
	danePomiaroweWykresChwilowy_->setAxes(QwtPlot::xBottom, QwtPlot::yRight);
}

void GlowneOkno::setupWykresDlugookresowy(void)
{
	wykresDlugookresowy_=new QwtPlot(okno_);
	wiersze_[0].addWidget(wykresDlugookresowy_);
	wykresDlugookresowy_->setTitle ("Temperatura próbki");
	wykresDlugookresowy_->setAxisScale (QwtPlot::xBottom, 0, 120);
	wykresDlugookresowy_->enableAxis(QwtPlot::yRight);
	wykresDlugookresowy_->enableAxis(QwtPlot::yLeft, false);
	wykresDlugookresowy_->setAxisScale (QwtPlot::yRight, 0, 800);
	wykresDlugookresowy_->setAxisTitle (QwtPlot::xBottom, "Czas /s");
	wykresDlugookresowy_->setAxisTitle (QwtPlot::yRight, "Temperatura /℃");
	wykresDlugookresowy_->setCanvasBackground(QBrush (Qt::white));
	
	danePomiaroweWykresDlugookresowy_=new QwtPlotCurve;
	danePomiaroweWykresDlugookresowy_->setTitle("Temperatura");
	danePomiaroweWykresDlugookresowy_->setPen(QPen(Qt::black, 3) ),
	danePomiaroweWykresDlugookresowy_->setRenderHint(QwtPlotItem::RenderAntialiased, true);	
	danePomiaroweWykresDlugookresowy_->setAxes(QwtPlot::xBottom, QwtPlot::yRight);
}

bool GlowneOkno::wyslijRozkaz(const char* rozkaz, const bool ask)
{
	QSerialPort::SerialPortError error;
	bool stan=true;
	do
	{
		error=rs232_->error();
		if((error!=QSerialPort::NoError)&&ask)
		{
			stan=obsluzBladRS(error);
		}
		else
		{
			stan=false;
		}
	}
	while(stan&&ask);
	
	if(error==QSerialPort::NoError)
	{
		rs232_->write(rozkaz);
		if(konsola_)
			std::cout<<rozkaz<<std::endl;
		return OK;
	}
	return BLAD_PORTU;
}

void GlowneOkno::ustawTemperature(bool ask)
{
	char tmp[4];
	int t=manual_->getTemperaturaDocelowa();
	sprintf(tmp,"T%03i",t);
	
	if(wyslijRozkaz(tmp,ask)==OK)
		std::cerr<<tmp<<std::endl;
	else
	{
		if(!ask)
			QMessageBox(QMessageBox::Critical, "Błąd portu szeregowego!", "Błąd portu szeregowego w czasie wysyłania.", QMessageBox::Ok).exec();
	}
}

void GlowneOkno::odbierzDane(void)
{
	char tmpTekst[1024];
	char tmp[256];
	
	uint32_t tmpCzas=0;
	uint32_t tmpTemperatura=0;
	uint32_t tmpMoc;
	
	rs232_->readLine(tmpTekst,1023);
	rs232_->clear();
	
	if(manual_->getZapisDoPliku()&&manual_->getOtwartyPlik())
	{
		fprintf(manual_->getPlikWyjsciowy(),"%s",tmpTekst);
	}
	
	if((tmpTekst[10]==',')&&(tmpTekst[17]==','))
	{
		sscanf(tmpTekst,"%u,%u,%u %s",&(tmpCzas),&(tmpTemperatura),&(tmpMoc),tmp);
		std::cout<<tmpCzas<<" "<<tmpTemperatura<<std::endl;
		
		manual_->setTemperatua(tmpTemperatura);
		manual_->setMoc((tmpMoc*mocGrzalki)/255);
		
		czasChwilowy_->push_back((double)tmpCzas);
		temperaturaChwilowa_->push_back((double)tmpTemperatura);
		
		czasDlugookresowy_->push_back((double)tmpCzas);
		temperaturaDlugookresowa_->push_back((double)tmpTemperatura);
	
		while(czasChwilowy_->size()>120)
		{
			czasChwilowy_->removeFirst();
			temperaturaChwilowa_->removeFirst();
		}
	
		if(!czasChwilowy_->empty())
		{
			danePomiaroweWykresChwilowy_->setSamples(*czasChwilowy_,*temperaturaChwilowa_);
			danePomiaroweWykresChwilowy_->attach(wykresChwilowy_);
	
			if(czasChwilowy_->last()>120)
				wykresChwilowy_->setAxisScale (QwtPlot::xBottom, czasChwilowy_->last()-120, czasChwilowy_->last());
			else
				wykresChwilowy_->setAxisScale (QwtPlot::xBottom, 0, 120);
			wykresChwilowy_->replot();
		}
		
		while(czasDlugookresowy_->size()>65535)
		{
			czasDlugookresowy_->removeFirst();
			temperaturaDlugookresowa_->removeFirst();
		}	
		
		if(!czasDlugookresowy_->empty())
		{
			danePomiaroweWykresDlugookresowy_->setSamples(*czasDlugookresowy_,*temperaturaDlugookresowa_);
			danePomiaroweWykresDlugookresowy_->attach(wykresDlugookresowy_);
			if(czasDlugookresowy_->last()>120)
			{
				if(czasDlugookresowy_->last()>65535)
					wykresDlugookresowy_->setAxisScale (QwtPlot::xBottom, czasDlugookresowy_->last()-65535, czasDlugookresowy_->last());
				else
					wykresDlugookresowy_->setAxisScale (QwtPlot::xBottom, 0, czasDlugookresowy_->last());
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

void GlowneOkno::zrestartujUrzadenie(bool manual)
{
	if(wyslijRozkaz("R")==OK)
	{
		czasChwilowy_->clear();
		temperaturaChwilowa_->clear();
		czasDlugookresowy_->clear();
		temperaturaDlugookresowa_->clear();
		if(manual)
			automat_->stop();
		
		if(konsola_)
			std::cout<<"Uruchomiono ponownie urządzenie."<<std::endl;
	}
	else
	{
		QMessageBox(QMessageBox::Critical, "Błąd!", "Nie można zrestartować urządzenia!", QMessageBox::Ok).exec();
	}
}

void GlowneOkno::zatrzymajGrzanie(bool manual)
{
	if(!wyslijRozkaz("T000")==OK)
	{
		QMessageBox(QMessageBox::Critical, "Błąd!", "Nie można zatrzymać grzania!", QMessageBox::Ok).exec();
		if(konsola_)
			std::cerr<<"NIE MOŻNA ZATRZYMAĆ GRZANIA!"<<std::endl;
		return;
	}
	else
	if(konsola_)
		std::cout<<"Zatrzymano grzanie (Ustawiono T=0)."<<std::endl;
	if(manual)
		automat_->stop();
}

#include "GlowneOkno.moc"
