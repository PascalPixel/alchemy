/* Draft, differ 66 of 48: the native keeps the output cursor in a stack slot
 * (str r2, [sp]; stmia r3!, {r6}; str back) although r9 and r11 are free, and
 * tests the limit before the loop; this form keeps the cursor in a register. */
#include "types.h"

s32 GameFlag_Test(s32 flag);

s32 Func_02000ba0(s32 limit, s32 group, s32 *out)
{
    s32 count = 0;
    s32 i;

    for (i = 7; i < limit; i++) {
        s32 flag;

        if (group == 0 && i == 7)
            continue;
        if (group == 1 && (i == 9 || i == 10))
            continue;
        flag = group * 20 + i + 48;
        if (GameFlag_Test(flag) == 0) {
            *out++ = flag;
            count++;
        }
    }
    return count;
}
