#ifndef CS265_TYPES // This is a header guard. It prevents the header from being included more than once.
#define CS265_TYPES
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

typedef int keyType;
typedef int valType;

#define GRAVEYARD -2147483648
#define CHAR_SIZE 40
#define EXTRA_SIZE 40

typedef struct data_chunk {
    keyType key;
    valType value;
} data_chunk;

typedef struct bloom {
    int m_bits;
    int k_hash;

    // our implementation is suboptimal, since we use an int array
    // could maybe be an optimization, but probably no time
    int *blm_array;
} bloom;

typedef struct run {
    int run_size;
    int *fence_p;
    char **files;
} run;

typedef struct level {
    int curr_runs;
    int curr_size;

    bloom *blm_f;
    run *runs;
} level;
#endif
