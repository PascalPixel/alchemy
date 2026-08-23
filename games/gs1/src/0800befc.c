#include "types.h"

s32 WaitFrames(s32);
void Func_0800be70(u8 *object, u32 position);

void Func_0800befc(u8 *object)
{
    u32 pos;

    pos = 0;
    do {
        Func_0800be70(object, pos);
        Func_0800be70(object, pos + 1);
        Func_0800be70(object, pos + 2);
        Func_0800be70(object, pos + 3);
        pos += 4;
        WaitFrames(1);
    } while (pos <= 0x7f);
}
