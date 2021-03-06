#ifndef WYBOR_PORTU_HPP
#define WYBOR_PORTU_HPP

//Nagłówki z katalogu QtCore
#include <QtCore/QStringList>

//Nagłówki z katalogu QtWidgets
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

class WyborPortu:public QDialog
{
	public:
		WyborPortu(const QStringList& items);
		~WyborPortu();
		
		inline QComboBox* poleKombi() {return kombi_;}

	private:
		QComboBox* kombi_;
		QPushButton* ok_;
        QPushButton* przerwij_;
        QGridLayout* rozmieszczacz_;
};

#endif
