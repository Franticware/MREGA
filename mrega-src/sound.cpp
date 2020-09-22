#include "sound.h"

#include "opl.h"
#include <stdint.h>

void snd_beepLeft()
{
    const uint8_t lightsSbiData[11] = {0x03, 0x01, 0x47, 0x00, 0x95, 0x95, 0x15, 0x15, 0x01, 0x00, (uint8_t)(0x08 | OPL_left)};
    FMSetVoice(3, (const FMInstrument*)lightsSbiData);
    FMVoiceOff(3);
    FMKeyOn(3, 580, 4);
}

void snd_beepRight()
{
    const uint8_t lightsSbiData[11] = {0x03, 0x01, 0x47, 0x00, 0x95, 0x95, 0x15, 0x15, 0x01, 0x00, (uint8_t)(0x08 | OPL_right)};
    FMSetVoice(4, (const FMInstrument*)lightsSbiData);
    FMVoiceOff(4);
    FMKeyOn(4, 580, 4);
}

void snd_beepLights(bool high)
{
    const uint8_t lightsSbiData[11] = {0x03, 0x01, 0x47, 0x00, 0x95, 0x95, 0x15, 0x15, 0x01, 0x00, (uint8_t)(0x08 | OPL_center)};
    FMSetVoice(0, (const FMInstrument*)lightsSbiData);
    FMVoiceOff(0);
    if (high)
    {
        FMKeyOn(0, 580, 5);
    }
    else
    {
        FMKeyOn(0, 580, 4);
    }
}

void snd_crash(uint8_t vol)
{
    const uint8_t crashSbiData[11] = {0x02, 0x01, 0x47, (uint8_t)(63-vol), 0xf8, 0xf5, 0xff, 0x7f, 0x00, 0x00, (uint8_t)(0x0e | OPL_center)};

    FMSetVoice(0, (const FMInstrument*)crashSbiData);
    FMVoiceOff(0);
    FMKeyOn(0, /*435*/773, 0);
}

int snd_octave[2] = {1,1};

void snd_motorSetup(int playerIndex, int insI)
{
    if (playerIndex >= 2 || insI >= 3)
        return;
    int voice = playerIndex + 1;
    uint8_t opl_ch = playerIndex == 0 ? OPL_left : OPL_right;

    const uint8_t skodaSbi[11] =   {0x21, 0xe2, 0x00, (uint8_t)(63-46), 0x87, 0x8b, 0x17, 0x0e, 0x00, 0x00, (uint8_t)(0x02 | opl_ch)};
    const uint8_t trabantSbi[11] = {0xa1&0x7f, 0x25, 0x03, (uint8_t)(63-54), 0x87, 0x8b, 0x17, 0x0e, 0x00, 0x01, (uint8_t)(0x02 | opl_ch)};
    const uint8_t beetleSbi[11] =  {0x62, 0x23, 0x03, (uint8_t)(63-54), 0x87, 0x8b, 0x17, 0x0e, 0x01, 0x01, (uint8_t)(0x02 | opl_ch)};

    const int octaves[3] = {1, 1, 0};

    const uint8_t* motorSbi[3] = {skodaSbi, trabantSbi, beetleSbi};

    snd_octave[playerIndex] = octaves[insI];

    FMSetVoice(voice, (const FMInstrument*)motorSbi[insI]);
    FMVoiceOff(voice);
    /*FMKeyOn(voice,
            527
            //790
            , 1);*/

}


void snd_motorFreq(int playerIndex, int fnum)
{
    int voice = playerIndex + 1;

    FMKeyOn(voice,
            //527
            fnum
            //790
            , snd_octave[playerIndex]);

}


void snd_motorStop(int playerIndex)
{
    FMVoiceOff(playerIndex + 1);
}

/*
Prelude> 3579000/49716
71.98889693458847
Prelude> 315/88
3.5795454545454546
Prelude> 315/88
3.5795454545454546
Prelude> 315/88/49716
7.199986834309789e-5
Prelude> 315/88/49716*1000000
71.99986834309789
Prelude> 315/88/72
4.971590909090909e-2
Prelude> 315000/88/72
49.71590909090909
Prelude> 315000000/88/72
49715.90909090909
Prelude> 440*2^(20-4)/(315000000/88/72)
580.0123245714286
Prelude> 880*2^(20-4)/(315000000/88/72)
1160.0246491428572
Prelude> 700*2^(20-4)/(315000000/88/72)
922.74688
Prelude> 700*2^(20-3)/(315000000/88/72)
1845.49376
Prelude> 700*2^(20-5)/(315000000/88/72)
461.37344
Prelude> 440*2^(20-5)/(315000000/88/72)
290.0061622857143
Prelude> 440*2^(20-3)/(315000000/88/72)
1160.0246491428572
Prelude>
*/
