#include "okno.hpp"
#include "rs232.h"

void clicked(GtkWidget *widget,gpointer *data)
{
	int temp=200;
	unsigned char buff[2];
	buff[0]=0xcc;
	//buff[1]=temp>>8;
	//buff[2]=temp&0xFF;
	buff[3]=0xCC;
    RS232_SendBuf(cport_nr, buff,2);
    
    /*int n;
	unsigned char buf[4096];
    for(int j=0; j < 3; j++)
	{
		n = RS232_PollComport(cport_nr, buf, 4095);

		if(n > 0)
		{
			buf[n] = 0;   //always put a "null" at the end of a string! 
			printf("received %i bytes: %u\n", n, ((buf[1])<<8)|buf[2]);
		}

		#ifdef _WIN32
		Sleep(100);
		#else
		usleep(100000);  // sleep for 100 milliSeconds 
		#endif
	}
    RS232_CloseComport(cport_nr);*/
}

Okno::Okno()
{
	okno_ = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(okno_), 640, 480);
	gtk_window_set_title (GTK_WINDOW(okno_), "Kontroler temperatury");
	
	GtkWidget *vbox;
	vbox = gtk_vbox_new(FALSE, 10);
    gtk_container_add(GTK_CONTAINER(okno_), vbox);
    
    GtkWidget *etykieta;
	etykieta = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(etykieta), "NIC");
    gtk_label_set_justify(GTK_LABEL(etykieta), GTK_JUSTIFY_CENTER);
    gtk_container_add(GTK_CONTAINER(vbox), etykieta);
    
    przycisk = gtk_button_new_with_label("Ustaw");
    gtk_widget_set_size_request(przycisk, 180, 35);
    
    gtk_box_pack_start (GTK_BOX(vbox), etykieta, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), przycisk, TRUE, TRUE, 0);
    
    gtk_signal_connect_object(GTK_OBJECT(przycisk), "clicked", GTK_SIGNAL_FUNC(clicked),NULL);
    
    gtk_widget_show_all (okno_);
	g_signal_connect(G_OBJECT(okno_), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
