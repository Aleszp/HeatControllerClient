#ifndef TRYB_PROGRAMOWALNY_HPP_
#define TRYB_PROGRAMOWALNY_HPP_

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtCore/QTimer>

class TrybProgramowalny : public QWidget
{
	Q_OBJECT
	
	private:
		QTimer zegar_;
	public:
		TrybProgramowalny(QWidget* parent);
		~TrybProgramowalny();
};

#endif
