//Nagłówki z katalogu QtWidgets
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

//Nagłówki z katalogu programu
#include "GlowneOkno.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GlowneOkno w;
    
    return a.exec();
}
