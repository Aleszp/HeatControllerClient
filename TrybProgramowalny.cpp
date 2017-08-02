//Nagłówki z katalogu QtCore
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>

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
	
	program_=new QVector <Rozkaz>;
	
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
	delete program_;
}

int TrybProgramowalny::obsluzMaszyneStanow(void)
{
	zegar_->start(1000);	//ponownie uruchom zegar
	
	
	/*Obsłuż maszynę stanów*/
	
	//Jeśli nie ma rozkazów do wykonania
	if(program_->empty())
	{
		/*Wyświetl komunikat o zakończeniu programu*/
		std::cout<<"KONIEC PROGRAMU"<<std::endl;
		stop();
		return OK;
	}
	
	//Jeśli program ma czekać
	if(program_->first().stan==CZEKAJ)
	{
		if(program_->first().wartosc>0)
			program_->first().wartosc--;	//Odlicz 1 sekundę od oczekiwania
		else
			program_->removeFirst();
			
		return OK;
	}
	
	//Jeśli program ma ustawić temperaturę
	if(program_->first().stan==USTAW_TEMPERATURE)
	{
		/*Ustaw porządaną temperaturę*/
		
		//Usuń ten rozkaz z kolejki
		program_->removeFirst();
		return OK;
	}	
	
	//Jeśli program nie ma ustalonego rozkazu
	if(program_->first().stan==BRAK_ROZKAZU)
	{
		//Usuń ten rozkaz z kolejki
		program_->removeFirst();
	}
	
	return OK;
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
    
    QFile plik(nazwaPliku);

    if (!plik.open(QIODevice::ReadOnly|QIODevice::Text)) 
    {
        QMessageBox::warning(this, tr("Nie można otwożyć pliku!"),plik.errorString());
        return BLAD_PLIKU;
	}
	
	QString tmp;
	
	QTextStream strumienWejsciowy(&plik);
	
	while(!strumienWejsciowy.atEnd())
	{
		tmp=strumienWejsciowy.readLine(1023);	//Wczytaj linię (o maksymalnej długości 1023 znaków
		
		if(tmp[0]=='#')				//Jeśli zaczyna się od #, to jest komentarzem, pomiń linię
			continue;
		
		//Jeśli jest to rozkaz ustawienia temperatury
		if(tmp[0]=='T')
		{
			
		}	
		
		//Jeśli jest to rozkaz czekania
		if(tmp[0]=='C')
		{
			
		}
	}
	
	plik.close();
	
	return OK;
}

#include "TrybProgramowalny.moc"
