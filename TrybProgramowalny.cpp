//Nagłówki z katalogu QtCore
#include <QtCore/QFile>

//Nagłówki z katalogu QtWidgets
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

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
	
	QObject::connect(wczytaj_, SIGNAL(clicked(bool)),this, SLOT(wczytajProgram()));
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

int TrybProgramowalny::wczytajProgram(void)
{
    QString nazwaPliku = QFileDialog::getOpenFileName(this,tr("Wczytaj program"), "",tr("Programy (*.pro);;Pliki tekstowe (*.txt);;Wszystkie pliki (*)"));
    
    if (nazwaPliku.isEmpty())
    {
		QMessageBox::warning(this, tr("Brak nazwy pliku!"),"Nie podano nazwy pliku do wczytania.");
        return BRAK_NAZWY_PLIKU;
	}
    else 
    {
        QFile plik(nazwaPliku);

        if (!plik.open(QIODevice::ReadOnly)) 
        {
            QMessageBox::warning(this, tr("Nie można otwożyć pliku!"),plik.errorString());
            return BLAD_PLIKU;
		}
	}
	
	return OK;
}

#include "TrybProgramowalny.moc"
