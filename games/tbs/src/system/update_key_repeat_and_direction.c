#include "types.h"
#include "io_reg.h"
#include "keystate.h"
#include "input.h"

#define KEY_REPEAT_FIRST 19
#define KEY_REPEAT_NEXT  6

void Input_UpdateKeyRepeatAndDirection(void)
{
    /* FAKEMATCH: volatile reads and the repeat clear preserve separate input-cell rereads. */
    s32 delay = gKeyRepeatTimer;
    u32 keys;
    u32 axis = 0;
    u32 dirs;
    u32 mask;
    volatile u32 *repeat;

    if (delay <= 0) {
        gKeysRepeat = gKeysHeld;
        keys = *(volatile u32 *)&gKeysRepeat;
        if (delay == 0)
            gKeyRepeatTimer = KEY_REPEAT_NEXT;
        else
            gKeyRepeatTimer = KEY_REPEAT_FIRST;
    } else {
        *(volatile u32 *)&gKeysRepeat = 0;
        keys = gKeysRepeat;
    }

    if (keys != 0) {
        dirs = 0;
        if (keys & KEY_UP)
            dirs = 1;
        if (keys & KEY_DOWN)
            dirs++;
        if (keys & KEY_LEFT)
            dirs++;
        if (keys & KEY_RIGHT)
            dirs++;

        repeat = &gKeysRepeatDirected;
        *repeat = keys;
        switch (dirs) {
        default:
            gKeysDirection = KEYS_HORIZONTAL;
            mask = 0xffff & ~KEYS_DPAD;
            *repeat &= mask;
            break;
        case 0:
            gKeysDirection = KEYS_HORIZONTAL;
            break;
        case 1:
            gKeysDirection = keys & KEYS_DPAD;
            break;
        case 2:
            if ((*(volatile u32 *)&gKeysDirection & *repeat) == 0)
                gKeysDirection = KEYS_HORIZONTAL;
            *repeat &= *(volatile u32 *)&gKeysDirection ^ 0xffff;
            break;
        case 3:
            if (gKeysDirection & KEYS_HORIZONTAL)
                axis = KEYS_HORIZONTAL;
            if (*(volatile u32 *)&gKeysDirection & KEYS_VERTICAL)
                axis = KEYS_VERTICAL;
            mask = 0xffff ^ axis;
            gKeysDirection = keys & mask;
            *repeat &= mask;
            break;
        }
    } else {
        gKeysRepeatDirected = keys;
    }

    *(volatile u32 *)&gKeyState =
        (gKeysHeld ^ gKeysHeldLast)
        & *(volatile u32 *)&gKeysHeld;
    gKeysHeldLast = *(volatile u32 *)&gKeysHeld;
}
