/* Menu rows: step the value or its sub-value with the direction and shoulder keys, returning -1 on A and -2 on B. */
#include "TYPES.H"

extern volatile u32 gKeysRepeat;
void Menu_DrawSelectionRow(s32 arg, s32 value, s16 *sub);

s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode_arg)
{
    register s16 *mode asm("r0") = mode_arg; /* FAKEMATCH: keeps the mode pointer in r0 */
    if (gKeysRepeat & 1)
        return -1;
    if (gKeysRepeat & 2)
        return -2;
    if ((gKeysRepeat & 0x80) || (gKeysRepeat & 0x40)) {
        *mode ^= 1;
    } else if (gKeysRepeat & 0x10) {
        if (*mode == 0) {
            value++;
        } else {
            (*sub)++;
            if (*sub > 99)
                *sub = 0;
        }
        if (value > 200)
            value = 0;
        Menu_DrawSelectionRow(arg, value, sub);
    } else if (gKeysRepeat & 0x20) {
        if (*mode == 0) {
            value--;
        } else {
            (*sub)--;
            if (*sub < 0)
                *sub = 99;
        }
        if (value < 0)
            value = 200;
        Menu_DrawSelectionRow(arg, value, sub);
    } else if (gKeysRepeat & 0x100) {
        if (*mode == 0) {
            *sub = 0;
            value += 10;
        } else {
            *sub += 10;
            if (*sub > 99)
                *sub -= 99;
        }
        if (value > 200)
            value = 0;
        Menu_DrawSelectionRow(arg, value, sub);
    } else if (gKeysRepeat & 0x200) {
        if (*mode == 0) {
            *sub = 0;
            value -= 10;
        } else {
            *sub -= 10;
            if (*sub < 0)
                *sub += 99;
        }
        if (value < 0)
            value = 200;
        Menu_DrawSelectionRow(arg, value, sub);
    }
    return value;
}
