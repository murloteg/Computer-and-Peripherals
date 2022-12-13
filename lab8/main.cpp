#include <iostream>
#include <random>
#define SIZE_COEFFICIENT 1.2

uint64_t rdtsc()
{
    unsigned int low, high;
    __asm__ __volatile__ ("rdtsc" : "=a" (low), "=d" (high));
    return ((uint64_t) high << 32) | low;
}

unsigned int* GenerateCycleArrayDirectBypass(int size)
{
    auto array = new unsigned int [size];
    for (int i = 0; i < size - 1; ++i)
    {
        array[i] = i + 1;
    }
    return array;
}

unsigned int* GenerateCycleArrayReverseBypass(int size)
{
    auto array = new unsigned int [size];
    for (int i = 0; i < size; ++i)
    {
        array[i] = size - i - 1;
    }
    return array;
}

void Swap(unsigned int* first, unsigned int* second)
{
    unsigned int temporary = *first;
    *first = *second;
    *second = temporary;
}

int GetNotVisitedIndex(const bool* used, int size)
{
    for (int i = 0; i < size; ++i)
    {
        if (!used[i])
        {
            return i;
        }
    }
    return 0;
}

void CheckAndRepairRandomCycle(unsigned int* array, int size)
{
    auto used = new bool [size];
    while (true)
    {
        for (int i = 0; i < size; ++i)
        {
            used[i] = false;
        }

        int counter = 0;
        unsigned int currentElem = 0;
        for (int i = 0; i < size; ++i)
        {
            if (used[currentElem] && counter > 0)
            {
                break;
            }
            used[currentElem] = true;
            ++counter;
            currentElem = array[currentElem];
        }

        if (counter < size)
        {
            int notVisitedIndex = GetNotVisitedIndex(used, size);
            Swap(&array[currentElem], &array[notVisitedIndex]);
        }

        else
        {
            delete[] used;
            return;
        }
    }
}

unsigned int* GenerateCycleArrayRandomBypass(int size)
{
    auto used = new bool [size];
    for (int i = 0; i < size; ++i)
    {
        used[i] = false;
    }
    std::random_device dev;
    auto array = new unsigned int [size];
    for (int i = 0; i < size; ++i)
    {
        std::mt19937 range(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distance(0, size - 1);
        unsigned int randomValue = distance(range);
        while (used[randomValue])
        {
            randomValue = distance(range);
        }
        array[i] = randomValue;
        used[randomValue] = true;
    }
    delete[] used;
    CheckAndRepairRandomCycle(array, size);
    return array;
}

void SingleBypassLaunch(const unsigned int* array, int size)
{
    unsigned int currentElem = 0;
    for (unsigned int i = 0; i < size; ++i)
    {
        currentElem = array[currentElem];
    }
}

void DirectBypass(int numberOfBypasses, int size)
{
    const auto array = GenerateCycleArrayDirectBypass(size);
    SingleBypassLaunch(array, size);
    uint64_t start = rdtsc();
    unsigned int currentElem = 0;
    for (unsigned long long int i = 0; i < numberOfBypasses * size; ++i)
    {
        currentElem = array[currentElem];
    }
    uint64_t end = rdtsc();
    unsigned long int totalClocksDirect = (end - start) / (size * numberOfBypasses);
    std::cout << "DIRECT,  number: " << size * sizeof(unsigned int) << "; clocks: " << totalClocksDirect << std::endl;
    delete[] array;
}

void ReverseBypass(int numberOfBypasses, int size)
{
    const auto array = GenerateCycleArrayReverseBypass(size);
    SingleBypassLaunch(array, size);
    uint64_t start = rdtsc();
    unsigned int currentElem = 0;
    for (unsigned long long int i = 0; i < numberOfBypasses * size; ++i)
    {
        currentElem = array[currentElem];
    }
    uint64_t end = rdtsc();
    unsigned long int totalClocksReverse = (end - start) / (size * numberOfBypasses);
    std::cout << "REVERSE, number: " << size * sizeof(unsigned int) << "; clocks: " << totalClocksReverse << std::endl;
    delete[] array;
}

void RandomBypass(int numberOfBypasses, int size)
{
    auto array = GenerateCycleArrayRandomBypass(size);
    SingleBypassLaunch(array, size);
    uint64_t start = rdtsc();
    unsigned int currentElem = 0;
    for (unsigned long long int i = 0; i < numberOfBypasses * size; ++i)
    {
        currentElem = array[currentElem];
    }
    uint64_t end = rdtsc();
    unsigned long int totalClocksRandom = (end - start) / (size * numberOfBypasses);
    std::cout << "RANDOM,  number: " << size * sizeof(unsigned int) << "; clocks: " << totalClocksRandom << std::endl;
    delete[] array;
}

void PrintSeparator()
{
    std::cout << "==================================" << std::endl;
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cout << "bad number of arguments!" << std::endl;
        return 0;
    }

    int minNumber = static_cast<int>(strtol(argv[1], nullptr, 10));
    int maxNumber = static_cast<int>(strtol(argv[2], nullptr, 10));
    int numberOfBypasses = static_cast<int>(strtol(argv[3], nullptr, 10));
    double currentNumber = minNumber;
    while (currentNumber <= maxNumber)
    {
        DirectBypass(numberOfBypasses, static_cast<int>(currentNumber));
        ReverseBypass(numberOfBypasses, static_cast<int>(currentNumber));
        RandomBypass(numberOfBypasses, static_cast<int>(currentNumber));
        PrintSeparator();
        currentNumber *= SIZE_COEFFICIENT;
    }

    return 0;
}
