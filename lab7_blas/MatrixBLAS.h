#ifndef LAB7_MATRIXBLAS_H
#define LAB7_MATRIXBLAS_H
#include <iostream>
#include "cblas.h"

class Matrix {
private:
    int matrixSize_;
    float* matrix_;
    float firstNorm_;
    float infinityNorm_;
    template<typename Type>
    void swap(Type& first, Type& second);
public:
    explicit Matrix(int matrixSize);
    Matrix(const Matrix& origin);
    void setIdentityMatrix();
    void setZeroMatrix();
    void setMatrix();
    Matrix transposeMatrix();
    void findMatrixNorms();
    int getMatrixSize() const {return matrixSize_;}
    float getFirstNorm() const {return firstNorm_;}
    float getInfinityNorm() const {return infinityNorm_;}
    void printMatrix();
    void printMatrix() const;
    bool operator==(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator+=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);
    Matrix& operator/=(float coefficient);
    ~Matrix();
};

Matrix operator-(const Matrix& first, const Matrix& second);
Matrix operator+(const Matrix& first, const Matrix& second);
Matrix operator*(const Matrix& first, const Matrix& second);
Matrix FindTransformedMatrix(Matrix& generalMatrix);
Matrix FindMatrixOfSeries(Matrix& generalMatrix, Matrix& transformedMatrix);
Matrix FindInverseMatrixAlgorithm(Matrix& generalMatrix, int numberOfIterations);

#endif //LAB7_MATRIXBLAS_H
