#ifndef __Ray__
#define __Ray__
#pragma once
#define INFINITE 0xFFFFFF
class Primitives;

struct HitRecord
{
	HitRecord() {}
	float t = INFINITE;
	float index = 1.0f;
	const Primitives* hitObject = nullptr;
	void Clear();
};

class Ray
{
public:
	Ray();
	Ray(const SlavMath::Point3D& origin, const SlavMath::Vector3& direction);
	void Init(const SlavMath::Point3D& origin, const SlavMath::Vector3& direction);
	void SetDirection(const SlavMath::Vector3& direction);
	void SetOrigin(const SlavMath::Point3D& origin);
	void SetRecord(const float& t, const Primitives* object);
	void SetRecordIndex(const float& index);
	void AddOffset(const float& epsilon);
	void ClearRecord();
	void Clear();

	SlavMath::Point3D At(const float& t) const;
	const Primitives* GetObject() const;
	bool hasHitObject() const;
	SlavMath::Point3D IntersectPoint() const;

	SlavMath::Vector3 GetDirection() const;
	SlavMath::Point3D GetOrigin() const;
	float GetRecordIndex() const;
	float GetRecordT() const;

private:
	SlavMath::Point3D m_Origin;
	SlavMath::Vector3 m_Direction;
	HitRecord m_Record;
};
#endif 
