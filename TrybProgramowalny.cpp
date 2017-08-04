//Nagłówki z katalogu QtCore
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>

//Nagłówki z katalogu QtWidgets
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLabel>

//Standardowe nagłówki C/C++
#include <iostream>
#include <cstdint>

//Nagłówki z katalogu programu
#include "TrybProgramowalny.hpp"
#include "GlowneOkno.hpp"

TrybProgramowalny::TrybProgramowalny(GlowneOkno* rodzic=0):QWidget((QMainWindow*)rodzic)
{
	czasCalkowity_= new uint32_t(0);
	rodzic_=rodzic;
	rozmieszczacz_=new QVBoxLayout(this);
	rozmieszczaczeWierszowe_=new QHBoxLayout[2];
	
	rozmieszczacz_->addLayout(&(rozmieszczaczeWierszowe_[0]));
	rozmieszczacz_->addLayout(&(rozmieszczaczeWierszowe_[1]));
	
	czasWskaznik_=new QLabel("Można wczytać program użytkownika.",this);
	rozmieszczaczeWierszowe_[0].addWidget(czasWskaznik_);
	
	wczytaj_=new QPushButton("Wczytaj",this);
	rozmieszczaczeWierszowe_[1].addWidget(wczytaj_);
	startStop_=new QPushButton("Start",this);
	rozmieszczaczeWierszowe_[1].addWidget(startStop_);
	
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
	delete czasWskaznik_;
	delete[] rozmieszczaczeWierszowe_;
	delete rozmieszczacz_;
	delete program_;
}

void TrybProgramowalny::obsluzMaszyneStanow(void)
{
	zegar_->start(1000);	//ponownie uruchom zegar
	
	if((*czasCalkowity_)>0)
		(*czasCalkowity_)--;
	
	//Jeśli nie ma rozkazów do wykonania
	if(program_->isEmpty())
	{
		/*Wyświetl komunikat o zakończeniu programu*/
		wyswietlTekst("Zakończono program użytkownika.",false);
		stop();
		return;
	}
	
	//Jeśli program ma czekać
	if(program_->first().stan==CZEKAJ)
	{
		if(program_->first().wartosc>1)
		{
			char tmp[64];
			program_->first().wartosc--;	//Odlicz 1 sekundę od oczekiwania
			sprintf(tmp,"Pozostały czas do końca kroku: %02u:%02u:%02u.",program_->first().wartosc/3600,(program_->first().wartosc%3600)/60,((program_->first().wartosc)%3600)%60);
			
			wyswietlTekst(tmp,true);
			
			//std::cout<<"Pozostały czas: "<<std::setfill('0') <<std::setw(2)<<program_->first().wartosc/3600<<":"<<std::setfill('0')<<std::setw(2)<<(program_->first().wartosc%3600)/60<<":"<<std::setfill('0')<<std::setw(2)<<((program_->first().wartosc)%3600)%60<<std::endl;
		}
		else
		{
			wyswietlTekst("Krok oczekiwania zakończony.",true);
			program_->removeFirst();
		}
			
		return;
	}
	
	//Jeśli program ma ustawić temperaturę
	if(program_->first().stan==USTAW_TEMPERATURE)
	{
		char tmp[4];
		sprintf(tmp,"T%03i",program_->first().wartosc);
	
		if(rodzic_->wyslijRozkaz(tmp)==OK)	//Ustaw porządaną temperaturę
		{
			rodzic_->setZadanaTemperatura_(program_->first().wartosc);
			std::cerr<<tmp<<std::endl;
			wyswietlTekst("Ustawiono temperaturę docelową.",true);
		}
			
		//Usuń ten rozkaz z kolejki
		program_->removeFirst();
		
		return;
	}	
	
	//Jeśli program ma zapytać urządzenie o średnią temperaturę
	if(program_->first().stan==SREDNIA_TEMPERATURA)
	{
		if(rodzic_->wyslijRozkaz("A")==OK)	//Wyślij zapytanie o średnią temperaturę
		{
			std::cerr<<"A"<<std::endl;
			wyswietlTekst("Pozyskano średnią temperaturę.",true);
		}
			
		//Usuń ten rozkaz z kolejki
		program_->removeFirst();
		
		return;	
	}
	
	//Jeśli program ma zapytać urządzenie o temperaturę docelową
	if(program_->first().stan==TEMPERATURA_DOCELOWA)
	{
		if(rodzic_->wyslijRozkaz("D")==OK)	//Wyślij zapytanie o średnią temperaturę
		{
			std::cerr<<"D"<<std::endl;
			wyswietlTekst("Pozyskano temperaturę docelową.",true);
		}
		//Usuń ten rozkaz z kolejki
		program_->removeFirst();
		
		return;	
	}
	
	//Jeśli program ma zrestartować urządzenie
	if(program_->first().stan==RESTART)
	{
		rodzic_->zrestartujUrzadenie();
		wyswietlTekst("Zrestartowano urządzenie.",true);
		//Usuń ten rozkaz z kolejki
		program_->removeFirst();
		
		return;	
	}
	
	//Jeśli program nie ma ustalonego rozkazu
	if(program_->first().stan==BRAK_ROZKAZU)
	{
		wyswietlTekst("Pominięto pusty rozkaz.",true);
		//Usuń ten rozkaz z kolejki
		program_->removeFirst();
	}
}

void TrybProgramowalny::start(void)
{
	startStop_->setText("Stop");
	QObject::connect(startStop_, SIGNAL(clicked(bool)),this, SLOT(stop()));
	
	obsluzMaszyneStanow();
}

void TrybProgramowalny::stop(void)
{
	zegar_->stop();
	startStop_->setText("Start");
	
	wyswietlTekst("Użytkownik zatrzymał program.",true);
	
	QObject::connect(startStop_, SIGNAL(clicked(bool)),this, SLOT(start()));
}

int TrybProgramowalny::wczytajProgram(void)
{
    QString nazwaPliku = QFileDialog::getOpenFileName(this,tr("Wczytaj program"), "",tr("Pliki tekstowe (*.txt);;Wszystkie pliki (*)"));
    
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
	Rozkaz tmpRozkaz;
	QTextStream strumienWejsciowy(&plik);
	
	while(!strumienWejsciowy.atEnd())
	{
		tmp=strumienWejsciowy.readLine(1023);	//Wczytaj linię (o maksymalnej długości 1023 znaków
		
		if(tmp[0]=='#')				//Jeśli zaczyna się od #, to jest komentarzem, pomiń linię
			continue;
		
		//Jeśli jest to rozkaz ustawienia temperatury
		if(tmp[0]=='T')
		{
			(*czasCalkowity_)++;
			tmpRozkaz.wartosc=tmp.mid(1,3).toUInt();
			tmpRozkaz.stan=USTAW_TEMPERATURE;
			program_->push_back(tmpRozkaz);
			
			std::cout<<"TEMPERATURA "<<tmpRozkaz.wartosc<<std::endl;
			
			continue;
		}	
		
		//Jeśli jest to rozkaz zapytania o średnią temperaturę
		if(tmp[0]=='A')
		{
			(*czasCalkowity_)++;
			tmpRozkaz.wartosc=0;
			tmpRozkaz.stan=SREDNIA_TEMPERATURA;
			program_->push_back(tmpRozkaz);
			
			std::cout<<"PODAJ ŚREDNIĄ TEMPERATURĘ"<<std::endl;
			
			continue;
		}
		
		//Jeśli jest to rozkaz pozyskania temperatury docelowej
		if(tmp[0]=='D')
		{
			(*czasCalkowity_)++;
			tmpRozkaz.wartosc=0;
			tmpRozkaz.stan=TEMPERATURA_DOCELOWA;
			program_->push_back(tmpRozkaz);
			
			std::cout<<"PODAJ DOCELOWĄ TEMPERATURĘ"<<std::endl;
			
			continue;
		}	
		
		//Jeśli jest to rozkaz restartu układu
		if(tmp[0]=='R')
		{
			(*czasCalkowity_)++;
			tmpRozkaz.wartosc=0;
			tmpRozkaz.stan=RESTART;
			program_->push_back(tmpRozkaz);
			
			std::cout<<"RESTART"<<std::endl;
			
			continue;
		}	
		
		//Jeśli jest to rozkaz czekania (w sekundach)
		if(tmp[0]=='S')
		{
			tmpRozkaz.wartosc=tmp.mid(1,8).toUInt();
			(*czasCalkowity_)+=tmpRozkaz.wartosc;
			
			if(program_->last().stan==CZEKAJ)	//Jeśli poprzedni rozkaz nakazywał czekać: dolicz do jego czasu oczekiwania nowe oczekiwanie
			{
				program_->last().wartosc+=tmpRozkaz.wartosc;
			}
			else
			{
				tmpRozkaz.stan=CZEKAJ;
				program_->push_back(tmpRozkaz);
			}
			std::cout<<"CZEKAJ "<<tmpRozkaz.wartosc<<std::endl;
			
			continue;
		}
		
		//Jeśli jest to rozkaz czekania (w minutach)
		if(tmp[0]=='M')
		{
			tmpRozkaz.wartosc=tmp.mid(1,8).toUInt()*60;
			(*czasCalkowity_)+=tmpRozkaz.wartosc;
			
			if(program_->last().stan==CZEKAJ) //Jeśli poprzedni rozkaz nakazywał czekać: dolicz do jego czasu oczekiwania nowe oczekiwanie
			{
				program_->last().wartosc+=tmpRozkaz.wartosc;
			}
			else
			{
				tmpRozkaz.stan=CZEKAJ;
				program_->push_back(tmpRozkaz);
			}
			std::cout<<"CZEKAJ "<<tmpRozkaz.wartosc<<std::endl;
			
			continue;
		}
		
		//Jeśli jest to rozkaz czekania (w godzinach)
		if(tmp[0]=='H')
		{
			tmpRozkaz.wartosc=tmp.mid(1,8).toUInt()*3600;
			(*czasCalkowity_)+=tmpRozkaz.wartosc;
			
			if(program_->last().stan==CZEKAJ)	//Jeśli poprzedni rozkaz nakazywał czekać: dolicz do jego czasu oczekiwania nowe oczekiwanie
			{
				program_->last().wartosc+=tmpRozkaz.wartosc;
			}
			else
			{
				tmpRozkaz.stan=CZEKAJ;
				program_->push_back(tmpRozkaz);
			}
			std::cout<<"CZEKAJ "<<tmpRozkaz.wartosc<<std::endl;
			
			continue;
		}
	}
	
	plik.close();
	
	wyswietlTekst("Wczytano program użytkownika.",true);
	
	return OK;
}

void TrybProgramowalny::wyswietlTekst(char const* tekst,bool czas, bool konsola)
{
	if(czas&&czasCalkowity_>0)
	{
		char tmp[256];
		sprintf(tmp,"%s\nPozostały czas trwania programu użytkownika: %02u:%02u:%02u.",tekst,*czasCalkowity_/3600,(*czasCalkowity_%3600)/60,((*czasCalkowity_)%3600)%60);
		czasWskaznik_->setText(tmp);
		if(konsola)
			std::cout<<tmp<<std::endl;
	}
	else
	{
		czasWskaznik_->setText(tekst);
		if(konsola)
			std::cout<<tekst<<std::endl;
	}
}

#include "TrybProgramowalny.moc"
