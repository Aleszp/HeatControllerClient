//Standardowe nagłówki C/C++
#include <iostream>

//Nagłówki z katalogu programu
#include "TrybManualny.hpp"
#include "GlowneOkno.hpp"

TrybManualny::TrybManualny(GlowneOkno* rodzic=0):QWidget((QMainWindow*)rodzic)
{
	rodzic_=rodzic;
	
	otwartyPlik_=false;
	zapisujDoPliku_=false;
	
	nazwaPliku_=new char[256];
	
	rozmieszczacz_=new QVBoxLayout(this);
	wiersze_=new QHBoxLayout[2];
	rozmieszczacz_->addLayout(&wiersze_[0]);
	rozmieszczacz_->addLayout(&wiersze_[1]);
	
	setupWyslij();
	setupZadanaTemperatura();
	setupZapisDoPliku();
	setupTemperatura();
    wiersze_[0].addStretch();
    
    setupZatrzymajGrzanie();
    setupReset();
    setupWybierzPlik();
    setupMoc();
	wiersze_[1].addStretch();
    
}

TrybManualny::~TrybManualny()
{
	rodzic_=nullptr;
	delete temperatura_;
	delete moc_;
	delete zadanaTemperatura_;
    delete wyslij_;
    delete reset_;
    delete wybierzPlik_;
    delete zapisDoPliku_;
    delete zatrzymajGrzanie_;
    delete[] nazwaPliku_;
    delete[] wiersze_;
    delete rozmieszczacz_;
}

void TrybManualny::setupZadanaTemperatura(void)
{
	zadanaTemperatura_=new QSpinBox;
	wiersze_[0].addWidget(zadanaTemperatura_);
	zadanaTemperatura_->setFixedSize(150,30);
    zadanaTemperatura_->setRange(0, 999);
    zadanaTemperatura_->setSingleStep(1);
    zadanaTemperatura_->setSuffix(" ℃");   
}

void TrybManualny::setupWyslij(void)
{
	wyslij_=new QPushButton("Ustaw",this);
	wyslij_->setFixedSize(150,30);
	wiersze_[0].addWidget(wyslij_);
	
	QObject::connect(wyslij_, SIGNAL(clicked(bool)),this, SLOT(ustawTemperature()));
}

void TrybManualny::setupZatrzymajGrzanie(void)
{
	QPalette* czerwony = new QPalette();
	czerwony->setColor(QPalette::ButtonText,Qt::red);
	
	zatrzymajGrzanie_=new QPushButton("Zatrzymaj grzanie",this);
	zatrzymajGrzanie_->setFixedSize(150,30);
	zatrzymajGrzanie_->setPalette(*czerwony);
	
	wiersze_[1].addWidget(zatrzymajGrzanie_);
	
	delete czerwony;
	
	QObject::connect(zatrzymajGrzanie_, SIGNAL(clicked(bool)),this, SLOT(zatrzymajGrzanie()));
}

void TrybManualny::setupReset(void)
{
	reset_=new QPushButton("Reset",this);
	reset_->setFixedSize(150,30);
	wiersze_[1].addWidget(reset_);
	
	QObject::connect(reset_, SIGNAL(clicked(bool)),this, SLOT(zrestartujUrzadenie()));
}

void TrybManualny::setupTemperatura(void)
{
	temperatura_=new QLabel("Bieżąca temperatura: ???℃");
	wiersze_[0].addWidget(temperatura_);
}

void TrybManualny::setupMoc(void)
{
	moc_=new QLabel("Bieżąca moc: ???W");
	wiersze_[1].addWidget(moc_);
}

void TrybManualny::setupWybierzPlik(void)
{
	wybierzPlik_=new QPushButton("Wybierz plik",this);
	wybierzPlik_->setFixedSize(150,30);
	wiersze_[1].addWidget(wybierzPlik_);
	
	QObject::connect(wybierzPlik_, SIGNAL(clicked(bool)),this, SLOT(wybierzPlik()));
}

void TrybManualny::setupZapisDoPliku(void)
{
	zapisDoPliku_=new QCheckBox("Zapis do pliku",this);
	zapisDoPliku_->setFixedSize(150,30);
	wiersze_[0].addWidget(zapisDoPliku_);
	
	QObject::connect(zapisDoPliku_, SIGNAL(clicked(bool)),this, SLOT(zapisujDoPliku(bool)));
}

void TrybManualny::setMoc(uint32_t moc)
{
	char tmp[32];
	sprintf(tmp,"Bieżąca moc: %03huW",moc);
	moc_->setText(tmp);
}

void TrybManualny::setTemperatua(uint32_t temperatura)
{
	char tmp[32];
	sprintf(tmp,"Bieżąca temperatura: %03hu℃",temperatura);
	temperatura_->setText(tmp);
}

int TrybManualny::otworzPlik(void)
{
	if(otwartyPlik_)
		return zamknijPlik();;	
	
	plikDoZapisu_=fopen(nazwaPliku_,"a");
	
	if(!plikDoZapisu_)
		return BLAD_OTWARCIA_PLIKU;
	otwartyPlik_=true;
	return OTWARTO_PLIK;
}

int TrybManualny::zamknijPlik(void)
{
	if(otwartyPlik_)
	{
		fflush(plikDoZapisu_);
		fclose(plikDoZapisu_);
		otwartyPlik_=false;
		return ZAMKNIETO_PLIK;
	}
	return NIE_ZAMKNIETO_PLIKU;
}

void TrybManualny::zrestartujUrzadenie(void)
{
	rodzic_->zrestartujUrzadenie(true);
}

void TrybManualny::ustawTemperature(bool ask)
{
	rodzic_->ustawTemperature(ask);
}

void TrybManualny::zatrzymajGrzanie(void)
{
	rodzic_->zatrzymajGrzanie(true);
}

void TrybManualny::wybierzPlik(void)
{
	//do zrobienia
}

void TrybManualny::zapisujDoPliku(bool stan)
{
	zapisujDoPliku_=stan;
}

#include "TrybManualny.moc"
