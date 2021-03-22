#include "DoomFire.h"

int fireColorsPalette[37][3] = { 
    {7, 7, 7}, {31, 7, 7}, {47, 15, 7}, {71, 15, 7}, {87, 23, 7}, {103, 31, 7}, {119, 31, 7}, {143, 39, 7},
    {159, 47, 7}, {175, 63, 7}, {191, 71, 7}, {199, 71, 7}, {223, 79, 7}, {223, 87, 7}, {223, 87, 7}, {215, 95, 7},
    {215, 95, 7}, {215, 103, 15}, {207, 111, 15}, {207, 119, 15}, {207, 127, 15}, {207, 135, 23}, {199, 135, 23},
    {199, 143, 23}, {199, 151, 31}, {191, 159, 31}, {191, 159, 31}, {191, 167, 39}, {191, 167, 39}, {191, 175, 47},
    {183, 175, 47}, {183, 183, 47}, {183, 183, 55}, {207, 207, 111}, {223, 223, 159}, {239, 239, 199}, {255, 255, 255} };

DoomFire::DoomFire()
{
    this->window = nullptr;
    this->render = nullptr;
    this->isRunning = false;
    this->fireTexture = nullptr;
    this->win_width = 0;
    this->win_height = 0;
    this->pixel_intesity = 0;
    this->fire_pixels = 0;
}

DoomFire::~DoomFire()
{
    
}

bool DoomFire::GetIsRunning() const
{
    return this->isRunning;
}

void DoomFire::Initialize(int posx, int posy, int win_width, int win_height)
{
    this->win_height = win_height;
    this->win_width = win_width;

    srand(time(NULL));

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    this->window = SDL_CreateWindow(
        "DOOM Fire",
        posx,
        posy,
        this->win_width,
        this->win_height,
        SDL_WINDOW_SHOWN);

    if (!this->window) {
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    this->render = SDL_CreateRenderer(this->window, -1, 0);

    if (!this->render) {
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_SetRenderDrawColor(this->render, 255, 255, 255, 255);

    std::cout << "Initialized SDL2!" << std::endl;
    this->isRunning = true;

}

void DoomFire::HandleEvents()
{
    SDL_Event event;

    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        this->isRunning = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            this->isRunning = false;
        break;
    default:
        break;
    }
}

void DoomFire::Destroy()
{
    delete[] fire_pixels;
    delete[] pixel_intesity;
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->render);
    SDL_DestroyTexture(this->fireTexture);
    SDL_Quit();
}

void DoomFire::UpdateFire()
{
    CalculateFirePropagation();
    SDL_UpdateTexture(this->fireTexture, NULL, this->fire_pixels, this->win_width * sizeof(Uint32));
}

void DoomFire::Render()
{
    SDL_RenderClear(this->render);
    SDL_RenderCopy(this->render, this->fireTexture, NULL, NULL);
    SDL_RenderPresent(this->render);
}

void DoomFire::InitFirePixels()
{
    if (fireTexture == nullptr) {
        this->fireTexture = SDL_CreateTexture(this->render,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STATIC,
            this->win_width,
            this->win_height
        );
    }

    int pixels_alloc = sizeof(Uint32) * this->win_height * this->win_width;

    this->fire_pixels = (Uint32*)malloc(pixels_alloc);
    this->pixel_intesity = (Uint32*)malloc(pixels_alloc);

    memset(this->fire_pixels, 0, pixels_alloc);
    memset(this->pixel_intesity, 0, pixels_alloc);

    CreateFireSource();
}

void DoomFire::CreateFireSource()
{
    for (int i = 0; i < this->win_width; i++) {
        int indexPixel = this->win_width * this->win_height - this->win_width + i;        
        this->pixel_intesity[indexPixel] = 36;
    }
}

void DoomFire::CalculateFirePropagation()
{
    for (int i = 0; i < this->win_width; i++) {
        for (int j = 0; j < this->win_height; j++) {       
            int pixelIndex = (this->win_width * j) + i;
            UpdateFireIntesityPerPixel(pixelIndex);            
            
            int indexIntensity = this->pixel_intesity[pixelIndex];
            Uint8 r = fireColorsPalette[indexIntensity][0];
            Uint8 g = fireColorsPalette[indexIntensity][1];
            Uint8 b = fireColorsPalette[indexIntensity][2];
            Uint32 fire_val = (0xff << 24) + (r << 16) + (g << 8) + b;      
            
            this->fire_pixels[pixelIndex] = fire_val;
        }
    }

}

void DoomFire::UpdateFireIntesityPerPixel(int currentPixelIndex)
{
    int belowPixel = currentPixelIndex + this->win_width;

    if (belowPixel > (this->win_height * this->win_width)) {
        return;
    }

    int decay = floor(rand() % 3);
    int belowPixelIntensity = this->pixel_intesity[belowPixel];
    Uint32 newFireIntensity = (belowPixelIntensity - decay <= 0) ? 0 : belowPixelIntensity - decay;
    this->pixel_intesity[currentPixelIndex] = newFireIntensity;
}

