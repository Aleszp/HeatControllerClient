#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include "MainWindow.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
    //w.show();
    
    return a.exec();
}
