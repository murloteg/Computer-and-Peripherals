#include <iostream>
#include <chrono>
#include <xmmintrin.h>

void SetMatrix(float* matrix, int matrixSize)
{
	int number = 1;
	for (int i = 0; i < matrixSize * matrixSize / 4; ++i)
	{
		__m128 value = _mm_setr_ps(static_cast<float>(number % 5), static_cast<float>((number + 1) % 5), static_cast<float>((number + 2) % 5), static_cast<float>((number + 3) % 5));
		_mm_store_ps(matrix + i * 4, value);
		number += 4;
	}
}

void SetIdentityMatrix(float* matrix, int matrixSize)
{
	for (int i = 0; i < matrixSize; ++i)
	{
		for (int j = 0; j < matrixSize; ++j)
		{
			matrix[i * matrixSize + j] = 0;
			if (i == j)
			{
				matrix[i * matrixSize + j] = 1;
			}
		}
	}
}

void SetZeroMatrix(float* matrix, int matrixSize)
{
	for (int i = 0; i < matrixSize; ++i)
	{
		for (int j = 0; j < matrixSize; ++j)
		{
			matrix[i * matrixSize + j] = 0;
		}
	}
}

void PrintMatrix(const float* matrix, int matrixSize)
{
	for (int i = 0; i < matrixSize; ++i)
	{
		for (int j = 0; j < matrixSize; ++j)
		{
			printf("%.3f ", matrix[i * matrixSize + j]);
		}
		printf("\n");
	}
	printf("\n");
}

void Swap(float& first, float& second)
{
	float temporary = first;
	first = second;
	second = temporary;
}

float* TransposeMatrix(const float* matrix, int matrixSize)
{
	auto transposedMatrix = (float*) _mm_malloc(matrixSize  * matrixSize * sizeof(float), 16);
	for (int i = 0; i < matrixSize * matrixSize / 4; ++i)
	{
		__m128 value = _mm_load_ps(matrix + i * 4);
		_mm_store_ps(transposedMatrix + i * 4, value);
	}

	for (int i = 0; i < matrixSize; ++i)
	{
		for (int j = i; j < matrixSize; ++j)
		{
			if (i == j)
			{
				continue;
			}
			Swap(transposedMatrix[i * matrixSize + j], transposedMatrix[j * matrixSize + i]);
		}
	}

	return transposedMatrix;
}

void FindMatrixNorms(float& firstNorm, float& infinityNorm, const float* matrix, int matrixSize)
{
	float maxSumOfColumn = 0;
	float maxSumOfLine = 0;
	for (int j = 0; j < matrixSize; ++j)
	{
		float currentSumOfColumn = 0;
		float currentSumOfLine = 0;
		for (int i = 0; i < matrixSize; ++i)
		{
			currentSumOfColumn += std::abs(matrix[i * matrixSize + j]);
			currentSumOfLine += std::abs(matrix[j * matrixSize + i]);
		}
		maxSumOfColumn = std::max(maxSumOfColumn, currentSumOfColumn);
		maxSumOfLine = std::max(maxSumOfLine, currentSumOfLine);
	}
	firstNorm = maxSumOfColumn;
	infinityNorm = maxSumOfLine;
}

void DivideByCoefficient(float* matrix, int matrixSize, float coefficient)
{
	for (int i = 0; i < matrixSize * matrixSize / 4; ++i)
	{
		__m128 firstValue = _mm_load_ps(matrix + i * 4);
		__m128 secondValue = _mm_set1_ps(coefficient);
		__m128 resultValue = _mm_div_ps(firstValue, secondValue);
		_mm_store_ps(matrix + i * 4, resultValue);
	}
}

float* FindTransformedMatrix(float* generalMatrix, int matrixSize)
{
	float* transformedMatrix = TransposeMatrix(generalMatrix, matrixSize);
	float firstNorm = 0, infinityNorm = 0;
	FindMatrixNorms(firstNorm, infinityNorm, generalMatrix, matrixSize);
	DivideByCoefficient(transformedMatrix, matrixSize, firstNorm * infinityNorm);
	return transformedMatrix;
}

float* MatrixMultiply(const float* first, const float* second, int matrixSize)
{
	auto result = (float*) _mm_malloc(matrixSize * matrixSize * sizeof(float), 16);
	for (int k = 0; k < matrixSize; ++k)
	{
		for (int i = 0; i < matrixSize; ++i)
		{
			float currentSum = 0;
			for (int j = 0; j < matrixSize; ++j)
			{
				currentSum += first[i * matrixSize + k] * second[k * matrixSize + j];
			}
			result[i * matrixSize + k] = currentSum;
		}
	}

	return result;
}

float* MatrixSubtract(const float* first, const float* second, int matrixSize)
{
	auto result = (float*) _mm_malloc(matrixSize * matrixSize * sizeof(float), 16);
	for (int i = 0; i < matrixSize * matrixSize / 4; ++i)
	{
		__m128 firstValue = _mm_load_ps(first + i * 4);
		__m128 secondValue = _mm_load_ps(second + i * 4);
		__m128 resultValue = _mm_sub_ps(firstValue, secondValue);
		_mm_store_ps(result + i * 4, resultValue);
	}

	return result;
}

void MatrixAddition(float* first, const float* second, int matrixSize)
{
	for (int i = 0; i < matrixSize * matrixSize / 4; ++i)
	{
		__m128 firstValue = _mm_load_ps(first + i * 4);
		__m128 secondValue = _mm_load_ps(second + i * 4);
		__m128 resultValue = _mm_add_ps(firstValue, secondValue);
		_mm_store_ps(first + i * 4, resultValue);
	}
}

float* FindMatrixOfSeries(float* generalMatrix, float* transformedMatrix, int matrixSize)
{
	auto identityMatrix = (float*) _mm_malloc(matrixSize * matrixSize * sizeof(float), 16);
	SetIdentityMatrix(identityMatrix, matrixSize);
	auto tempMatrix = MatrixMultiply(transformedMatrix, generalMatrix, matrixSize);
	auto matrixOfSeries = MatrixSubtract(identityMatrix, tempMatrix, matrixSize);
	_mm_free(identityMatrix);
	_mm_free(tempMatrix);
	return matrixOfSeries;
}

float* FindInverseMatrixAlgorithm(float* generalMatrix, int numberOfIterations, int matrixSize)
{
	float* transformedMatrix = FindTransformedMatrix(generalMatrix, matrixSize);
	float* matrixOfSeries = FindMatrixOfSeries(generalMatrix, transformedMatrix, matrixSize);
	auto result = (float*) _mm_malloc(matrixSize * matrixSize * sizeof(float), 16);
	SetZeroMatrix(result, matrixSize);
	auto currentMultiplier = (float*) _mm_malloc(matrixSize * matrixSize * sizeof(float), 16);
	SetIdentityMatrix(currentMultiplier, matrixSize);
	for (int i = 0; i < numberOfIterations; ++i)
	{
		MatrixAddition(result, currentMultiplier, matrixSize);
		currentMultiplier = MatrixMultiply(currentMultiplier, matrixOfSeries, matrixSize);
	}

	result = MatrixMultiply(result, transformedMatrix, matrixSize);
	_mm_free(transformedMatrix);
	return result;
}

using namespace std;
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
	auto* generalMatrix = (float*) _mm_malloc(matrixSize * matrixSize * sizeof(float), 16);
	SetMatrix(generalMatrix, matrixSize);

	auto result = FindInverseMatrixAlgorithm(generalMatrix, numberOfIterations, matrixSize);
	auto checkMatrix = MatrixMultiply(result, generalMatrix, matrixSize);
	float firstNorm = 0, infinityNorm = 0;
	FindMatrixNorms(firstNorm, infinityNorm, checkMatrix, matrixSize);
//	PrintMatrix(checkMatrix, matrixSize);

	std::cout << "The first and the infinity NORMS: " << firstNorm << " " << infinityNorm << std::endl;
	std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> totalTime = end - start;
	std::cout << "SSE2: total time: " << totalTime.count() <<  " sec." << std::endl;

	_mm_free(generalMatrix);
	_mm_free(result);
	_mm_free(checkMatrix);

/*	1.290 0.000 0.000 0.000
	0.000 0.724 0.000 0.000
	0.000 0.000 0.856 0.000
	0.000 0.000 0.000 1.000

	1.29024 1.29024
    SSE2: total time: 4.80716 sec.

    BIG:
    The first and the infinity NORMS: 1.00059 1.00059
	SSE2: total time: 28.4058 sec.
*/

    return 0;
}
