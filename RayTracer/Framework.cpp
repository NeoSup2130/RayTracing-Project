#include "precomp.h"
#include "Framework.h"
#include "RayTracer.h"

uint64_t Performance_TImer::average = 0;
uint64_t Performance_TImer::count = 0;

Framework::Framework() :
	m_Screen(nullptr),
	m_Current_Camera(nullptr),
	m_RayTracer(nullptr),
	m_ExitPtr(nullptr),
	m_SDL_Window(nullptr),
	m_SDL_Buffer(nullptr),
	m_SDL_Renderer(nullptr),
	m_Accumulator(nullptr),
	m_FrameCount(0),
	m_RNG(nullptr), 
	m_ProgramSeed{0xABCDEF}
{
}

bool Framework::Init(bool* exit)
{
	SetExit(exit);
	m_Screen = new Screen(SCREENWIDTH, SCREENHEIGHT);
	m_Accumulator = new SlavMath::Color[SCREENWIDTH * SCREENHEIGHT];
	m_Current_Camera = new Camera();
	m_RayTracer = new RayTracer();
	
	m_RNG = new MyRandom(m_ProgramSeed[0]);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL video failed to initialize. %s\n", SDL_GetError());
		return false;
	}
	m_SDL_Window = SDL_CreateWindow("Ray Tracing by Slava 200001", 100, 100, SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_SHOWN);

	if (m_SDL_Window == NULL)
	{
		printf("Window creation failed. %s\n", SDL_GetError());
		return false;
	}
	m_SDL_Renderer = SDL_CreateRenderer(m_SDL_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//--> Hardware accelaration, VRAM used, and RendererPresent is Synced with refresh rate
	m_SDL_Buffer = SDL_CreateTexture(m_SDL_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREENWIDTH, SCREENHEIGHT);
	//--> Streaming flags lockable for texture.
	m_Screen->Clear();

	printf("SDL initialization completed.\n");

	m_RayTracer->SetScene(&m_Scene.GetScene());
	m_RayTracer->SetCamera(m_Current_Camera);
	m_RayTracer->SetScreen(m_Screen);
	m_RayTracer->SetAccumulator(m_Accumulator);
	m_RayTracer->SetFrameCounter(&m_FrameCount);
	m_RayTracer->SetRNG(m_RNG);
	m_RayTracer->Init();

	return true;
}

void Framework::Start_Tick()
{
	m_FrameTimer.LAST = m_FrameTimer.NOW;
}

float Framework::Get_Delta_Time()
{
	m_FrameTimer.NOW = SDL_GetPerformanceCounter();
	float delta = static_cast<float>(m_FrameTimer.NOW - m_FrameTimer.LAST);
	return delta / static_cast<float>(SDL_GetPerformanceFrequency());
}

void Framework::Screen_To_Buffer()
{
	//-Start: Buffer copied to screen buffer through SDL framework.
	void* address = 0;
	int pitch = SCREENWIDTH * sizeof(int);

	SDL_LockTexture(m_SDL_Buffer, NULL, &address, &pitch);
	memcpy(address, m_Screen->GetBuffer(), SCREENWIDTH * SCREENHEIGHT * 4);
	SDL_UnlockTexture(m_SDL_Buffer);
	SDL_RenderCopy(m_SDL_Renderer, m_SDL_Buffer, NULL, NULL);
	SDL_RenderPresent(m_SDL_Renderer);
	//-End
}

void Framework::Update(const float& deltaTime)
{
	Screen_To_Buffer();
	m_RayTracer->Update(deltaTime);
	HandleInput();
}

void Framework::SetExit(bool* exit)
{
	m_ExitPtr = exit;
}

void Framework::ResetAccumulator()
{
	m_FrameCount = 0;
	memset(m_Accumulator, 0, sizeof(SlavMath::Vector3) * SCREENHEIGHT * SCREENWIDTH);
}

void Framework::HandleInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			*m_ExitPtr = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_q:
				*m_ExitPtr = true;
				break;
			case SDLK_w:
				m_Current_Camera->MoveBackForth(1.0f);
				ResetAccumulator();
				break;
			case SDLK_d:
				m_Current_Camera->MoveSideWays(-1.0f);
				ResetAccumulator();
				break;
			case SDLK_a:
				m_Current_Camera->MoveSideWays(1.0f);
				ResetAccumulator();
				break;
			case SDLK_s:
				m_Current_Camera->MoveBackForth(-1.0f);
				ResetAccumulator();
				break;
			case SDLK_z:
				m_Current_Camera->RotateAroundY(1.0f);
				ResetAccumulator();
				break;
			case SDLK_x:
				m_Current_Camera->RotateAroundY(-1.0f);
				ResetAccumulator();
				break;
			case SDLK_r:
				m_Current_Camera->MoveUpDown(-1.0f);
				ResetAccumulator();
				break;
			case SDLK_f:
				m_Current_Camera->MoveUpDown(1.0f);
				ResetAccumulator();
				break;
			}
			break;
		default:
			break;
		}
		//Causes delays whenever input is detected. Therefore, "lag" is caused.
		//SDL_Delay(16); //Prevents program from sucking up all the CPU time.
	}
}

Tick_Timer::Tick_Timer()
	: NOW(SDL_GetTicks()), LAST(0)
{
}

Performance_TImer::Performance_TImer() : START(SDL_GetPerformanceCounter()), END(0) {
	printf("Perf started; duration: ");
}

Performance_TImer::~Performance_TImer()
{
	END = SDL_GetPerformanceCounter();
	//Time is given in seconds. However, I need milliseconds so we multiply by 1000.0f.
	float time = (END - START) / static_cast<float>(SDL_GetPerformanceFrequency());
	time *= SEC;
	average += (END - START);
	count++;
	printf("%f ms || ", time);
	time = (average / count) / static_cast<float>(SDL_GetPerformanceFrequency());
	printf("%f s average time\n", time);
}
