#include "MatrixBLAS.h"

Matrix::Matrix(int matrixSize)
{
    matrixSize_ = matrixSize;
    firstNorm_ = infinityNorm_ = 0;
    matrix_ = new float [matrixSize_ * matrixSize_];
    setZeroMatrix();
}

Matrix::Matrix(const Matrix& origin)
{
    matrixSize_ = origin.matrixSize_;
    firstNorm_ = infinityNorm_ = 0;
    matrix_ = new float [matrixSize_ * matrixSize_];
    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            matrix_[i * matrixSize_ + j] = origin.matrix_[i * matrixSize_ + j];
        }
    }
}

void Matrix::setZeroMatrix() 
{
    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            matrix_[i * matrixSize_ + j] = 0;
        }
    }
}

void Matrix::setMatrix() 
{
    int number = 1;
    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            matrix_[i * matrixSize_ + j] = static_cast<float> (number % 5);
            ++number;
        }
    }
}

void Matrix::setIdentityMatrix()
{
    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            matrix_[i * matrixSize_ + j] = 0;
            if (i == j)
            {
                matrix_[i * matrixSize_ + j] = 1;
            }
        }
    }
}

Matrix Matrix::transposeMatrix()
{
    Matrix result = *this;
    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = i; j < matrixSize_; ++j)
        {
            if (i == j)
            {
                continue;
            }
            swap(result.matrix_[i * matrixSize_ + j], result.matrix_[j * matrixSize_ + i]);
        }
    }
    return result;
}

void Matrix::findMatrixNorms()
{
    float maxSumOfColumn = std::numeric_limits<float>::min();
    float maxSumOfLine = std::numeric_limits<float>::min();
    for (int j = 0; j < matrixSize_; ++j)
    {
        float currentSumOfColumn = 0;
        float currentSumOfLine = 0;
        for (int i = 0; i < matrixSize_; ++i)
        {
            currentSumOfColumn += std::abs(matrix_[i * matrixSize_ + j]);
            currentSumOfLine += std::abs(matrix_[j * matrixSize_ + i]);
        }
        maxSumOfColumn = std::max(maxSumOfColumn, currentSumOfColumn);
        maxSumOfLine = std::max(maxSumOfLine, currentSumOfLine);
    }
    firstNorm_ = maxSumOfColumn;
    infinityNorm_ = maxSumOfLine;
}

void Matrix::printMatrix()
{
    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            printf("%.3f ", matrix_[i * matrixSize_ + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void Matrix::printMatrix() const
{
    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            printf("%.3f ", matrix_[i * matrixSize_ + j]);
        }
        printf("\n");
    }
    printf("\n");
}

template<typename Type>
void Matrix::swap(Type &first, Type &second)
{
    Type temporary = first;
    first = second;
    second = temporary;
}

Matrix::~Matrix()
{
    delete[] matrix_;
    matrix_ = nullptr;
}

bool Matrix::operator==(const Matrix &other)
{
    if (matrixSize_ != other.matrixSize_)
    {
        return false;
    }

    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            if (matrix_[i * matrixSize_ + j] != other.matrix_[i * matrixSize_ + j])
            {
                return false;
            }
        }
    }
    return true;
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if (this == &other)
    {
        return *this;
    }

    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            matrix_[i * matrixSize_ + j] = other.matrix_[i * matrixSize_ + j];
        }
    }

    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other)
{
    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            matrix_[i * matrixSize_ + j] -= other.matrix_[i * matrixSize_ + j];
        }
    }

    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other)
{
    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            matrix_[i * matrixSize_ + j] += other.matrix_[i * matrixSize_ + j];
        }
    }

    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other)
{
    Matrix result(this->getMatrixSize());
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, matrixSize_, matrixSize_, matrixSize_, 1, matrix_, matrixSize_, other.matrix_, matrixSize_, 1, result.matrix_, matrixSize_);
    *this = result;
    return *this;
}

Matrix &Matrix::operator/=(float coefficient)
{
    for (int i = 0; i < matrixSize_; ++i)
    {
        for (int j = 0; j < matrixSize_; ++j)
        {
            matrix_[i * matrixSize_ + j] /= coefficient;
        }
    }
    return *this;
}

Matrix operator-(const Matrix& first, const Matrix& second)
{
    Matrix result(first);
    result -= second;
    return result;
}

Matrix operator+(const Matrix& first, const Matrix& second)
{
    Matrix result(first);
    result += second;
    return result;
}

Matrix operator*(const Matrix& first, const Matrix& second)
{
    Matrix result(first);
    result *= second;
    return result;
}

Matrix FindTransformedMatrix(Matrix& generalMatrix)
{
    Matrix transformedMatrix = generalMatrix.transposeMatrix();
    generalMatrix.findMatrixNorms();
    transformedMatrix /= (generalMatrix.getFirstNorm() * generalMatrix.getInfinityNorm());
    return transformedMatrix;
}

Matrix FindMatrixOfSeries(Matrix& generalMatrix, Matrix& transformedMatrix)
{
    Matrix identityMatrix(generalMatrix.getMatrixSize());
    identityMatrix.setIdentityMatrix();
    Matrix matrixOfSeries = identityMatrix - (transformedMatrix * generalMatrix);
    return matrixOfSeries;
}

Matrix FindInverseMatrixAlgorithm(Matrix& generalMatrix, int numberOfIterations)
{
    Matrix transformedMatrix = FindTransformedMatrix(generalMatrix);
    Matrix matrixOfSeries = FindMatrixOfSeries(generalMatrix, transformedMatrix);
    Matrix result(generalMatrix.getMatrixSize());
    Matrix currentMultiplier(generalMatrix.getMatrixSize());
    currentMultiplier.setIdentityMatrix();
    for (int i = 0; i < numberOfIterations; ++i)
    {
        result += currentMultiplier;
        currentMultiplier *= matrixOfSeries;
    }

    result *= transformedMatrix;
    return result;
}
