#include "Utility.h"
#include "Vector4.h"
using namespace SlavMath;

Vector4 Vector4::operator - () 
{ 
	return Vector4(-x, -y, -z, -w); 
}
Vector4 Vector4::operator - (const Vector4& other) const 
{ 
	return Vector4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w); 
}
Vector4 Vector4::operator + (const Vector4& other) const 
{ 
	return Vector4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w); 
}
Vector4 Vector4::operator * (const Vector4& other) const 
{ 
	return Vector4(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w); 
}
Vector4 Vector4::operator * (const float& scalar) const 
{ 
	return Vector4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar); 
}
Vector4 Vector4::operator * (float&& scalar) const 
{ 
	return Vector4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar); 
}
void Vector4::operator -= (const Vector4& other) 
{ 
	this->x -= other.x; this->y -= other.y; this->z -= other.z; this->w -= other.w; 
}
void Vector4::operator += (const Vector4& other) 
{ 
	this->x += other.x; this->y += other.y; this->z += other.z; this->w += other.w; 
}
void Vector4::operator *= (const Vector4& other) 
{ 
	this->x *= other.x; this->y *= other.y; this->z *= other.z; this->w *= other.w; 
}
void Vector4::operator *= (float&& scalar) 
{ 
	this->x *= scalar; this->y *= scalar; this->z *= scalar; this->w *= scalar; 
}
void Vector4::operator *= (const float& scalar) 
{ 
	this->x *= scalar; this->y *= scalar; this->z *= scalar; this->w *= scalar; 
}
//No bounds check for index
float& Vector4::operator [] (int&& index) { return cell[index]; }

float Vector4::magnitude() const 
{ 
	return sqrtf(x * x + y * y + z * z + w * w); 
}
float Vector4::magnitudeRoot() const
{ 
	return x * x + y * y + z * z + w * w; 
}
void Vector4::normalize() 
{ 
	float length = 1 / magnitude(); 
	x *= length; y *= length; z *= length; w *= length; 
}

SlavMath::Vector4 crossProduct(const SlavMath::Vector4& a, const SlavMath::Vector4& b)
{
	return Vector4(
		a.w * b.x - a.x * b.w,
		a.z * b.w - a.w * b.z,
		a.y * b.z - a.z * b.y,
		a.x * b.y - a.y * b.x);
}

float dot(const SlavMath::Vector4& a, const SlavMath::Vector4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
