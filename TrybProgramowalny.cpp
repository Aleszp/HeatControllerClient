#include "TrybProgramowalny.hpp"

TrybProgramowalny::TrybProgramowalny(QWidget* parent=0):QWidget(parent)
{
	zegar_=new QTimer(this);
	zegar_->start(1000);
	
	QObject::connect(zegar_, SIGNAL(timeout()),this, SLOT(obsluzProgram()));
}

TrybProgramowalny::~TrybProgramowalny()
{
	delete zegar_;
}

void TrybProgramowalny::obsluzMaszyneStanow(void)
{
	/*Obsłuż maszynę stanów*/
	
	zegar_->start(1000);	//ponownie uruchom zegar
}


#include "TrybProgramowalny.moc"
