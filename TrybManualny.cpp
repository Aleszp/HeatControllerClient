//Standardowe nagłówki C/C++
#include <iostream>

//Nagłówki z katalogu programu
#include "TrybManualny.hpp"
#include "GlowneOkno.hpp"

TrybManualny::TrybManualny(GlowneOkno* rodzic=0):QWidget((QMainWindow*)rodzic)
{
	rodzic_=rodzic;
	
	rozmieszczacz_=new QVBoxLayout(this);
	wiersze_=new QHBoxLayout[2];
	rozmieszczacz_->addLayout(&wiersze_[0]);
	rozmieszczacz_->addLayout(&wiersze_[1]);
	
	setupWyslij();
	setupZadanaTemperatura();
	setupTemperatura();
    wiersze_[0].addStretch();
    
    setupZatrzymajGrzanie();
    setupReset();
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
    delete zatrzymajGrzanie_;
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


#include "TrybManualny.moc"
