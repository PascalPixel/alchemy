#include "types.h"

s32 Func_08079338(s32 arg0);

s32 Func_080795fc(void)
{
    s32 flag;
    s32 count;

    count = 0;
    flag = 0;
    do {
        if (Func_08079338(flag) != 0) {
            count += 1;
        }
        flag += 1;
    } while (flag <= 7);
    return count;
}
