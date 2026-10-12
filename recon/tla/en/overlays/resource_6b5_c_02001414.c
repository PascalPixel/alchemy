/* Near miss, overlay 6b5 Func_02001414 (0x02009414), The Broken Seal's
 * LinkLobby_DrawThreeDigitValue: differ 2 of 32 (one reordered pair). The
 * native builds the cell size inside the loop (movs r3, #1 after the
 * remainder's call), then starts the column (movs r2, #16) and only then
 * copies the remainder into r1; this copies the remainder first.
 * Measured: the twin's plain call, also through an inline layer, moves the
 * shared 1 out of the loop into r7 (the loop dump: 23 real insns, life 3,
 * so 45 >= 23; differ 12). The barrier below keeps it inside; with it the
 * remainder's copy is emitted at its statement, ahead of the barrier.
 * Holding the remainder on the same barrier gives the same order (2); the
 * barrier inside the argument list moves the 1 back out (30). */
#include "types.h"

void Func_0200197c(void);
void Func_02001984(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);

// 3 桁の数字をマップのセルで描く（上限 999）
s32 Func_02001414(s32 value)
{
    s32 digit;

    if (value > 999)
        value = 999;
    for (digit = 0; digit <= 2; digit++) {
        s32 ones = value % 10;
        s32 size = 1;

        /* FAKEMATCH: ⚓️ builds the cell size on every pass; passed as two constants, loop.c moves the shared 1 out of the loop. The barrier keeps it inside the loop. */
        asm volatile("" : "+l"(size));
        Func_02001984(27, ones, 16 - digit, 8, size, size);
        value = value / 10;
    }
    Func_0200197c();
}
