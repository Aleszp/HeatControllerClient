//Standardowe nagłówki C/C++
#include <iostream>

//Nagłówki z katalogu programu
#include "TrybProgramowalny.hpp"

TrybProgramowalny::TrybProgramowalny(QWidget* parent=0):QWidget(parent)
{
	rozmieszczacz_=new QHBoxLayout(this);
	wczytaj_=new QPushButton("Wczytaj",this);
	rozmieszczacz_->addWidget(wczytaj_);
	startStop_=new QPushButton("Start",this);
	rozmieszczacz_->addWidget(startStop_);
	
	zegar_=new QTimer(this);
	
	QObject::connect(startStop_, SIGNAL(clicked(bool)),this, SLOT(start()));
	QObject::connect(zegar_, SIGNAL(timeout()),this, SLOT(obsluzMaszyneStanow()));
}

TrybProgramowalny::~TrybProgramowalny()
{
	delete zegar_;
	delete wczytaj_;
	delete startStop_;
	delete rozmieszczacz_;
}

void TrybProgramowalny::obsluzMaszyneStanow(void)
{
	zegar_->start(1000);	//ponownie uruchom zegar
	
	/*Obsłuż maszynę stanów*/
	std::cout<<"test"<<std::endl;
}

void TrybProgramowalny::start(void)
{
	zegar_->start(1000);
	startStop_->setText("Stop");
	
	QObject::connect(startStop_, SIGNAL(clicked(bool)),this, SLOT(stop()));
}

void TrybProgramowalny::stop(void)
{
	zegar_->stop();
	startStop_->setText("Start");
	
	QObject::connect(startStop_, SIGNAL(clicked(bool)),this, SLOT(start()));
}

#include "TrybProgramowalny.moc"
