//Nagłówki z katalogu QtWidgets
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

//Nagłówki z katalogu programu
#include "GlowneOkno.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication Aplikacja(argc, argv);
	GlowneOkno Okno;
    
    return Aplikacja.exec();
}
