#include "trial_road.h"

extern const u8 gData_020063ac[];

void Func_02004d84(u8 *text, u32 value)
{
    s32 i;
    const u8 *digits = gData_020063ac;

    /* FAKEMATCH: the native loads the digit table before it advances the text (differ 2 without the barrier). */
    __asm__("");
    text += 8;
    *text = 0;
    text--;
    i = 7;
    /* FAKEMATCH: the loop is written with goto: as a for loop the 15 is moved out of it (differ 13), where the native builds it on every pass. */
next:
    *text = digits[value & 15];
    value >>= 4;
    text--;
    i--;
    if (i >= 0)
        goto next;
}
