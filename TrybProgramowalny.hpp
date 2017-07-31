#ifndef TRYB_PROGRAMOWALNY_HPP_
#define TRYB_PROGRAMOWALNY_HPP_

#include <QtCore/QTimer>
#include <QtCore/QVector>

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

class TrybProgramowalny : public QWidget
{
	Q_OBJECT
	
	private:
		QTimer* zegar_;
	public:
		TrybProgramowalny(QWidget* parent);
		~TrybProgramowalny();
		
	public slots:
		void obsluzMaszyneStanow(void);
};

#endif
