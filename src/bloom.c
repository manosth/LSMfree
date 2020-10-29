#include "bloom.h"

// we're using 3 hash functions from this link online:
// https://gist.github.com/badboy/6267743

// might change it to this if I have time:
// https://github.com/jvirkki/libbloom
// as it feels like a more general implementation, but didn't want to copy
// paste the whole thing, and it might take a while to adapt it to what I want

// right now we have a problem; since we have bloom filters per level, we keep
// the number of bits multiple times (and also on the lsm tree level).
// To fix this, I either need to rethink how I store my bloom filters (probably best)
// or pass the lsmt as an argument to all the functions that need it (very bad idea).
// Might fix in the end if I have time.

int allocate_filter(bloom** blm_f, int n_elements, double error) {
    int idx, m, k;
    double num, denom;

    // Check if the pointer is NULL
    if ((*blm_f) != NULL) {
        return -1;
    }

    // Allocate the memory for fitler
    (*blm_f) = malloc(sizeof(bloom));
    // Check if malloc failed
    if ((*blm_f) == NULL) {
        return -1;
    }

    num = log(error);
    denom = 0.480453013918201; // ln(2)^2

    m = (int) (-n_elements * num / denom);
    k = (int) ceil(-0.693147180559945 * num / denom); // ln(2)

    (**blm_f).m_bits = m;
    (**blm_f).k_hash = k;

    (**blm_f).blm_array = (int *) malloc(m * sizeof(int));
    if ((**blm_f).blm_array == NULL) {
        return -1;
    }
    for (idx=0; idx < m; idx++) {
        (**blm_f).blm_array[idx] = 0;
    }
    return 0;
}

// technically could be void, but we keep everything as ints so
// that we can manage future error codes
int hash(bloom* blm_f, keyType key) {
    unsigned int hash_a, hash_b, hashed;
    int idx;

    hash_a = murmurhash2(&key, sizeof(int), 0x9747b28c);
    hash_b = murmurhash2(&key, sizeof(int), hash_a);

    for (idx=0; idx < (*blm_f).k_hash; idx++) {
        hashed = (hash_a + idx * hash_b) % (*blm_f).m_bits;
        (*blm_f).blm_array[hashed] = 1;
    }
    return 0;
}

int check_hash(bloom* blm_f, keyType key) {
    unsigned int hash_a, hash_b, hashed;
    int idx;

    hash_a = murmurhash2(&key, sizeof(int), 0x9747b28c);
    hash_b = murmurhash2(&key, sizeof(int), hash_a);

    for (idx=0; idx < (*blm_f).k_hash; idx++) {
        hashed = (hash_a + idx * hash_b) % (*blm_f).m_bits;
        if (!(*blm_f).blm_array[hashed]) {
            return 0;
        }
    }
    return 1;
}

// the only thing to deallocate is the array
int deallocate_filter(bloom* blm_f) {
    free((*blm_f).blm_array);
    free(blm_f);
    return 0;
}
