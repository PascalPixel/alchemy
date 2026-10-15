/* Draft, overlay 6b5 Func_02001414 (0x02009414), The Broken Seal's
 * LinkLobby_DrawThreeDigitValue: differ 0, but only through a goto loop,
 * which new C may not add (S2: the goto count may only fall). Adopted with
 * the goto at 12/12 on its lane, it waits here until a loop form without
 * goto is found. The native builds the cell size inside the loop (movs r3,
 * #1 after the remainder's call), then starts the column (movs r2, #16) and
 * only then copies the remainder into r1.
 * Measured: as a for loop, loop.c keeps the stacked 1 in r7 (the loop dump:
 * 23 real insns, life 3, so 45 >= 23; differ 12). An empty asm barrier on
 * the size inside a for loop keeps it inside, but the remainder's copy is
 * then emitted at its statement, ahead of the barrier (differ 2 of 32, one
 * reordered pair); holding the remainder on the same barrier gives the same
 * order (2); the barrier inside the argument list moves the 1 back out
 * (30). */
#include "types.h"

void Func_0200197c(void);
void Func_02001984(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);

// Draws value, at most 999, as three digits from right to left.
void Func_02001414(s32 value)
{
    s32 i;

    if (value > 999)
        value = 999;
    i = 0;
next:
    Func_02001984(27, value % 10, 16 - i, 8, 1, 1);
    value /= 10;
    i++;
    if (i <= 2)
        goto next;
    Func_0200197c();
}
