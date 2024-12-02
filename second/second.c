# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>


void read_arrays(const char *filename, int array_of_reports[][8], int max_rows) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char line[256]; // this buffer will hold each line
    int row = 0;
    // fgets gets a line
    while (row < max_rows && fgets(line, sizeof(line), file)) {
        int nums[8] = {0}; // Initialize array with zeros
        int n = 0;
        // this will split the line into tokens
        char *token = strtok(line, " \t\n");
        while (token != NULL && n < 8) {
            // atoi convertes token(string) into an integer
            nums[n++] = atoi(token);
            token = strtok(NULL, " \t\n");
        }
        // copy nums into array_of_reports
        for (int j = 0; j < 8; j++) {
            array_of_reports[row][j] = nums[j];
        }
        row++;
    }
    fclose(file);
}


bool is_row_valid(int row[], int size) {
    int prev = -1; // we know that we do not have negative numbers
    int dir = 0; // direction, +1 for increase, -1 for decrease
    for (int i = 0; i < size; i++) {
        if (row[i] == 0) {
            continue; // ignore zeroes
        }
        if (prev == -1) {
            prev = row[i];
            continue;
        }

        int diff = row[i] - prev;
        // establish direction
        if (dir == 0) {
            if (diff == -3 || diff == -2 || diff == -1) {
                dir = -1;
            } else if (diff == 3 || diff == 2 || diff == 1) {
                dir = 1;
            } else { return false;}}

        // check whether the array follows the direction
        if ((dir == 1 && (diff != 1 && diff != 2 && diff != 3)) ||
            (dir == -1 && (diff != -1 && diff != -2 && diff != -3))) {
            return false;
        }
        prev = row[i];
    }
    return true;
}



bool is_row_valid_without_one_element(int row[], int size, int skip_index) {
    // this is almost the same as the one for the first part one, I added an index to skip here
    // keeping the functions separate to show the pattern of thought here
    int prev = -1;
    int dir = 0; // direction, +1 for increase, -1 for decrease

    for (int i = 0; i < size; i++) {
        if (i == skip_index || row[i] == 0) {
            continue; // skip the specified index or zero values
        }

        if (prev == -1) {
            prev = row[i];
            continue;
        }

        int diff = row[i] - prev;
        // establish direction
        if (dir == 0) {
            if (diff == -3 || diff == -2 || diff == -1) {
                dir = -1;
            } else if (diff == 3 || diff == 2 || diff == 1) {
                dir = 1;
            } else { return false;}}

        // check whether the array follows the direction
        if ((dir == 1 && (diff != 1 && diff != 2 && diff != 3)) ||
            (dir == -1 && (diff != -1 && diff != -2 && diff != -3))) {
            return false;
        }
        prev = row[i];
    }

    return true;
}


// I know that the input data has no 0
// Maximum row element count of 8,
// The cleaner solution would be to create a dynamic array, but I am low on time
int main() {
    int array_of_reports[1000][8] = {0}; // this initializes the array, filling it with 0

    const char *filename = "input.txt";
    read_arrays(filename, array_of_reports, 1000);
    for (int i = 0; i < 1000; i++) {
        int empty_row = 1;
        printf("Row %d: ", i + 1);
        for (int j = 0; j < 8; j++) {
            printf("%d ", array_of_reports[i][j]);
            if (array_of_reports[i][j] != 0) {
                empty_row = 0;
            }
        }
        printf("\n");
    }
    int valid_row_count_one = 0;
    for (int i = 0; i < 1000; i++) {
        if (is_row_valid(array_of_reports[i], 8)) {
            valid_row_count_one++;
        }
    }
    printf("Part One: Valid row count: %d", valid_row_count_one);

    int valid_row_count_two = 0;
    for (int i = 0; i < 1000; i++) {
        // first check if it is just fine
        if (is_row_valid_without_one_element(array_of_reports[i], 8, -1)) {
                valid_row_count_two ++;
        } else {
            // then check whether it is fine removing one element
            for (int skip = 0; skip < 8; skip++) {
                if (is_row_valid_without_one_element(array_of_reports[i], 8, skip)) {
                    valid_row_count_two++;
                    break;
                }
            }
        }
    }
    printf("Part Two: Valid row count with safety: %d", valid_row_count_two);
}
