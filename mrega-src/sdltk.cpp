#ifdef USESDL

#include "anytk.h"

#ifndef WIDTH
#define WIDTH 320
#endif

#ifndef HEIGHT
#define HEIGHT 200
#endif

#include <stdint.h>
#include <SDL.h>
#include <cstring>

bool g_quit = false;

const uint32_t vga16[16]
{
    0xff000000,
    0xff0000aa,
    0xff00aa00,
    0xff00aaaa,
    0xffaa0000,
    0xffaa00aa,
    0xffaa5500,
    0xffaaaaaa, // k

    0xff555555,
    0xff5555ff,
    0xff55ff55,
    0xff55ffff,
    0xffff5555,
    0xffff55ff,
    0xffffff55,
    0xffffffff,
};


uint32_t pixels[WIDTH*HEIGHT];
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

uint32_t frameTicker = 0;

#define FRAME_MS 50

SDL_AudioDeviceID audioDevice = 0;

void SDLCALL oplEmulationCallback(void *userdata, Uint8 * stream, int len);
void initOplEmulation();

void initAudio()
{
    SDL_AudioSpec want, have;


    SDL_memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
    want.freq = 48000;
    want.format = AUDIO_S16LSB;
    want.channels = 2;
    want.samples = 512;
    want.callback = oplEmulationCallback; /* you wrote this function elsewhere -- see SDL_AudioSpec for details */

    audioDevice = SDL_OpenAudioDevice(0, 0, &want, &have, 0);

    initOplEmulation();

}

void getBuildDate(char bd[9]);

void initTk()
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);

    char windowTitle[32] = "MREGA - Build ";
    getBuildDate(windowTitle + strlen(windowTitle));

    window = SDL_CreateWindow(windowTitle,
        //SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    initAudio();



    frameTicker = SDL_GetTicks();
}

void deinitTk()
{
    if (audioDevice)
    {
        SDL_CloseAudioDevice(audioDevice);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();
}


unsigned char keyb_state_int[KEYB_SIZE] = {0};

typedef struct QueueT {
    uint8_t begin;
    uint8_t end;
    uint16_t data[0x100];
} Queue;

#define QueueNotEmpty(q) ((q).begin != (q).end)
#define QueueGet(q) ((q).data[(q).begin++])
#define QueuePut(q, d) do { (q).data[(q).end++] = (d); } while (0)

Queue keyb_queue = { /*.begin =*/ 0, /*.end =*/ 0, {0} };

int keyb_get(uint8_t* keydown, uint8_t* code)
{
    if (QueueNotEmpty(keyb_queue))
    {
        uint16_t v = QueueGet(keyb_queue);
        *keydown = (uint8_t)(v >> 8);
        *code = (uint8_t)v;
        return 1;
    }
    else
    {
        return 0;
    }
}

int getKeyEvent(uint8_t* keydown, uint8_t* code)
{
    return keyb_get(keydown, code);
}

int getKeyState(uint8_t keyCode)
{
    return keyb_state_int[keyCode];
}


uint8_t keycodeToDos(SDL_Keycode sym)
{
    switch (sym)
    {
    case SDLK_UP:
        return K_UP;
    case SDLK_DOWN:
        return K_DOWN;
    case SDLK_LEFT:
        return K_LEFT;
    case SDLK_RIGHT:
        return K_RIGHT;
    case SDLK_RCTRL:
        return K_RCTRL;
    case SDLK_LCTRL:
        return K_LCTRL;
    case SDLK_RSHIFT:
        return K_RSHIFT;
    case SDLK_LSHIFT:
        return K_LSHIFT;
    case SDLK_ESCAPE:
        return K_ESC;
    case SDLK_RETURN:
        return K_ENTER;
    case SDLK_a:
        return K_A;
    case SDLK_s:
        return K_S;
    case SDLK_w:
        return K_W;
    case SDLK_d:
        return K_D;
    default:
        return 0;
    }
}

void processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            g_quit = true;
            break;
        case SDL_KEYDOWN:
            {
                uint8_t k = keycodeToDos(event.key.keysym.sym);
                if (k != 0 && keyb_state_int[k] != 1)
                {
                    keyb_state_int[k] = 1;
                    QueuePut(keyb_queue, k | 0x100);
                }
                break;
            }
        case SDL_KEYUP:
            {
                uint8_t k = keycodeToDos(event.key.keysym.sym);
                if (k != 0 && keyb_state_int[k] != 0)
                {
                    keyb_state_int[k] = 0;
                    QueuePut(keyb_queue, k | 0x000);
                }
                break;
            }
        }
    }
}

void flip(const uint8_t* frb)
{
    for (int i = 0; i != WIDTH * HEIGHT; ++i)
        pixels[i] = vga16[frb[i] & 0xf];

    for (;;)
    {
        //break;
        uint32_t currentTick = SDL_GetTicks();
        if (currentTick - frameTicker >= FRAME_MS)
        {
            frameTicker += FRAME_MS;
            break;
        }
        // wait here
        SDL_Delay(5);
    }

    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

}

bool getQuit()
{
    return g_quit;
}

void setQuit(bool q)
{
    g_quit = q;
}

#endif
