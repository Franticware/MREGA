#ifndef DOSUTIL_H
#define DOSUTIL_H

#include <dos.h> // REGS
#include <conio.h> // inp outp
#include <stdint.h> // uint8_t

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define NUM_COLORS          256       /* number of colors in mode 0x13 */

#define INPUT_STATUS_1      0x03da
#define VRETRACE            0x08

#define VSYNC_ENABLE 1

#define LC 34
#define LF 20
#define RC 36
#define RF 23
#define MD 57

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

extern uint8_t* VGA;

inline void set_mode(byte mode)
{
    union REGS regs;

    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int386(VIDEO_INT, &regs, &regs);
}

void timer_init(uint16_t count, void (__interrupt __far *timer_new_int) ());

void timer_quit (void);

const char* buildYearStr(void);

void setVgaPaletteFrom16(const uint8_t* pal);

#endif // DOSUTIL_H
