#include <iostream>

enum Consts
{
    L3_CACHE_SIZE = 24 * 1024 * 1024 / sizeof(int),
    OFFSET = 24 * 1024 * 1024 / sizeof(int),
    MAX_NUMBER_OF_FRAGMENTS = 32
};

uint64_t rdtsc()
{
    unsigned int low, high;
    __asm__ __volatile__ ("rdtsc" : "=a" (low), "=d" (high));
    return ((uint64_t) high << 32) | low;
}

void InitializeArray(int* array, int fragmentSize, int numberOfFragments)
{
    for(int i = 0; i < fragmentSize; ++i)
    {
        int currentIndex = i;
        for(size_t j = 1; j < numberOfFragments; ++j)
        {
            array[currentIndex] = currentIndex + OFFSET;
            currentIndex += OFFSET;
        }
        array[currentIndex] = (i + 1) % fragmentSize;
    }
}

void Bypass(const int* array, int numberOfFragments, int numberOfBypasses)
{
    uint64_t minTime = UINT64_MAX;
    for (int i = 0; i < numberOfBypasses; ++i)
    {
        uint64_t start = rdtsc();
        int element = 0;
        for (int j = 0; j < numberOfFragments * L3_CACHE_SIZE; ++j)
        {
            element = array[element];
        }
        uint64_t end = rdtsc();
        minTime = (end - start) < minTime ? (end - start) : minTime;
    }
    std::cout << "CLOCKS: " << minTime / (numberOfFragments * L3_CACHE_SIZE) << ", NUMBER OF FRAGMENTS: " << numberOfFragments << std::endl;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "bad number of arguments!" << std::endl;
        return -1;
    }

    int numberOfBypasses = static_cast<int>(strtol(argv[1], nullptr, 10));
    auto array = new int[L3_CACHE_SIZE * MAX_NUMBER_OF_FRAGMENTS];
    for (int numberOfFragments = 1; numberOfFragments <= MAX_NUMBER_OF_FRAGMENTS; ++numberOfFragments)
    {
        InitializeArray(array, L3_CACHE_SIZE / numberOfFragments, numberOfFragments);
        Bypass(array, numberOfFragments, numberOfBypasses);
    }
    delete[] array;

    return 0;
}
