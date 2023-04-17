#ifndef __RAY_RENDERER__
#define __RAY_RENDERER__
#pragma once
#include "Ray.h"

class Camera;
class Screen;
class SlavMath::Vector3;
class MyRandom;
struct SceneData;

class RayTracer
{
public:
	void Update(const float& deltaTime);
	void Init();
	void SetScreen(Screen* screen);
	void SetCamera(Camera* camera);
	void SetScene(const SceneData* scene);
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
private:
	Screen* m_Screen = nullptr;
	Camera* m_Camera = nullptr;
	SlavMath::Vector3* m_Accumulator = nullptr;
	int* m_FrameCount = nullptr;
	MyRandom* m_Generator = nullptr;
	const SceneData* m_Scene;
};

#endif 

