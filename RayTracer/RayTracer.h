#ifndef __RAY_RENDERER__
#define __RAY_RENDERER__
#pragma once
#include "Ray.h"

class Primitives;
class LightObject;
class Camera;
class Screen;
class MaterialSystem;
class SlavMath::Vector3;
class MyRandom;

class RayTracer
{
public:
	void Update(const float& deltaTime);
	void Init();
	void SetScreen(Screen* screen);
	void SetCamera(Camera* camera);
	void SetAccumulator(SlavMath::Vector3* accumulator);
	void SetFrameCounter(int* counter);
	void SetRNG(MyRandom* rng);
private:
	void CastRay(Ray& a_Ray);
	void Trace(Ray& a_Ray, SlavMath::Color& a_Color, SlavMath::Color& a_Emittance, char recur = 0);
	void Illumination(Ray& a_Ray, SlavMath::Color& a_Emittance);
	bool Occluded(const SlavMath::Point3D& a_Origin, const SlavMath::Vector3& a_Direction, const float& a_Max_T);
	float Fresnel(Ray& a_Ray, SlavMath::Vector3& a_Direction);

	void SetRayToReflected(Ray& a_Ray, const SlavMath::Vector3& a_Normal);
	bool SetRayToRefracted(Ray& a_Ray, const SlavMath::Vector3& a_Normal);
private:
	Ray m_PrimaryRay; 
	Ray m_ShadowRay; 
	Ray m_RefractedRay; 
	SlavMath::Point3D m_HitPoint;
	SlavMath::Vector3 m_Direction, m_Length;
	float m_NdotL = 0;
	float m_RayDistance = 0;
private:
	void HandleBufferFilter();
	void SceneCreation();
	void CornellBox();
	void AddSphere(SlavMath::Point3D position, float radius, int&& a_Material_Index);
	void AddCube(SlavMath::Vector3 position, float size, int&& a_Material_Index);
	void AddCube(SlavMath::Vector3 Vmin, SlavMath::Vector3 Vmax, int&& a_Material_Index);
	
	void AddCube(SlavMath::Vector3&& position, SlavMath::Vector3&& size, SlavMath::Matrix3&& modelmatrix, int&& a_Material_Index);
	void AddCube(SlavMath::Vector3&& position, SlavMath::Vector3&& size, const SlavMath::Matrix3& modelmatrix, int&& a_Material_Index);

	void AddPlane(SlavMath::Vector3 normal, float d, int&& a_Material_Index, const bool& is_Checker);
	void AddPointLight(SlavMath::Point3D position, SlavMath::Color&& color);
	void AddRectLight(SlavMath::Point3D position, SlavMath::Vector2 dimensions, SlavMath::Vector3 normal, SlavMath::Color&& color);
private:
	Screen* m_Screen = nullptr;
	Camera* m_Camera = nullptr;
	SlavMath::Vector3* m_Accumulator = nullptr;
	int* m_FrameCount = nullptr;
	MyRandom* m_Generator = nullptr;
	Primitives** m_PrimList = nullptr;
	LightObject** m_LigthList = nullptr;
	unsigned int m_PrimIt = 0, m_LightIt = 0;
	MaterialSystem* m_MaterialSystem = nullptr;
};

#endif 

