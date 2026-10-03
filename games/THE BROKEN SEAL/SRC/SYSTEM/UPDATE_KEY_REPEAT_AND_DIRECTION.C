#include "TYPES.H"
#include "IO_REG.H"
#include "KEYSTATE.H"
#include "INPUT.H"

#define KEY_REPEAT_FIRST 19
#define KEY_REPEAT_NEXT  6

/* Repeats held keys after a delay and resolves diagonal input using the
   previous direction mask. */
void Input_UpdateKeyRepeatAndDirection(void)
{
    /* FAKEMATCH: the volatile accesses preserve the measured input-cell
       rereads without giving the shared declarations conflicting types. */
    s32 delay = *(volatile s32 *)&Data_03001b00;
    u32 keys;
    u32 axis = 0;
    u32 dirs;
    u32 mask;
    volatile u32 *repeat;

    if (delay <= 0) {
        *(volatile u32 *)&gKeysRepeat = *(volatile u32 *)&gKeysHeld;
        keys = *(volatile u32 *)&gKeysRepeat;
        if (delay == 0)
            *(volatile s32 *)&Data_03001b00 = KEY_REPEAT_NEXT;
        else
            *(volatile s32 *)&Data_03001b00 = KEY_REPEAT_FIRST;
    } else {
        *(volatile u32 *)&gKeysRepeat = 0;
        keys = *(volatile u32 *)&gKeysRepeat;
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

        repeat = (volatile u32 *)&Data_03001afc;
        *repeat = keys;
        switch (dirs) {
        default:
            *(volatile u32 *)&Data_03001d04 = KEYS_HORIZONTAL;
            mask = 0xffff & ~KEYS_DPAD;
            *repeat &= mask;
            break;
        case 0:
            *(volatile u32 *)&Data_03001d04 = KEYS_HORIZONTAL;
            break;
        case 1:
            *(volatile u32 *)&Data_03001d04 = keys & KEYS_DPAD;
            break;
        case 2:
            if ((*(volatile u32 *)&Data_03001d04 & *repeat) == 0)
                *(volatile u32 *)&Data_03001d04 = KEYS_HORIZONTAL;
            *repeat &= *(volatile u32 *)&Data_03001d04 ^ 0xffff;
            break;
        case 3:
            if (*(volatile u32 *)&Data_03001d04 & KEYS_HORIZONTAL)
                axis = KEYS_HORIZONTAL;
            if (*(volatile u32 *)&Data_03001d04 & KEYS_VERTICAL)
                axis = KEYS_VERTICAL;
            mask = 0xffff ^ axis;
            *(volatile u32 *)&Data_03001d04 = keys & mask;
            *repeat &= mask;
            break;
        }
    } else {
        *(volatile u32 *)&Data_03001afc = keys;
    }

    *(volatile u32 *)&gKeyState =
        (*(volatile u32 *)&gKeysHeld ^ *(volatile u32 *)&Data_03001cf4)
        & *(volatile u32 *)&gKeysHeld;
    *(volatile u32 *)&Data_03001cf4 = *(volatile u32 *)&gKeysHeld;
}
