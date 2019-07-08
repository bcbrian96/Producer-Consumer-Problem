#include "bbuff.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>

void* buffer[BUFFER_SIZE]; //buffer array

sem_t mutex;
sem_t full;
sem_t empty;
int index = 0;
int out = 0;
int emptyVal;

//initialize bounded buffer module (constructor)
void bbuff_init(void)
{
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	index = 0;
}

//synchronize producer and consumer processes
void bbuff_blocking_insert(void* item)
{
	sem_wait(&empty);
	sem_wait(&mutex);
	
	//add item to buffer
	buffer[index] = item;
	index = (index+1) % BUFFER_SIZE;
	
	
	sem_post(&mutex);
	sem_post(&full);
	
}

void* bbuff_blocking_extract(void)
{
	sem_wait(&full);
	sem_wait(&mutex);
	
	//remove item from buffer
	/* void* item = buffer[index-1];
	buffer[index-1] = NULL;
	index--; */
	
	void* item = buffer[out];
	out = (out + 1) % BUFFER_SIZE;
	
	sem_post(&mutex);
	sem_post(&empty);
	
	return item;
	
}


/* _Bool bbuff_is_empty(void)
{
	if(index == 0){
		return true;
	}
	else{
		return false;
	}
	
} */

_Bool bbuff_is_empty(void)
{

	sem_getvalue(&empty, &emptyVal);
	if(emptyVal == 10){
		return true;
	}
	return false;
	
}

