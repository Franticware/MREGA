#ifndef MENU_H
#define MENU_H

#include "menudata.h"

#include "track.h" // for color definitions

// not really include, just for code model
#include "anytk.h"
#include "game.h"

#define TRACK_COUNT 3
#define CAR_COUNT 3

struct MenuPix
{
    MenuPix()
    {
        for (int i = 0; i != CAR_COUNT * 4; ++i)
            menuCar[i] = 0;
        for (int i = 0; i != TRACK_COUNT * 2; ++i)
            menuTrack[i] = 0;
        menuDown = 0;
    }
    ~MenuPix()
    {
        for (int i = 0; i != CAR_COUNT * 4; ++i)
            delete[] menuCar[i];
        for (int i = 0; i != TRACK_COUNT * 2; ++i)
            delete[] menuTrack[i];
        delete[] menuDown;

    }

    uint8_t* menuCar[CAR_COUNT * 4];
    uint8_t* menuTrack[TRACK_COUNT * 2];

    uint8_t* menuDown;
};

MenuPix menuPix;

#if menuskoda_w != menutrabant_w || menuskoda_w != menubeetle_w || menuskoda_h != menutrabant_h || menuskoda_h != menubeetle_h
#error Menu car sprites must have identical dimensions
#endif

#if menuboul_w != menusuburb_w || menuboul_w != menuiron_w || menuboul_h != menusuburb_h || menuboul_h != menuiron_h
#error Menu track sprites must have identical dimensions
#endif

const uint8_t carColorD[CAR_COUNT * 2] = {CL_DRED, CL_TEAL, CL_DBLUE, CL_YELLOW, CL_DGREEN, CL_VIOLET};
const uint8_t carColorL[CAR_COUNT * 2] = {CL_LRED, CL_AZURE, CL_LBLUE, CL_YELLOW, CL_LGREEN, CL_PINK};

const int8_t* carData[CAR_COUNT] = {menuskoda_d, menutrabant_d, menubeetle_d};
const int8_t* trackData[TRACK_COUNT] = {menuboul_d, menusuburb_d, menuiron_d};


int trackSelected = 0;
int carSelected[2] = {0, 0};

int menuSelTrack = 0;
int menuSelCar[2] = {0, 0};



void initMenu()
{
    for (int i = 0; i != CAR_COUNT * 2; ++i)
    {
        int carI = i / 2;

        for (int j = 0; j != 2; ++j)
        {
            menuPix.menuCar[i * 2 + j] = new uint8_t[menuskoda_w * menuskoda_h];
            for (int k = 0; k != menuskoda_w * menuskoda_h; ++k)
            {
                uint8_t c = carData[carI][k];
                if (c == CL_LBLUE)
                    c = carColorL[i];
                else if (c == CL_DBLUE)
                    c = carColorD[i];
                if (j == 0 && c == CL_WHITE)
                    c = CL_LGRAY;
                menuPix.menuCar[i * 2 + j][k] = c;
            }
        }
    }

    for (int i = 0; i != TRACK_COUNT; ++i)
    {
        for (int j = 0; j != 2; ++j)
        {
            menuPix.menuTrack[i * 2 + j] = new uint8_t[menuboul_w * menuboul_h];
            for (int k = 0; k != menuboul_w * menuboul_h; ++k)
            {
                uint8_t c = trackData[i][k];
                if (j == 0 && c == CL_WHITE)
                    c = CL_LGRAY;
                menuPix.menuTrack[i * 2 + j][k] = c;
            }
        }
    }

    menuPix.menuDown = new uint8_t[menuup_w * menuup_h];

    for (int j = 0; j != menuup_h; ++j)
    {
        for (int i = 0; i != menuup_w; ++i)
        {
            menuPix.menuDown[i + j * menuup_w] = menuup_d[i + (menuup_h - j - 1) * menuup_w];
        }
    }
}

void drawCarMenu(uint8_t* fb, int playerI)
{
    fb += playerI * 161;

    for (int i = 0; i != CAR_COUNT; ++i)
    {
        if (!carSelected[playerI] || menuSelCar[playerI] == i)
        {
            int j = 0;
            if (menuSelCar[playerI] == i)
                ++j;
            drawRect(fb, menuPix.menuCar[i * 4 + 2 * playerI + j], menuskoda_w, menuskoda_h, 8, 31 + (79-31) * i);
        }
    }

    if (!carSelected[playerI])
    {
        drawRect(fb, menuup_d, menuup_w, menuup_h, 64, 13);
        drawRect(fb, menuPix.menuDown, menuup_w, menuup_h, 64, 174);
        drawRect(fb, menuok_d, menuok_w, menuok_h, 138, 84);
    }
}

void drawMenu(uint8_t* fb)
{
    if (!trackSelected)
    {
        // track selection
        for (int i = 0; i != HEIGHT; ++i)
        {
            memcpy(fb + i * WIDTH, color320[CL_LGRAY], WIDTH);
        }

        for (int i = 0; i != 3; ++i)
        {
            int j = 0;
            if (menuSelTrack == i)
                ++j;
            drawRect(fb, menuPix.menuTrack[i * 2 + j], menuboul_w, menuboul_h, 78, 31 + (79-31) * i);
        }

        drawRect(fb, menuup_d, menuup_w, menuup_h, 148, 13);
        drawRect(fb, menuPix.menuDown, menuup_w, menuup_h, 148, 174);
        drawRect(fb, menuok_d, menuok_w, menuok_h, 239, 84);
    }
    else
    {
        // car selection
        for (int i = 0; i != HEIGHT; ++i)
        {
            memcpy(fb + i * WIDTH, color320[CL_LGRAY], 159);
            memcpy(fb + i * WIDTH + 159, color320[CL_BLACK], 2);
            memcpy(fb + i * WIDTH + 161, color320[CL_LGRAY], 159);
        }


        drawCarMenu(fb, 0);
        drawCarMenu(fb, 1);






    }


}

/*
#define K_ESC 1

#define K_W 17

#define K_ENTER 28

#define K_A 30
#define K_S 31
#define K_D 32

#define K_LSHIFT 42
#define K_RSHIFT 54

#define K_LCTRL 29
#define K_RCTRL 125

#define K_LEFT 171
#define K_RIGHT 173
#define K_UP 168
#define K_DOWN 176
*/

void processMenuKeys()
{
    uint8_t keyDown;
    uint8_t keyCode;
    while (getKeyEvent(&keyDown, &keyCode))
    {
        if (!trackSelected)
        {
            carSelected[0] = 0;
            carSelected[1] = 0;

            if (keyDown)
            {
                switch (keyCode)
                {
                case K_W:
                case K_UP:
                case K_LSHIFT:
                case K_RSHIFT:
                    menuSelTrack = (menuSelTrack + (TRACK_COUNT - 1)) % TRACK_COUNT;
                    break;
                case K_S:
                case K_DOWN:
                case K_LCTRL:
                case K_RCTRL:
                    menuSelTrack = (menuSelTrack + 1) % TRACK_COUNT;
                    break;
                case K_D:
                case K_RIGHT:
                case K_ENTER:
                    trackSelected = 1;
                    break;
                }
            } else { // key up

                if (keyCode == K_ESC)
                {
                    setQuit(true);
                }
            }
        }
        else
        {
            if (keyDown)
            {
                switch (keyCode)
                {
                case K_W:
                case K_LSHIFT:
                    if (!carSelected[0])
                        menuSelCar[0] = (menuSelCar[0] + (CAR_COUNT - 1)) % CAR_COUNT;
                    break;
                case K_UP:
                case K_RSHIFT:
                    if (!carSelected[1])
                        menuSelCar[1] = (menuSelCar[1] + (CAR_COUNT - 1)) % CAR_COUNT;
                    break;
                case K_S:
                case K_LCTRL:
                    if (!carSelected[0])
                        menuSelCar[0] = (menuSelCar[0] + 1) % CAR_COUNT;
                    break;
                case K_DOWN:
                case K_RCTRL:
                    if (!carSelected[1])
                        menuSelCar[1] = (menuSelCar[1] + 1) % CAR_COUNT;
                    break;
                case K_D:
                    carSelected[0] = 1;
                    break;
                case K_RIGHT:
                    carSelected[1] = 1;
                    break;
                case K_ENTER:
                    carSelected[0] = 1;
                    carSelected[1] = 1;
                    break;
                }
            } else { // key up

                if (keyCode == K_ESC)
                {
                    trackSelected = 0;
                }
            }




        }

        //printf("%u %u\n", (uint32_t)keyDown, (uint32_t)keyCode);
    }
}

#endif // MENU_H
