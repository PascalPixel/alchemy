#include "TYPES.H"
#include "GLOBAL_CELLS.H"

#define KEY_RIGHT 0x10
#define KEY_LEFT  0x20
#define KEY_UP    0x40
#define KEY_DOWN  0x80
#define KEYS_HORIZONTAL (KEY_RIGHT | KEY_LEFT)
#define KEYS_VERTICAL   (KEY_UP | KEY_DOWN)
#define KEYS_DPAD       (KEYS_HORIZONTAL | KEYS_VERTICAL)

#define KEY_REPEAT_FIRST 19
#define KEY_REPEAT_NEXT  6

/* FAKEMATCH: volatile on plain RAM keeps every re-read of the input cells */
extern volatile s32 Data_03001b00; /* key repeat delay */
extern volatile u32 Data_03001b04; /* keys repeating this frame */
extern volatile u32 Data_03001ae8; /* keys held */
extern volatile u32 Data_03001afc; /* repeated keys, one direction only */
extern volatile u32 Data_03001d04; /* axis last chosen */
extern volatile u32 Data_03001c94; /* keys newly pressed */
extern volatile u32 Data_03001cf4; /* keys held last update */

/* Repeats held keys after a delay and reduces diagonal input to one axis,
   keeping the axis that was pressed most recently. */
void Input_UpdateKeyRepeatAndDirection(void)
{
    s32 delay = Data_03001b00;
    u32 keys;
    u32 axis = 0;
    u32 dirs;
    u32 mask;
    volatile u32 *repeat;

    if (delay <= 0) {
        Data_03001b04 = Data_03001ae8;
        keys = Data_03001b04;
        if (delay == 0)
            Data_03001b00 = KEY_REPEAT_NEXT;
        else
            Data_03001b00 = KEY_REPEAT_FIRST;
    } else {
        Data_03001b04 = 0;
        keys = Data_03001b04;
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

        repeat = &Data_03001afc;
        *repeat = keys;
        switch (dirs) {
        default:
            Data_03001d04 = KEYS_HORIZONTAL;
            mask = 0xffff & ~KEYS_DPAD;
            *repeat &= mask;
            break;
        case 0:
            Data_03001d04 = KEYS_HORIZONTAL;
            break;
        case 1:
            Data_03001d04 = keys & KEYS_DPAD;
            break;
        case 2:
            if ((Data_03001d04 & *repeat) == 0)
                Data_03001d04 = KEYS_HORIZONTAL;
            *repeat &= Data_03001d04 ^ 0xffff;
            break;
        case 3:
            if (Data_03001d04 & KEYS_HORIZONTAL)
                axis = KEYS_HORIZONTAL;
            if (Data_03001d04 & KEYS_VERTICAL)
                axis = KEYS_VERTICAL;
            mask = 0xffff ^ axis;
            Data_03001d04 = keys & mask;
            *repeat &= mask;
            break;
        }
    } else {
        Data_03001afc = keys;
    }

    Data_03001c94 = (Data_03001ae8 ^ Data_03001cf4) & Data_03001ae8;
    Data_03001cf4 = Data_03001ae8;
}
