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
		QLabel* temperatura_;
		QLabel* moc_;
		
		void setupZadanaTemperatura(void);
		void setupWyslij(void);
		void setupZatrzymajGrzanie(void);
		void setupReset(void);
		
		void setupTemperatura(void);
		void setupMoc(void);
		
	public:
		TrybManualny(GlowneOkno* parent);
		~TrybManualny();
		inline int getTemperaturaDocelowa(void){return zadanaTemperatura_->value();}
		inline void setTemperaturaDocelowa(uint16_t temperatura){zadanaTemperatura_->setValue(temperatura);}
		void setMoc(uint32_t moc);
		void setTemperatua(uint32_t temperatura);
		
	public slots:
		void zrestartujUrzadenie(void);
		void ustawTemperature(bool ask=true);
		void zatrzymajGrzanie(void);
		
};

#endif
