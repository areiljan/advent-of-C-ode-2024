// import regex
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MATCHES 4
#define BUFFER_SIZE 20

// Function prototypes
// the standard way to use files in C is to use a file pointer
FILE *open_file(const char *filename);
// the regex_t type is specific to C for handling compiled regular expressions.
regex_t compile_regex(const char *pattern);
void process_line(const char *line, regex_t *regex, int *match_count, int *instruction_count, int *multiplication_sum, bool *state);
void extract_numbers(const char *input, regmatch_t *matches, int *num1, int *num2);
bool handle_instruction(const char *instruction);

int main() {
    const char *filename = "input.txt";
    const char *pattern = "mul\\(([0-9]+),([0-9]+)\\)|(do\\(\\)|don't\\(\\))";

    FILE *file = open_file(filename);
    regex_t regex = compile_regex(pattern);

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int total_matches = 0;
    int total_instructions = 0;
    int sum_of_multiplications = 0;
    bool enabled = true;

    while ((read = getline(&line, &len, file)) != -1) {
        process_line(line, &regex, &total_matches, &total_instructions, &sum_of_multiplications, &enabled);
    }

    free(line);
    fclose(file);
    regfree(&regex);

    printf("Total matches: %d\n", total_matches);
    printf("Total instructions: %d\n", total_instructions);
    printf("Sum of multiplications: %d\n", sum_of_multiplications);

    return 0;
}

FILE *open_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    return file;
}

// C needs to compile regex before usage
regex_t compile_regex(const char *pattern) {
    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        fprintf(stderr, "Could not compile regex\n");
        exit(EXIT_FAILURE);
    }
    return regex;
}

void process_line(const char *line, regex_t *regex, int *match_count, int *instruction_count, int *multiplication_sum, bool *state) {
    const char *ptr = line;
    regmatch_t matches[MAX_MATCHES];

    while (regexec(regex, ptr, MAX_MATCHES, matches, 0) == 0) {
        if (matches[2].rm_so != -1) {
            int num1 = 0, num2 = 0;
            extract_numbers(ptr, matches, &num1, &num2);

            if (*state) {
                (*match_count)++;
                *multiplication_sum += num1 * num2;
            }
        } else if (matches[3].rm_so != -1) {
            char instruction[BUFFER_SIZE] = {0};
            snprintf(instruction, matches[3].rm_eo - matches[3].rm_so + 1, "%s", ptr + matches[3].rm_so);

            (*instruction_count)++;
            *state = handle_instruction(instruction);
        }

        ptr += matches[0].rm_eo;
    }
}

void extract_numbers(const char *input, regmatch_t *matches, int *num1, int *num2) {
    char num1_str[BUFFER_SIZE] = {0};
    char num2_str[BUFFER_SIZE] = {0};

    // snprintf is a safer alternative to sprintf for formatting strings in C.
    snprintf(num1_str, matches[1].rm_eo - matches[1].rm_so + 1, "%s", input + matches[1].rm_so);
    snprintf(num2_str, matches[2].rm_eo - matches[2].rm_so + 1, "%s", input + matches[2].rm_so);

    // atoi converts strings of digits to integer
    *num1 = atoi(num1_str);
    *num2 = atoi(num2_str);
}

bool handle_instruction(const char *instruction) {
    // string compare
    if (strcmp(instruction, "don't()") == 0) {
        return false;
    } else if (strcmp(instruction, "do()") == 0) {
        return true;
    }
    return true;
}
