#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

void snd_beepLeft();
void snd_beepRight();

void snd_beepLights(bool high);
void snd_crash(uint8_t vol);

void snd_motorSetup(int playerIndex, int insI);
void snd_motorFreq(int playerIndex, int freq);
void snd_motorStop(int playerIndex);

#endif // SOUND_H
