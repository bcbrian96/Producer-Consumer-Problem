#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>

typedef struct  { 
    int factory_number;
	int num_candies_created;
	int num_candies_consumed;
	double min_delay;
	double max_delay;
	double avg_delay;
	double total_delay;
	
} stats_t;

stats_t* statsArray;
int num_factories = 0;

void stats_init(int num_producers)
{
	num_factories = num_producers;
	statsArray = malloc(num_producers*sizeof(stats_t));
	
	for(int i = 0; i < num_producers; i++){
		statsArray[i].factory_number = i;
		statsArray[i].num_candies_created = 0;
		statsArray[i].num_candies_consumed = 0;
		statsArray[i].min_delay = -1;
		statsArray[i].max_delay = -1;
		statsArray[i].avg_delay = -1;
		statsArray[i].total_delay = 0;
	}
}

void stats_cleanup(void)
{
	free(statsArray);
	statsArray = NULL;
}

void stats_record_produced(int factory_number)
{
	statsArray[factory_number].num_candies_created++;
}

void stats_record_consumed(int factory_number, double delay_in_ms)
{
	statsArray[factory_number].num_candies_consumed++;
	
	if(statsArray[factory_number].min_delay == -1){
		
		statsArray[factory_number].min_delay = delay_in_ms;
		statsArray[factory_number].max_delay = delay_in_ms;
		statsArray[factory_number].avg_delay = delay_in_ms;
		statsArray[factory_number].total_delay = delay_in_ms;
		
	}
	else {
		if(statsArray[factory_number].min_delay > delay_in_ms) {
			statsArray[factory_number].min_delay = delay_in_ms;
		}
		if(statsArray[factory_number].max_delay < delay_in_ms) {
			statsArray[factory_number].max_delay = delay_in_ms;
		}
		
		statsArray[factory_number].total_delay = statsArray[factory_number].total_delay + delay_in_ms;
		statsArray[factory_number].avg_delay = statsArray[factory_number].total_delay / (double)statsArray[factory_number].num_candies_consumed;
	}
}

void stats_display(void)
{
	printf("\n");
	printf("Statistics:\n");
	
	printf("%8s%10s%10s%18s%18s%18s\n", "Factory#", "#Made", "#Eaten", "Min Delay [ms]", "Avg Delay [ms]", "Max Delay [ms]");
	for(int i = 0; i < num_factories; i++){
		printf("%8d%10d%10d%18f%18f%18f\n", 
		statsArray[i].factory_number,
		statsArray[i].num_candies_created,
		statsArray[i].num_candies_consumed,
		statsArray[i].min_delay,
		statsArray[i].avg_delay,
		statsArray[i].max_delay);
	}
	
}
