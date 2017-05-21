#include <gtk/gtk.h>

extern const int cport_nr;        /* /dev/ttyUSB0  */

class Okno
{
	public:
	GtkWidget *okno_;
	GtkWidget *kontener;
	GtkWidget *przycisk;
	
	Okno();
    
};
