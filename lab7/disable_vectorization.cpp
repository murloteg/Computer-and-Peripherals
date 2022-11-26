#include "Matrix.h"

int main(int argc, char** argv)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    if (argc != 3)
    {
        printf("bad number of arguments");
        return 0;
    }

    int matrixSize = static_cast<int> (strtol(argv[1], nullptr, 10));
    int numberOfIterations = static_cast<int> (strtol(argv[2], nullptr, 10));
    Matrix generalMatrix = Matrix(matrixSize);
    generalMatrix.setMatrix();

    Matrix result = FindInverseMatrixAlgorithm(generalMatrix, numberOfIterations);
    Matrix checkMatrix = result * generalMatrix;
    checkMatrix.findMatrixNorms();
    std::cout << "FIRST && SECOND NORMS: " << checkMatrix.getFirstNorm() << " " << checkMatrix.getInfinityNorm() << std::endl;
    std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = end - start;
    std::cout << "total time: " << totalTime.count() <<  " sec." << std::endl;

    return 0;
}
