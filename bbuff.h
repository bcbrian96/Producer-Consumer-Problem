#ifndef BBUFF_H
#define BBUFF_H

#define BUFFER_SIZE 10

//data type to represent candy
typedef struct  { 
    int factory_number; //tracks which factory thread produced the candy item
    double time_stamp_in_ms; //tracks when item was created
} candy_t;

void bbuff_init(void);
void bbuff_blocking_insert(void* item);
void* bbuff_blocking_extract(void);
_Bool bbuff_is_empty(void);

#endif
