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
    void setBlendMode(SDL_BlendMode blend_mode);
    void setAlpha(Uint8 alpha);

    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

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

LTexture gArrow;

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

bool LTexture::loadFromFile(std::string path)
{
    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load("./arrow.png");

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

void LTexture::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::setBlendMode(SDL_BlendMode blend_mode)
{
    SDL_SetTextureBlendMode(mTexture, blend_mode);
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
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
                    std::cout << "SDL_Image could not initialized" << std::endl;
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

    if (!gArrow.loadFromFile("./arrow.png"))
    {
        std::cout << "Image could not be loaded" << std::endl;
        success = false;
    }

    return success;
}

void close()
{
    gArrow.free();

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

            double degrees = 0;

            SDL_RendererFlip flipType = SDL_FLIP_NONE;

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
                        case SDLK_a:
                            degrees -= 60;
                            break;
                        case SDLK_d:
                            degrees += 60;
                            break;
                        case SDLK_q:
                            flipType = SDL_FLIP_HORIZONTAL;
                            break;
                        case SDLK_w:
                            flipType = SDL_FLIP_NONE;
                            break;
                        case SDLK_e:
                            flipType = SDL_FLIP_VERTICAL;
                            break;
                        }
                    }
                }

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                gArrow.render((SCREEN_WIDTH - gArrow.getWidth()) / 2, (SCREEN_HEIGHT - gArrow.getHeight()) / 2, NULL, degrees, NULL, flipType);

                SDL_RenderPresent(gRenderer);
            }
        }
    }

    close();

    return 0;
}
