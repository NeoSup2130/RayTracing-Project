#include "precomp.h"
#include "Camera.h"
#include "RayTracer.h"
#include "Scene.h"
#include "Screen.h"
#include <SDL_thread.h>


Screen::Screen(int width, int height)
	: m_Width(width), m_Height(height), m_ThreadPool(new SDL_Thread* [SDL_GetCPUCount()])
{
	m_Buffer = new unsigned int[GetSize()];
}

Screen::~Screen()
{
	if (m_ThreadPool)
	{
		delete[] m_ThreadPool;
	}
}

void Screen::Init()
{
	// Create worker threads to do ray tracing
	for (size_t i = 0, max = SDL_GetCPUCount(); i < max; i++)
	{
		m_ThreadPool[i] = SDL_CreateThread(doThreadedTrace, "TracerThread", (void*)this);
		SDL_DetachThread(m_ThreadPool[i]);
	}
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

int doThreadedTrace(void* owner)
{
	Screen* screen = reinterpret_cast<Screen*>(owner);
	ThreadSafeQueue<Screen::traceData>& taskQueue =	screen->GetTaskQueue();
	RayTracer tracer;
	MyRandom RNG;
	tracer.SetScene(&screen->GetScene().GetScene());
	tracer.SetRNG(&RNG);

	SlavMath::Color result;

	size_t i = 0, input = 0;
	const size_t maxData = 16;

	Screen::traceData dataPack[maxData];

	//While thread queue is alive
	while (screen)
	{
		while (screen->IsTaskQueueReady())
		{
			taskQueue.popN(maxData, input, dataPack);
			for (i = 0; i < input; i++)
			{
				result = tracer.GetPixelColor(dataPack[i].pos, dataPack[i].direction);
				screen->GetBufferIndex(dataPack[i].dest) += result;
			}
		}
		SDL_Delay(17);
	}
	return 0;
}

void Screen::Draw()
{
	SlavMath::Vector3 camPos = m_Camera->GetPosition();

	float u, v;
	const float HeightInverse = 1.0f / m_Height, WidthInverse = 1.0f / m_Height;

	m_TasksReady = false;

	for (int start = m_Height - 1,  y = start; y > -1; y--)
	{
		v = (float)y * HeightInverse;

		for (int x = 0; x < m_Width; x++)
		{
			u = (float)x * WidthInverse;

			// Calculate buffer index with "x + (start - y) * m_Width" since y start at max height
			m_WorkQueue.push({ static_cast<unsigned int>(x + (start - y) * m_Width), 
				camPos, m_Camera->GetRayDirection(
				u + m_RNG.GetAFloatBetween(0.001f, 0.0005f),
				v + m_RNG.GetAFloatBetween(0.001f, 0.0005f))});
		}
	}

	m_TasksReady = true;

	// Wait while worker threads ray trace our data
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
