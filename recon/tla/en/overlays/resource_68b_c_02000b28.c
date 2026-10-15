/* Near miss, differ 2 of 65: the wave scanline table of map layer 1, from its sibling in overlay 674.
 * The sine and the counter live on the stack as an array (sp and sp + 4), the
 * counter through a pointer, as the native reloads it after each call; a
 * one-pass block stores the sine before the layer pointer is reloaded (alchemy
 * permute found the same block). Left: the native stores the counter's 0
 * before it copies the layer and line pointers to r10 and r8; here the reload
 * copies go first (all three tie at priority 1 and the copies come first in
 * insn order). The counter set before the pointers are formed (differ 16) and
 * the pointers copied through an operand asm after it (52) were worse. */
#include "types.h"
#include "frame.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gData_02002590;
extern s16 gData_020025a0[2][160];

s32 Math_Sine(s32 angle);

void Func_02000b28(void)
{
    s16 *layer = (s16 *)&Ram_HeapSlots->mapWork->layers[1];
    s16 *lines = gData_020025a0[gData_02002590 ^ 1];
    s32 v[2];
    s32 *count = &v[1];
    s32 value;
    s32 sine;

    for (*count = 0; *count != 160; (*count)++) {
        /* FAKEMATCH: the one-pass block stores the sine before the layer pointer is reloaded into r3 (differ 4 without it). */
        do {
            sine = Math_Sine((*count + *(u8 *)&gFrameCount + layer[3]) << 9);
            v[0] = sine;
        } while (0);
        value = layer[1] + (sine >> 15);
        value += *count % 3;
        *lines++ = value - 1;
    }
    gData_02002590 ^= 1;
}
