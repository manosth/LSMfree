#ifndef CS265_FUNCTIONS // This is a header guard. It prevents the header from being included more than once.
#define CS265_FUNCTIONS
#include "typedefs.h"

int merge(data_chunk *data_array, int start, int mid_start, int mid_end, int end);
int merge_sort(data_chunk *data_array, int start, int end);
int binary_fences(int *fences, int start, int end, keyType key);
int binary_values(data_chunk *values, int start, int end, keyType key);
#endif
