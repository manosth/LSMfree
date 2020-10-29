#ifndef CS265_BLOOM // This is a header guard. It prevents the header from being included more than once.
#define CS265_BLOOM
#include "typedefs.h"
#include "murmurhash2.h"

int allocate_filter(bloom** blm_f, int n_elements, double error);
int hash(bloom* blm_f, keyType key);
int check_hash(bloom* blm_f, keyType key);
int deallocate_filter(bloom* blm_f);

#endif
