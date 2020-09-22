#ifndef GAME_H
#define GAME_H

#include "gamedata.h"

#include "sim.h"

#include <cstring>
#include <cmath>
#include <cstdio>

#ifndef WIDTH
#define WIDTH 320
#endif

#ifndef HEIGHT
#define HEIGHT 200
#endif

using std::memset;
using std::memcpy;

#define LINES 72

const int lines = LINES;
const Real trackY = 2.5;
const Real scale = 128;
const int carLine = 64;
const Real carZpos = trackY / ((carLine+12) * 1.2) * scale;

int selTrack = 0;
int selCarA = 0;
int selCarB = 0;

inline int compU(Real z, int t, Real xpos, Real off)
{
    int ret = (t/64.0 - xpos - 4)/z*scale - off;
    return ret;
}

uint8_t color320[16][320];

Real tableZ[80];

void initTableZ()
{
    for (int v = 0; v != 80; ++v)
    {
        Real rv = ((v+12) * 1.2) / scale;
        Real z = trackY / rv;
        tableZ[v] = z;
    }
}

struct Mip
{
    Mip()
    {
        for (int i = 0; i != TLI_SIZE; ++i)
            t[i] = 0;
    }
    ~Mip()
    {
        for (int i = 0; i != TLI_SIZE; ++i)
            delete[] t[i];
    }
    uint8_t* t[TLI_SIZE];
    uint16_t len;
};

Mip mips[LINES];

void initMips()
{
    for (int i = 0; i != LINES; ++i)
    {
        mips[i].len = compU(tableZ[i], ROAD_SIZE, 0, 0) - compU(tableZ[i], 0, 0, 0);
        for (int j = 0; j != TLI_SIZE; ++j)
        {
            mips[i].t[j] = new uint8_t[mips[i].len];
            for (int k = 0; k != mips[i].len; ++k)
            {
                mips[i].t[j][k] = tlis[ROAD_SIZE * j + k * ROAD_SIZE / mips[i].len];
            }
        }
    }
}

const uint8_t CAR_SIZES[] =
{
    /*0*/ 0,9,7,
    /*1*/ 4,13,10,
    /*2*/ 8,16,13,
    /*3*/ 12,19,15,
    /*4*/ 16,22,18,
    /*5*/ 20,26,21,
    /*6*/ 24,28,23,
    /*7*/ 28,31,25,
    /*8*/ 32,34,28,
    /*9*/ 36,38,31,
    /*10*/ 40,41,34,
    /*11*/ 44,44,36,
    /*12*/ 48,48,40,
    /*13*/ 52,51,42,
    /*14*/ 56,53,44,
    /*15*/ 60,56,46,

    /*16*/ 64,60,50,

    /*17*/ 68,63,52,
    /*18*/ 72,66,55,
    /*19*/ 76,69,57,
};

struct Sprite
{
    Sprite()
    {
        pix = 0;
        run = 0;
    }
    ~Sprite()
    {
        delete[] pix;
        delete[] run;
    }
    uint8_t w, h;
    uint8_t l; // car specific - line (not very nice)
    uint8_t* pix;
    uint8_t* run; // memcpy runs - scanline, offset, start, length
    uint16_t runSize;
};

struct CarSprite
{
    Sprite p[20];
};

CarSprite cars[6];

Player playerA;
Player playerB;

inline void copyLineSafe(int w, int& inX, int& outX, int& outX2)
{
    if (outX2 > w) outX2 = w;
    if (outX < 0)
    {
        inX -= outX;
        outX = 0;
    }
}

// safe
void drawSpriteSafe(uint8_t* fb, int fbw, int fbh, const Sprite& car, int x, int y)
{
    // memcpy runs - scanline, offset, start, length

    for (int r = 0; r != car.runSize; ++r)
    {
        int outY = car.run[(r << 2) | 1] + y;
        if (outY >= 0 && outY < fbh)
        {
            int outX = car.run[(r << 2)] + x;
            int inX = car.run[(r << 2)];
            int outX2 = outX + car.run[(r << 2) | 3];

            copyLineSafe(fbw, inX, outX, outX2);

            int outLen = outX2 - outX;

            if (outLen > 0)
            {
                memcpy(fb + outX + outY * WIDTH, car.pix + inX + car.run[(r << 2) | 1] * car.w, outLen);
            }
        }
    }
}

// fast, unsafe
void drawSpriteFast(uint8_t* fb, const Sprite& car, int x, int y)
{
    // memcpy runs - scanline, offset, start, length

    for (int r = 0; r != car.runSize; ++r)
    {

        memcpy(fb + car.run[(r << 2)] + x + (car.run[(r << 2) | 1] + y) * WIDTH, car.pix + car.run[(r << 2)] + car.run[(r << 2) | 1] * car.w, car.run[(r << 2) | 3]);
    }
}

void drawRect(uint8_t* fb, const uint8_t* src, int srcW, int srcH, int x, int y)
{
    fb = fb + x + y * WIDTH;
    for (int i = 0; i != srcH; ++i)
    {
        memcpy(fb, src, srcW);
        fb = fb + WIDTH;
        src = src + srcW;
    }
}

void initialHudDraw(uint8_t* fb)
{
    drawRect(fb, hudData, hudDataW, hudDataH, 30, 156);
    drawRect(fb, hudData, hudDataW, hudDataH, 30 + 161, 156);
}

Sprite arrowLeft;
Sprite arrowRight;

Sprite carsLL[6];
Sprite carsL[6];
Sprite carsRR[6];
Sprite carsR[6];

struct SpriteSet
{
    Sprite* parts[5];
};

SpriteSet spriteSet[6];

void initSpriteSet(SpriteSet spriteSet[6])
{
    for (int i = 0; i != 6; ++i)
    {
        spriteSet[i].parts[0] = &carsLL[i];
        spriteSet[i].parts[1] = &carsL[i];
        spriteSet[i].parts[2] = &cars[i].p[16];
        spriteSet[i].parts[3] = &carsR[i];
        spriteSet[i].parts[4] = &carsRR[i];
    }
}

void initSpriteRun(Sprite& part)
{
    part.runSize = 0;
    for (int y = 0; y != part.h; ++y)
    {
        uint8_t colorPrev = 255;
        int xa = 0;
        for (int x = 0; x != part.w + 1; ++x)
        {
            // memcpy runs - scanline, offset, start, length
            uint8_t color;
            if (x == part.w)
            {
                color = 255;
            }
            else
            {
                color = part.pix[x + y * part.w];
            }

            if (color != 255 && colorPrev == 255)
                xa = x;

            if (color == 255 && colorPrev != 255)
            {
                if (x - xa > 0)
                {
                    if (part.run)
                    {
                        part.run[part.runSize * 4 + 0] = xa;
                        part.run[part.runSize * 4 + 1] = y;
                        part.run[part.runSize * 4 + 2] = 0;
                        part.run[part.runSize * 4 + 3] = x - xa;
                    }
                    ++part.runSize;
                }

            }
            colorPrev = color;
        }
    }
}


void initCarSprite(CarSprite& sprite, const int8_t* data, uint8_t colorDark, uint8_t colorLight)
{
    sprite.p[16].w = CAR_SIZES[16*3+1];
    sprite.p[16].h = CAR_SIZES[16*3+2];
    sprite.p[16].l = CAR_SIZES[16*3+0];

    sprite.p[16].pix = new uint8_t[sprite.p[16].w*sprite.p[16].h];
    for (int y = 0; y != 50; ++y)
    {
        for (int x = 0; x != 30; ++x)
        {
            uint8_t c = data[x + y * 30];

            if (c == CL_DBLUE) c = colorDark;
            else if (c == CL_LBLUE) c = colorLight;

            sprite.p[16].pix[x + y * sprite.p[16].w] = sprite.p[16].pix[59-x + y * sprite.p[16].w] = c;
        }
    }

    for (int p = 0; p != 20; ++p)
    {
        if (p != 16)
        {
            sprite.p[p].w = CAR_SIZES[p*3+1];
            sprite.p[p].h = CAR_SIZES[p*3+2];
            sprite.p[p].l = CAR_SIZES[p*3+0];

            sprite.p[p].pix = new uint8_t[sprite.p[p].w*sprite.p[p].h];

            for (int y = 0; y != sprite.p[p].h; ++y)
            {
                for (int x = 0; x != sprite.p[p].w; ++x)
                {
                    sprite.p[p].pix[x + y * sprite.p[p].w] = sprite.p[16].pix[(x*sprite.p[16].w/sprite.p[p].w) + (y*sprite.p[16].h/sprite.p[p].h) * sprite.p[16].w];
                }
            }

        }

        sprite.p[p].run = 0;
        initSpriteRun(sprite.p[p]);
        sprite.p[p].run = new uint8_t[sprite.p[p].runSize * 4];
        initSpriteRun(sprite.p[p]);
    }
}

void initSprite(Sprite& sprite, const int8_t* data, int w, int h, uint8_t colorDark = 255, uint8_t colorLight = 255)
{
    sprite.w = w;
    sprite.h = h;
    sprite.l = 0;

    sprite.pix = new uint8_t[sprite.w*sprite.h];
    for (int y = 0; y != h; ++y)
    {
        for (int x = 0; x != w; ++x)
        {
            uint8_t c = data[x + y * w];

            if (c == CL_DBLUE && colorDark != 255) c = colorDark;
            else if (c == CL_LBLUE && colorLight != 255) c = colorLight;

            sprite.pix[x + y * sprite.w] = c;
        }
    }

    sprite.run = 0;
    initSpriteRun(sprite);
    sprite.run = new uint8_t[sprite.runSize * 4];
    initSpriteRun(sprite);
}

void initSpriteHflip(Sprite& sprite, const int8_t* data, int w, int h, uint8_t colorDark = 255, uint8_t colorLight = 255)
{
    sprite.w = w;
    sprite.h = h;
    sprite.l = 0;

    sprite.pix = new uint8_t[sprite.w*sprite.h];
    for (int y = 0; y != h; ++y)
    {
        for (int x = 0; x != w; ++x)
        {
            uint8_t c = data[w - x - 1 + y * w];

            if (c == CL_DBLUE && colorDark != 255) c = colorDark;
            else if (c == CL_LBLUE && colorLight != 255) c = colorLight;

            sprite.pix[x + y * sprite.w] = c;
        }
    }

    sprite.run = 0;
    initSpriteRun(sprite);
    sprite.run = new uint8_t[sprite.runSize * 4];
    initSpriteRun(sprite);
}

void initSprites()
{
#if 0
    int pixSum = 0;
    for (int i = 0; i != 80; i += 4)
    {
        int spriteW = compU(tableZ[i], 0.82*64, 0, 0) - compU(tableZ[i], -0.83*64, 0, 0);
        int spriteH = 50 * spriteW / 60;
        pixSum += spriteW * spriteH;
#ifdef USESDL
        printf("/*%d*/ %d,%d,%d,\n", i/4, i, spriteW, spriteH);
#endif
    }
#ifdef USESDL
    printf("----\n");
    printf("%d\n", pixSum);
#endif
#endif

    //
    //LOADING_PROG(0);
    initCarSprite(cars[0], skoda0_d, CL_DRED, CL_LRED);
    LOADING_PROG(6);
    initCarSprite(cars[1], skoda0_d, CL_TEAL, CL_AZURE);
    LOADING_PROG(11);

    initCarSprite(cars[2], trabant0_d, CL_DBLUE, CL_LBLUE);
    LOADING_PROG(15);
    initCarSprite(cars[3], trabant0_d, CL_YELLOW, CL_YELLOW);
    LOADING_PROG(19);

    initCarSprite(cars[4], beetle0_d, CL_DGREEN, CL_LGREEN);
    LOADING_PROG(23);
    initCarSprite(cars[5], beetle0_d, CL_VIOLET, CL_PINK);
    //LOADING_PROG(0);

    initSprite(arrowLeft, arrowLeftData, arrowLeftW, arrowLeftH);
    initSpriteHflip(arrowRight, arrowLeftData, arrowLeftW, arrowLeftH);
    LOADING_PROG(27);

    initSprite(carsLL[0], skoda2_d, skoda2_w, skoda2_h, CL_DRED, CL_LRED);
    initSprite(carsLL[1], skoda2_d, skoda2_w, skoda2_h, CL_TEAL, CL_AZURE);
    LOADING_PROG(28);
    initSprite(carsLL[2], trabant2_d, trabant2_w, trabant2_h, CL_DBLUE, CL_LBLUE);
    initSprite(carsLL[3], trabant2_d, trabant2_w, trabant2_h, CL_YELLOW, CL_YELLOW);
    LOADING_PROG(29);
    initSprite(carsLL[4], beetle2_d, beetle2_w, beetle2_h, CL_DGREEN, CL_LGREEN);
    initSprite(carsLL[5], beetle2_d, beetle2_w, beetle2_h, CL_VIOLET, CL_PINK);
    //LOADING_PROG(0);

    initSprite(carsL[0], skoda1_d, skoda1_w, skoda1_h, CL_DRED, CL_LRED);
    initSprite(carsL[1], skoda1_d, skoda1_w, skoda1_h, CL_TEAL, CL_AZURE);
    LOADING_PROG(30);
    initSprite(carsL[2], trabant1_d, trabant1_w, trabant1_h, CL_DBLUE, CL_LBLUE);
    initSprite(carsL[3], trabant1_d, trabant1_w, trabant1_h, CL_YELLOW, CL_YELLOW);
    LOADING_PROG(31);
    initSprite(carsL[4], beetle1_d, beetle1_w, beetle1_h, CL_DGREEN, CL_LGREEN);
    initSprite(carsL[5], beetle1_d, beetle1_w, beetle1_h, CL_VIOLET, CL_PINK);
    LOADING_PROG(32);

    initSpriteHflip(carsRR[0], skoda2_d, skoda2_w, skoda2_h, CL_DRED, CL_LRED);
    initSpriteHflip(carsRR[1], skoda2_d, skoda2_w, skoda2_h, CL_TEAL, CL_AZURE);
    LOADING_PROG(33);
    initSpriteHflip(carsRR[2], trabant2_d, trabant2_w, trabant2_h, CL_DBLUE, CL_LBLUE);
    initSpriteHflip(carsRR[3], trabant2_d, trabant2_w, trabant2_h, CL_YELLOW, CL_YELLOW);
    //LOADING_PROG(0);
    initSpriteHflip(carsRR[4], beetle2_d, beetle2_w, beetle2_h, CL_DGREEN, CL_LGREEN);
    initSpriteHflip(carsRR[5], beetle2_d, beetle2_w, beetle2_h, CL_VIOLET, CL_PINK);

    LOADING_PROG(34);
    initSpriteHflip(carsR[0], skoda1_d, skoda1_w, skoda1_h, CL_DRED, CL_LRED);
    initSpriteHflip(carsR[1], skoda1_d, skoda1_w, skoda1_h, CL_TEAL, CL_AZURE);
    LOADING_PROG(35);
    initSpriteHflip(carsR[2], trabant1_d, trabant1_w, trabant1_h, CL_DBLUE, CL_LBLUE);
    initSpriteHflip(carsR[3], trabant1_d, trabant1_w, trabant1_h, CL_YELLOW, CL_YELLOW);
    LOADING_PROG(36);
    initSpriteHflip(carsR[4], beetle1_d, beetle1_w, beetle1_h, CL_DGREEN, CL_LGREEN);
    initSpriteHflip(carsR[5], beetle1_d, beetle1_w, beetle1_h, CL_VIOLET, CL_PINK);
    LOADING_PROG(37);
    initSpriteSet(spriteSet);
    //LOADING_PROG(0);
}

const uint32_t bkg1_w = bkg_boul_w;
const uint32_t bkg1_h = bkg_boul_h;

const uint32_t backg_over = 160;
const uint32_t backg_w = bkg1_w + backg_over;

const uint8_t* backg = 0;

struct Backgs
{
    Backgs()
    {
        backg0 = 0;
        backg1 = 0;
        backg2 = 0;
    }
    ~Backgs()
    {
        delete[] backg0;
        delete[] backg1;
        delete[] backg2;
    }
    uint8_t* backg0;
    uint8_t* backg1;
    uint8_t* backg2;
};

Backgs backgs;

uint8_t* initBackg(const uint8_t* bkg_d)
{
    uint8_t* backg = new uint8_t[backg_w*bkg1_h];
    for (int y = 0; y != bkg1_h; ++y)
    {
        memcpy(backg + y * backg_w, bkg_d + y * bkg1_w, bkg1_w);
        memcpy(backg + y * backg_w + bkg1_w, bkg_d + y * bkg1_w, backg_over);
    }
    return backg;
}

void initBackg()
{
    backgs.backg0 = initBackg(bkg_boul_d);
    backgs.backg1 = initBackg(bkg_sub_d);
    backgs.backg2 = initBackg(bkg_iron_d);
}

void precompute()
{
    //LOADING_PROG(0);
    initTableZ();
    //LOADING_PROG(0);
    initMips();
    LOADING_PROG(3);
    initSprites();
    //LOADING_PROG(0);
    initBackg();
    //LOADING_PROG(0);

    for (int i = 0; i != 16; ++i)
        memset(color320[i], i, 320);
    //LOADING_PROG(0);

    /*
    for (int i = 0; i <= 30; ++i)
    {
        printf("%f,",std::cos(i * M_PI / 180.0));
    }
    printf("\n");

    for (int i = 0; i <= 30; ++i)
    {
        printf("%f,",std::sin(i * M_PI / 180.0));
    }
    printf("\n");
    */
}


Real myAbs(Real v)
{
    if (v < 0)
        return -v;
    else
        return v;
}

const Real arrowThresh = 12;

void drawCars(uint8_t* fb, int fbw, int fbh, const Player& thisPlayer, const Player& otherPlayer, const SpriteSet& thisCar, const CarSprite& otherCar, const Real offset[80], uint32_t trackSize)
{
    Real relX = otherPlayer.x - thisPlayer.x;
    Real relZ = otherPlayer.z - thisPlayer.z;

    Real trackSizeR = trackSize;

    Real relZ1 = relZ + trackSizeR;
    Real relZ2 = relZ - trackSizeR;

    if (myAbs(relZ1) < myAbs(relZ))
        relZ = relZ1;

    if (myAbs(relZ2) < myAbs(relZ))
        relZ = relZ2;

    Real rv = trackY/(relZ + carZpos);
    int otherSpriteZ = rv * scale / 1.2 - 12;

    int rotI = thisPlayer.sprA;

    rotI += 2;

    if (otherSpriteZ > carLine)
    {
        //drawSpriteFast(fb, thisCar.p[16], 80 - thisCar.p[16].w / 2, 80 + carLine - thisCar.p[16].h);
        drawSpriteFast(fb, *thisCar.parts[rotI], 80 - thisCar.parts[rotI]->w / 2, 80 + carLine - thisCar.parts[rotI]->h);
    }

    if (otherSpriteZ >= 0 && otherSpriteZ < 80)
    {
        int othI = otherSpriteZ >> 2;
        int posX = relX / (relZ + carZpos) * scale - offset[otherSpriteZ];
        drawSpriteSafe(fb, fbw, fbh, otherCar.p[othI], 80 + posX - otherCar.p[othI].w / 2, 80 + otherSpriteZ - otherCar.p[othI].h);
    }

    if (otherSpriteZ <= carLine)
    {
        //drawSpriteFast(fb, thisCar.p[16], 80 - thisCar.p[16].w / 2, 80 + carLine - thisCar.p[16].h);
        drawSpriteFast(fb, *thisCar.parts[rotI], 80 - thisCar.parts[rotI]->w / 2, 80 + carLine - thisCar.parts[rotI]->h);
    }

    int lights = getLights();
    if (lights)
    {
        switch (lights)
        {
        case 1:
            drawRect(fb, lightsOffData, 32, 70, 67, 4);
            break;
        case 2:
            drawRect(fb, lightsOnData, 32, 24, 67, 4);
            drawRect(fb + 24 * WIDTH, lightsOffData + 32 * 24, 32, 70-24, 67, 4);
            break;
        case 3:
            drawRect(fb, lightsOnData, 32, 46, 67, 4);
            drawRect(fb + 46 * WIDTH, lightsOffData + 32 * 46, 32, 70-46, 67, 4);
            break;
        case 4:
            drawRect(fb, lightsOffData, 32, 46, 67, 4);
            drawRect(fb + 46 * WIDTH, lightsOnData + 32 * 46, 32, 70-46, 67, 4);
            break;

        }


    }
}

void drawProto2(uint8_t* fb, /*uint16_t fbx, Real xpos, Real zpos,*/ const Player& thisPlayer, const Player& otherPlayer, const SpriteSet& thisCar, const CarSprite& otherCar)
{
    Real xpos = thisPlayer.x;
    Real zpos = thisPlayer.z;

    Real offset[80] = {0};

    Real offsum = 0;
    Real offdir = 0;

    int offsetLines = 80;

    // compute offset from the top
    for (int _v = 0; _v != offsetLines; ++_v)
    {
        int v = offsetLines - 1 - _v;
        /*Real rv = ((v+12) * 1.2) / scale;
        Real z = trackY / rv;*/
        Real z = tableZ[v];

        int tz = z + zpos - carZpos + trackSize;
        tz = (tz) % trackSize;

        uint8_t off = (track[tz] >> 8) & 0xff;
        Real offdiradd = 1.0 / (v + 2) * 3;

        if (off == 0xff)
        {
            offdir += offdiradd;
        }
        else if (off == 1)
        {
            offdir -= offdiradd;
        }
        offsum += offdir;
        offset[v] = offsum;
    }

    Real carOffset = offset[carLine];

    for (int v = 0; v != offsetLines; ++v)
    {
        offset[v] -= carOffset;
    }

    int bkgZ = std::floor(zpos);
    Real bkgZFract = zpos - bkgZ;
    bkgZ = (bkgZ + trackSize) % trackSize;
    uint32_t bkgTrackData = track[bkgZ];

    int8_t bkgOff = (bkgTrackData >> 8) & 0xff;
    Real bkgOffReal = (int32_t)bkgOff;

    int8_t bkgAng = (bkgTrackData >> 16) & 0xff;
    Real bkgAngReal = (int32_t)bkgAng;

    /*printf("%d | %08x %d %d\n", bkgZ, bkgTrackData, bkgAng, bkgOff);
    fflush(stdout);*/

    Real bkgAngFull = bkgAngReal + bkgOffReal * bkgZFract + xpos * 0.4;

    while (bkgAngFull < 0)
        bkgAngFull += 72;

    const int32_t bkgW = bkg1_w;

    uint32_t bkgX = ((uint32_t)(std::floor(bkgAngFull/72.0*bkgW))) % bkgW;

    for (int v = 0; v != 80; ++v)
    {
        memcpy(fb + v * WIDTH, backg + bkgX + v * backg_w, 159);
    }

/*
    for (int v = 0; v != 80; ++v)
    {
        memcpy(fb + v * WIDTH, color320[CL_AZURE], 159);
    }
*/
    // scanning from the top
    for (int v = 0; v != lines; ++v)
    {
        /*Real rv = ((v+12) * 1.2) / scale;
        Real z = trackY / rv;*/

        Real z = tableZ[v];

        int tz = z + zpos - carZpos + trackSize;

        tz = (tz) % trackSize;

        int tlii = track[tz] & 0xff;

        uint8_t cl = (track[tz] >> 24) & 0xf; // surrounding color

        memcpy(fb + (v + 80) * WIDTH, color320[cl], 159);

        int u0 = compU(z, 0, xpos, offset[v]) + 79;
        int u1 = u0 + mips[v].len;
        int inX = 0;

        copyLineSafe(159, inX, u0, u1);

        int outLen = u1 - u0;

        if (outLen > 0)
        {
            memcpy(fb + u0 + (v + 80) * (WIDTH), mips[v].t[tlii] + inX, outLen);
        }
    }

    drawCars(fb, 159, 152, thisPlayer, otherPlayer, thisCar, otherCar, offset, trackSize);


    if (thisPlayer.x < -arrowThresh)
    {
        drawSpriteFast(fb, arrowRight, 120+8, 100);
    }

    if (thisPlayer.x > arrowThresh)
    {
        drawSpriteFast(fb, arrowLeft, 20-8, 100);
    }

    const Real carLength = 4; // m
    Real tmpThisZ = thisPlayer.z + carLength + trackSize;
    Real tmpOtherZ = otherPlayer.z + carLength + trackSize;

    while (tmpThisZ >= trackSize)
        tmpThisZ -= trackSize;

    while (tmpOtherZ >= trackSize)
        tmpOtherZ -= trackSize;


    int positionIndex = 1;
    if (thisPlayer.lap > otherPlayer.lap)
    {
        positionIndex = 0;
    }
    if (thisPlayer.lap == otherPlayer.lap && tmpThisZ >= tmpOtherZ)
    {
        positionIndex = 0;
    }

#ifdef USESDL
    //printf("L %d, Z %f | ", thisPlayer.lap, thisPlayer.z);
#endif

    int lapIndex = thisPlayer.lap;
    if (lapIndex < 1) lapIndex = 1;
    if (lapIndex > 3) lapIndex = 3;
    --lapIndex;

    drawRect(fb, digitData[lapIndex], digitDataW, digitDataH, 87, 156);
    drawRect(fb, digitData[positionIndex], digitDataW, digitDataH, 87, 178);

    if (thisPlayer.wins)
    {
        if (thisPlayer.wins == 1)
        {
            drawRect(fb, first_d, first_w, first_h, 0, 152);
        }
        else if (thisPlayer.wins == 2)
        {
            drawRect(fb, last_d, last_w, last_h, 0, 152);
        }
    }

}

///////////////////////////
/// slow

#if 0
void drawProto1(uint8_t* fb, uint16_t fbx, Real zpos)
{
    const uint32_t* track = boulTrack;
    const uint32_t trackSize = boulTrackSize;



    Real offset[80] = {0};

    Real offsum = 0;
    Real offdir = 0;

    // compute offset from the top
    for (int _v = 0; _v != lines; ++_v)
    {
        int v = lines - 1 - _v;
        Real rv = ((v+12) * 1.2) / scale;
        Real z = trackY / rv;

        int tz = z + zpos - carZpos + trackSize;
        tz = (tz) % trackSize;

        uint8_t off = (track[tz] >> 8) & 0xff;
        Real offdiradd = 1.0 / (v + 2) * 3;

        if (off == 0xff)
        {
            offdir += offdiradd;
        }
        else if (off == 1)
        {
            offdir -= offdiradd;
        }
        offsum += offdir;
        offset[v] = offsum;
    }

    Real carOffset = offset[carLine];

    for (int v = 0; v != lines; ++v)
    {
        offset[v] -= carOffset;
    }

    for (int v = 0; v != 80; ++v)
    {
        memset(fb + fbx + v * WIDTH, CL_AZURE, 159);
    }

    // scanning from the top
    for (int v = 0; v != lines; ++v)
    {
        Real rv = ((v+12) * 1.2) / scale;
        Real z = trackY / rv;

        int tz = z + zpos - carZpos + trackSize;

        tz = (tz) % trackSize;

        int tlii = track[tz] & 0xff;

        for (int u = -79; u != 80; ++u)
        {
            Real x = (u + offset[v]) * z / scale;

            int tx = (x + xpos + 4) * 64;

            if (tx >= 0 && tx < ROAD_SIZE)
            {
                fb[u + fbx + 79 + (v + 80) * (WIDTH)] = tlis[tlii*ROAD_SIZE + tx];
            }
            else
            {
                fb[u + fbx + 79 + (v + 80) * (WIDTH)] = CL_DGREEN;
            }

            if (v == carLine)
            {
                if (x >= -0.8 && x < 0.8)
                {
                    fb[u + fbx + 79 + (v + 80) * (WIDTH)] = CL_BLACK;
                }
            }
        }
    }
}
#endif

#include "menu.h"

#endif // GAME_H
