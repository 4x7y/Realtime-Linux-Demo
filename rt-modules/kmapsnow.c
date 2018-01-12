//-------------------------------------------------------------------
//	kmapsnow.cpp
//
//	This program can show a dynamic view of page-table entries
//	that are actively mapped in the kernel's pkmap_page_table,
//	updated once-per-second.  Use CONTROL-<C> for termination.
//
//	NOTE: Before this program is ready to be executed, we need
//	to determine the physical address for the pkmap_page_table
//	and assign it as the value of our 'zone_base' variable; it
//	is left as as in-class exercise to determine this address.
//
//	programmer: ALLAN CRUSE
//	written on: 07 FEB 2005
//-------------------------------------------------------------------

#include <stdio.h>	// for printf(), perror() 
#include <fcntl.h>	// for open() 
#include <stdlib.h>	// for exit() 
#include <unistd.h>	// for lseek(), read(), close(), sleep() 

unsigned long	zone_base;   //<--- TODO: correct page-address here

char filename[] = "/dev/dram";

int main( int argc, char **argv )
{
	int	fd = open( filename, O_RDONLY );
	if ( fd < 0 ) { perror( filename ); exit(1); }

	printf( "\e[H\e[J" );	// home cursor and erase screen	
	for(;;)	
	{
		// reread the current page-table
		unsigned long	pgtbl[ 1024 ] = {0};
		lseek( fd, zone_base, SEEK_SET );
		int	nbytes = read( fd, pgtbl, 4096 );
		if ( nbytes < 4096 ) { perror( "read" ); break; }

		// redraw the page-table display
		printf( "\e[H" );		
		for (int i = 0; i < 1024; i++)
			{
			char	ch = ( pgtbl[i] ) ? '*' : '.';
			if ( ( i % 64 ) == 0 ) printf( "\n%03X: ", i*4 );
			else	if ( ( i % 8 ) == 0 ) printf( " " );
			printf( "%c", ch );
			}
		printf( "\n" );	
		sleep( 1 );	// delay for one second
	}
}