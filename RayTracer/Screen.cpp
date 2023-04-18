#include "precomp.h"
#include "Camera.h"
#include "RayTracer.h"
#include "Scene.h"
#include "Screen.h"
#include <ctime>

Screen::Screen(int width, int height)
	: m_Width(width), m_Height(height), RNG(0)
{
	time_t now = time(NULL);
	tm timestruct; gmtime_s(&timestruct, &now);
	MyRandom temp(timestruct.tm_sec + timestruct.tm_min);
	RNG = temp;

	m_Buffer = new unsigned int[GetSize()];
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

void Screen::Draw()
{
	float u, v;
	const float HeightInverse = 1.0f / m_Height, WidthInverse = 1.0f / m_Height;
	
	tracer.SetScene(&m_Scene->GetScene());
	tracer.SetRNG(&RNG);

	SlavMath::Vector3* buffer = m_Accumulator;

	for (int y = (m_Height - 1); y > -1; y--)
	{
		v = (float)y * HeightInverse;

		for (int x = 0; x < m_Width; x++)
		{
			u = (float)x * WidthInverse;
			buffer[x] += tracer.GetPixelColor(m_Camera->GetPosition(), m_Camera->GetRayDirection(
				u + RNG.GetAFloatBetween(0.01f, 0.015f),
				v + RNG.GetAFloatBetween(0.01f, 0.015f)
			));
		}
		buffer += m_Width;
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
