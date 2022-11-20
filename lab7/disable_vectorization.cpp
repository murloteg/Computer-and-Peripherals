#include <cstdio>
#include <cstdlib>

void SetIdentityMatrix(int* identityMatrix, long int matrixSize)
{
    for (int i = 0; i < matrixSize; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
            identityMatrix[i * matrixSize + j] = 0;
            if (i == j)
            {
                identityMatrix[i * matrixSize + j] = 1;
            }
        }
    }
}

void SetMatrix(int* matrix, long int matrixSize)
{
    int number = 1;
    for (int i = 0; i < matrixSize; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
            matrix[i * matrixSize + j] = number % 5;
            ++number;
        }
    }
}

void Swap(int& first, int& second)
{
    int temporary = first;
    first = second;
    second = temporary;
}

void TransposeMatrix(int* matrix, long int matrixSize)
{
    for (int i = 0; i < matrixSize; ++i)
    {
        for (int j = i; j < matrixSize; ++j)
        {
            if (i == j)
            {
                continue;
            }
            Swap(matrix[i * matrixSize + j], matrix[j * matrixSize + i]);
        }
    }
}

void PrintMatrix(const int* matrix, long int matrixSize)
{
    for (int i = 0; i < matrixSize; ++i)
    {
        for (int j = 0; j < matrixSize; ++j)
        {
            printf("%d ", matrix[i * matrixSize + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("bad number of arguments");
        return 0;
    }

    long int matrixSize = strtol(argv[1], nullptr, 10);
    long int numberOfIterations = strtol(argv[2], nullptr, 10);
    int* identityMatrix = new int [matrixSize * matrixSize];
    SetIdentityMatrix(identityMatrix, matrixSize);
    PrintMatrix(identityMatrix, matrixSize);
    int* generalMatrix = new int [matrixSize * matrixSize];
    SetMatrix(generalMatrix, matrixSize);
    PrintMatrix(generalMatrix, matrixSize);

    TransposeMatrix(identityMatrix, matrixSize);
    PrintMatrix(identityMatrix, matrixSize);

    delete[] identityMatrix;
    delete[] generalMatrix;
    return 0;
}
