#include "Utility.h"
#include "Matrix3x3.h"
using namespace SlavMath;
void Matrix3::setToIdentity()
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = 1.0f; cell[4] = 1.0f; cell[8] = 1.0f;
}

Matrix3 SlavMath::Matrix3::getIdentity()
{
	return Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);
}

void Matrix3::setToScale(const float& scalar)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = scalar; cell[4] = scalar; cell[8] = scalar;
}

void Matrix3::setToScale(const float& x, const float& y, const float& z)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = x; cell[4] = y; cell[8] = z;
}

void Matrix3::setToTransform(const float& x, const float& y)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = 1.0f; cell[4] = 1.0f; cell[8] = 1.0f;
	cell[2] = x; cell[5] = y;
}

void Matrix3::setToRotationX(const float& angle)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = 1.0f;
	cell[4] = cosf(ANGLE_TO_RADIAN(angle));
	cell[5] = -sinf(ANGLE_TO_RADIAN(angle));
	cell[7] = sinf(ANGLE_TO_RADIAN(angle));
	cell[8] = cosf(ANGLE_TO_RADIAN(angle));
}

void Matrix3::setToRotationY(const float& angle)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = cosf(ANGLE_TO_RADIAN(angle));
	cell[2] = sinf(ANGLE_TO_RADIAN(angle));
	cell[4] = 1.0f;
	cell[6] = -sinf(ANGLE_TO_RADIAN(angle));
	cell[8] = cosf(ANGLE_TO_RADIAN(angle));
}

void Matrix3::setToRotationZ(const float& angle)
{
	memset(&cell, 0, sizeof(cell));
	cell[0] = cosf(ANGLE_TO_RADIAN(angle));
	cell[1] = -sinf(ANGLE_TO_RADIAN(angle));
	cell[3] = sinf(ANGLE_TO_RADIAN(angle));
	cell[4] = cosf(ANGLE_TO_RADIAN(angle));
	cell[8] = 1.0f;
}

float& Matrix3::operator[](const size_t& index)
{
	return cell[index];
}

Matrix3 SlavMath::Matrix3::operator-() const
{
	Matrix3 mat;
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = -cell[i];
	}
	return mat;
}

Matrix3 Matrix3::operator*(const float& scalar)
{
	Matrix3 mat;
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] * scalar;
	}
	return mat;
}

Matrix3 Matrix3::operator+(const float& scalar)
{
	Matrix3 mat;
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] + scalar;
	}
	return mat;
}

Matrix3 Matrix3::operator-(const float& scalar)
{
	Matrix3 mat;
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] - scalar;
	}
	return mat;
}

Matrix3 Matrix3::operator*(const Matrix3& other)
{
	Matrix3 mat;

	size_t j = 0;
	for (size_t i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		j = (i / (size_t)3) * (size_t)3;
		mat.cell[i] =
			cell[j] * other.cell[i % (size_t)3] +
			cell[j + (size_t)1] * other.cell[(size_t)3 + i % (size_t)3] +
			cell[j + (size_t)2] * other.cell[(size_t)6 + i % (size_t)3];
	}
	return mat;
}

Matrix3 Matrix3::operator+(const Matrix3& other)
{
	Matrix3 mat;
	for (size_t i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] + other.cell[i];
	}
	return mat;
}

Matrix3 Matrix3::operator-(const Matrix3& other)
{
	Matrix3 mat;
	for (size_t i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] - other.cell[i];
	}
	return mat;
}

Matrix3 Matrix3::operator/(const float& scalar)
{
	Matrix3 mat;
	for (size_t i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		mat.cell[i] = cell[i] / scalar;
	}
	return mat;
}

void Matrix3::operator*=(const float& scalar)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] *= scalar;
	}
}

void Matrix3::operator+=(const float& scalar)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] += scalar;
	}
}

void Matrix3::operator-=(const float& scalar)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] -= scalar;
	}
}

void Matrix3::operator/=(const float& scalar)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] /= scalar;
	}
}

void Matrix3::operator*=(const Matrix3& other)
{
	float inv[9];
	memcpy(&inv, this, sizeof(inv));
	size_t j = 0;
	for (size_t i = 0; i < 9; i++)
	{
		j = (i / (size_t)3) * (size_t)3;
		cell[i] =
			inv[j] * other.cell[i % (size_t)3] +
			inv[j + (size_t)1] * other.cell[(size_t)3 + i % (size_t)3] +
			inv[j + (size_t)2] * other.cell[(size_t)6 + i % (size_t)3];
	}
}

void Matrix3::operator+=(const Matrix3& other)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] += other.cell[i];
	}
}

void Matrix3::operator-=(const Matrix3& other)
{
	for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
	{
		cell[i] -= other.cell[i];
	}
}

void Matrix3::Transpose()
{
	float temp = 0;
	FLOAT_SWAP(cell[1], cell[3], temp);
	FLOAT_SWAP(cell[2], cell[6], temp);
	FLOAT_SWAP(cell[5], cell[7], temp);
}

float Matrix3::Det() const
{
	return (cell[0] * cell[4] * cell[8] - cell[0] * cell[5] * cell[7]) +
		(cell[1] * cell[5] * cell[6] - cell[1] * cell[3] * cell[8]) +
		(cell[2] * cell[3] * cell[7] - cell[2] * cell[4] * cell[6]);
}

void Matrix3::Invert()
{
	float inv[9] = {
			   cell[4] * cell[8] - cell[5] * cell[7],
			   -(cell[5] * cell[6] - cell[3] * cell[8]),
			   cell[3] * cell[7] - cell[4] * cell[6],
			   -(cell[7] * cell[2] - cell[1] * cell[8]),
			   cell[0] * cell[8] - cell[2] * cell[6],
			   -(cell[0] * cell[7] - cell[1] * cell[6]),
			   cell[1] * cell[5] - cell[2] * cell[4],
			   -(cell[0] * cell[5] - cell[2] * cell[3]),
			   cell[0] * cell[4] - cell[1] * cell[3]
	};

	float det = cell[0] * inv[0] - cell[1] * inv[1] + cell[2] * inv[2];
	if (det != 0)
	{
		det = 1.0f / det;
		for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
		{
			cell[i] = inv[i] * det;
		}
	}
}

Matrix3 Matrix3::Inverse() const
{
	Matrix3 mat{
	cell[4] * cell[8] - cell[5] * cell[7],
	-(cell[5] * cell[6] - cell[3] * cell[8]),
	cell[3] * cell[7] - cell[4] * cell[6],
	-(cell[7] * cell[2] - cell[1] * cell[8]),
	cell[0] * cell[8] - cell[2] * cell[6],
	-(cell[0] * cell[7] - cell[1] * cell[6]),
	cell[1] * cell[5] - cell[2] * cell[4],
	-(cell[0] * cell[5] - cell[2] * cell[3]),
	cell[0] * cell[4] - cell[1] * cell[3]
	};
	float det = cell[0] * mat.cell[0] - cell[1] * mat.cell[1] + cell[2] * mat.cell[2];
	if (det != 0)
	{
		mat *= (1.0f / det);
		return mat;
	}
	mat *= 0;
	return mat;
}


float Matrix3::GetX() const
{
	return cell[0];
}

float Matrix3::GetY() const
{
	return cell[4];
}

float Matrix3::GetZ() const
{
	return cell[8];
}
