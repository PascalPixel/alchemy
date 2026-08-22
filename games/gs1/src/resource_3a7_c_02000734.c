#include "types.h"

extern u8 *Func_02001870(s32);

void Func_02000734(void)
{
    s32 index = 16;
    s32 flag = 1;
    s32 remaining = 5;

    do {
        u8 *entry = Func_02001870(index);

        remaining--;
        entry[35] = flag;
        index++;
    } while (remaining >= 0);
}
