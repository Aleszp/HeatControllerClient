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
	setupTemperatura();
    wiersze_[0].addStretch();
    
    setupReset();
    setupZatrzymajGrzanie();
	wiersze_[1].addStretch();
    
}

TrybManualny::~TrybManualny()
{
	delete zadanaTemperatura_;
    delete wyslij_;
    delete reset_;
    delete zatrzymajGrzanie_;
    
    delete rozmieszczacz_;
    delete[] wiersze_;
}

void TrybManualny::setupTemperatura(void)
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

void TrybManualny::zrestartujUrzadenie(void)
{
	rodzic_->zrestartujUrzadenie();
}

void TrybManualny::ustawTemperature(bool ask)
{
	rodzic_->ustawTemperature(ask);
}

void TrybManualny::zatrzymajGrzanie(void)
{
	rodzic_->zatrzymajGrzanie();
}


#include "TrybManualny.moc"
