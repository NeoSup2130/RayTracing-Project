#ifndef __SlavFrame__
#define __SlavFrame__
#pragma once
#define USE_TEMPLATE_STRUCTURE false
#include "Math.h"

#if USE_TEMPLATE_STRUCTURE
#include "Primitives.h"
#include "Nodes.h"
#else
class Primitives;
class PointLight;
#endif

class Camera;

class Screen
{
public:
	unsigned int* m_Buffer;
private:
	int m_Width, m_Height;
	int m_Size;
public:
	void Clear() const;
	Screen(int m_Width, int m_Height);
};


class SlavFrame
{
public:
	SlavFrame();
	void Update();
	void Init();
	void SetScreen(Screen* screen);
	void SetCamera(Camera* camera);
	
	void AddSphere(SlavMath::Point3D position, float radius, SlavMath::Color color);
	void AddPlane(SlavMath::Vector3 normal, float d, SlavMath::Color color);
	void AddLightPoint(SlavMath::Point3D position, SlavMath::Color color);

public:
	Screen* m_Screen;
	Camera* m_Camera;

	#if USE_TEMPLATE_STRUCTURE
	MyQueue<Primitives*> m_ObjectsList;
	MyQueue<PointLight*> m_LightList;
	#else
	int const m_POOLSIZE{ 100 };
	int m_ObjectIt{ 0 }, m_LightIt{ 0 };
	Primitives** m_ObjectsList;
	PointLight** m_LightList;
	#endif
};

#endif