#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtCore/QStringList>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

class WyborPortu:public QDialog
{
	public:
    WyborPortu(const QStringList& items)
    {
        setLayout(new QHBoxLayout());

        box = new QComboBox;
        box->addItems(items);
        layout()->addWidget(box);
		setWindowTitle("Wybierz port");
		resize(300,50);
        QPushButton* ok = new QPushButton("ok");
        layout()->addWidget(ok);
        connect(ok, &QPushButton::clicked, this, [this]()
        {
           accept();
        });
    }

    inline QComboBox* poleKombi() {return box;}

	private:
    QComboBox* box;
};
