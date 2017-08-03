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

//Nagłówki z katalogu programu
#include "TypyWyliczeniowe.hpp"
#include "Struktury.hpp"


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
		QHBoxLayout* rozmieszczacz_;
		
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
