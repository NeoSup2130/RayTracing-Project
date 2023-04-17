#include "precomp.h"
#include "Ray.h"

Ray::Ray()
	: m_Origin(0), m_Direction(0)
{

}
Ray::Ray(const SlavMath::Point3D& origin, const SlavMath::Vector3& direction)
	: m_Origin(origin), m_Direction(direction)
{

}
void Ray::Init(const SlavMath::Point3D& origin, const SlavMath::Vector3& direction)
{
	memcpy(&m_Origin, &origin, sizeof(m_Origin));
	memcpy(&m_Direction, &direction, sizeof(m_Direction));
	m_Record.Clear();
}
void Ray::SetDirection(const SlavMath::Vector3& direction)
{
	m_Direction = direction;
}
void Ray::SetOrigin(const SlavMath::Point3D& origin)
{
	m_Origin = origin;
}
void Ray::SetRecord(const float& t, const Primitives* object)
{
	m_Record.t = t;
	m_Record.hitObject = object;
}
void Ray::SetRecordIndex(const float& index)
{
	m_Record.index = index;
}
void Ray::AddOffset(const float& epsilon)
{
	m_Origin += m_Direction * epsilon;
}
void Ray::Clear()
{
	m_Origin = 0; m_Direction = 0;
}
//Return a point based on t
SlavMath::Point3D Ray::At(const float& t) const
{
	return SlavMath::Point3D(m_Origin + m_Direction * t);
}

SlavMath::Point3D Ray::IntersectPoint() const
{
	return SlavMath::Point3D(At(m_Record.t));
}

SlavMath::Vector3 Ray::GetDirection() const
{
	return m_Direction;
}

SlavMath::Point3D Ray::GetOrigin() const
{
	return m_Origin;
}

float Ray::GetRecordIndex() const
{
	return m_Record.index;
}

float Ray::GetRecordT() const
{
	return m_Record.t;
}

void Ray::ClearRecord()
{
	m_Record.Clear();
}

const Primitives* Ray::GetObject() const
{
	return m_Record.hitObject;
}

bool Ray::hasHitObject() const
{
	return m_Record.hitObject != nullptr;
}

void HitRecord::Clear()
{
	t = INFINITE;
	hitObject = nullptr;
	index = 1.0f;
}
