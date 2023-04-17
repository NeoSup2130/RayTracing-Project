#ifndef __Framework__
#define __Framework__
#pragma once
#include "Scene.h"

struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;
class Screen;
class Camera;
class RayTracer;
class SlavMath::Vector3;
class MyRandom;

struct Tick_Timer
{
	Tick_Timer();
	Uint32 NOW, LAST;
};

struct Performance_TImer
{
	Performance_TImer();
	~Performance_TImer();
private:
	uint64_t START, END;
	static uint64_t average;
	static uint64_t count;
};

class Framework
{
public:
	Framework();
	bool Init(bool* exit);
	void Start_Tick();
	float Get_Delta_Time();
	void Update(const float& deltaTime);
private:
	void Screen_To_Buffer();
	void SetExit(bool* exit);
	void ResetAccumulator();
	void HandleInput();
private:
	Tick_Timer m_FrameTimer;
	MyRandom* m_RNG;
	unsigned int m_ProgramSeed[1];

	Screen* m_Screen;
	SlavMath::Vector3* m_Accumulator;
	int m_FrameCount;

	Camera* m_Current_Camera;
	RayTracer* m_RayTracer;
	Scene m_Scene;
	bool* m_ExitPtr = nullptr;

	SDL_Window* m_SDL_Window;
	SDL_Texture* m_SDL_Buffer;
	SDL_Renderer* m_SDL_Renderer;
};

#endif // !__Framework__