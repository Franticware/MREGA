#include "anytk.h"

#include "dosutil.h"
#include "doskeyb.h"

#include <cstring>
#include <ctime>
#include <cstdio>

bool g_quit = false;

using std::printf;
using std::fflush;

//using namespace std;

#define mul320(value) (((value) << 8) + ((value) << 6))
#define FRAMES 100

uint8_t frcnt = 0; // t
uint8_t flipFlag = 0;

uint32_t fpsTimer = 0;

uint64_t hsec = 0;

void timer_dummy(void)
{
}

void timer_smart(void)
{
    ++hsec;
    ++frcnt;
    if (frcnt == 5)
    {
        frcnt = 0;
        flipFlag = 1;
    }
}

void (*timer_fun) (void) = timer_dummy;

void __interrupt __far timer_int(void)
{
    timer_fun();
    outp(0x20,0x20);
}

void setVgaPalette(const uint8_t* pal)
{
    int i;
    outp(0x03c8, 16);
    for (i = 0; i < (768 - 16*3); ++i)
    {
        outp(0x03c9, pal[i] >> 2);
    }
}

void initTk()
{
    set_mode(VGA_256_COLOR_MODE);       /* set the video mode. */

    keyb_hook_int();

    timer_fun = timer_smart;
    timer_init(315000000/22/12/FRAMES, timer_int);

    fpsTimer = hsec / 100;
}

void deinitTk()
{
    timer_quit();
    set_mode(TEXT_MODE);                /* set the video mode back to
                                         text mode. */
    keyb_unhook_int();
}

int getKeyEvent(uint8_t* keydown, uint8_t* code)
{
    return keyb_get(keydown, code);
}

int getKeyState(uint8_t keyCode)
{
    return keyb_state_int[keyCode];
}

void processEvents()
{
    // do nothing
}


int fpsCounter = 0;
int fps = 0;

void flip(const uint8_t* frb)
{
    while (!flipFlag); // frame limiter

    flipFlag = 0;

    while ((inp(INPUT_STATUS_1) & VRETRACE));
    while (!(inp(INPUT_STATUS_1) & VRETRACE));

#define SHOW_FPS 0
#if SHOW_FPS
    for (int i = 0; i != 8; ++i)
    {
        std::memcpy(VGA + WIDTH * i + 24, frb + WIDTH * i + 24, WIDTH - 24);
    }
    std::memcpy(VGA + WIDTH * 8, frb + WIDTH * 8, WIDTH*HEIGHT - WIDTH * 8);
#else
    std::memcpy(VGA, frb, WIDTH*HEIGHT);
#endif

    //




    uint64_t fpsTimerNext = hsec / 100;

    ++fpsCounter;

    if (fpsTimer != fpsTimerNext)
    {
        fps = fpsCounter;
        fpsCounter = 0;

        fpsTimer = fpsTimerNext;
    }

#if SHOW_FPS
    printf("\r%d ", fps);
    fflush(stdout);
#endif

}

bool getQuit()
{
    return g_quit;
}

void setQuit(bool q)
{
    g_quit = q;
}
