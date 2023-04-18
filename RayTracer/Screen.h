#ifndef __MY_SCREEN_RAY__
#pragma once
#include "MyRandom.h"
#include "RayTracer.h"
#include "ThreadSafeQueue.h"

class Camera;
class SlavMath::Vector3;
struct SDL_Thread;

class Screen
{
public:
	struct traceData
	{
		traceData(SDL_mutex* mtx, const SceneData& scene, MyRandom& rng, SlavMath::Vector3* dest, SlavMath::Vector3 pos, SlavMath::Vector3 direction)
			: mtx(mtx), scene(scene), rng(rng), dest(dest), pos(pos), direction(direction) {};
		~traceData() = default;

		SDL_mutex* mtx;
		const SceneData& scene;
		MyRandom& rng;
		SlavMath::Vector3* dest;
		SlavMath::Vector3 pos;
		SlavMath::Vector3 direction;
	};

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

	int* m_FrameCount = nullptr;
	unsigned int* m_Buffer = nullptr;
	SlavMath::Vector3* m_Accumulator = nullptr;
	const Camera* m_Camera = nullptr;
	const Scene* m_Scene = nullptr;

	SDL_Thread** m_ThreadPool = nullptr;
	ThreadSafeQueue<traceData> m_WorkQueue;
};

int doThreadedTrace(void* traceData);

Screen::traceData CreateTraceData(SDL_mutex* mtx, const SceneData& scene, MyRandom& rng, SlavMath::Vector3* dest, SlavMath::Vector3 pos, SlavMath::Vector3 direction);

#define __MY_SCREEN_RAY__
#endif // !__MY_SCREEN_RAY__

