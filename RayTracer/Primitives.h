#ifndef __Primitives__
#define __Primitives__
#pragma once

class Ray;
class Material;

class Primitives
{
public:
	virtual void IntersectionRay(Ray& ray) const = 0;
	virtual SlavMath::Vector3 GetNormal(const Ray& ray) const = 0;
	virtual void GetColorAtPoint(const SlavMath::Point3D& point, SlavMath::Color& receiver) const = 0;
	virtual const float& GetReflectivity() const = 0;
	virtual const float& GetIndex() const = 0;
	virtual bool IsDieletric() const = 0;
	virtual bool IsReflective() const = 0;
private:
	virtual void SetRayRecord(Ray& a_Ray, float& a_T) const = 0;
protected:
	const Material* m_Material{nullptr};
};

class Plane : public Primitives
{	
public:
	/*Vector3 normal, float distance, Color color */
	Plane(SlavMath::Vector3 normal, float d, Material* const material, const bool& is_Checkered);
	void IntersectionRay(Ray& ray) const;
	SlavMath::Vector3 GetNormal(const Ray& ray) const;
	void GetColorAtPoint(const SlavMath::Point3D& point, SlavMath::Color& receiver) const;
	const float& GetReflectivity() const;
	const float& GetIndex() const;
	bool IsDieletric() const;
	bool IsReflective() const;
private:
	void SetRayRecord(Ray& a_Ray, float& a_T) const;
private:
	SlavMath::Vector3 m_Normal;
	float m_DistanceOrigin;
	const bool m_IsCheckered;
};

class Sphere : public Primitives
{
public:
	/*Point3D position, float radius, Color color*/
	Sphere(SlavMath::Point3D position, float radius, Material* const material);
	void IntersectionRay(Ray& ray) const;
	SlavMath::Vector3 GetNormal(const Ray& ray) const;
	void GetColorAtPoint(const SlavMath::Point3D& point, SlavMath::Color& receiver) const;
	const float& GetReflectivity() const;
	const float& GetIndex() const;
	bool IsDieletric() const;
	bool IsReflective() const;
private:
	void SetRayRecord(Ray& a_Ray, float& a_T) const;
	
private:
	//Radius will be stored as a squared radius.
	float m_Radius;
	SlavMath::Point3D m_Position;
};

class AxisAlignedBox : public Primitives
{
public:
	AxisAlignedBox(const SlavMath::Vector3& Vmin, const SlavMath::Vector3& Vmax, Material* const material);
	AxisAlignedBox(const SlavMath::Vector3& position, const float& size, Material* const material);
	void IntersectionRay(Ray& ray) const;
	SlavMath::Vector3 GetNormal(const Ray& ray) const;
	void GetColorAtPoint(const SlavMath::Point3D& point, SlavMath::Color& receiver) const;
	const float& GetReflectivity() const;
	const float& GetIndex() const;
	bool IsDieletric() const;
	bool IsReflective() const;
private:
	void SetRayRecord(Ray& a_Ray, float& a_T) const;

private:
	SlavMath::Vector3 m_Bounds[2];
	SlavMath::Vector3 m_Position;
};

class OrientatedBox : public Primitives
{
public:
	OrientatedBox(const SlavMath::Vector3& position, const SlavMath::Vector3& size, const SlavMath::Matrix3& modelmatrix, Material* const material);
	void IntersectionRay(Ray& ray) const;
	SlavMath::Vector3 GetNormal(const Ray& ray) const;
	void GetColorAtPoint(const SlavMath::Point3D& point, SlavMath::Color& receiver) const;
	const float& GetReflectivity() const;
	const float& GetIndex() const;
	bool IsDieletric() const;
	bool IsReflective() const;
private:
	void TransformModel(const SlavMath::Matrix3& a_Matrix);
	void SetRayRecord(Ray& a_Ray, float& a_T) const;

private:
	//The box's normal vectors; used for orientation.
	SlavMath::Vector3 m_BasisVectors[3];

	//The center point of the box; origin of object world space.
	SlavMath::Vector3 m_Position;

	//Uses for keeping track of the rotation.
	SlavMath::Matrix3 m_ModelMatrix;

	//The Length, heigth and depth
	SlavMath::Vector3 m_Dimensions;
};

class LightObject
{
public:
	virtual SlavMath::Point3D GetPosition() const = 0;
	virtual SlavMath::Point3D GetColor() const = 0;
	virtual SlavMath::Point3D GetPointOnArea(const float& a_U, const float& a_V) const = 0;
};

class PointLight : public LightObject
{
public:
	//void update();
	PointLight(SlavMath::Point3D position, SlavMath::Color color);
	SlavMath::Point3D GetPosition() const;
	SlavMath::Color GetColor() const;
	SlavMath::Point3D GetPointOnArea(const float& a_U, const float& a_V) const;
private:
	SlavMath::Point3D m_Position;
	SlavMath::Color m_Color;
	//bool m_ShakeAndBake = true;
	//int m_Hue = 0;
};

class RectangleLight : public LightObject
{
public:
	RectangleLight(SlavMath::Point3D position, SlavMath::Vector2 dimensions, SlavMath::Vector3 normal, SlavMath::Color color);
	SlavMath::Point3D GetPosition() const;
	SlavMath::Color GetColor() const;
	SlavMath::Point3D GetPointOnArea(const float& a_U, const float& a_V) const;
private:
	SlavMath::Vector3 GetVectorU() const;
	SlavMath::Vector3 GetVectorV(const SlavMath::Vector3& a_U) const;
private:
	SlavMath::Point3D m_Position;
	SlavMath::Color m_Color;
	SlavMath::Vector3 m_Normal;
	SlavMath::Vector2 m_Dimension;
};
#endif 