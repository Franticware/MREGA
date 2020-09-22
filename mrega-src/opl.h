#ifndef OPL_H
#define OPL_H

#include <stdint.h>

struct FMInstrument
{
   uint8_t SoundCharacteristic[2];
   uint8_t Level[2];
   uint8_t AttackDecay[2];
   uint8_t SustainRelease[2];
   uint8_t WaveSelect[2];
   uint8_t Feedback;
};

extern int OPL_mode; // 0 - mute, 2 - opl2, 3 - opl3
extern uint8_t OPL_left;
extern uint8_t OPL_right;

#define OPL_center (OPL_left|OPL_right)

void WriteFM(uint16_t regs, uint8_t value);
void FMSetVoice(int voiceNum, const FMInstrument* ins);
void FMKeyOn(int voice, int freq, int octave);
void FMKeyOff(int voice, int freq, int octave);
void FMVoiceOff(int voice);

int opl_init(int addr, int mode);
void opl_deinit();
void opl_swap();

#endif // OPL_H
