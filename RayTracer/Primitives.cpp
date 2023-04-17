#include "precomp.h"
#include "Primitives.h"
#include "Ray.h"
#include "Material.h"

using namespace SlavMath;

Sphere::Sphere(SlavMath::Point3D position, float radius, Material* const material)
	: m_Radius(radius*radius)
{
	m_Material = material;
	m_Position = position;
}

PointLight::PointLight(SlavMath::Point3D position, SlavMath::Color color)
{
	m_Position = position;
	m_Color = color;
}

SlavMath::Point3D PointLight::GetPosition() const
{
	return m_Position;
}

SlavMath::Color PointLight::GetColor() const
{
	return m_Color;
}

SlavMath::Point3D PointLight::GetPointOnArea(const float& a_U, const float& a_V) const
{
	(void)a_U; (void)a_V;
	return m_Position;
}

Plane::Plane(SlavMath::Vector3 normal, float d, Material* const material, const bool& is_Checkered)
	: m_DistanceOrigin(d), m_IsCheckered(is_Checkered)
{
	m_Material = material;
	m_Normal = normal;
}

/* Reference code for fast intersection from Jacco Bikker*/
void Plane::IntersectionRay(Ray& ray) const
{
	float t = -(dot(ray.GetOrigin(), m_Normal) + m_DistanceOrigin) / dot(ray.GetDirection(), m_Normal);
	if (t < ray.GetRecordT() && t > EPSILON)
	{
		SetRayRecord(ray, t);
	}
}

Vector3 Plane::GetNormal(const Ray& ray) const
{
	if (dot(ray.GetDirection(), m_Normal) > 0)
	return -m_Normal;
	else return m_Normal;
}

void Plane::GetColorAtPoint(const SlavMath::Point3D& point, SlavMath::Color& receiver) const
{
	if (this->IsDieletric() || !m_IsCheckered)
	{
		memcpy(&receiver, m_Material->GetTexture(), sizeof(receiver));
		return;
	}
	
	if (((int)(point.x * 4) + (int)(point.z * 4)) & 1)
	{
		memcpy(&receiver, m_Material->GetTexture(), sizeof(receiver));
	}
	else
	{
		receiver = 0;
	}
}

const float& Plane::GetReflectivity() const
{
	return m_Material->GetReflectivity();
}

const float& Plane::GetIndex() const
{
	return m_Material->GetIndex();
}

bool Plane::IsDieletric() const
{
	return m_Material->IsDieletric();
}

bool Plane::IsReflective() const
{
	return m_Material->IsReflective();
}

void Plane::SetRayRecord(Ray& a_Ray, float& a_T) const
{
	a_Ray.SetRecord(a_T, this);
}

/* Reference code, modified, for sphere intersection from Jacco Bikker*/
void Sphere::IntersectionRay(Ray& ray) const
{
	Vector3 center = m_Position - ray.GetOrigin();
	float t = dot(center, ray.GetDirection());
	Vector3 q = center - ray.GetDirection() * t;
	float p = dot(q,q);

	if (p > m_Radius) return;
	float x = sqrtf(m_Radius - p);
	if (t - x > EPSILON) t -= x;
	else t += x;
	if (t < ray.GetRecordT() && t > EPSILON)
	{
		SetRayRecord(ray, t);
	}
}

Vector3 Sphere::GetNormal(const Ray& ray) const
{
	Vector3 normal = (ray.At(ray.GetRecordT()) - m_Position) / sqrtf(m_Radius);
	if (dot(ray.GetDirection(), normal) > 0)
		return -normal;
	else return normal;
}

void Sphere::GetColorAtPoint(const SlavMath::Point3D& point, SlavMath::Color& receiver) const
{
	//Temporary fix:
	(void)point;
	//To do: Added textures for objects so that we can use point to determine the pixel location at the texture.
	memcpy(&receiver, m_Material->GetTexture(), sizeof(receiver));
}

const float& Sphere::GetReflectivity() const
{
	return m_Material->GetReflectivity();
}

const float& Sphere::GetIndex() const
{
	return m_Material->GetIndex();
}

bool Sphere::IsDieletric() const
{
	return m_Material->IsDieletric();
}

bool Sphere::IsReflective() const
{
	return m_Material->IsReflective();;
}

void Sphere::SetRayRecord(Ray& a_Ray, float& a_T) const
{
	a_Ray.SetRecord(a_T, this);
}

RectangleLight::RectangleLight(SlavMath::Point3D position, SlavMath::Vector2 dimensions, SlavMath::Vector3 normal, SlavMath::Color color) :
	m_Position(position), m_Color(color), m_Dimension(dimensions), m_Normal(normal)
{
}

SlavMath::Point3D RectangleLight::GetPosition() const
{
	return m_Position;
}

SlavMath::Color RectangleLight::GetColor() const
{
	return m_Color;
}

SlavMath::Point3D RectangleLight::GetPointOnArea(const float& a_U, const float& a_V) const
{
	Vector3 U = GetVectorU();
	Vector3 V = GetVectorV(U);
	Point3D point = m_Position + m_Dimension.x * (0.5f - a_U) * U + m_Dimension.y * (0.5f - a_V) * V;
	return point;
}

SlavMath::Vector3 RectangleLight::GetVectorU() const
{
	return m_Normal.perpendicular();
}

SlavMath::Vector3 RectangleLight::GetVectorV(const SlavMath::Vector3& a_U) const
{
	return SlavMath::Vector3(crossProduct(m_Normal, a_U));
}

AxisAlignedBox::AxisAlignedBox(const SlavMath::Vector3& Vmin, const SlavMath::Vector3& Vmax, Material* const material) :
	m_Bounds{Vmin, Vmax}
{
	m_Material = material;
	m_Position = { (Vmin + Vmax) * 0.50f };
}

AxisAlignedBox::AxisAlignedBox(const SlavMath::Vector3& position, const float& size, Material* const material) :
	m_Position(position), m_Bounds{position - size, position + size}
{
	m_Material = material;
}

void AxisAlignedBox::IntersectionRay(Ray& ray) const
{
	Vector3 invRayDir = ray.GetDirection().inversed();
	Vector3 t0 = (m_Bounds[0]- ray.GetOrigin()) * invRayDir;
	Vector3 t1 = (m_Bounds[1] - ray.GetOrigin()) * invRayDir;

	float tmin = max(max(min(t0.x, t1.x), min(t0.y, t1.y)), min(t0.z, t1.z));
	float tmax = min(min(max(t0.x, t1.x), max(t0.y, t1.y)), max(t0.z, t1.z));

	if (tmax >= tmin && tmax > EPSILON)
	{
		if (tmin < ray.GetRecordT() && tmin > EPSILON)
		{
			SetRayRecord(ray, tmin);
		}
	}
}

SlavMath::Vector3 AxisAlignedBox::GetNormal(const Ray& ray) const
{
	Vector3 point = ray.At(ray.GetRecordT());

	if (abs(m_Bounds[0].z - point.z) < EPSILON)
		return Vector3(0, 0, -1);

	if (abs(point.z - m_Bounds[1].z) < EPSILON)
		return Vector3(0, 0, 1);

	if (abs(m_Bounds[0].x - point.x) < EPSILON)
		return Vector3(1,0,0);

	if (abs(point.x - m_Bounds[1].x) < EPSILON)
		return Vector3(-1,0,0);

	if (abs(m_Bounds[0].y - point.y) < EPSILON)
		return Vector3(0, -1, 0);

	if (abs(point.y - m_Bounds[1].y) < EPSILON)
		return Vector3(0, 1, 0);

	return Vector3(0, 0, 0);

	// point z near m_Bounds[0].z then normal is -Z
	// point z near m_Bounds[1].z then normal is +z
	
	//point x near m_Bounds[0] then normal is +X
	//point x near m_Bounds[1] then normal is -X

	//point y near m_Bounds[0] then normal is -Y
	//point y near m_Bounds[1] then normal is +Y


	
}

void AxisAlignedBox::GetColorAtPoint(const SlavMath::Point3D& point, SlavMath::Color& receiver) const
{
	(void)point;
	//To do: Added textures for objects so that we can use point to determine the pixel location at the texture.
	memcpy(&receiver, m_Material->GetTexture(), sizeof(receiver));
}

const float& AxisAlignedBox::GetReflectivity() const
{
	return m_Material->GetReflectivity();
}

const float& AxisAlignedBox::GetIndex() const
{
	return m_Material->GetIndex();
}

bool AxisAlignedBox::IsDieletric() const
{
	return m_Material->IsDieletric();
}

bool AxisAlignedBox::IsReflective() const
{
	return m_Material->IsReflective();;
}

void AxisAlignedBox::SetRayRecord(Ray& a_Ray, float& a_T) const
{
	a_Ray.SetRecord(a_T, this);
}

OrientatedBox::OrientatedBox(const SlavMath::Vector3& position, const SlavMath::Vector3& size, const SlavMath::Matrix3& modelmatrix, Material* const material)
	: m_Position(position), m_Dimensions(size), m_ModelMatrix(modelmatrix), 
	m_BasisVectors{ 
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1 } }
{
	m_Material = material;
	TransformModel(modelmatrix);
}

void OrientatedBox::IntersectionRay(Ray& ray) const
{
	Vector3 delta = m_Position - ray.GetOrigin();

	float t0, t1, e, scale;
	float tMin = EPSILON;
	float tMax = INFINITE;
	
	for (int i = 0; i < 3; i++)
	{
		e = dot(m_BasisVectors[i], delta);
		scale = 1.0f / dot(ray.GetDirection(), m_BasisVectors[i]);

		t0 = (e + m_Dimensions.cell[i]) * scale;
		t1 = (e - m_Dimensions.cell[i]) * scale;

		if (t0 > t1)
		{
			float w = t0;
			t0 = t1;
			t1 = w;
		}
		if (t1 < tMax) tMax = t1;
		if (t0 > tMin) tMin = t0;
	}

	if (tMax > tMin && tMax > EPSILON)
	{
		if (tMin < ray.GetRecordT() && tMin > EPSILON)
		{
			SetRayRecord(ray, tMin);
		}
	}
}

SlavMath::Vector3 OrientatedBox::GetNormal(const Ray& ray) const
{
	Vector3 Face, Hit = ray.At(ray.GetRecordT());
	float cos;

	for (int i = 0; i < 6; i++)
	{
		if (i < 3)
		{
			Face = m_Position + -m_BasisVectors[i] * m_Dimensions.cell[i];
			cos = dot(Hit - Face, -m_BasisVectors[i]);
			if (cos < EPSILON && cos > -EPSILON)
				return -m_BasisVectors[i];
		}
		else
		{
			Face = m_Position + m_BasisVectors[i - 3] * m_Dimensions.cell[i - 3];
			cos = dot(Hit - Face, m_BasisVectors[i - 3]);
			if (cos < EPSILON && cos > -EPSILON)
				return m_BasisVectors[i - 3];
		}
	}
	return Vector3(1, 1, 1);
}

void OrientatedBox::GetColorAtPoint(const SlavMath::Point3D& point, SlavMath::Color& receiver) const
{
	(void)point;
	//To do: Added textures for objects so that we can use point to determine the pixel location at the texture.
	memcpy(&receiver, m_Material->GetTexture(), sizeof(receiver));
}

const float& OrientatedBox::GetReflectivity() const
{
	return m_Material->GetReflectivity();
}

const float& OrientatedBox::GetIndex() const
{
	return m_Material->GetIndex();
}

bool OrientatedBox::IsDieletric() const
{
	return m_Material->IsDieletric();
}

bool OrientatedBox::IsReflective() const
{
	return m_Material->IsReflective();
}

void OrientatedBox::TransformModel(const SlavMath::Matrix3& a_Matrix)
{
	for (size_t i = 0; i < sizeof(m_BasisVectors) / sizeof(Vector3); i++)
	{
		m_BasisVectors[i].Transform(a_Matrix);
		m_BasisVectors[i].normalize();
	}
}

void OrientatedBox::SetRayRecord(Ray& a_Ray, float& a_T) const
{
	a_Ray.SetRecord(a_T, this);
}

