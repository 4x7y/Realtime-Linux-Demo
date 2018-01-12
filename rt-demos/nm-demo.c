 
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/mman.h>

void *thread_func(void *ptr)
{
	 /* Do RT specific stuff here */
    FILE *kmaptest;
    char tempstring[1024];
    if(!(kmaptest=fopen("/proc/kmaptest","wb")))
    {
         fprintf(stderr,"Could not open kmaptest\n");
         exit(1);
    }
    sprintf(tempstring, "%s", "0x6b341000");
    fwrite(tempstring, sizeof(char), strlen(tempstring) + 1, kmaptest);
    fclose(kmaptest);

	return NULL;
}

int main()
{
	/* this variable is our reference to the second thread */
	pthread_t inc_x_thread;

	/* create a second thread which executes inc_x(&x) */
	if(pthread_create(&inc_x_thread, NULL, thread_func, NULL)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;

	}
	
	/* wait for the second thread to finish */
	if(pthread_join(inc_x_thread, NULL))
	{
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}

	return 0;
}