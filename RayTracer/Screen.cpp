#include "precomp.h"
#include "Camera.h"
#include "RayTracer.h"
#include "Scene.h"
#include "Screen.h"
#include <SDL_thread.h>
#include <ctime>

Screen::Screen(int width, int height)
	: m_Width(width), m_Height(height), RNG(0), m_ThreadPool(new SDL_Thread* [SDL_GetCPUCount()])
{
	time_t now = time(NULL);
	tm timestruct; gmtime_s(&timestruct, &now);
	MyRandom temp(timestruct.tm_sec + timestruct.tm_min);
	RNG = temp;

	m_Buffer = new unsigned int[GetSize()];

	for (int i = 0; i < SDL_GetCPUCount(); i++)
		m_ThreadPool[i] = nullptr;
}

void Screen::Clear() const
{
	memset(m_Buffer, 0, GetSize() * sizeof(int));
}

unsigned int* Screen::GetBuffer()
{
	return m_Buffer;
}

const int Screen::GetSize() const
{
	return m_Height * m_Width;
}

int doThreadedTrace(void* threadQueue)
{
	ThreadSafeQueue<Screen::traceData>& queue = *(reinterpret_cast<ThreadSafeQueue<Screen::traceData>*>(threadQueue));
	RayTracer tracer;
	SlavMath::Color result;

	while (!queue.Empty())
	{
		Screen::traceData data = queue.pop();

		tracer.SetScene(&data.scene);
		tracer.SetRNG(&data.rng);

		result = tracer.GetPixelColor(data.pos, data.direction);

		SDL_LockMutex(data.mtx);
		(*data.dest) += result;
		SDL_UnlockMutex(data.mtx);
	}
	return 0;
}

Screen::traceData CreateTraceData(SDL_mutex* mtx, const SceneData& scene, MyRandom& rng, SlavMath::Vector3* dest, SlavMath::Vector3 pos, SlavMath::Vector3 direction)
{
	return Screen::traceData(mtx, scene, rng, dest, pos, direction);
}

void Screen::Draw()
{
	SlavMath::Vector3 camPos = m_Camera->GetPosition();

	float u, v;
	const float HeightInverse = 1.0f / m_Height, WidthInverse = 1.0f / m_Height;

	SlavMath::Vector3* buffer = m_Accumulator;
	SDL_mutex* mtx = SDL_CreateMutex();


	for (int y = (m_Height - 1); y > -1; y--)
	{
		v = (float)y * HeightInverse;

		for (int x = 0; x < m_Width; x++)
		{
			u = (float)x * WidthInverse;

			traceData data = CreateTraceData(mtx, m_Scene->GetScene(), RNG, &buffer[x], camPos, m_Camera->GetRayDirection(
				u + RNG.GetAFloatBetween(0.01f, 0.015f),
				v + RNG.GetAFloatBetween(0.01f, 0.015f)));
			
			m_WorkQueue.push(data);
		}
		buffer += m_Width;
	}

	for (int i = 0; i < SDL_GetCPUCount(); i++)
	{
		m_ThreadPool[i] = SDL_CreateThread(doThreadedTrace, "TracerThread", (void*)&m_WorkQueue);
		SDL_DetachThread(m_ThreadPool[i]);
	}

	while (!m_WorkQueue.Empty())
	{
		SDL_Delay(10);
	}

	HandleBuffer();
}

void Screen::SetCamera(const Camera* camera)
{
	m_Camera = camera;
}

void Screen::SetScene(const Scene* scene)
{
	m_Scene = scene;
}

void Screen::SetAccumulator(SlavMath::Vector3* accumulator, int* frameCount)
{
	m_Accumulator = accumulator;
	m_FrameCount = frameCount;
}

void Screen::HandleBuffer()
{
	(*m_FrameCount)++;
	float count = 1.0f / (float)(*m_FrameCount);
	for (size_t i = 0, max = GetSize(); i < max; i++)
	{
		m_Buffer[i] = getVectorColor(m_Accumulator[i] * count);
	}
}
