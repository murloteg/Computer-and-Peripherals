#include "Matrix.h"

Matrix::Matrix(int matrixSize)
{
    matrixSize_ = matrixSize;
    firstNorm_ = infinityNorm_ = 0;
    matrix_ = new float [matrixSize_ * matrixSize_];
//    matrix_ = (float*) _mm_malloc(matrixSize_ * matrixSize_ * sizeof(float), 32); // for aligned memory allocation.
    setZeroMatrix();
}

Matrix::Matrix(const Matrix& origin)
{
    matrixSize_ = origin.matrixSize_;
    firstNorm_ = infinityNorm_ = 0;
    matrix_ = new float [matrixSize_ * matrixSize_];
//    matrix_ = (float*) _mm_malloc(matrixSize_ * matrixSize_ * sizeof(float), 32); // for aligned memory allocation.
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
    for (int i = 0; i < matrixSize_ * matrixSize_ / 8; ++i)
    {
        __m256 value = _mm256_setr_ps(static_cast<float>(number % 5), static_cast<float>((number + 1) % 5),
                                      static_cast<float>((number + 2) % 5), static_cast<float>((number + 3) % 5),
                                      static_cast<float>((number + 4) % 5), static_cast<float>((number + 5) % 5),
                                      static_cast<float>((number + 6) % 5), static_cast<float>((number + 7) % 5));
//        _mm256_store_ps(matrix_ + i * 8, value); // for aligned memory allocation.
        _mm256_storeu_ps(matrix_ + i * 8, value);
        number += 8;
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
            if (matrix_[i * matrixSize_ + j] >= 0)
            {
                printf(" ");
            }
            printf("%.4f ", matrix_[i * matrixSize_ + j]);
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
            if (matrix_[i * matrixSize_ + j] >= 0)
            {
                printf(" ");
            }
            printf("%.4f ", matrix_[i * matrixSize_ + j]);
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
//    _mm_free(matrix_); // for aligned memory allocation.
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
    for (int i = 0; i < matrixSize_ * matrixSize_ / 8; ++i)
    {
        /* for aligned memory allocation. */
//        __m256 firstValue = _mm256_load_ps(this->matrix_ + i * 8);
//        __m256 secondValue = _mm256_load_ps(other.matrix_ + i * 8);
//        __m256 resultValue = _mm256_sub_ps(firstValue, secondValue);
//        _mm256_store_ps(this->matrix_ + i * 8, resultValue);

        /* for unaligned memory allocation. */
        __m256 firstValue = _mm256_loadu_ps(this->matrix_ + i * 8);
        __m256 secondValue = _mm256_loadu_ps(other.matrix_ + i * 8);
        __m256 resultValue = _mm256_sub_ps(firstValue, secondValue);
        _mm256_storeu_ps(this->matrix_ + i * 8, resultValue);
    }

    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other)
{
    for (int i = 0; i < matrixSize_ * matrixSize_ / 8; ++i)
    {
        /* for aligned memory allocation. */
//        __m256 firstValue = _mm256_load_ps(this->matrix_ + i * 8);
//        __m256 secondValue = _mm256_load_ps(other.matrix_ + i * 8);
//        __m256 resultValue = _mm256_add_ps(firstValue, secondValue);
//        _mm256_store_ps(this->matrix_ + i * 8, resultValue);

        /* for unaligned memory allocation. */
        __m256 firstValue = _mm256_loadu_ps(this->matrix_ + i * 8);
        __m256 secondValue = _mm256_loadu_ps(other.matrix_ + i * 8);
        __m256 resultValue = _mm256_add_ps(firstValue, secondValue);
        _mm256_storeu_ps(this->matrix_ + i * 8, resultValue);
    }

    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other)
{
    Matrix result(this->matrixSize_);
    for (int i = 0; i < matrixSize_; ++i)
    {
        float* resultMatrix = result.matrix_ + i * matrixSize_;
        for (int j = 0; j < matrixSize_; j += 8)
        {
            /* for aligned memory allocation. */
//            _mm256_store_ps(resultMatrix + j + 0, _mm256_setzero_ps());

            /* for unaligned memory allocation. */
            _mm256_storeu_ps(resultMatrix + j + 0, _mm256_setzero_ps());
        }
        for (int k = 0; k < matrixSize_; ++k)
        {
            const float* secondMatrix = other.matrix_ + k * matrixSize_;
            __m256 firstMatrix = _mm256_set1_ps(matrix_[i * matrixSize_ + k]);
            for (int j = 0; j < matrixSize_; j += 16)
            {
                /* for aligned memory allocation. */
//                _mm256_store_ps(resultMatrix + j + 0, _mm256_fmadd_ps(firstMatrix, _mm256_load_ps(secondMatrix + j + 0), _mm256_load_ps(resultMatrix + j + 0)));
//                _mm256_store_ps(resultMatrix + j + 8, _mm256_fmadd_ps(firstMatrix, _mm256_load_ps(secondMatrix + j + 8), _mm256_load_ps(resultMatrix + j + 8)));

                /* for unaligned memory allocation. */
                _mm256_storeu_ps(resultMatrix + j + 0, _mm256_fmadd_ps(firstMatrix, _mm256_loadu_ps(secondMatrix + j + 0), _mm256_loadu_ps(resultMatrix + j + 0)));
                _mm256_storeu_ps(resultMatrix + j + 8, _mm256_fmadd_ps(firstMatrix, _mm256_loadu_ps(secondMatrix + j + 8), _mm256_loadu_ps(resultMatrix + j + 8)));
            }
        }
    }

    *this = result;
    return *this;
}

Matrix &Matrix::operator/=(float coefficient)
{
    for (int i = 0; i < matrixSize_ * matrixSize_ / 8; ++i)
    {
        /* for aligned memory allocation. */
//        __m256 firstValue = _mm256_load_ps(matrix_ + i * 8);
//        __m256 secondValue = _mm256_set1_ps(coefficient);
//        __m256 resultValue = _mm256_div_ps(firstValue, secondValue);
//        _mm256_store_ps(matrix_ + i * 8, resultValue);

        /* for unaligned memory allocation. */
        __m256 firstValue = _mm256_loadu_ps(matrix_ + i * 8);
        __m256 secondValue = _mm256_set1_ps(coefficient);
        __m256 resultValue = _mm256_div_ps(firstValue, secondValue);
        _mm256_storeu_ps(matrix_ + i * 8, resultValue);
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
