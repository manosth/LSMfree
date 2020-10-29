#ifndef CS265_BUFFER // This is a header guard. It prevents the header from being included more than once.
#define CS265_BUFFER
#include "typedefs.h"

typedef struct buffer {
// define the components of the hash table here (e.g. the array, bookkeeping for number of elements, etc)
    int max_size;
    int current_index;

    data_chunk *bfr_array;
} buffer;

int allocate_buffer(buffer** bfr, int max_size);
int put_buffer(buffer* bfr, keyType key, valType value);
int get_buffer(buffer* bfr, keyType key, valType* value);
int deallocate_buffer(buffer* bfr);

#endif
