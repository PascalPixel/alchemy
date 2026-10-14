/* Draft, differ 10 of 30: as a labelled loop (a for loop strength-reduces the flag id, differ 33), only group * 4 is left: gcse hoists it out of the loop into r7. Tried u32/u16/u8/s16 group and a spelled-out multiply; alchemy permute matched only by taking the address of group. */
#include "types.h"
s32 GameFlag_Test(s32 flag);
s32 Func_02000b60(s32 group)
{
    s32 i = 17;

loop:
    {
        s32 flag = group * 20 + i + 48;

        if (group == 0)
        {
            if (i == 7)
                goto next;
        }
        if (group == 1)
        {
            if (i == 9)
                goto next;
            if (i == 10)
                goto next;
        }
        if (GameFlag_Test(flag))
            return i + 1;
    }
next:
    if (--i > 6)
        goto loop;
    return -1;
}
