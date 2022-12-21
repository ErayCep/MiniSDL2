#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <string>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();

SDL_Surface *loadSurface(std::string path);
SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gPNGSurface = NULL;

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialized! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            std::cout << "Window could not created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                std::cout << "SDL_image could initialized! SDL_image Error: " << IMG_GetError() << std::endl;
                success = false;
            }
            else
            {
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
        }
    }
    return success;
}

SDL_Surface *loadSurface(std::string path)
{
    SDL_Surface *optimizedSurface = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image" << std::endl;
    }
    else
    {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL)
        {
            std::cout << "Screen could not optimized!" << std::endl;
        }

        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;
}

bool loadMedia()
{
    bool success = true;

    gPNGSurface = loadSurface("./loaded.png");

    if (gPNGSurface == NULL)
    {
        std::cout << "Failed to load image" << std::endl;
    }

    return success;
}

void close()
{
    SDL_FreeSurface(gPNGSurface);
    gPNGSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char const *argv[])
{
    if (!init())
    {
        std::cout << "SDL could not initialized" << std::endl;
    }
    else
    {
        if (!loadMedia())
        {
            std::cout << "Image coult not loaded" << std::endl;
        }
        else
        {
            bool quit = false;

            SDL_Event e;

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT or e.type == SDLK_ESCAPE)
                    {
                        quit = true;
                    }
                }
                SDL_BlitSurface(gPNGSurface, NULL, gScreenSurface, NULL);

                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

    close();

    return 0;
}
