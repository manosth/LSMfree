#include "functions.h"

// merging function that returns 0 if everything went smoothly, or
// the number of "deleted" items (duplicate entries), in order to
// update the current statistics
int merge(data_chunk *data_array, int start, int mid_start, int mid_end, int end) {
    int i, j, k;
    int n1 = mid_start - start + 1;
    int n2 =  end - mid_end;
    int dup_count = 0;

    /* create temp arrays */
    data_chunk *L, *R;
    L = (data_chunk *) malloc(n1 * sizeof(data_chunk));
    R = (data_chunk *) malloc(n2 * sizeof(data_chunk));

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = data_array[start + i];
    for (j = 0; j < n2; j++)
        R[j] = data_array[mid_end + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = start; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].key < R[j].key) {
            data_array[k] = L[i];
            i++;
        } else if (L[i].key == R[j].key) {
            data_array[k] = R[j];
            while (L[i].key == R[j].key) {
                i++;
                dup_count++;
            }
            j++;
        } else {
            data_array[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1) {
        data_array[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2) {
        data_array[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);

    return dup_count;
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
int merge_sort(data_chunk *data_array, int start, int end) {
    if (start < end) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int mid = start + (end - start) / 2;
        int output, out_start, out_end;

        // Sort first and second halves
        out_start = merge_sort(data_array, start, mid);
        out_end = merge_sort(data_array, mid + 1, end);

        output = merge(data_array, start, mid - out_start, mid, end - out_end);
        return output + out_start + out_end;
    }
    return 0;
}

int binary_fences(int *fences, int start, int end, keyType key) {
    int mid = start + (end - start) / 2, ret;

    if (end == start) {
        if (fences[mid] >= key) {
            return mid;
        } else {
            return -1;
        }

    }

    if (key > fences[mid])
        return binary_fences(fences, mid + 1, end, key);

    ret = binary_fences(fences, start, mid, key);
    if (ret == -1) {
        return mid;
    } else {
        return ret;
    }
}

int binary_values(data_chunk *values, int start, int end, keyType key) {
    if (end >= start) {
        int mid = start + (end - start) / 2;

        // If the element is present at the middle
        // itself
        if (values[mid].key == key)
            return mid;

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if (values[mid].key > key)
            return binary_values(values, start, mid - 1, key);

        // Else the element can only be present
        // in right subarray
        return binary_values(values, mid + 1, end, key);
    }

    // We reach here when element is not
    // present in array
    return -1;
}
