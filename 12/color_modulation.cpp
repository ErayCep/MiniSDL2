#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture
{
public:
    LTexture();

    ~LTexture();

    bool loadFromFile(std::string path);

    void free();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    void render(int x, int y, SDL_Rect *clip = NULL);

    int getWidth();
    int getHeight();

private:
    SDL_Texture *mTexture;
    int mWidth;
    int mHeight;
};

bool init();
bool loadMedia();
void close();

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

LTexture modulatedTexture;

LTexture::LTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    free();
}

void LTexture::free()
{
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);

        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

bool LTexture::loadFromFile(std::string path)
{
    free();

    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image" << std::endl;
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            std::cout << "Texture could not be created" << std::endl;
        }
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

void LTexture::render(int x, int y, SDL_Rect *clip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
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
        gWindow = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            std::cout << "Window could not be created" << std::endl;
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                std::cout << "Renderer could not be created" << std::endl;
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    std::cout << "Image could not initialized" << std::endl;
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

    if (!modulatedTexture.loadFromFile("./colors.png"))
    {
        std::cout << "Unable to load file" << std::endl;
        success = false;
    }

    return success;
}

void close()
{
    modulatedTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gRenderer = NULL;
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
            std::cout << "Unable to load image" << std::endl;
        }
        else
        {
            bool quit = false;

            SDL_Event e;

            Uint8 r = 0;
            Uint8 g = 0;
            Uint8 b = 0;

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if (e.type == SDL_KEYDOWN)
                    {
                        switch (e.key.keysym.sym)
                        {
                        case SDLK_q:
                            r += 32;
                            break;

                        case SDLK_w:
                            g += 32;
                            break;

                        case SDLK_e:
                            b += 32;
                            break;

                        case SDLK_a:
                            r -= 32;
                            break;

                        case SDLK_s:
                            g -= 32;
                            break;

                        case SDLK_d:
                            b -= 32;
                            break;
                        }
                    }
                }

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                modulatedTexture.setColor(r, g, b);
                modulatedTexture.render(0, 0);

                SDL_RenderPresent(gRenderer);
            }
        }
    }

    close();

    return 0;
}
