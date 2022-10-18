#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define RADIAN 0.017453

double CalcSin(int number, double argument) {
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

    int number = (int) strtol(argv[1], NULL, 10);
    double argument = strtod(argv[2], NULL);
    struct timespec start_clock, end_clock;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_clock);
    printf("%lf\n", CalcSin(number, argument * RADIAN));
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_clock);
    printf("(clock_gettime) total time: %lf sec.\n", (double) (end_clock.tv_sec - start_clock.tv_sec) +
                    0.000000001 * (double) (end_clock.tv_nsec - start_clock.tv_nsec));

    return 0;
}
