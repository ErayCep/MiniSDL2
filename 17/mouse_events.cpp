#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPROTE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

class LTexture
{
public:
    LTexture();
    ~LTexture();

    bool loadFromFile(std::string path);

    void free();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setAlpha(Uint8 alpha);
    void setBlendmode(SDL_BlendMode blend_mode);

    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth();
    int getHeight();

private:
    SDL_Texture *mTexture;

    int mWidth;
    int mHeight;
};

class Button
{
public:
    Button();

    void handleEvent(SDL_Event *e);
    void render();
    void setPosition(int x, int y);

private:
    SDL_Point position;
    LButtonSprite currentSprite;
};

bool init();
bool loadMedia();
void close();

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Rect spriteClips[BUTTON_SPRITE_TOTAL];
LTexture buttonSprite;

Button buttons[TOTAL_BUTTONS];

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
    free();

    SDL_Texture *newTexture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image from file" << std::endl;
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

void LTexture::setBlendmode(SDL_BlendMode blend_mode)
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

    SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

Button::Button()
{
    position.x = 0;
    position.y = 0;

    currentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void Button::handleEvent(SDL_Event *e)
{
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        bool inside = true;

        if (x < position.x)
        {
            inside = false;
        }
        else if (x > position.x + BUTTON_WIDTH)
        {
            inside = false;
        }
        else if (y < position.y)
        {
            inside = false;
        }
        else if (y > position.y + BUTTON_HEIGHT)
        {
            inside = false;
        }

        if (!inside)
        {
            currentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        else
        {
            switch (e->type)
            {
            case SDL_MOUSEMOTION:
                currentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;
            case SDL_MOUSEBUTTONDOWN:
                currentSprite = BUTTON_SPROTE_MOUSE_DOWN;
                break;
            case SDL_MOUSEBUTTONUP:
                currentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}

void Button::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
}

void Button::render()
{
    buttonSprite.render(position.x, position.y, &spriteClips[currentSprite]);
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

    if (!buttonSprite.loadFromFile("./button.png"))
    {
        std::cout << "Button sprite could not be loaded" << std::endl;
        success = false;
    }
    else
    {
        for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
        {
            spriteClips[i].x = 0;
            spriteClips[i].y = i * 200;
            spriteClips[i].w = BUTTON_WIDTH;
            spriteClips[i].h = BUTTON_HEIGHT;
        }

        buttons[0].setPosition(0, 0);
        buttons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
        buttons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
        buttons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
    }

    return success;
}

void close()
{
    buttonSprite.free();

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
            std::cout << "Unable to load image" << std::endl;
        }
        else
        {
            bool quit = false;
            SDL_Event e;

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }

                    for (int i = 0; i < TOTAL_BUTTONS; i++)
                    {
                        buttons[i].handleEvent(&e);
                    }
                }

                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(renderer);

                for (int i = 0; i < TOTAL_BUTTONS; i++)
                {
                    buttons[i].render();
                }

                SDL_RenderPresent(renderer);
            }
        }
    }

    close();

    return 0;
}
