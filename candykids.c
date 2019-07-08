#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "stats.h"
#include "bbuff.h"

#define NUM_ARGS 3

double current_time_in_ms(void)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

_Bool stop_thread = false; //flag to end thread

 void *factoryThread(void *param) {
	
	int factoryCount = *(int*)param;
	int waitSecond = 0;
	
	while(!stop_thread){
	
	waitSecond = (rand() % (3 + 1));
	printf("\tFactory %d ships candy & waits %ds\n", factoryCount, waitSecond);
	
	candy_t *candy = malloc(sizeof(candy_t));
	candy->factory_number = factoryCount;
	candy->time_stamp_in_ms = current_time_in_ms();
	
	bbuff_blocking_insert(candy);
	stats_record_produced(factoryCount);
	
	sleep(waitSecond);

	}
	
	printf("Candy-factory %d done\n", factoryCount);
	pthread_exit(NULL);
	
} 

void *kidThread(void *param) {
	
	//int kidCount = *(int*)param;
	int waitSecond = 0;
	
	while(true){
		
		waitSecond = (rand() % (1 + 1));	
		
		candy_t* candy = (candy_t*)bbuff_blocking_extract();
		stats_record_consumed(candy->factory_number, current_time_in_ms() - candy->time_stamp_in_ms);
		
		//printf("\tKid %d eats candy & waits %ds\n", kidCount, waitSecond);
		
		//if(candy == NULL){
		//	printf("testing: candy_ptr is null.\n");
		//}
		sleep(waitSecond);
	}
	
	pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
	int numFactories = 0;
	int numKids = 0;
	int numSeconds = 0;
	
	// 1.  Extract arguments
	if (argc != 4){
		printf("ERROR: Invalid Argument.\n");
		exit(-1);
	}
	numFactories = atoi(argv[1]);
	numKids = atoi(argv[2]);
	numSeconds = atoi(argv[3]);
	
	if (numFactories <= 0 || numKids <= 0 || numSeconds <= 0){
		printf("ERROR: Arguments must be greater than 0.\n");
		exit(-1);
	}
	
	// 2.  Initialize modules
	bbuff_init();
	stats_init(numFactories);
	
	srand(time(NULL));
	
	// 3.  Launch candy-factory threads
	int factoryStatus = 0;
	pthread_t factory_tid[numFactories];
	int factoryCount[numFactories];
	
	for(int i = 0; i < numFactories; i++){
		//printf("Main here. Creating thread %d\n", i);
		factoryCount[i] = i;
		factoryStatus = pthread_create(&factory_tid[i], NULL, factoryThread, (void *)&factoryCount[i]);
		
		if (factoryStatus != 0){
			printf("pthread_create returned error code.\n");
			exit(-1);
		}
	}
	
	// 4.  Launch kid threads
	int kidStatus = 0;
	pthread_t kid_tid[numKids];
	int kidCount[numKids];
	
	for (int i = 0; i < numKids; i++){
		kidCount[i] = i;
		kidStatus = pthread_create(&kid_tid[i], NULL, kidThread, (void *)&kidCount[i]);
	
		if (kidStatus != 0){
				printf("pthread_create returned error code.\n");
				exit(-1);
			}
	}
	
	// 5.  Wait for requested time
	for(int i = 0; i < numSeconds; i++){
		printf("Time %ds:\n", i);
		sleep(1);
	}
	
	// 6.  Stop candy-factory threads
	int joinStatus = 0;
	stop_thread = true;
	
	printf("Stopping candy factories...\n");

	for(int i = 0; i < numFactories; i++){
		joinStatus = pthread_join(factory_tid[i], NULL);
		
		if (joinStatus != 0){
			printf("Error joining thread.\n");
			exit(-1);
		}
	}
	
	// 7.  Wait until no more candy
	while(!bbuff_is_empty()){
		printf("Waiting for all candy to be consumed.\n");
		sleep(1);
	}
	
	// 8.  Stop kid threads
	printf("Stopping kids.\n");
	
	for(int i = 0; i < numKids; i++){
		
		if (pthread_cancel(kid_tid[i]) != 0) {
			printf("Error canceling thread.\n");
			exit(-1);
		}
		
		if (pthread_join(kid_tid[i], NULL) != 0) {
			printf("Error joining thread.\n");
			exit(-1);
		}
	}
	
	// 9.  Print statistics
	stats_display();
	
	// 10. Cleanup any allocated memory
	stats_cleanup();
	
	return 0;
}
