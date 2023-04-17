#include "Utility.h"
#include "Vector3.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
using namespace SlavMath;
Vector3 operator*(float val, const Vector3& rhs)
{
    return Vector3(rhs.x * val, rhs.y * val, rhs.z * val);
}

bool operator==(const Vector3& a, const Vector3& b)
{
    return (a.x == b.x && a.y == b.y && a.z == b.z);
}

bool operator!=(const Vector3& a, const Vector3& b)
{
    return !(a.x == b.x && a.y == b.y && a.z == b.z);
}

float dot(const Vector3& a, const Vector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 crossProduct(const Vector3& a, const Vector3& b)
{
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Pixel getVectorColor(const SlavMath::Vector3& vector)
{
    int R, G, B;
    R = (int)(vector.x * 255.0f); R = R > 255 ? 255 : R < 0 ? 0 : R;
    G = (int)(vector.y * 255.0f); G = G > 255 ? 255 : G < 0 ? 0 : G;
    B = (int)(vector.z * 255.0f); B = B > 255 ? 255 : B < 0 ? 0 : B;
    return R << 16 | G << 8 | B;
}

Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vector3 Vector3::operator*(const Vector3& other) const
{
    return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
}

Vector3 Vector3::operator*(const float& scalar) const
{
    return Vector3(this->x * scalar, this->y * scalar, this->z * scalar);
}

Vector3 Vector3::operator*(float&& scalar) const
{
    return Vector3(this->x * scalar, this->y * scalar, this->z * scalar);
}

Vector3 Vector3::operator/(float&& scalar) const
{
    return Vector3(this->x / scalar, this->y / scalar, this->z / scalar);
}

Vector3 SlavMath::Vector3::operator/(const Vector3& other) const
{
    return Vector3(this->x / other.x, this->y / other.y, this->z / other.z);
}

void Vector3::operator-=(const Vector3& other)
{
    this->x -= other.x; this->y -= other.y; this->z -= other.z;
}

void Vector3::operator+=(const Vector3& other)
{
    this->x += other.x; this->y += other.y; this->z += other.z;
}

void Vector3::operator*=(const Vector3& other)
{
    this->x *= other.x; this->y *= other.y; this->z *= other.z;
}

void Vector3::operator*=(const float& scalar)
{
    this->x *= scalar; this->y *= scalar; this->z *= scalar;
}

void Vector3::operator*=(float&& scalar)
{
    this->x *= scalar; this->y *= scalar; this->z *= scalar;
}
void Vector3::operator/=(float&& scalar) 
{
    this->x /= scalar; this->y /= scalar; this->z /= scalar;
}

float& Vector3::operator[](const int& index)
{
    return cell[index];
}

float Vector3::magnitude() const
{
    float root = x * x + y * y + z * z;
    assert(root > 0);
    return sqrtf(root);
}

float Vector3::magnitudeRoot() const
{
    return x * x + y * y + z * z;
}

float SlavMath::Vector3::getMaxComponent() const
{
    float max{ x };
    if (max < y) max = y;
    if (max < z) max = z;
    return max;
}

float SlavMath::Vector3::getMinComponent() const
{
    float min{ x };
    if (min > y) min = y;
    if (min > z) min = z;
    return min;
}

void SlavMath::Vector3::cleanNanInf()
{
    if (isinf(x)) x = 0xFFFFFF;
    else if (isnan(x)) x = 0xFFFFFF;
    if (isinf(y)) y = 0xFFFFFF;
    else if (isnan(y)) y = 0xFFFFFF;
    if (isinf(z)) z = 0xFFFFFF;
    else if (isnan(z)) z = 0xFFFFFF;
}

void Vector3::normalize()
{
    float length = 1 / magnitude();
    x *= length; y *= length; z *= length;
}

Vector3 SlavMath::Vector3::normalized() const
{
    float length = 1 / magnitude();
    return Vector3{ x * length, y * length, z * length };
}

Vector3 SlavMath::Vector3::perpendicular() const
{
    Vector3 result;
    if (fabsf(this->y) == 1.0f)
        result = { this->y, 0, 0 };
    else {
        result = { this->z, 0, -this->x };
        result.normalize();
    }
    return result;
}

Vector3 SlavMath::Vector3::inversed() const
{
   assert(x != 0); assert(y != 0); assert(z != 0);
   return Vector3( 1.0f / x, 1.0f / y, 1.0f / z);
}

void Vector3::clear()
{
    x = 0; y = 0; z = 0;
}

Vector3 Vector3::Transformed(const Matrix3& matrix) const
{
    Vector3 vec;
    for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
    {
        vec[i] = matrix.cell[i * 3] * cell[0] +
            matrix.cell[i * 3 + 1] * cell[1] +
            matrix.cell[i * 3 + 2] * cell[2];
    }
    return vec;
}

void Vector3::Transform(const Matrix3& matrix)
{
    Vector3 vec;
    for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
    {
        vec[i] = matrix.cell[i * 3] * cell[0] +
            matrix.cell[i * 3 + 1] * cell[1] +
            matrix.cell[i * 3 + 2] * cell[2];
    }
    memcpy(this, &vec, sizeof(Vector3));
}

void Vector3::Transform(const Matrix4& matrix)
{
    Vector3 vec;
    for (int i = 0; i < sizeof(cell) / sizeof(float); i++)
    {
        vec[i] = matrix.cell[i * 4] * cell[0] +
            matrix.cell[i * 4 + 1] * cell[1] +
            matrix.cell[i * 4 + 2] * cell[2] +
            matrix.cell[i * 4 + 3] * 1.0f;
    }
    memcpy(this, &vec, sizeof(Vector3));
}

Vector3 Vector3::unit() const
{
    Vector3 result = *this; result.normalize(); return result;
}
