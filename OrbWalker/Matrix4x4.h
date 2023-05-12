#pragma once

#include <algorithm>

struct Matrix4x4
{
	static const size_t size = 16;
	float matrix[size];

	float operator [] (size_t i)
	{
		return this->matrix[i];
	}

	void Transpose()
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < i; j++) {
				std::swap(matrix[i * 4 + j], matrix[j * 4 + i]);
			}
		}
	}

	static Matrix4x4 Multiply(Matrix4x4 a, Matrix4x4 b)
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float sum = 0.f;
				for (int k = 0; k < 4; k++)
					sum = sum + a.matrix[i * 4 + k] * b.matrix[k * 4 + j];
				result.matrix[i * 4 + j] = sum;
			}
		}
		return result;
	}
};