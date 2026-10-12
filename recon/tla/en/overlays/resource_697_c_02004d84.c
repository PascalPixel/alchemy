/* Draft, differ 13 of 17: the twin of The Broken Seal WriteU32AsHex (korosseo_maruta/effect.c). The native keeps the digit table in r4 and builds 15 inside the loop (movs r3, #15; ands r3, r1); this form hoists the 15 into r4 and saves r5 for the table. Tried 2026-10-12: the table taken in a local inside the loop, (u8) and (u16) reads of the value, a one-pass block around the store: 15 is still moved out of the loop. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

extern const u8 Data_020063ac[];

void Func_02004d84(u8 *text, u32 value)
{
    s32 i;

    text += 8;
    *text = 0;
    text--;
    for (i = 7; i >= 0; i--) {
        *text = Data_020063ac[value & 15];
        value >>= 4;
        text--;
    }
}
