/*
 * Draft of Func_080aff94. 2 instructions differ: the listing loads the flags
 * word before it builds the bit (ldr r3; movs r2, #1), the draft builds the
 * bit first. Tried: "*state = *state | (1 << bit)" (same 2), a named value
 * with or without an empty asm barrier and the swapped OR (the two registers
 * trade places, 6 to 10 differ).
 */
#include "types.h"

u32 *Trade_GetOfferState();

void Func_080aff94(s32 bit)
{
    u32 *state = Trade_GetOfferState(0);

    *state |= 1 << bit;
}
