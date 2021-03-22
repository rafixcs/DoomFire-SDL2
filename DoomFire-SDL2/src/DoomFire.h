#ifndef DOOMFIRE_H
#define DOOMFIRE_H

#include <iostream>
#include <SDL.h>
#include <math.h>
#include <time.h>

class DoomFire {
private:
	SDL_Window* window;
	SDL_Renderer* render;
	int win_width;
	int win_height;
	bool isRunning;
	
	// **Temporary declarament
	SDL_Texture* fireTexture;
	Uint32 *fire_pixels;
	Uint32* pixel_intesity;

public:
	DoomFire();
	~DoomFire();

	bool GetIsRunning() const;
	void Initialize(int posx, int posy, int win_width, int win_height);
	void HandleEvents();
	void Destroy();
	void UpdateFire();
	void Render();
	
	void InitFirePixels();

private:
	void CreateFireSource();
	void CalculateFirePropagation();
	void UpdateFireIntesityPerPixel(int currentPixelIndex);

};


#endif // !DOOMFIRE_H

