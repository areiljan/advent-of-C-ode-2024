# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <regex.h>

# define MAX_LINE_LENGTH 65536 // 64KB buffer
# define MAX_MATCHES 2000


int extract_valid_mul_instructions(const char* input, int* results) {
    printf("%s", input);
    regex_t regex;
    regmatch_t matches[MAX_MATCHES];
    int num_results = 0;

    // Compile the regex pattern
    int reti = regcomp(&regex, "mul\\(([0-9]+),([0-9]+)\\)", REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    const char* p = input;

    while (num_results < MAX_MATCHES &&
           regexec(&regex, p, MAX_MATCHES, matches, 0) == 0) {

        char num1[20] = {0};
        char num2[20] = {0};

        int len1 = matches[1].rm_eo - matches[1].rm_so;
        strncpy(num1, p + matches[1].rm_so, len1);

        int len2 = matches[2].rm_eo - matches[2].rm_so;
        strncpy(num2, p + matches[2].rm_so, len2);

        results[num_results] = atoi(num1) * atoi(num2);
        num_results++;

        p += matches[0].rm_eo;
    }

    regfree(&regex);

    return num_results;
}


int extract_valid_mul_instructions_with_do(const char* input, int* results) {
    printf("%s\n", input);

    const char *pattern = "mul\\(([0-9]+),([0-9]+)\\)|(do\\(\\)|don't\\(\\))";
    regex_t regex;
    regmatch_t matches[4];
    bool is_enabled = true; // Start enabled
    int num_results = 0;

    // Compile the combined regex
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Could not compile combined regex\n");
        return 0;
    }

    const char *p = input;
    size_t len = strlen(input);

    // We'll iterate over the input and find all matches
    while ((p - input) < (int)len && num_results < MAX_MATCHES) {
        // Execute the regex on the remaining string
        if (regexec(&regex, p, 4, matches, 0) == 0) {
            // Check which type of instruction was matched:
            // matches[1] and matches[2] => mul(x,y)
            // matches[3] => do() or don't()

            if (matches[2].rm_so != -1) {
                // We have a mul(x,y)
                // Extract the numbers from the capturing groups
                char num1[20] = {0};
                char num2[20] = {0};

                int len1 = matches[1].rm_eo - matches[1].rm_so;
                strncpy(num1, p + matches[1].rm_so, len1);
                int len2 = matches[2].rm_eo - matches[2].rm_so;
                strncpy(num2, p + matches[2].rm_so, len2);

                if (is_enabled) {
                    int val1 = atoi(num1);
                    int val2 = atoi(num2);
                    results[num_results] = val1 * val2;
                    num_results++;
                    printf("Computed %d * %d = %d at position %ld\n",
                           val1, val2, results[num_results - 1],
                           (long)(p - input + matches[0].rm_so));
                }

            } else if (matches[3].rm_so != -1) {
                // We have an instruction: do() or don't()
                char instruction[10] = {0};
                int instr_len = matches[3].rm_eo - matches[3].rm_so;
                strncpy(instruction, p + matches[3].rm_so, instr_len);

                if (strcmp(instruction, "don't()") == 0) {
                    is_enabled = false;
                    printf("Disabled at position %ld\n", (long)(p - input + matches[0].rm_so));
                } else {
                    is_enabled = true;
                    printf("Enabled at position %ld\n", (long)(p - input + matches[0].rm_so));
                }
            }

            // Move past the entire matched substring
            p += matches[0].rm_eo;
        } else {
            // No match here, move forward one character
            p++;
        }
    }
}


int read_file(const char* filename, int* results, int version) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // read the entire file into a buffer
    char buffer[MAX_LINE_LENGTH];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);

    buffer[bytes_read] = '\0';

    fclose(file);
    if (version == 1) {
        return extract_valid_mul_instructions(buffer, results);
    } else if (version == 2) {
        return extract_valid_mul_instructions_with_do(buffer, results);
    } return 1;
}


int main() {
    int results[MAX_MATCHES] = {0};

    int count = read_file("input.txt", results, 1); // version 1

    printf("Part One: Valid mul() instructions found: %d\n", count);

    int total_sum = 0;
    for (int i = 0; i < count; i++) {
        printf("Result %d: %d\n", i+1, results[i]);
        total_sum += results[i];
    }

    printf("Total sum: %d\n", total_sum);

    int results2[MAX_MATCHES] = {0};
    int count2 = read_file("input.txt", results2, 2); // version 2

    printf("Part Two: Valid mul() instructions found: %d\n", count2);

    int total_sum2 = 0;
    for (int i = 0; i < count2; i++) {
        printf("Result %d: %d\n", i+1, results2[i]);
        total_sum2 += results2[i];
    }

    printf("Total Two: %d\n", total_sum2);

    return 0;
}
