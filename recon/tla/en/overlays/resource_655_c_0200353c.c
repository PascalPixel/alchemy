/* Draft of Func_0200353c (overlay 655; Func_020035ec is the same search with
 * other cells): the block layout and the instructions match, the registers
 * do not (differ 85 of 79 by position): the native keeps the flag in r5 and
 * the cell pointer in r6 and builds the shared 1 in r2; this has r6, r5 and
 * r3. The search is a goto loop: as a for loop the not-found step is placed
 * ahead of the test. */
#include "types.h"

extern const u16 Data_02004276[];

s32 GameFlag_Test(s32 flag);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width, s32 height);

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003be0(a, b, c, d, e, f);
}

void Func_0200353c(void)
{
    s32 offset = 0;
    s32 flag = 800;

next:
    {
        if (GameFlag_Test(flag)) {
            const u16 *cell = (const u16 *)((const u8 *)Data_02004276 + offset);
            s32 x = *cell++;
            s32 y = *cell++;

            flag -= 800;
            Map_CopyCells(3, 8, x, y, 1, 1);
            x = *cell++;
            y = *cell;
            Map_CopyCells(67, 3, x, y, 1, 5);
            if (flag == 1)
                Map_CopyCells(67, 4, 69, 3, 1, 1);
            Map_CopyCells(3, 69, x - 64, y + 66, 1, 4);
            return;
        }
        if (flag != 863) {
            offset += 8;
            flag++;
            goto next;
        }
    }
}
