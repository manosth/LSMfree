#include "buffer.h"
// Initialize the components of a hashtable.
// The size parameter is the expected number of elements to be inserted.
// This method returns an error code, 0 for success and -1 otherwise (e.g., if the parameter passed to the method is not null, if malloc fails, etc).
int allocate_buffer(buffer** bfr, int max_size) {
    // Check if the pointer is NULL
    if ((*bfr) != NULL) {
        return -1;
    }

    // Allocate the memory for the hashtable
    (*bfr) = malloc(sizeof(buffer));
    // Check if malloc failed
    if ((*bfr) == NULL) {
        return -1;
    }

    (**bfr).max_size = max_size;
    (**bfr).current_index = 0;
    (**bfr).bfr_array = (data_chunk *) malloc(max_size * sizeof(data_chunk));
    if ((**bfr).bfr_array == NULL) {
        return -1;
    }
    return 0;
}

// This method inserts a key-value pair into the hash table.
// It returns an error code, 0 for success and -1 otherwise (e.g., if malloc is called and fails).
int put_buffer(buffer* bfr, keyType key, valType value) {
    // Check if the hashtable is allocated
    if (bfr == NULL) {
        return -1;
    }

    // Check if we still have space in the buffer
    if ((*bfr).current_index < (*bfr).max_size) {
        if ((*bfr).bfr_array == NULL) {
            return -1;
        } else {
            (*bfr).bfr_array[(*bfr).current_index].key = key;
            (*bfr).bfr_array[(*bfr).current_index++].value = value;
        }
    } else {
        // -2 to denote put on a full buffer
        return -2;
    }
    return 0;
}

// This method retrieves entries with a matching key and stores the corresponding values in the
// values array. The size of the values array is given by the parameter
// num_values. If there are more matching entries than num_values, they are not
// stored in the values array to avoid a buffer overflow. The function returns
// the number of matching entries using the num_results pointer. If the value of num_results is greater than
// num_values, the caller can invoke this function again (with a larger buffer)
// to get values that it missed during the first call.
// This method returns an error code, 0 for success and -1 otherwise (e.g., if the hashtable is not allocated).
int get_buffer(buffer* bfr, keyType key, valType *value) {
    int idx;
    // Check if the hashtable is allocated
    if (bfr == NULL) {
        return -1;
    }

    if ((*bfr).bfr_array == NULL) {
        // -3 to differentiate from the -1 of core structure missalocation
        return -3;
    } else {
        // check in reverse order to make sure we get the most recent one
        for (idx=(*bfr).current_index - 1; idx>=0; idx--) {
            if ((*bfr).bfr_array[idx].key == key) {
                if ((*bfr).bfr_array[idx].value != GRAVEYARD) {
                    (*value) = (*bfr).bfr_array[idx].value;
                    return 0;
                } else {
                    // -5 denotes deleted value
                    return -5;
                }
            }
        }
    }
    return -1;
}

// This method frees all memory occupied by the hash table.
// It returns an error code, 0 for success and -1 otherwise.
int deallocate_buffer(buffer* bfr) {
    free((*bfr).bfr_array);
    free(bfr);
    return 0;
}
