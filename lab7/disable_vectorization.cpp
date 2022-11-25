#include "Matrix.h"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("bad number of arguments");
        return 0;
    }

    int matrixSize = static_cast<int> (strtol(argv[1], nullptr, 10));
    int numberOfIterations = static_cast<int> (strtol(argv[2], nullptr, 10));
    Matrix generalMatrix = Matrix(matrixSize);
    generalMatrix.setMatrix();
    generalMatrix.printMatrix();

    Matrix result = FindInverseMatrixAlgorithm(generalMatrix, numberOfIterations);
    result.printMatrix();
    Matrix checkMatrix = result * generalMatrix;
    checkMatrix.printMatrix();
    checkMatrix.findMatrixNorms();
    std::cout << "FIRST && SECOND NORMS: " << checkMatrix.getFirstNorm() << " " << checkMatrix.getInfinityNorm() << std::endl;

    return 0;
}
