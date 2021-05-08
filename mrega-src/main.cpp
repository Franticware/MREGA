#include <cstring>
#include <cstdio>
#include "anytk.h"
#include "real.h"
#include "game.h"

#include "opl.h"

#include <time.h>
#include <stdlib.h>

#ifndef USESDL
using std::fputs;
using std::printf;
using std::fflush;
using std::strcpy;
using std::strlen;
using std::sscanf;
using std::strcmp;
#endif

// keys: lshift, lctrl, a, s, w, d, rshift, rctrl, up, down, left, right, enter, escape

uint8_t testColor = 0;

char buildDate[9] = "";

void getBuildDate(char bd[9])
{
    bd[0] = '?';
    for (int i = 1; i < 9; ++i)
        bd[i] = 0;
    const char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                                "Sep", "Oct", "Nov", "Dec"};
    const char* monthsNum[] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};

    const char date[] = __DATE__;

    int dateLen = strlen(date);

    if (dateLen == 10 || dateLen == 11)
    {
        for (int i = 0; i != 12; ++i)
        {
            const char* month = months[i];
            const char* monthNum = monthsNum[i];

            int yes = 1;
            for (int j = 0; j != 3; ++j)
            {
                if (date[j] != month[j])
                    yes = 0;
            }
            if (yes)
            {
                bd[4] = monthNum[0];
                bd[5] = monthNum[1];
            }
        }

        if (dateLen == 11)
        {
            bd[0] = date[7];
            bd[1] = date[8];
            bd[2] = date[9];
            bd[3] = date[10];
            bd[6] = date[4];
            bd[7] = date[5];
        }
        else if (dateLen == 10)
        {
            bd[0] = date[6];
            bd[1] = date[7];
            bd[2] = date[8];
            bd[3] = date[9];
            bd[6] = '0';
            bd[7] = date[4];
        }
    }

    if (bd[6] == ' ')
    {
        bd[6] = '0';
    }
}

#if 0
#ifdef USESDL
void generateSpeedToFnumTable()
{
    const double multiplier = 5.272839314285714;
    printf("const uint16_t speedx10_2fnum[400] = {");
    for (int i = 0; i != 400; ++i)
    {
        double kmh = i / 10.0 * 3.6;
        double freq = 0;
        if (kmh < 30)
        {
            freq = kmh/30*150;
        }
        else if (kmh < 60)
        {
            freq = kmh/60.0*150;
        }
        else if (kmh < 90)
        {
            freq = kmh/90.0*150;
        }
        else
        {
            freq = kmh/100.0*120;
        }
        if (freq < 70)
            freq = 70;

        int fnum = std::floor(freq * multiplier);
        if (fnum >= 1024)
            fnum = 1023;

        printf("%d,", fnum);

    }
    printf("};\n");
    fflush(stdout);
}
#endif
#endif

struct Frb
{
    Frb()
    {
        frb = 0;
    }
    ~Frb()
    {
        delete[] frb;
    }
    uint8_t* frb;
};

/*

-mute
-swap
-a388

*/

int main(int argc, char** argv)
{
#ifdef USESDL
    /*generateSpeedToFnumTable();
    return 0;*/
#endif
    uint16_t oplAddr = 0x388;
    int soundMode = 1; // auto
    for (int i = 1; i < argc; ++i)
    {
        const char* arg = argv[i];
        if (strcmp(arg, "-mute") == 0)
        {
            soundMode = 0;
        }
        /*else if (strcmp(arg, "-opl2") == 0)
        {
            soundMode = 2;
        }*/
        else if (strcmp(arg, "-swap") == 0)
        {
            opl_swap();
        }
        else if (arg[0] == '-' && arg[1] == 'a')
        {
            unsigned oplAddrTmp = oplAddr;
            sscanf(arg + 2, "%x", &oplAddrTmp);
            oplAddr = (uint16_t)oplAddrTmp;
        }
        else
        {
            printf("Unknown argument '%s'\n", arg);
            return 1;
        }
    }

    //clock_t t_ = clock();

    time_t t_ = time(NULL);

    srand((uint32_t)t_);

#ifndef USESDL
    getBuildDate(buildDate);
    printf("Build %s\n", buildDate);
#endif

#ifndef USESDL
    int opl =
#endif

    opl_init(oplAddr, soundMode);

#ifndef USESDL
    if (opl == 0)
    {
        fputs("No", stdout);
    }
    else if (opl == 2)
    {
        fputs("OPL2", stdout);
    }
    else if (opl == 3)
    {
        fputs("OPL3", stdout);
    }

    //printf("%d\n", sizeof(FMInstrument));

    fputs(" synth chip detected.\n", stdout);

    fflush(stdout);

#endif
    //fputs("MREGA is loading, please wait...", stdout);


    // framebuffer 64000 bytes
    Frb frb;
    frb.frb = new uint8_t[WIDTH*HEIGHT];
    //uint8_t* frb = new uint8_t[WIDTH*HEIGHT];

    // initialize data
    initData();

    std::memset(frb.frb, 0, WIDTH*HEIGHT);

    uint8_t keyDown;
    uint8_t keyCode;

    //return 1;

    initTk();

    /*fputs("\n Boulevard\n\n Suburb\n\n Iron Works\n", stdout);
    fflush(stdout);

    for (;!getQuit();)
    {
        processEvents();
        while (getKeyEvent(&keyDown, &keyCode))
        {
        }
    }*/

    // title screen
    memcpy(frb.frb, title_d, 320*200);
    bool exitTitle = false;
    for (;!getQuit();)
    {
        processEvents();
        while (getKeyEvent(&keyDown, &keyCode))
        {
            //printf("%u %u\n", (uint32_t)keyDown, (uint32_t)keyCode);
            if (keyDown && keyCode == K_ENTER)
            {
                exitTitle = true;
            }

            if (keyDown)
            {
                if (keyCode == K_LEFT)
                {
                    snd_beepLeft();
                }
                else if (keyCode == K_RIGHT)
                {
                    snd_beepRight();
                }
            }
        }
        flip(frb.frb);

        if (exitTitle)
            break;
    }


    for (;!getQuit();)
    {
        trackSelected = 0;
        carSelected[0] = 0;
        carSelected[1] = 0;

        for (;!getQuit();)
        {
            processEvents();
            processMenuKeys();

            drawMenu(frb.frb);
            flip(frb.frb);

            if (carSelected[0] && carSelected[1])
                break;
        }


        int playerASpriteI = menuSelCar[0] * 2 + 0;
        int playerBSpriteI = menuSelCar[1] * 2 + 1;

        switch (menuSelTrack)
        {
        case 0:
            track = boulTrack.track;
            trackSize = boulTrack.size;
            backg = backgs.backg0;
            break;
        case 1:
            track = suburbTrack.track;
            trackSize = suburbTrack.size;
            backg = backgs.backg1;
            break;
        default:
            track = ironTrack.track;
            trackSize = ironTrack.size;
            backg = backgs.backg2;
            break;
        }

        playerA.lap = 0;
        playerA.angle = 0;
        playerA.sprA = 0;
        playerA.speed = 0;
        playerA.x = -2;
        playerA.z = -5;
        playerA.finished = 0;
        playerA.wins = 0;

        playerB.lap = 0;
        playerB.angle = 0;
        playerB.sprA = 0;
        playerB.speed = 0;
        playerB.x = 2;
        playerB.z = -5;
        playerB.finished = 0;
        playerB.wins = 0;

        if (rand() % 2)
        {
            playerA.x = -playerA.x;
            playerB.x = -playerB.x;
        }

        resetState();

        // draw static hud part
        std::memset(frb.frb, 0, WIDTH*HEIGHT);
        initialHudDraw(frb.frb);

        bool exitToMenu = false;

        snd_motorSetup(0, menuSelCar[0]);
        snd_motorSetup(1, menuSelCar[1]);

        // gameloop
        for (;!getQuit();)
        {
            processEvents();

            while (getKeyEvent(&keyDown, &keyCode))
            {
                if (!keyDown && keyCode == K_ESC)
                {
                    exitToMenu = true;
                }
            }

            simStep(playerA, playerB, track, trackSize);

            drawProto2(frb.frb, playerA, playerB, spriteSet[playerASpriteI], cars[playerBSpriteI]);
            drawProto2(frb.frb + 161, playerB, playerA, spriteSet[playerBSpriteI], cars[playerASpriteI]);

            flip(frb.frb);

            if (exitToMenu || getExitToMenu())
                break;
        }

        snd_motorStop(0);
        snd_motorStop(1);
    }

    deinitTk();

    opl_deinit();

    //delete[] frb;
    return 0;
}
