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

unsigned int* GenerateCycleArrayDirectBypass(int number)
{
    auto array = new unsigned int [number];
    for (int i = 0; i < number - 1; ++i)
    {
        array[i] = i + 1;
    }
    return array;
}

unsigned int* GenerateCycleArrayReverseBypass(int number)
{
    auto array = new unsigned int [number];
    for (int i = 0; i < number; ++i)
    {
        array[i] = number - i - 1;
    }
    return array;
}

unsigned int* CheckAndUpdateRandomCycle(unsigned int* array, int size);

unsigned int* GenerateCycleArrayRandomBypass(int number)
{
    auto used = new bool [number];
    for (int i = 0; i < number; ++i)
    {
        used[i] = false;
    }
    std::random_device dev;
    auto array = new unsigned int [number];
    for (int i = 0; i < number; ++i)
    {
        std::mt19937 range(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distance(0, number - 1);
        unsigned int randomValue = distance(range);
        while (used[randomValue])
        {
            randomValue = distance(range);
        }
        array[i] = randomValue;
        used[randomValue] = true;
    }
    delete[] used;
//    array = CheckAndUpdateRandomCycle(array, number);
    return array;
}

//unsigned int* CheckAndUpdateRandomCycle(unsigned int* array, int size) // FIXME: fix this logic.
//{
//    int counter = 0;
//    auto used = new bool [size];
//    for (int i = 0; i < size; ++i)
//    {
//        used[i] = false;
//    }
//
//    unsigned int currentElem = 0;
//    for (int i = 0; i < size; ++i)
//    {
//        if (counter > 0 && currentElem == 0)
//        {
//            break;
//        }
//        used[currentElem] = true;
//        ++counter;
//        currentElem = array[currentElem];
//    }
//
//    auto newArray = new unsigned int [size];
//    if (counter < size)
//    {
//        delete[] used;
//        delete[] array;
//        newArray = GenerateCycleArrayRandomBypass(size);
//        newArray = CheckAndUpdateRandomCycle(newArray, size);
//    }
//
//    else
//    {
//
//        delete[] used;
//    }
//    return newArray;
//}

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
