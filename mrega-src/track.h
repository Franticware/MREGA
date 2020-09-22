#ifndef TRACK_H
#define TRACK_H

#include <stdint.h>

#define SGM_S 0
#define SGM_T 1

#define TEX_START 0
#define TEX_DASHED 1
#define TEX_TURN 2
#define TEX_FINISH 3

#define CL_BLACK 0
#define CL_DBLUE 1
#define CL_DGREEN 2
#define CL_TEAL 3
#define CL_DRED 4
#define CL_VIOLET 5
#define CL_BROWN 6
#define CL_LGRAY 7

#define CL_DGRAY 8
#define CL_LBLUE 9
#define CL_LGREEN 10
#define CL_AZURE 11
#define CL_LRED 12
#define CL_PINK 13
#define CL_YELLOW 14
#define CL_WHITE 15

struct TrSegm
{
    uint8_t type; // SGM_*
    int16_t la; // length/angle
    uint8_t tex; // TEX_*
    uint8_t cl, cr; // color left, color right
};

#define TLI_CHECKL 0 // checkered white at left
#define TLI_CHECKR 1 // at right
#define TLI_PLAIN 2 // black road
#define TLI_LINE 3  // line in the middle
#define TLI_TURNW 4 // white
#define TLI_TURNL 5 // light
#define TLI_TURND 6 // dark
#define TLI_SIZE 7

#endif // TRACK_H
