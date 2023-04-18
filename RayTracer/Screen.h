#ifndef __MY_SCREEN_RAY__
#pragma once
#include "MyRandom.h"
#include "RayTracer.h"

class Camera;
class SlavMath::Vector3;

class Screen
{
public:
	Screen(int width, int height);
	void Clear() const;
	unsigned int* GetBuffer();
	const int GetSize() const;
	void Draw();
	void SetCamera(const Camera* camera);
	void SetScene(const Scene* scene);
	void SetAccumulator(SlavMath::Vector3* accumulator, int* frameCount);
private:
	void HandleBuffer();
private:
	int m_Width = 0, m_Height = 0;
	MyRandom RNG;
	RayTracer tracer;

	int* m_FrameCount = nullptr;
	unsigned int* m_Buffer = nullptr;
	SlavMath::Vector3* m_Accumulator = nullptr;
	const Camera* m_Camera = nullptr;
	const Scene* m_Scene = nullptr;
};

#define __MY_SCREEN_RAY__
#endif // !__MY_SCREEN_RAY__

