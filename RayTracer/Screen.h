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
		traceData() = default;
		traceData(unsigned int dest, SlavMath::Vector3 pos, SlavMath::Vector3 direction)
			: dest(dest), pos(pos), direction(direction) {};
		~traceData() = default;

		SlavMath::Vector3 pos;
		SlavMath::Vector3 direction;
		unsigned int dest;
	};

public:
	Screen(int width, int height);
	~Screen();

	void Init();
	void Clear() const;
	unsigned int* GetBuffer();
	const int GetSize() const;
	void Draw();

	void SetCamera(const Camera* camera);
	void SetScene(const Scene* scene);
	void SetAccumulator(SlavMath::Vector3* accumulator, int* frameCount);

	SlavMath::Vector3& GetBufferIndex(unsigned int handle) { return m_Accumulator[handle]; };
	ThreadSafeQueue<traceData>& GetTaskQueue() { return m_WorkQueue; };
	const Scene& GetScene() { return *m_Scene; };
	bool IsTaskQueueReady() const { return m_TasksReady; };
private:
	void HandleBuffer();
private:
	ThreadSafeQueue<traceData> m_WorkQueue;

	SlavMath::Vector3* m_Accumulator = nullptr;
	int* m_FrameCount = nullptr;

	unsigned int* m_Buffer = nullptr;

	const Camera* m_Camera = nullptr;
	const Scene* m_Scene = nullptr;
	
	SDL_Thread** m_ThreadPool = nullptr;
	MyRandom m_RNG;

	int m_Width = 0, m_Height = 0;
	bool m_TasksReady = false;
};

int doThreadedTrace(void* traceData);

#define __MY_SCREEN_RAY__
#endif // !__MY_SCREEN_RAY__

