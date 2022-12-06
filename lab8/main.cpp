#include <iostream>
#include <random>

union ticks
{
    unsigned long long t64;
    struct s32
    {
        long th, tl;
    } t32;
};

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

void Swap(unsigned int& first, unsigned int& second)
{
    unsigned int temporary = first;
    first = second;
    second = temporary;
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

void CheckAndRepairRandomCycle(unsigned int* array, int size) // FIXME
{
    auto used = new bool [size];
    for (int i = 0; i < size; ++i)
    {
        used[i] = false;
    }

    while (true)
    {
        int counter = 0;
        unsigned int currentElem = 0;
        int lastIndexInCycle = 0;
        for (int i = 0; i < size; ++i)
        {
            if (used[currentElem])
            {
                break;
            }
            used[currentElem] = true;
            ++counter;
            lastIndexInCycle = i;
            currentElem = array[currentElem];
        }

        if (counter < size)
        {
            int notVisitedIndex = GetNotVisitedIndex(used, size);
            Swap(array[lastIndexInCycle], array[notVisitedIndex]);
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
    ticks start{}, end{};
    asm volatile("rdtsc\n":"=a"(start.t32.th),"=d"(start.t32.tl)::"memory");
    unsigned int currentElem = 0;
    for (unsigned long long int i = 0; i < numberOfBypasses * size; ++i)
    {
        currentElem = array[currentElem];
    }
    asm volatile("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl)::"memory");
    unsigned int totalClocksDirect = static_cast<unsigned int>(end.t64-start.t64) / (size * numberOfBypasses);
    std::cout << "DIRECT,  number: " << size << "; clocks: " << totalClocksDirect << std::endl;
    delete[] array;
}

void ReverseBypass(int numberOfBypasses, int size)
{
    const auto array = GenerateCycleArrayReverseBypass(size);
    SingleBypassLaunch(array, size);
    ticks start{}, end{};
    asm volatile("rdtsc\n":"=a"(start.t32.th),"=d"(start.t32.tl)::"memory");
    unsigned int currentElem = 0;
    for (unsigned long long int i = 0; i < numberOfBypasses * size; ++i)
    {
        currentElem = array[currentElem];
    }
    asm volatile("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl)::"memory");
    unsigned int totalClocksReverse = static_cast<unsigned int>(end.t64-start.t64) / (size * numberOfBypasses);
    std::cout << "REVERSE, number: " << size << "; clocks: " << totalClocksReverse << std::endl;
    delete[] array;
}

void Print(unsigned int* arr, int size)
{
    for (int i = 0; i < size; ++i)
    {
        std::cout << i << " " << arr[i] << std::endl;
    }
}

void RandomBypass(int numberOfBypasses, int size)
{
    auto array = GenerateCycleArrayRandomBypass(size);
    SingleBypassLaunch(array, size);
    ticks start{}, end{};
    asm volatile("rdtsc\n":"=a"(start.t32.th),"=d"(start.t32.tl)::"memory");
    unsigned int currentElem = 0;
    for (unsigned long long int i = 0; i < numberOfBypasses * size; ++i)
    {
        currentElem = array[currentElem];
    }
    asm volatile("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl)::"memory");
    unsigned int totalClocksRandom = static_cast<unsigned int>(end.t64-start.t64) / (size * numberOfBypasses);
    std::cout << "RANDOM,  number: " << size << "; clocks: " << totalClocksRandom << std::endl;
    delete[] array;
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
    int currentNumber = minNumber;
    while (currentNumber <= maxNumber)
    {
        DirectBypass(numberOfBypasses, currentNumber);
        ReverseBypass(numberOfBypasses, currentNumber);
        RandomBypass(numberOfBypasses, currentNumber);
        std::cout << std::endl;
        currentNumber *= 2;
    }

    return 0;
}
