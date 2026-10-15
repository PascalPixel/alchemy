/* Near miss, differ 10 of 66, three reordered places and nothing else: the native loads the fill routine's
 * entry before it starts the size (ldr r3; movs r1, #128), clears the row counter before it copies y
 * (movs r6, #0; adds r1, r5, #0) and loads the map base before the row's shift (ldr r5; lsls r3, r6, #5).
 * The screen position needs counters of its own beside row and column (differ 88 with y + row). Setting sy
 * before row is differ 54. The permuter found nothing better in five minutes. */
#include "types.h"
#include "iwram_call.h"

extern const u16 gData_080f39c4[];

// 16x16 の絵を BG の地図へ写す。画面（30x20 タイル）の外になるマスは写さない
void Func_080e7fec(s32 x, s32 y)
{
    const u16 *tiles = gData_080f39c4;
    s32 row;
    s32 column;
    s32 sx;
    s32 sy;

    x /= 8;
    y /= 8;
    Iwram_FillWords((void *)0x06002000, 0x400, 0);
    for (row = 0, sy = y; row < 16; row++, sy++) {
        if (sy < 0)
            continue;
        if (sy > 19)
            break;
        for (sx = x, column = 0; column < 16; column++, sx++) {
            if (sx < 0)
                continue;
            if (sx > 29)
                break;
            ((u16 *)0x06002000)[row * 32 + column] = tiles[row * 16 + column];
        }
    }
}
