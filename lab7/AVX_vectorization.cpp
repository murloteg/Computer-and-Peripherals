#include <iostream>
#include <immintrin.h>

int main(int argc, char** argv)
{
    int size = 4;
    auto array = new double [size];
    for (int i = 0; i < size; ++i)
    {
        array[i] = i;
    }

    __m256 test = _mm256_load_pd(array);
    double* ppp;
    _mm256_store_pd(ppp, test);
    for (int i = 0; i < size; ++i)
    {
        std::cout << ppp[i] << std::endl;
    }

    __m256 second = _mm256_load_pd(array);
    __m256 res = _mm256_add_pd(test, second);
    double *result = nullptr;
    _mm256_store_pd(result, res);
    for (int i = 0; i < size; ++i)
    {
        std::cout << result[i] << std::endl;
    }

    return 0;
}
