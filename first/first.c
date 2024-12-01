#include <stdio.h>
#include <stdlib.h>

void read_arrays(const char *filename, int *array1, int *array2, int size) {
    FILE *file = fopen(filename, "r");
    for (int i = 0; i < size; i++) {
        fscanf(file, "%5d", &array1[i]);
        fscanf(file, "%5d", &array2[i]);
    }
    fclose(file);
}

int compare( const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}


int sum_absolute_differences(const int *array1, const int *array2, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) sum += abs(array1[i] - array2[i]);
    return sum;
}

int main() {
    int array1[1000], array2[1000];
    read_arrays("input.txt", array1, array2, 1000);

    qsort(array1, 1000, sizeof(int), compare);
    qsort(array2, 1000, sizeof(int), compare);

    int sum = 0;
    for (int u = 0; u < 1000; u++) {
        sum += abs(array1[u] - array2[u]);
    }

    printf("Part One answer: Sum of absolute differences: %d\n", sum);

    int total_similarity_score = 0;
    // as I am still learning C, I will document some steps and write some redundancy, I already know that this is 1000 :)
    int array1totalmemory = sizeof(array1);
    int array1oneelementmemory = sizeof(array1[0]);
    int array2totalmemory = sizeof(array2);
    int array2oneelementmemory = sizeof(array2[0]);
    int array1size = array1totalmemory / array1oneelementmemory;
    int array2size = array2totalmemory / array2oneelementmemory;

    // O(Logn^2)
    for (int i = 0; i < array1size; i++) {
            int count = 0;
            for (int j = 0; j < array2size; j++) {
                if (array1[i] == array2[j]) {
                    count++;
                }
            }
        total_similarity_score += array1[i] * count;
    }

    printf("Part Two answer: Calculated similarity score %d\n", total_similarity_score);

    return 0;
}
