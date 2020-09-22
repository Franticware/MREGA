#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <stdint.h>
#include <vector>
#include <cstring>

#include "real.h"
#include "track.h"
#include "spritedata.h"

#include <time.h>
#include <cstdio>

#ifndef USESDL
using std::memset;
using std::printf;
using std::fflush;
using std::fputs;
#endif

const TrSegm suburbSegm[] = {
    {SGM_S, 1, TEX_START, CL_LGRAY, 0},

    {SGM_S, 92, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, 40, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 53, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 60, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 55, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, -15, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 34, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 30, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 63, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 90, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 56, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, 90, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 77, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, -30, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 28, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, -90, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 24, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, -20, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 34, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, -65, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 12, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, -50, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 21, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 40, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 15, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 30, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 28, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 60, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 18, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 30, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 67, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, 90, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 40, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, -50, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 40, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, 30, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 18, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, 30, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 28, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, 60, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 43, TEX_DASHED, CL_LGRAY, 0},
    {SGM_S, 1, TEX_FINISH, CL_LGRAY, 0},

};

const TrSegm boulSegm[] = {
    {SGM_S, 1, TEX_START, CL_DGREEN, 0},

    {SGM_S, 37, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, -60, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 41, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 50, TEX_TURN, CL_DGREEN, 0},


    {SGM_S, 16, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, 55, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 22, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, 45, TEX_TURN, CL_LGRAY, 0},


    {SGM_S, 59, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 85, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 57, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, -70, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 63, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, 90, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 28, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, 35, TEX_TURN, CL_LGRAY, 0},

    {SGM_S, 25, TEX_DASHED, CL_LGRAY, 0},
    {SGM_T, -35, TEX_TURN, CL_LGRAY, 0},


    {SGM_S, 29, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 70, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 28, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, -70, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 19, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 80, TEX_TURN, CL_DGREEN, 0},


    {SGM_S, 31, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 90, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 40, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, -5, TEX_TURN, CL_DGREEN, 0},


    {SGM_S, 30, TEX_DASHED, CL_DGREEN, 0},
    {SGM_S, 1, TEX_FINISH, CL_DGREEN, 0},
};


const TrSegm ironSegm[] = {
    {SGM_S, 1, TEX_START, CL_BROWN, 0},

    {SGM_S, 38, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 45, TEX_TURN, CL_LGREEN, 0},


    {SGM_S, 48, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 45, TEX_TURN, CL_BROWN, 0},

    {SGM_S, 18, TEX_DASHED, CL_BROWN, 0},
    {SGM_T, 45, TEX_TURN, CL_BROWN, 0},

    {SGM_S, 23, TEX_DASHED, CL_BROWN, 0},
    {SGM_T, 45, TEX_TURN, CL_BROWN, 0},

    {SGM_S, 85, TEX_DASHED, CL_BROWN, 0},
    {SGM_T, -45, TEX_TURN, CL_BROWN, 0},

    {SGM_S, 12, TEX_DASHED, CL_BROWN, 0},
    {SGM_T, -45, TEX_TURN, CL_BROWN, 0},

    {SGM_S, 22, TEX_DASHED, CL_BROWN, 0},
    {SGM_T, 90, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 53, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, -10, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 24, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 20, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 24, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, -10, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 31, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 45, TEX_TURN, CL_LGREEN, 0},

    {SGM_S, 23, TEX_DASHED, CL_LGREEN, 0},
    {SGM_T, 50, TEX_TURN, CL_LGREEN, 0},


    {SGM_S, 34, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, -40, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 44, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 30, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 43, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 45, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 88, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 15, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 26, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 20, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 47, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 90, TEX_TURN, CL_DGREEN, 0},

    {SGM_S, 27, TEX_DASHED, CL_DGREEN, 0},
    {SGM_T, 35, TEX_TURN, CL_BROWN, 0},


    {SGM_S, 28, TEX_DASHED, CL_BROWN, 0},
    {SGM_T, -60, TEX_TURN, CL_BROWN, 0},

    {SGM_S, 25, TEX_DASHED, CL_BROWN, 0},
    {SGM_T, 40, TEX_TURN, CL_BROWN, 0},

    {SGM_S, 31, TEX_DASHED, CL_BROWN, 0},
    {SGM_T, -45, TEX_TURN, CL_BROWN, 0},

    {SGM_S, 25, TEX_DASHED, CL_BROWN, 0},
    {SGM_T, -45, TEX_TURN, CL_BROWN, 0},

    {SGM_S, 38, TEX_DASHED, CL_BROWN, 0},
    {SGM_S, 1, TEX_FINISH, CL_BROWN, 0},
};

// MSB -> |CL|CR|OFF|TLI| <- LSB
// MSB -> |CL|ANG|OFF|TLI| <- LSB
//std::vector<uint32_t> track;

struct Track {
    Track()
    {
        track = 0;
        size = 0;
    }
    ~Track()
    {
        delete[] track;
    }
    uint32_t* track;
    uint32_t size;
};

const uint16_t suburbSegmSize = sizeof(suburbSegm)/sizeof(TrSegm);
Track suburbTrack;
/*uint32_t* suburbTrack;
uint32_t suburbTrackSize;*/


const uint16_t boulSegmSize = sizeof(boulSegm)/sizeof(TrSegm);
Track boulTrack;
/*uint32_t* boulTrack;
uint32_t boulTrackSize;*/

const uint16_t ironSegmSize = sizeof(ironSegm)/sizeof(TrSegm);
Track ironTrack;
/*uint32_t* ironTrack;
uint32_t ironTrackSize;*/

uint32_t* track = 0;
uint32_t trackSize = 0;

int generateTrack(uint32_t* track, uint32_t* trackSize, const TrSegm* segm, const uint16_t segmSize)
{
    const int angleDiv = 5;

    //track.clear();

    if (segmSize < 1)
        return __LINE__;

    int angleSum = 0;

    for (uint16_t i = 0; i != segmSize; ++i)
    {
        if (segm[i].type == SGM_T)
        {
            if (segm[i].la < -180 || segm[i].la > 180)
                return __LINE__;

            angleSum += segm[i].la;

            if (((segm[i].la + 360) % angleDiv) != 0)
                return __LINE__;
        }
    }

    while (angleSum < 0)
        angleSum += 360;

    if ((angleSum % 360) != 0)
        return __LINE__;

    uint32_t trackAngle = 720; // div 5

    for (uint16_t i = 0; i != segmSize; ++i)
    {
        uint16_t prevI = (i + segmSize - 1) % segmSize;
        uint16_t nextI = (i + 1) % segmSize;

        uint16_t neI[2] = {prevI, nextI};

        int segmLen = 0;
        int32_t offset = 0;

        if (segm[i].type == SGM_T)
        {
            if (segm[i].la < 0)
            {
                segmLen = segm[i].la / -angleDiv;
                offset = 1;
            }
            else
            {
                segmLen = segm[i].la / angleDiv;
                offset = -1;
            }
        }
        else if (segm[i].type == SGM_S)
        {
            segmLen = segm[i].la;
            for (int j = 0; j != 2; ++j)
            {
                if (segm[neI[j]].type == SGM_T)
                {
                    int turnLen = segm[i].la / angleDiv;
                    if (turnLen < 0) turnLen = -turnLen;
                    segmLen -= turnLen;
                    if (segmLen < 0) segmLen = 0;
                }
            }
        }

        uint32_t uoff = offset;

        uoff = (uoff & 0xff) << 8;

        uint32_t ucl = segm[i].cl;
        uint32_t ucr = 0; //segm[i].cr;

        ucl = ucl << 24;
        ucr = ucr << 16;

        for (int j = 0; j < segmLen; ++j)
        {
            uint32_t item = 0;
            switch (segm[i].tex)
            {
            case TEX_START:
                item |= TLI_CHECKL;
                break;
            case TEX_DASHED:
                if ((j % 6) < 3)
                {
                    item |= TLI_PLAIN;
                }
                else
                {
                    item |= TLI_LINE;
                }
                break;
            case TEX_TURN:
                switch (j % 4)
                {
                case 0:
                    item |= TLI_TURNL;
                    break;
                case 1:
                case 3:
                    item |= TLI_TURNW;
                    break;
                case 2:
                    item |= TLI_TURND;
                    break;
                }
                break;
            case TEX_FINISH:
                item |= TLI_CHECKR;
                break;
            }

            uint32_t trackAngleTmp = trackAngle % 72;
            trackAngleTmp = trackAngleTmp << 16;

            item |= trackAngleTmp;

            item |= uoff;
            item |= ucl;
            //item |= ucr;

            //track.push_back(item);
            if (track)
            {
                track[*trackSize] = item;
            }
            ++(*trackSize);
            trackAngle += offset;
        }
    }
    return 0;
}


// 1m = 64pix

//#define TLI_TEST TLI_CHECKR

#define ROAD_SIZE (8*64)
uint8_t tlis[ROAD_SIZE * TLI_SIZE]; // base texture lines

void generateTlis()
{
    memset(tlis, 0x8, ROAD_SIZE * TLI_SIZE);

    for (int i = 0; i != 16; ++i)
    {
        tlis[ROAD_SIZE * TLI_PLAIN + i + 16] = 0xf;
        tlis[ROAD_SIZE * TLI_PLAIN + ROAD_SIZE-1-(i + 16)] = 0xf;
    }

    for (int i = 0; i != 16; ++i)
    {
        tlis[ROAD_SIZE * TLI_LINE + i + 16] = 0xf;
        tlis[ROAD_SIZE * TLI_LINE + ROAD_SIZE-1-(i + 16)] = 0xf;
    }
    for (int i = 0; i != 12; ++i)
    {
        tlis[ROAD_SIZE * TLI_LINE + ROAD_SIZE / 2 - i - 1] = 0xf;
        tlis[ROAD_SIZE * TLI_LINE + ROAD_SIZE / 2 + i] = 0xf;
    }

    for (int i = 0; i != 48; ++i)
    {
        tlis[ROAD_SIZE * TLI_TURNW + i] = 0xf;
        tlis[ROAD_SIZE * TLI_TURNW + ROAD_SIZE-1-i] = 0xf;

        tlis[ROAD_SIZE * TLI_TURNL + i] = 0xc;
        tlis[ROAD_SIZE * TLI_TURNL + ROAD_SIZE-1-i] = 0xc;

        tlis[ROAD_SIZE * TLI_TURND + i] = 0x4;
        tlis[ROAD_SIZE * TLI_TURND + ROAD_SIZE-1-i] = 0x4;
    }

    for (int i = 0; i != 16; ++i)
    {
        tlis[ROAD_SIZE * TLI_CHECKL + i + 16] = 0xf;
        tlis[ROAD_SIZE * TLI_CHECKL + ROAD_SIZE-1-(i + 16)] = 0xf;
        tlis[ROAD_SIZE * TLI_CHECKR + i + 16] = 0xf;
        tlis[ROAD_SIZE * TLI_CHECKR + ROAD_SIZE-1-(i + 16)] = 0xf;
    }
    for (int j = 0; j != 3; ++j)
    {
        for (int i = 0; i != 64; ++i)
        {
            tlis[ROAD_SIZE * TLI_CHECKL + i + j * 128 + 64] = 0xf;
            tlis[ROAD_SIZE * TLI_CHECKR + i + j * 128 + 128] = 0xf;
        }
    }
}


void precompute();
void initMenu();

#ifndef USESDL

#define LOADING_PROG(c) do { fputs("\rMREGA is loading - [", stdout); for (int i = 0; i < c; ++i) fputs(".", stdout); for (int i = c; i < 40; ++i) fputs(" ", stdout); fputs("] ", stdout); fflush(stdout);  } while (0)
//#define LOADING_PROG(c) do { int cl = (int)clock(); int pr = cl * 40 / 6920; printf("%s:%d=%d;%d\n", __FILE__, __LINE__, cl, pr); fflush(stdout);  } while (0)

#else

inline void LOADING_PROG(int c) { (void)(c); }

#endif

void initData()
{
    LOADING_PROG(0);
    generateTlis();
    //LOADING_PROG(0);
    generateTrack(0, &suburbTrack.size, suburbSegm, suburbSegmSize);
    suburbTrack.track = new uint32_t[suburbTrack.size];
    suburbTrack.size = 0;
    generateTrack(suburbTrack.track, &suburbTrack.size, suburbSegm, suburbSegmSize);
    //LOADING_PROG(0);
    generateTrack(0, &boulTrack.size, boulSegm, boulSegmSize);
    boulTrack.track = new uint32_t[boulTrack.size];
    boulTrack.size = 0;
    generateTrack(boulTrack.track, &boulTrack.size, boulSegm, boulSegmSize);
    //LOADING_PROG(0);
    generateTrack(0, &ironTrack.size, ironSegm, ironSegmSize);
    ironTrack.track = new uint32_t[ironTrack.size];
    ironTrack.size = 0;
    generateTrack(ironTrack.track, &ironTrack.size, ironSegm, ironSegmSize);
    //LOADING_PROG(0);
    precompute();
    //LOADING_PROG(0);
    initMenu();
    LOADING_PROG(40);
}


#endif // GAMEDATA_H
