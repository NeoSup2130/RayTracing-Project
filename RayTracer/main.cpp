#include "precomp.h"

int main(int argc, char* argv[])
{
	(void)argc; (void)argv;
	printf("Build completed.\n");

	bool exit{ false };

	Framework* FrameCore = new Framework();
	if (!FrameCore->Init(&exit))
		return 0;

	float deltaTime = 0;

	while (!exit)
	{
		FrameCore->Start_Tick();

		FrameCore->Update(deltaTime);

		deltaTime = FrameCore->Get_Delta_Time();
	}
	SDL_Quit();
	return 1;
}