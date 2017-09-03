//Nagłówki z katalogu QtWidgets
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

//Standardowe nagłówki C/C++
#include <iostream>
#include <ctime>

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
	wiersze_=new QHBoxLayout[4];
	rozmieszczacz_->addLayout(&wiersze_[0]);
	rozmieszczacz_->addLayout(&wiersze_[1]);
	rozmieszczacz_->addLayout(&wiersze_[2]);
	rozmieszczacz_->addLayout(&wiersze_[3]);
	
	setupZadanaTemperatura();
	setupWyslij();
	setupTemperatura();
    wiersze_[0].addStretch();
    
    setupZatrzymajGrzanie();
    setupReset();
    setupMoc();
	wiersze_[1].addStretch();
    
    setupWybierzPlik();
    setupZapisDoPliku();
    wiersze_[2].addStretch();
    
    setupKomenda();
    setupWyslijRozkaz();
    wiersze_[3].addStretch();
}

TrybManualny::~TrybManualny()
{
	rodzic_=nullptr;
	zamknijPlik();
	delete temperatura_;
	delete moc_;
	delete zadanaTemperatura_;
    delete wyslij_;
    delete reset_;
    delete wybierzPlik_;
    delete zapisDoPliku_;
    delete zatrzymajGrzanie_;
    delete wyslijRozkaz_;
    delete komenda_;
    delete[] nazwaPliku_;
    delete[] wiersze_;
    delete rozmieszczacz_;
}

void TrybManualny::setupZadanaTemperatura(void)
{
	zadanaTemperatura_=new QSpinBox;
	wiersze_[0].addWidget(zadanaTemperatura_);
	zadanaTemperatura_->setFixedSize(180,30);
    zadanaTemperatura_->setRange(0, 999);
    zadanaTemperatura_->setSingleStep(1);
    zadanaTemperatura_->setSuffix(" ℃");   
}

void TrybManualny::setupWyslij(void)
{
	wyslij_=new QPushButton("Ustaw",this);
	wyslij_->setFixedSize(180,30);
	wiersze_[0].addWidget(wyslij_);
	
	QObject::connect(wyslij_, SIGNAL(clicked(bool)),this, SLOT(ustawTemperature()));
}

void TrybManualny::setupZatrzymajGrzanie(void)
{
	QPalette* czerwony = new QPalette();
	czerwony->setColor(QPalette::ButtonText,Qt::red);
	
	zatrzymajGrzanie_=new QPushButton("Zatrzymaj grzanie",this);
	zatrzymajGrzanie_->setFixedSize(180,30);
	zatrzymajGrzanie_->setPalette(*czerwony);
	
	wiersze_[1].addWidget(zatrzymajGrzanie_);
	
	delete czerwony;
	
	QObject::connect(zatrzymajGrzanie_, SIGNAL(clicked(bool)),this, SLOT(zatrzymajGrzanie()));
}

void TrybManualny::setupReset(void)
{
	reset_=new QPushButton("Reset",this);
	reset_->setFixedSize(180,30);
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
	wybierzPlik_->setFixedSize(180,30);
	wiersze_[2].addWidget(wybierzPlik_);
	
	QObject::connect(wybierzPlik_, SIGNAL(clicked(bool)),this, SLOT(wybierzPlik()));
}

void TrybManualny::setupZapisDoPliku(void)
{
	zapisDoPliku_=new QCheckBox("Zapis do pliku",this);
	zapisDoPliku_->setFixedSize(180,30);
	zapisDoPliku_->setChecked(false);
	zapisDoPliku_->setEnabled(false);
	wiersze_[2].addWidget(zapisDoPliku_);
	
	QObject::connect(zapisDoPliku_, SIGNAL(clicked(bool)),this, SLOT(zapisujDoPliku(bool)));
}

void TrybManualny::setupWyslijRozkaz(void)
{
	wyslijRozkaz_=new QPushButton("Wyślij rozkaz",this);
	wyslijRozkaz_->setFixedSize(180,30);
	wiersze_[3].addWidget(wyslijRozkaz_);
	
	QObject::connect(wyslijRozkaz_, SIGNAL(clicked(bool)),this, SLOT(wyslijRozkaz()));
}

void TrybManualny::setupKomenda(void)
{
	komenda_=new QLineEdit(this);
	komenda_->setFixedSize(180,30);
	komenda_->setMaxLength(32);
	wiersze_[3].addWidget(komenda_);
	komenda_->setPlaceholderText("Wprowadź rozkaz");
	
	QObject::connect(komenda_, SIGNAL(returnPressed()),this, SLOT(wyslijRozkaz()));
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
	
	time_t czas;
	struct tm* czas2;
	char data[128];
	
	time(&czas);
	czas2=localtime(&czas);

	strftime(data,sizeof(data),"%d-%m-%y,%H:%M:%S",czas2);

	plikDoZapisu_=fopen(nazwaPliku_,"a");
	
	if(!plikDoZapisu_)
		return BLAD_OTWARCIA_PLIKU;
	otwartyPlik_=true;
	fprintf(plikDoZapisu_,"Pomiar,%s\n",data);
	fprintf(plikDoZapisu_,"t \\s,T \\℃, P [0-255]\n");
	return OTWARTO_PLIK;
}

int TrybManualny::zamknijPlik(void)
{
	if(otwartyPlik_)
	{
		zapisujDoPliku_=false;
		fflush(plikDoZapisu_);
		fprintf(plikDoZapisu_,"\n");
		fclose(plikDoZapisu_);
		otwartyPlik_=false;
		zapisDoPliku_->setChecked(false);
		zapisDoPliku_->setEnabled(false);
		return ZAMKNIETO_PLIK;
	}
	return NIE_ZAMKNIETO_PLIKU;
}

void TrybManualny::zrestartujUrzadenie(void)
{
	rodzic_->zrestartujUrzadenie(true);
}

void TrybManualny::ustawTemperature(const bool ask)
{
	rodzic_->ustawTemperature(ask);
}

void TrybManualny::zatrzymajGrzanie(void)
{
	rodzic_->zatrzymajGrzanie(true);
}

void TrybManualny::wyslijRozkaz(const bool ask)
{
	rodzic_->wyslijRozkaz(komenda_->text().toLocal8Bit().data(), ask);
}

void TrybManualny::wybierzPlik(void)
{
	zamknijPlik();
	if(wskazPlik())
	{
		return;
	}
	if(otworzPlik()==BLAD_OTWARCIA_PLIKU)
	{
		QMessageBox::warning(this, tr("Błąd otwarcia pliku!"),"Nie można otworzyć pliku.");
		return;
	}
	zapisDoPliku_->setChecked(false);
	zapisDoPliku_->setEnabled(true);
}

int TrybManualny::wskazPlik(void)
{
	QString nazwaPlikuTmp = QFileDialog::getSaveFileName(this,tr("Wybierz plik do zapisu"), "",tr("Dane rozdzielone przecinkami (*.csv);;Pliki dziennika (*.log);;Wszystkie pliki (*)"));
    
    if (nazwaPlikuTmp.isEmpty())
    {
		QMessageBox::warning(this, tr("Brak nazwy pliku!"),"Nie podano nazwy pliku do zapisu.");
        return BRAK_NAZWY_PLIKU;
	}
    
    strncpy(nazwaPliku_,nazwaPlikuTmp.toLocal8Bit().data(),255);
    return OK;
}

void TrybManualny::zapisujDoPliku(const bool stan)
{
	zapisujDoPliku_=stan;
}

#include "TrybManualny.moc"
