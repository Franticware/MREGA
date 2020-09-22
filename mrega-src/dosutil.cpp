#include "dosutil.h"

#include <string.h>

uint8_t* VGA=(uint8_t*)0xA0000L;

#define TIMERINT 8

void (__interrupt __far *timer_old_int) ();

void timer_set(uint16_t prm)
{
    if (prm > 0 && prm < 150)
        return;

    outp(0x43,0x36);
    outp(0x40,prm);
    outp(0x40,prm >> 8);
}

void timer_init(uint16_t count, void (__interrupt __far *timer_new_int) ())
{
    timer_old_int = _dos_getvect(TIMERINT);

    _dos_setvect (0x8000 | TIMERINT, timer_new_int);
    timer_set(count);
}

void timer_quit (void)
{
    if (timer_old_int)
    {
        timer_set(0);
        _dos_setvect(TIMERINT, timer_old_int);
    }
}


const char* BUILD_DATE_STR = __DATE__;

const char* buildYearStr(void)
{
    return BUILD_DATE_STR + (strlen(BUILD_DATE_STR) - 4);
}

void setVgaPaletteFrom16(const uint8_t* pal)
{
    int i;
    outp(0x03c8, 16);
    for (i = 0; i < (768 - 16*3); ++i)
    {
        outp(0x03c9, pal[i] >> 2);
    }

}
