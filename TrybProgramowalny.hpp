#ifndef TRYB_PROGRAMOWALNY_HPP_
#define TRYB_PROGRAMOWALNY_HPP_

//Nagłówki z katalogu QtCore
#include <QtCore/QTimer>
#include <QtCore/QVector>

//Nagłówki z katalogu QtWidgets
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>

//Standardowe nagłówki C/C++
#include <cstdint>

//Nagłówki z katalogu programu
#include "TypyWyliczeniowe.hpp"
#include "Struktury.hpp"

//Zadeklaruj istnienie klasy GlowneOkno
class GlowneOkno;

class TrybProgramowalny : public QWidget
{
	Q_OBJECT
	
	private:
		GlowneOkno* rodzic_;
		QTimer* zegar_;
		QVector <Rozkaz>* program_;
		QPushButton* wczytaj_;
		QPushButton* startStop_;
		QLabel* pozostalyCzas_;
		QVBoxLayout* rozmieszczacz_;
		QHBoxLayout* rozmieszczaczeWierszowe_;
		QLabel* czasWskaznik_;
		uint32_t* czasCalkowity_;
		
		void wyswietlTekst(char const* tekst, bool czas=false, bool konsola=false);
		
	public:
		TrybProgramowalny(GlowneOkno* parent);
		~TrybProgramowalny();
		
	public slots:
		void obsluzMaszyneStanow(void);
		void start(void);
		void stop(void);
		int wczytajProgram(void);
};

#endif
