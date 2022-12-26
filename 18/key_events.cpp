#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Texture
{
public:
    Texture();
    ~Texture();

    bool loadFromFile(std::string path);

    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void free();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setAlpha(Uint8 alpha);
    void setBlendMode(SDL_BlendMode blend_mode);

    int getWidth();
    int getHeight();

private:
    SDL_Texture *texture = NULL;
    int tWidth;
    int tHeight;
};

bool init();
bool loadMedia();
void close();

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

Texture pressTexture;
Texture upTexture;
Texture downTexture;
Texture leftTexture;
Texture rightTexture;

Texture::Texture()
{
    texture = NULL;
    tWidth = 0;
    tHeight = 0;
}

Texture::~Texture()
{
    free();
}

bool Texture::loadFromFile(std::string path)
{
    free();

    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cout << "Surface could not be created" << std::endl;
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            std::cout << "Texture could not be created" << std::endl;
        }
        else
        {
            tWidth = loadedSurface->w;
            tHeight = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
    }

    texture = newTexture;
    return texture != NULL;
}

void Texture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, tWidth, tHeight};

    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetTextureColorMod(texture, red, green, blue);
}

void Texture::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(texture, alpha);
}

void Texture::setBlendMode(SDL_BlendMode blend_mode)
{
    SDL_SetTextureBlendMode(texture, blend_mode);
}

void Texture::free()
{
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
        tWidth = 0;
        tHeight = 0;
    }
}

int Texture::getWidth()
{
    return tWidth;
}

int Texture::getHeight()
{
    return tHeight;
}

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialized" << std::endl;
        success = false;
    }
    else
    {
        window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            std::cout << "Window could not be created" << std::endl;
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                std::cout << "Renderer could not be created" << std::endl;
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    std::cout << "SDL_image could not initialized" << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    bool success = true;

    if (!pressTexture.loadFromFile("./press.png"))
    {
        std::cout << "Unable to load press image" << std::endl;
        success = false;
    }

    if (!upTexture.loadFromFile("./up.png"))
    {
        std::cout << "Unable to load up image" << std::endl;
        success = false;
    }

    if (!downTexture.loadFromFile("./down.png"))
    {
        std::cout << "Unable to load down image" << std::endl;
        success = false;
    }

    if (!leftTexture.loadFromFile("./left.png"))
    {
        std::cout << "Unable to load left image" << std::endl;
        success = false;
    }

    if (!rightTexture.loadFromFile("./right.png"))
    {
        std::cout << "Unable to load right image" << std::endl;
        success = false;
    }

    return success;
}

void close()
{
    pressTexture.free();
    upTexture.free();
    downTexture.free();
    rightTexture.free();
    leftTexture.free();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    renderer = NULL;
    window = NULL;

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
            std::cout << "Unable to load images" << std::endl;
        }
        else
        {
            bool quit = false;
            SDL_Event e;
            Texture *currentTexture = NULL;

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }

                const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
                if (currentKeyStates[SDL_SCANCODE_UP])
                {
                    currentTexture = &upTexture;
                }

                else if (currentKeyStates[SDL_SCANCODE_DOWN])
                {
                    currentTexture = &downTexture;
                }

                else if (currentKeyStates[SDL_SCANCODE_RIGHT])
                {
                    currentTexture = &rightTexture;
                }

                else if (currentKeyStates[SDL_SCANCODE_LEFT])
                {
                    currentTexture = &leftTexture;
                }

                else
                {
                    currentTexture = &pressTexture;
                }

                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);

                currentTexture->render(0, 0);

                SDL_RenderPresent(renderer);
            }
        }
    }

    close();

    return 0;
}
