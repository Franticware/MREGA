#ifndef SIM_H
#define SIM_H

#include <stdint.h>
#include "real.h"
#include "sound.h"

struct Player
{
    int lap;
    int angle;
    int sprA;
    int finished;
    int wins; // 1 - wins, 2 - loses
    Real speed;    
    Real x;
    Real z;
};

void resetState();
int getExitToMenu();
int getLights();
void simStep(Player& playerA, Player& playerB, uint32_t* boulTrack, uint32_t boulTrackSize);

#endif // SIM_H
