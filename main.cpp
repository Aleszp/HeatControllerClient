#include <gtk/gtk.h>
#include "okno.hpp"
#include "rs232.h"

int cport_nr=17;        // 16 - /dev/ttyUSB0 17 - /dev/ttyUSB1  

void* thread_func(void *args)
{
	int n;
    while( TRUE )
    {
		g_thread_yield ();
        #ifdef _WIN32
		Sleep(100);
		#else
		usleep(100000);  // sleep for 100 milliSeconds 
		#endif

        gdk_threads_enter();
        unsigned char buf[4096];
		for(int j=0; j < 3; j++)
		{
			n = RS232_PollComport(cport_nr, buf, 4095);

			if(n > 0)
			{
				buf[n] = 0;   //always put a "null" at the end of a string! 
				printf("received %i bytes: %s\n", n, buf);
			}

		}
        gdk_threads_leave();
    }

    return NULL;
}

int main (int argc, char *argv[]) 
{
    int bdrate=57600;
    char mode[]={'8','N','1',0};
	
    if(RS232_OpenComport(cport_nr, bdrate, mode))
	{
		printf("Can not open comport\n");
		return 1;
	}
	
	gtk_init (&argc, &argv);
	
	GThread   *thread;
	GError    *error = NULL;
	void* data;
    thread = g_thread_create(thread_func,&data,FALSE,&error);
    if( ! thread )
    {
        g_print( "Error: %s\n", error->message );
        return( -1 );
    }
	
	Okno okno;
    
    gtk_main ();
    
    

    return 0;
}
