#ifndef ANYTK_H
#define ANYTK_H

#include <stdint.h>

#define WIDTH 320
#define HEIGHT 200

#define KEYB_E 0x60
#define KEYB_SIZE (KEYB_E * 2)

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

void initTk();
void deinitTk();
int keyb_get(uint8_t* keydown, uint8_t* code);
int getKeyEvent(uint8_t* keydown, uint8_t* code);
int getKeyState(uint8_t keyCode);
void processEvents();
void flip(const uint8_t* frb);
bool getQuit();
void setQuit(bool q);

#endif // ANYTK_H
