#include "doskeyb.h"
#include <string.h> // memcpy
#include <dos.h> // _dos_getvect _dos_setvect
#include <conio.h> // inp

void (__interrupt __far *keyb_old_int)( void );

unsigned char keyb_state_int[KEYB_SIZE] = {0};

typedef struct QueueT {
    uint8_t begin;
    uint8_t end;
    uint16_t data[0x100];
} Queue;

#define QueueNotEmpty(q) ((q).begin != (q).end)
#define QueueGet(q) ((q).data[(q).begin++])
#define QueuePut(q, d) do { (q).data[(q).end++] = (d); } while (0)

Queue keyb_queue = { /*.begin =*/ 0, /*.end =*/ 0};

int keyb_get(uint8_t* keydown, uint8_t* code)
{
    if (QueueNotEmpty(keyb_queue))
    {
        uint16_t v = QueueGet(keyb_queue);
        *keydown = (uint8_t)(v >> 8);
        *code = (uint8_t)v;
        return 1;
    }
    else
    {
        return 0;
    }
}

static void __interrupt keyb_int() {
    static unsigned char buffer = 0;
    unsigned char rawcode;
    unsigned char make_break;
    unsigned char scancode;
    uint16_t v;
    rawcode = inp(0x60); /* read scancode from keyboard controller */
    make_break = !(rawcode & 0x80); /* bit 7: 0 = make, 1 = break */
    scancode = rawcode & 0x7F;
    if (buffer == 0xE0) { /* second byte of an extended key */
        if (scancode < 0x60) {
            if (keyb_state_int[scancode + KEYB_E] != !!make_break)
            {
                keyb_state_int[scancode + KEYB_E] = !!make_break;
                v = ((uint16_t)(scancode + KEYB_E) | ((uint16_t)(!!make_break) << 8));
                QueuePut(keyb_queue, v);
            }
        }
        buffer = 0;
    } else if (buffer >= 0xE1 && buffer <= 0xE2) {
        buffer = 0; /* ingore these extended keys */
    } else if (rawcode >= 0xE0 && rawcode <= 0xE2) {
        buffer = rawcode; /* first byte of an extended key */
    } else if (scancode < 0x60) {
        if (keyb_state_int[scancode] != !!make_break)
        {
            keyb_state_int[scancode] = !!make_break;
            v = ((uint16_t)(scancode) | ((uint16_t)(!!make_break) << 8));
            QueuePut(keyb_queue, v);
        }
    }
    outp(0x20, 0x20); /* must send EOI to finish interrupt */
}

void keyb_hook_int(void) {
    keyb_old_int = _dos_getvect(0x09);
    _dos_setvect(0x09, keyb_int);
}

void keyb_unhook_int(void) {
    if (keyb_old_int != NULL) {
        _dos_setvect(0x09, keyb_old_int);
        keyb_old_int = NULL;
    }
}
