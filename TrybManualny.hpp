#ifndef TRYB_MANUALNY_HPP_
#define TRYB_MANUALNY_HPP_

//Nagłówki z katalogu QtCore
#include <QtCore/QTimer>
#include <QtCore/QVector>

//Nagłówki z katalogu QtWidgets
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
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

class TrybManualny : public QWidget
{
	Q_OBJECT
	
	private:
		GlowneOkno* rodzic_;
		
		QVBoxLayout* rozmieszczacz_;
		QHBoxLayout* wiersze_;
		
		QSpinBox* zadanaTemperatura_;
		QPushButton* wyslij_;
		QPushButton* zatrzymajGrzanie_;
		QPushButton* reset_;
		
		void setupTemperatura(void);
		void setupWyslij(void);
		void setupZatrzymajGrzanie(void);
		void setupReset(void);
		
	public:
		TrybManualny(GlowneOkno* parent);
		~TrybManualny();
		inline int getTemperatura(void){return zadanaTemperatura_->value();}
		inline void setTemperatura(uint16_t temperatura){zadanaTemperatura_->setValue(temperatura);}
		
	public slots:
		void zrestartujUrzadenie(void);
		void ustawTemperature(bool ask=true);
		void zatrzymajGrzanie(void);
		
};

#endif
