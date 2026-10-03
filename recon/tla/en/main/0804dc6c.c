/* Menu rows: step the value or its sub-value with the direction and shoulder keys, returning -1 on A and -2 on B. */
#include "TYPES.H"
#include "IO_REG.H"

extern volatile u32 gKeysRepeat;
void Menu_DrawSelectionRow(s32 arg, s32 value, s16 *sub);

s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode_arg)
{
    register s16 *mode asm("r0") = mode_arg; /* FAKEMATCH: keeps the mode pointer in r0 */
    if (gKeysRepeat & KEY_A)
        return -1;
    if (gKeysRepeat & KEY_B)
        return -2;
    if ((gKeysRepeat & KEY_DOWN) || (gKeysRepeat & KEY_UP)) {
        *mode ^= 1;
    } else if (gKeysRepeat & KEY_RIGHT) {
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
    } else if (gKeysRepeat & KEY_LEFT) {
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
    } else if (gKeysRepeat & KEY_R) {
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
    } else if (gKeysRepeat & KEY_L) {
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
