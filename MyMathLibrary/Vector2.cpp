#include "Utility.h"
#include "Vector2.h"
using namespace SlavMath;
float dot(const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

Vector2 operator*(float val, const Vector2& rhs)
{
	return Vector2(rhs.x * val, rhs.y * val);
}

bool operator==(const Vector2& a, const Vector2& b)
{
	return (a.x == b.x && a.y == b.y);
}

bool operator!=(const Vector2& a, const Vector2& b)
{
	return !(a.x == b.x && a.y == b.y);
}

Vector2 Vector2::operator-()
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(this->x - other.x, this->y - other.y);
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(this->x + other.x, this->y + other.y);
}

Vector2 Vector2::operator*(const Vector2& other) const
{
	return Vector2(this->x * other.x, this->y * other.y);
}

Vector2 Vector2::operator*(const float& scalar) const
{
	return Vector2(this->x * scalar, this->y * scalar);
}

Vector2 Vector2::operator*(float&& scalar) const
{
	return Vector2(this->x * scalar, this->y * scalar);
}

Vector2 Vector2::operator/(float&& scalar) const
{
	return Vector2(this->x / scalar, this->y / scalar);
}

Vector2 Vector2::operator/(const float& scalar) const
{
	return Vector2(this->x / scalar, this->y / scalar);
}

Vector2 Vector2::operator/(const Vector2& other) const
{
	return Vector2(this->x / other.x, this->y / other.y);
}

void Vector2::operator-=(const Vector2& other)
{
	this->x -= other.x;	this->y -= other.y;
}

void Vector2::operator+=(const Vector2& other)
{
	this->x += other.x; this->y += other.y;
}

void Vector2::operator*=(const Vector2& other)
{
	this->x *= other.x; this->y *= other.y;
}

void Vector2::operator*=(const float& scalar)
{
	this->x *= scalar; this->y *= scalar;
}

void Vector2::operator*=(float&& scalar)
{
	this->x *= scalar; this->y *= scalar;
}

float& Vector2::operator[](int&& index)
{
	return cell[index];
}

float Vector2::magnitude() const
{
	return sqrtf(x * x + y * y);
}

float Vector2::magnitudeRoot() const
{
	return x * x + y * y;
}

void Vector2::normalize()
{
	float length = 1 / magnitude(); x *= length; y *= length;
}

Vector2 Vector2::unit() const
{
	Vector2 result = *this; result.normalize(); return result;
}

Vector2 Vector2::perpendicular() const
{
	Vector2 result(y, -x); return result;
}

void Vector2::clear()
{
	x = 0; y = 0;
}

float Vector2::sqaureDistance(const Vector2& other) const
{
	return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
}

float Vector2::dotProduct(const Vector2& other) const
{
	return x * other.x + y * other.y;
}

void Vector2::trim(float&& scalar)
{
	if (this->magnitude() != scalar)
	{
		this->normalize();
		(*this) *= scalar;
	}
}
