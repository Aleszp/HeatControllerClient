#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtCore/QStringList>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

class WyborPortu:public QDialog
{
	public:
    WyborPortu(const QStringList& items)
    {
		setWindowTitle("Wybierz port");
		resize(300,50);
		
		rozmieszczacz_=new QGridLayout(this);
        setLayout(rozmieszczacz_);
        
        kombi_=new QComboBox;
        kombi_->addItems(items);
        ok_=new QPushButton("OK");
        przerwij_=new QPushButton("Przerwij");
        
        rozmieszczacz_->addWidget(kombi_,0,0,1,2);
        rozmieszczacz_->addWidget(ok_,1,0);
        rozmieszczacz_->addWidget(przerwij_,1,1);
        
        connect(ok_, &QPushButton::clicked, this, [this]()
        {
           accept();
        });
        connect(przerwij_, &QPushButton::clicked, this, [this]()
        {
           reject();
        });
    }

    inline QComboBox* poleKombi() {return kombi_;}

	private:
		QComboBox* kombi_;
		QPushButton* ok_;
        QPushButton* przerwij_;
        QGridLayout* rozmieszczacz_;
};
