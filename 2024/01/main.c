#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/** The number of lines of input. */
#define NUMBER_OF_LINES (1000u)

/** The left list of numbers from the input text. */
static uint32_t Left_List[NUMBER_OF_LINES] = {0};

/** The left list of numbers from the input text. */
static uint32_t Right_List[NUMBER_OF_LINES] = {0};

/** Pair type that has a first and a second number,
    used to represent the split apart lines of input. */
typedef struct {
    uint32_t first;
    uint32_t second;
} Pair_T;

/**
   Spit the numbers apart in the given line.

   @param[in] line  The line to parse and split the numbers apart
   @return The pair of numbers that were separated
 */
static Pair_T SplitNumbers(char* line) {
    Pair_T pair = {0};
    char* p = line;

    /* Loop through the string until we hit a space. */
    while (*p != ' ') {
        /* If we hit the end of the line without finding a space, the input is malformed. */
        if (*p == '\0' || *p == '\n') {
            fprintf(stderr, "ERROR: Failed to find the first number in the given line: %s\n", line);
            exit(1);
        }
        /* Adjust the first number and add the next digit. */
        pair.first *= 10;
        pair.first += *p - '0';
        /* Increment the pointer. */
        p++;
    }

    /* The remainder of the string will just be the second number, so atoi can
       read the rest for us. */
    pair.second = (uint32_t)atoi(p);
    
    return pair;
}

/**
   Read and parse the input file.

   @note This handles splitting the two numbers apart in each line,
   and stores them in the static Left_List and Right_List variables.
 */
static void ReadInput(void) {
    char line[256] = {0};
    FILE *fp = fopen("input.txt", "r");
    uint32_t idx = 0u;
    if (NULL != fp) {
        /* Loop through the file line-by-line. */
        while (fgets(line, sizeof(line), fp)) {
            if (idx < NUMBER_OF_LINES) {
                /* Do something with the input here. */
                Pair_T pair = SplitNumbers(line);
                Left_List[idx] = pair.first;
                Right_List[idx] = pair.second;
                idx++;
            }
            else {
                fprintf(stderr, "ERROR: Reached the max number of lines!\n");
                exit(1);
            }
        }
    }
    else {
        fprintf(stderr, "ERROR: Could not open file input.txt!\n");
        exit(1);
    }
}

/**
   Callback for the `qsort` function.
*/
static int CompareCallback(const void* a, const void* b) {
    return (int)(*(uint32_t*)a - *(uint32_t*)b);
}

/**
   Sort the Left_List and Right_List static variables.
 */
static void SortLists(void) {
    /* Sort the left list. */
    qsort(Left_List, NUMBER_OF_LINES, sizeof(uint32_t), CompareCallback);
    
    /* Sort the right list. */
    qsort(Right_List, NUMBER_OF_LINES, sizeof(uint32_t), CompareCallback);
}

/**
   Calculate the difference between two given uint32_t numbers.

   @param[in] a  The first number
   @param[in] b  The second number
   @return The difference between the two numbers
 */
static uint32_t Diff(uint32_t a, uint32_t b) {
    uint32_t result = 0u;
    if (a > b) {
        result = a - b;
    }
    else {
        result = b - a;
    }
    return result;
}

/**
   Solve the first part of the puzzle.

   @note Find the difference between the lists and add them together,
   starting with the smallest in each list and progressing from there.
 */
static void SolveFirstPart(void) {
    uint32_t result = 0u;
    for (uint32_t i = 0u; i < NUMBER_OF_LINES; ++i) {
        /* As the lists are now sorted, we can just compare the same index for each list. */
        result += Diff(Left_List[i], Right_List[i]);
    }

    printf("Solution to first part: %u\n", result);
}

/**
   Find the number of occurances of the given number in the Right_List.

   @TODO This process could be made more efficient by keeping a hash table
   of the input number and the number of its occurances, so that once it's
   calculated, we can just look it up and not have to calculate it each time.

   @param number  The number to find the number of occurances for
   @return The number of occurances
 */
static uint32_t FindNumOccurances(uint32_t number) {
    uint32_t result = 0u;
    for (uint32_t i = 0u; i < NUMBER_OF_LINES; ++i) {
        if (Right_List[i] == number) {
            result++;
        }
        else if (Right_List[i] > number) {
            /* The list is already sorted, so if the entries start
               getting larger than the desired number, there are no more
               of that number possible, so just break and return. */
            break;
        }
    }

    return result;
}

/**
   Solve the second part of the puzzle.

   @note Find the "similarity score" of the lists by summing
   the result of taking each number in the Left_List and multiplying
   it by how many times it appears in the Right_List.
*/
static void SolveSecondPart(void) {
    uint32_t result = 0u;
    for (uint32_t i = 0u; i < NUMBER_OF_LINES; ++i) {
        result += Left_List[i] * FindNumOccurances(Left_List[i]);
    }

    printf("Solution to second part: %u\n", result);
}

int main(void) {
    ReadInput();
    SortLists();
    SolveFirstPart();
    SolveSecondPart();
    return 0;
}
