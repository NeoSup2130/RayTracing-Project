#include "Utility.h"
#include "Matrix4x4.h"
using namespace SlavMath;
Matrix4::Matrix4(bool is_Identity)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = (float)is_Identity; cell[5] = (float)is_Identity;
	cell[10] = (float)is_Identity; cell[15] = (float)is_Identity;
}

void Matrix4::setToIdentity()
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = 1.0f; cell[5] = 1.0f; cell[10] = 1.0f; cell[15] = 1.0f;
}

void Matrix4::setToScale(const float& scalar)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = scalar; cell[5] = scalar; cell[10] = scalar; cell[15] = 1.0f;
}

void Matrix4::setToScale(const float& x, const float& y, const float& z)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = x; cell[5] = y; cell[10] = z; cell[15] = 1.0f;
}

void Matrix4::setToTransform(const float& x, const float& y, const float& z)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = 1.0f; cell[5] = 1.0f; cell[10] = 1.0f; cell[15] = 1.0f;
	cell[3] = x; cell[7] = y; cell[11] = z;
}

void Matrix4::setToRotationX(const float& angle)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = 1.0f;
	cell[5] = cosf(ANGLE_TO_RADIAN(angle));
	cell[6] = -sinf(ANGLE_TO_RADIAN(angle));
	cell[9] = sinf(ANGLE_TO_RADIAN(angle));
	cell[10] = cosf(ANGLE_TO_RADIAN(angle));
	cell[15] = 1.0f;
}

void Matrix4::setToRotationY(const float& angle)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = cosf(ANGLE_TO_RADIAN(angle));
	cell[2] = sinf(ANGLE_TO_RADIAN(angle));
	cell[5] = 1.0f;
	cell[8] = -sinf(ANGLE_TO_RADIAN(angle));
	cell[10] = cosf(ANGLE_TO_RADIAN(angle));
	cell[15] = 1.0f;
}

void Matrix4::setToRotationZ(const float& angle)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = cosf(ANGLE_TO_RADIAN(angle));
	cell[1] = -sinf(ANGLE_TO_RADIAN(angle));
	cell[4] = sinf(ANGLE_TO_RADIAN(angle));
	cell[5] = cosf(ANGLE_TO_RADIAN(angle));
	cell[10] = 1.0f;
	cell[15] = 1.0f;
}

float& Matrix4::operator[](const size_t& index)
{
	return cell[index];
}

Matrix4 Matrix4::operator*(const float& scalar)
{
	Matrix4 mat;
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] * scalar;
	}
	return mat;
}

Matrix4 Matrix4::operator+(const float& scalar)
{
	Matrix4 mat;
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] + scalar;
	}
	return mat;
}

Matrix4 Matrix4::operator-(const float& scalar)
{
	Matrix4 mat;
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] - scalar;
	}
	return mat;
}

Matrix4 Matrix4::operator*(const Matrix4& other)
{
	Matrix4 mat;

	size_t j = 0;
	for (size_t i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		j = (i / (size_t)4) * (size_t)4;
		mat.cell[i] =
			cell[j] * other.cell[i % (size_t)4] +
			cell[j + (size_t)1] * other.cell[(size_t)4 + i % (size_t)4] +
			cell[j + (size_t)2] * other.cell[(size_t)8 + i % (size_t)4] +
			cell[j + (size_t)3] * other.cell[(size_t)12 + i % (size_t)4];
	}
	return mat;
}

Matrix4 Matrix4::operator+(const Matrix4& other)
{
	Matrix4 mat;
	for (size_t i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] + other.cell[i];
	}
	return mat;
}

Matrix4 Matrix4::operator-(const Matrix4& other)
{
	Matrix4 mat;
	for (size_t i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] - other.cell[i];
	}
	return mat;
}

Matrix4 Matrix4::operator/(const float& scalar)
{
	Matrix4 mat;
	for (size_t i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] / scalar;
	}
	return mat;
}

void Matrix4::operator*=(const float& scalar)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] *= scalar;
	}
}

void Matrix4::operator+=(const float& scalar)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] += scalar;
	}
}

void Matrix4::operator-=(const float& scalar)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] -= scalar;
	}
}

void Matrix4::operator/=(const float& scalar)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] /= scalar;
	}
}

void Matrix4::operator*=(const Matrix4& other)
{
	float inv[sizeof(Matrix4) / sizeof(float)];
	memcpy(&inv, this, sizeof(inv));
	size_t j = 0;
	for (size_t i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		j = (i / (size_t)4) * (size_t)4;
		cell[i] =
			inv[j] * other.cell[i % (size_t)4] +
			inv[j + (size_t)1] * other.cell[(size_t)4 + i % (size_t)4] +
			inv[j + (size_t)2] * other.cell[(size_t)8 + i % (size_t)4] +
			inv[j + (size_t)3] * other.cell[(size_t)12 + i % (size_t)4];
	}
}

void Matrix4::operator+=(const Matrix4& other)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] += other.cell[i];
	}
}

void Matrix4::operator-=(const Matrix4& other)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] -= other.cell[i];
	}
}

void Matrix4::Transpose()
{
	float temp = 0;
	FLOAT_SWAP(cell[1], cell[4], temp);
	FLOAT_SWAP(cell[2], cell[8], temp);
	FLOAT_SWAP(cell[3], cell[12], temp);
	FLOAT_SWAP(cell[6], cell[9], temp);
	FLOAT_SWAP(cell[7], cell[13], temp);
	FLOAT_SWAP(cell[11], cell[14], temp);
}

float Matrix4::Det() const
{
	float i{ cell[0] }, j{ cell[1] }, k{ cell[2] }, l{ cell[3] };

	/*
	5	6	7
	9	10	11
	13	14	15
	*/
	i *= (cell[5] * cell[10] * cell[15] - cell[5] * cell[11] * cell[14])
		- (cell[6] * cell[9] * cell[15] - cell[6] * cell[11] * cell[13])
		+ (cell[7] * cell[9] * cell[14] - cell[7] * cell[10] * cell[13]);

	/*
	4	6	7
	8	10	11
	12	14	15
	*/
	j *= (cell[4] * cell[10] * cell[15] - cell[4] * cell[11] * cell[14])
		- (cell[6] * cell[8] * cell[15] - cell[6] * cell[11] * cell[12])
		+ (cell[7] * cell[8] * cell[14] - cell[7] * cell[10] * cell[12]);
	/*
	4	5	7
	8	9	11
	12	13	15
	*/
	k *= (cell[4] * cell[9] * cell[15] - cell[4] * cell[11] * cell[13])
		- (cell[5] * cell[8] * cell[15] - cell[5] * cell[11] * cell[12])
		+ (cell[7] * cell[8] * cell[13] - cell[7] * cell[9] * cell[12]);

	/*
	4	5	6
	8	9	10
	12	13	14
	*/
	l *= (cell[4] * cell[9] * cell[14] - cell[4] * cell[10] * cell[13])
		- (cell[5] * cell[8] * cell[14] - cell[5] * cell[10] * cell[12])
		+ (cell[6] * cell[8] * cell[13] - cell[6] * cell[9] * cell[12]);

	return i - j + k - l;
}

Matrix4 Matrix4::Inverse() const
{
	Matrix4 inv;

	// I could not bother myself with writting down each 3 by 3 matrix determinant equation.
	// So I took the an already working calculation and copied it into here.
	// Source comes here https://stackoverflow.com/a/1148405
	// The provided answer works for both column major and row major matrices. 
	// This due to the fact that inverse of transposed A == transposed of inverse A.
	inv[0] = cell[5] * cell[10] * cell[15] -
		cell[5] * cell[11] * cell[14] -
		cell[9] * cell[6] * cell[15] +
		cell[9] * cell[7] * cell[14] +
		cell[13] * cell[6] * cell[11] -
		cell[13] * cell[7] * cell[10];

	inv[1] = -cell[1] * cell[10] * cell[15] +
		cell[1] * cell[11] * cell[14] +
		cell[9] * cell[2] * cell[15] -
		cell[9] * cell[3] * cell[14] -
		cell[13] * cell[2] * cell[11] +
		cell[13] * cell[3] * cell[10];

	inv[2] = cell[1] * cell[6] * cell[15] -
		cell[1] * cell[7] * cell[14] -
		cell[5] * cell[2] * cell[15] +
		cell[5] * cell[3] * cell[14] +
		cell[13] * cell[2] * cell[7] -
		cell[13] * cell[3] * cell[6];

	inv[3] = -cell[1] * cell[6] * cell[11] +
		cell[1] * cell[7] * cell[10] +
		cell[5] * cell[2] * cell[11] -
		cell[5] * cell[3] * cell[10] -
		cell[9] * cell[2] * cell[7] +
		cell[9] * cell[3] * cell[6];

	inv[4] = -cell[4] * cell[10] * cell[15] +
		cell[4] * cell[11] * cell[14] +
		cell[8] * cell[6] * cell[15] -
		cell[8] * cell[7] * cell[14] -
		cell[12] * cell[6] * cell[11] +
		cell[12] * cell[7] * cell[10];

	inv[5] = cell[0] * cell[10] * cell[15] -
		cell[0] * cell[11] * cell[14] -
		cell[8] * cell[2] * cell[15] +
		cell[8] * cell[3] * cell[14] +
		cell[12] * cell[2] * cell[11] -
		cell[12] * cell[3] * cell[10];

	inv[6] = -cell[0] * cell[6] * cell[15] +
		cell[0] * cell[7] * cell[14] +
		cell[4] * cell[2] * cell[15] -
		cell[4] * cell[3] * cell[14] -
		cell[12] * cell[2] * cell[7] +
		cell[12] * cell[3] * cell[6];

	inv[7] = cell[0] * cell[6] * cell[11] -
		cell[0] * cell[7] * cell[10] -
		cell[4] * cell[2] * cell[11] +
		cell[4] * cell[3] * cell[10] +
		cell[8] * cell[2] * cell[7] -
		cell[8] * cell[3] * cell[6];

	inv[8] = cell[4] * cell[9] * cell[15] -
		cell[4] * cell[11] * cell[13] -
		cell[8] * cell[5] * cell[15] +
		cell[8] * cell[7] * cell[13] +
		cell[12] * cell[5] * cell[11] -
		cell[12] * cell[7] * cell[9];

	inv[9] = -cell[0] * cell[9] * cell[15] +
		cell[0] * cell[11] * cell[13] +
		cell[8] * cell[1] * cell[15] -
		cell[8] * cell[3] * cell[13] -
		cell[12] * cell[1] * cell[11] +
		cell[12] * cell[3] * cell[9];

	inv[10] = cell[0] * cell[5] * cell[15] -
		cell[0] * cell[7] * cell[13] -
		cell[4] * cell[1] * cell[15] +
		cell[4] * cell[3] * cell[13] +
		cell[12] * cell[1] * cell[7] -
		cell[12] * cell[3] * cell[5];

	inv[11] = -cell[0] * cell[5] * cell[11] +
		cell[0] * cell[7] * cell[9] +
		cell[4] * cell[1] * cell[11] -
		cell[4] * cell[3] * cell[9] -
		cell[8] * cell[1] * cell[7] +
		cell[8] * cell[3] * cell[5];

	inv[12] = -cell[4] * cell[9] * cell[14] +
		cell[4] * cell[10] * cell[13] +
		cell[8] * cell[5] * cell[14] -
		cell[8] * cell[6] * cell[13] -
		cell[12] * cell[5] * cell[10] +
		cell[12] * cell[6] * cell[9];

	inv[13] = cell[0] * cell[9] * cell[14] -
		cell[0] * cell[10] * cell[13] -
		cell[8] * cell[1] * cell[14] +
		cell[8] * cell[2] * cell[13] +
		cell[12] * cell[1] * cell[10] -
		cell[12] * cell[2] * cell[9];

	inv[14] = -cell[0] * cell[5] * cell[14] +
		cell[0] * cell[6] * cell[13] +
		cell[4] * cell[1] * cell[14] -
		cell[4] * cell[2] * cell[13] -
		cell[12] * cell[1] * cell[6] +
		cell[12] * cell[2] * cell[5];

	inv[15] = cell[0] * cell[5] * cell[10] -
		cell[0] * cell[6] * cell[9] -
		cell[4] * cell[1] * cell[10] +
		cell[4] * cell[2] * cell[9] +
		cell[8] * cell[1] * cell[6] -
		cell[8] * cell[2] * cell[5];

	float det = cell[0] * inv[0] + cell[1] * inv[4] + cell[2] * inv[8] + cell[3] * inv[12];

	if (det != 0)
	{
		det = 1.0f / det;
		inv *= det;
	}
	else memset(&inv, 0, sizeof(inv));
	return inv;
}
