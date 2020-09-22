#include "opl.h"

#ifndef USESDL
#include <conio.h>
#include <dos.h>

//using std::fputs;
//using std::fflush;
#endif

int OPL_mode = 0; // 0 - mute, 2 - opl2, 3 - opl3

uint8_t OPL_left = 0x10;
uint8_t OPL_right = 0x20;

uint16_t OPL_addr = 0x388;

#ifndef USESDL
void WriteFM(uint16_t regs, uint8_t value)
{
    uint8_t reg = (uint8_t)regs;
    if (regs < 0x100)
    {
        int i;
        outp(OPL_addr, reg);              /* set up the register  */
        for (i = 0; i < 6; i++) inp(OPL_addr);  /* wait 12 cycles       */
        outp(OPL_addr+1, value);               /* write out the value  */
        for (i = 0; i < 35; i++) inp(OPL_addr); /* wait 84 cycles       */
    }
    else
    {
        int i;
        outp(OPL_addr+2, reg);              /* set up the register  */
        for (i = 0; i < 6; i++) inp(OPL_addr);  /* wait 12 cycles       */
        outp(OPL_addr+3, value);               /* write out the value  */
        for (i = 0; i < 35; i++) inp(OPL_addr); /* wait 84 cycles       */
    }
}
#endif

uint8_t ReadFM(void)
{
#ifndef USESDL
   return (uint8_t)inp(OPL_addr);
#else
   return 0;
#endif
}

void FMSetVoice(int voiceNum, const FMInstrument* ins)
{
    if (!OPL_mode) return;
    int oplReg, oplVoiceOffset;

    voiceNum %= 11;
    oplVoiceOffset = voiceNum % 3 + ((voiceNum / 3) << 3);

    /* set sound characteristic */
    oplReg = 0x20 + (char)oplVoiceOffset;
    WriteFM(oplReg, ins->SoundCharacteristic[0]);
    oplReg += 3;
    WriteFM(oplReg, ins->SoundCharacteristic[1]);

    /* set level/output */
    oplReg = 0x40 + (char)oplVoiceOffset;
    WriteFM(oplReg, ins->Level[0]);
    oplReg += 3;
    WriteFM(oplReg, ins->Level[1]);

    /* set Attack/Decay */
    oplReg = 0x60 + (char)oplVoiceOffset;
    WriteFM(oplReg, ins->AttackDecay[0]);
    oplReg += 3;
    WriteFM(oplReg, ins->AttackDecay[1]);

    /* set Sustain/Release */
    oplReg = 0x80 + (char)oplVoiceOffset;
    WriteFM(oplReg, ins->SustainRelease[0]);
    oplReg += 3;
    WriteFM(oplReg, ins->SustainRelease[1]);

    /* set Wave Select */
    oplReg = 0xE0 + (char)oplVoiceOffset;
    WriteFM(oplReg, ins->WaveSelect[0]);
    oplReg += 3;
    WriteFM(oplReg, ins->WaveSelect[1]);

    /* set Feedback/Selectivity */
    oplReg = (uint8_t)0xC0 + (uint8_t)voiceNum;
    WriteFM(oplReg, ins->Feedback);
} /* End of FMSetVoice */

void FMKeyOn(int voice, int freq, int octave)
{
    if (!OPL_mode) return;
   int regNum, tmp;

   regNum = 0xA0 + voice % 11;
   WriteFM(regNum, freq & 0xff);
   regNum = 0xB0 + voice % 11;
   tmp = (freq >> 8) | (octave << 2) | 0x20;
   WriteFM(regNum, tmp);

} /* End of FMKeyOn */

void FMKeyOff(int voice, int freq, int octave)
{
    if (!OPL_mode) return;
   int regNum, tmp;

   regNum = 0xA0 + voice % 11;
   WriteFM(regNum, freq & 0xff);
   regNum = 0xB0 + voice % 11;
   tmp = (freq >> 8) | (octave << 2);
   WriteFM(regNum, tmp);

} /* End of FMKeyOn */

void FMVoiceOff(int voice)
{
    if (!OPL_mode) return;
   int regNum;

   /* turn voice off */
   regNum = 0xB0 + voice % 11;
   WriteFM(regNum, 0);
}


void opl_swap()
{
    OPL_left  = 0x20;
    OPL_right = 0x10;
}


// modes
// 0 - mute
// 1 - auto
// 2 - opl2
// 3 - opl3

#ifndef USESDL

int opl_init(int addr, int mode)
{
    int ret = 0;
    OPL_addr = addr;

    if (mode)
    {

        WriteFM(0x04, 0x60);            /* reset both timers        */
        WriteFM(0x04, 0x80);            /* enable timer interrupts  */
        uint8_t stat1 = ReadFM();               /* read status register     */
        WriteFM(0x02, 0xFF);
        WriteFM(0x04, 0x21);            /* start timer 1            */
        //Wait(10);                       /* could do something useful*/
#ifndef USESDL
        delay(1);
#endif
        uint8_t stat2 = ReadFM();               /* read status register     */
        WriteFM(0x04, 0x60);            /* reset both timers        */
        WriteFM(0x04, 0x80);            /* enable timer interrupts  */

        if (((stat1 & 0xE0) == 0x00) && ((stat2 & 0xE0) == 0xC0))
        {
            WriteFM(1, 0x20);           /* enable wave select */
            ret = 2;
        }

        if (ret && mode != 2)
        {
            uint8_t stat3 = ReadFM();               /* read status register     */

            if ((stat3 & 0x06) == 0)
            {
                /*fputs("enable OPL3 mode\n", stdout);
                fflush(stdout);*/
                //WriteFM3(0x05, 0x01);
                WriteFM(0x105, 0x01);
                ret = 3;
            }
        }
    }

    OPL_mode = ret;
    return ret;

}

void opl_deinit()
{
    for (int i = 0; i != 6; ++i)
    {
        FMVoiceOff(i);
    }
}

#endif
