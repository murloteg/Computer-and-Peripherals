#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define RADIAN 0.017453

enum Statuses {
    OK = 0,
    BAD_INPUT = 1
};

void CheckInput(const char* firstParameter, const char* secondParameter, enum Statuses* status) {
    for (size_t i = 0; i < strlen(firstParameter); ++i) {
        if (firstParameter[i] < '0' || firstParameter[i] > '9') {
            *status = BAD_INPUT;
            return;
        }
    }

    for (size_t i = 0; i < strlen(secondParameter); ++i) {
        if ((secondParameter[i] < '0' || secondParameter[i] > '9') && secondParameter[i] != '.') {
            *status = BAD_INPUT;
            return;
        }
    }
}

double CalcSin(long long int number, double argument) {
    double result = 0;
    int signStatus = 1;
    double nextMultiplier = 1;
    int factorial = 1;
    for (int i = 0; i < number; ++i) {
        nextMultiplier *= (argument / factorial);
        ++factorial;
        result += signStatus * nextMultiplier;
        nextMultiplier *= (argument / factorial);
        ++factorial;
        signStatus *= -1;
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("too few arguments. try again\n");
        return 0;
    } else if (argc > 3) {
        printf("too many arguments. try again\n");
        return 0;
    }

    enum Statuses status = OK;
    CheckInput(argv[1], argv[2], &status);
    if (status != OK) {
        printf("bad input\n");
        return 0;
    }

    long long int number = strtoll(argv[1], NULL, 10);
    double argument = strtod(argv[2], NULL);

    /* clock_gettime */
    struct timespec start_clock, end_clock;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_clock);
    printf("%lf\n", CalcSin(number, argument * RADIAN));
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_clock);
    printf("(clock_gettime) total time: %lf sec.\n", (double) (end_clock.tv_sec - start_clock.tv_sec) +
                    0.000000001 * (double) (end_clock.tv_nsec - start_clock.tv_nsec));

    return 0;
}
