#include "DoomFire.h"

const int window_height = 50;
const int window_width = 640;

int main(int argc, char **argv) {
	
	DoomFire doomFire;
	doomFire.Initialize(
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,		
		window_width,
		window_height
	);
	doomFire.InitFirePixels();

	std::cout << "Running SDL2 DOOM Fire" << std::endl;

	while (doomFire.GetIsRunning()) {
		doomFire.HandleEvents();
		doomFire.UpdateFire();
		doomFire.Render();
	}

	doomFire.Destroy();

	return EXIT_SUCCESS;
}