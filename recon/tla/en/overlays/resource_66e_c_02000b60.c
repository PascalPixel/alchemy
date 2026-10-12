/* Draft, differ 33 of 30: the native builds the flag id (group * 20 + i + 48)
 * straight into r0 at the top of every pass and keeps i in r5; this form
 * strength-reduces the id into its own register. Nested tests keep the two
 * separate compares of 9 and 10 that the native has. */
#include "types.h"

s32 GameFlag_Test(s32 flag);

s32 Func_02000b60(s32 group)
{
    s32 i;

    for (i = 17; i > 6; i--) {
        s32 flag = group * 20 + i + 48;

        if (group == 0) {
            if (i == 7)
                continue;
        }
        if (group == 1) {
            if (i == 9)
                continue;
            if (i == 10)
                continue;
        }
        if (GameFlag_Test(flag))
            return i + 1;
    }
    return -1;
}
