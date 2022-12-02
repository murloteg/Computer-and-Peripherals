#include "MatrixDEFAULT.h"
#include <chrono>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "bad number of arguments" << std::endl;
        return 0;
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    int matrixSize = static_cast<int> (strtol(argv[1], nullptr, 10));
    int numberOfIterations = static_cast<int> (strtol(argv[2], nullptr, 10));
    Matrix generalMatrix(matrixSize);
    generalMatrix.setMatrix();
    Matrix result = FindInverseMatrixAlgorithm(generalMatrix, numberOfIterations);

    Matrix checkMatrix = result * generalMatrix;
//    checkMatrix.printMatrix();
    checkMatrix.findMatrixNorms();
    std::cout << "The first and the infinity NORMS: " << checkMatrix.getFirstNorm() << " " << checkMatrix.getInfinityNorm() << std::endl;
    std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = end - start;
    std::cout << "DEFAULT: total time: " << totalTime.count() <<  " sec." << std::endl;

    return 0;
}
