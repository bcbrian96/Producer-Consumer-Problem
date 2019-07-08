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
	
} stats_t;

stats_t* statsArray;

void stats_init(int num_producers)
{
	statsArray = malloc(num_producers*sizeof(stats_t));
	
	for(int i = 0; i < num_producers; i++){
		statsArray[i].factory_number = i;
		statsArray[i].num_candies_created = 0;
		statsArray[i].num_candies_consumed = 0;
		statsArray[i].min_delay = 0;
		statsArray[i].max_delay = 0;
		statsArray[i].avg_delay = 0;
	}
}

void stats_cleanup(void)
{
	free(statsArray);
	statsArray = NULL;
}

void stats_record_produced(int factory_number)
{
	
}

void stats_record_consumed(int factory_number, double delay_in_ms)
{
	
}

void stats_display(void)
{
	
}
