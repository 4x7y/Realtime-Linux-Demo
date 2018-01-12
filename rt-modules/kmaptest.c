//-------------------------------------------------------------------
//	pkmap.c
//
//	This module shows values for some kernel memory-management
//	constants and variables, displays some address ranges, and
//	tests use of the 'kmap_atomic()' function (which creates a 
//	temporary mapping for a physical page in high memory).
//
//	NOTE: Written and tested using kernel version 2.4.20-smp.
//
//	programmer: YUECHUAN XUE
//	written on: 09 MAR 2003
//-------------------------------------------------------------------

#include <linux/module.h>	// for init_module() 
#include <linux/proc_fs.h>	// for create_proc_read_entry() 
#include <linux/utsname.h>	// for system_utsname
#include <linux/highmem.h>	// for PKMAP_BASE
#include <linux/pagemap.h>
#include <linux/fs.h>

#define	SUCCESS	0

static char modname[] = "kmaptest";

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yuechuan Xue");
MODULE_DESCRIPTION("A Simple rt_kmap test module");

static int kmap_open(struct inode *inodep, struct file *filep);
static int kmap_release(struct inode *inodep, struct file *filep);
static int kmap_read(struct file *filep, char *buf, size_t count, loff_t *off);
static ssize_t kmap_write(struct file *filep, const char *buffer, size_t len, loff_t *offset);

/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
   .open = kmap_open,
   .read = kmap_read,
   .write = kmap_write,
   .release = kmap_release,
};

static char dmsg[1024];
static char message[256] = "Hello!";
static int  size_of_message;

static int kmap_open(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO "YUECHUAN KMAPTEST: kmap test open\n");
   return 0;
}

static int kmap_release(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO "YUECHUAN KMAPTEST: kmap test successfully closed\n");
   return 0;
}

static ssize_t kmap_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
	// store the length of the stored message
	printk(KERN_INFO "YUECHUAN KMAPTEST: Received %d characters from the user\n", count);
	copy_from_user(dmsg, buf, count);
	dmsg[count] = '\0';
	printk(KERN_INFO "YUECHUAN KMAPTEST: Receive message = %s\n", dmsg);

	unsigned long uaddr;
	uaddr = simple_strtoul(dmsg, NULL, 0);
	printk(KERN_INFO "YUECHUAN KMAPTEST: ACCESS ADDR = 0x%08x (or %d in dec)\n", uaddr, uaddr);


	// printk(KERN_INFO "\nhost: \'%s\'\n", system_utsname.nodename );
	printk(KERN_INFO "YUECHUAN KMAPTEST: PKMAP_BASE=%08X  \n", PKMAP_BASE );
	printk(KERN_INFO "YUECHUAN KMAPTEST: LAST_PKMAP=%08X  \n", LAST_PKMAP );

	unsigned long	struct_page_size = sizeof( struct page );
	//unsigned long	mem_map_size = num_physpages * struct_page_size;
	unsigned long	mem_map_start, mem_map_end, pkmap_end;
	mem_map_start = (unsigned long)mem_map;
	//mem_map_end = mem_map_start + mem_map_size;
	printk(KERN_INFO "\nmem_map: %08X-", mem_map_start );
	
	struct task_struct *task = current;
	printk(KERN_INFO "YUECHUAN KMAPTEST: comm = %s, pid = %d\n", task->comm, task->pid);
	printk(KERN_INFO "YUECHUAN KMAPTEST: prio = %d, static_prio = %d, normal_prio = %d\n", 
		task->prio, task->static_prio, task->normal_prio);
	printk(KERN_INFO "YUECHUAN KMAPTEST: rt_priority = %d\n", task->rt_priority);
	do
    {
        task = task->parent;
        printk(KERN_NOTICE "YUECHUAN KMAPTEST:\t parent process: %s, PID: %d\n", task->comm, task->pid);
    } while (task->pid != 0);


	int page_size   = sizeof(struct page);
	int page_index  = uaddr / 4096;
	int page_offset = uaddr % 4096; 
	printk(KERN_INFO "YUECHUAN KMAPTEST: page_index = 0x%08x, page_offset = 0x%08x\n", page_index, page_offset);

	int i;
	for (i = 0; i < 10; ++i)
	{
		struct page * page_ptr = &mem_map[page_index + i];
		printk(KERN_INFO "YUECHUAN KMAPTEST: page->flags = 0x%08x, page->counters = %d\n",
		page_ptr->flags, page_ptr->counters);

		void * vir_addr = kmap(page_ptr);	
		printk(KERN_INFO "YUECHUAN KMAPTEST: vir_addr = 0x%08x\n", vir_addr);
		kmap_pkcount_avail();
	}

	return count;
}

static int kmap_read(struct file *filep, char *buf, size_t len, loff_t *off)
{
	// getting global current pointer
	struct task_struct *task = current;
	printk(KERN_INFO "YUECHUAN KMAPTEST: comm = %s, pid = %d\n", task->comm, task->pid);
	printk(KERN_INFO "YUECHUAN KMAPTEST: prio = %d, static_prio = %d, normal_prio = %d\n", 
		task->prio, task->static_prio, task->normal_prio);
	printk(KERN_INFO "YUECHUAN KMAPTEST: rt_priority = %d\n", task->rt_priority);
	do
    {
        task = task->parent;
        printk(KERN_NOTICE "YUECHUAN KMAPTEST:\t parent process: %s, PID: %d\n", task->comm, task->pid);
    } while (task->pid != 0);

    // return 0;
	return simple_read_from_buffer(buf, len, off, message, strlen(message) + 1);

	printk(KERN_INFO "YUECHUAN KMAPTEST: Sent %d characters to the user\n", strlen(message) + 1);
	printk(KERN_INFO "YUECHUAN KMAPTEST: Sent \"%s\" to the user\n", message);
}


static void __exit kmap_cleanup_module( void )
{
	remove_proc_entry( modname, NULL );
	printk(KERN_INFO "YUECHUAN KMAPTEST: Removing \'%s\' module\n", modname );
}


static int __init kmap_init_module( void )
{
	printk(KERN_INFO "YUECHUAN KMAPTEST: Installing \'%s\' module\n", modname );
	proc_create( modname, 0, NULL, &fops );
	return	SUCCESS;
}

module_init(kmap_init_module);
module_exit(kmap_cleanup_module);
// module_core(my_proc_read);

MODULE_LICENSE("GPL");
