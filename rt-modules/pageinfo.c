//-------------------------------------------------------------------
//	pageinfo.c
//
//	This module creates a pseudo-file named '/proc/pageinfo' to
//	show the sizes of various data-types used in the definition 
//	for the kernel's 'struct page' object, and the total length 
//	of a 'struct page' object for our kernel's configuration.
//
//	NOTE: Developed and tested with Linux kernel version 2.6.10
//
//	programmer: ALLAN CRUSE
//	written on: 02 FEB 2005
//-------------------------------------------------------------------

#include <linux/module.h>	// for init_module() 
#include <linux/proc_fs.h>	// for create_proc_info_entry() 
#include <linux/mm.h>		// for 'struct page' definition

static char modname[] = "pageinfo";

static int my_get_info( char *buf, char **start, off_t off, int count )
{
	int	pfsz = sizeof( page_flags_t );
	int	atsz = sizeof( atomic_t );
	int	pvsz = sizeof( unsigned long );
	int	assz = sizeof( struct address_space * );
	int	posz = sizeof( pgoff_t );
	int	lhsz = sizeof( struct list_head );
	int	pgsz = sizeof( struct page );
	int	len = 0;

	len += sprintf( buf+len, "\nsizeof( page_flags_t ) = %d bytes ", pfsz );
	len += sprintf( buf+len, "\nsizeof( atomic_t ) = %d bytes ", atsz );
	len += sprintf( buf+len, "\nsizeof( unsigned long ) = %d bytes", pvsz );
	len += sprintf( buf+len, "\nsizeof( struct address_space * ) " );
	len += sprintf( buf+len, "= %d bytes ", assz );
	len += sprintf( buf+len, "\nsizeof( pgoff_t ) = %d bytes ", posz );
	len += sprintf( buf+len, "\nsizeof( pgoff_t ) = %d bytes ", posz );
	len += sprintf( buf+len, "\nsizeof( struct list_head ) = " );
	len += sprintf( buf+len, "%d bytes  ", lhsz );
	len += sprintf( buf+len, "\nsizeof( struct page ) = %d bytes ", pgsz );
	len += sprintf( buf+len, "\n\n" );
	return	len;
}

int init_module( void )
{
	printk( "<1>\nInstalling \'%s\' module\n", modname );
	create_proc_info_entry( modname, 0, NULL, my_get_info );
	return	0;  //SUCCESS
}

void cleanup_module( void )
{
	remove_proc_entry( modname, NULL );
	printk( "<1>Removing \'%s\' module\n", modname );
}

MODULE_LICENSE("GPL");
