#include "types.h"

extern s32 Func_02001bbe(s32);
extern void Func_02001b80(s32, s32);

void Func_0200007c(void)
{
    s32 handle = Func_02001bbe(8);

    if (handle != 0) {
        Func_02001b80(handle, 0);
    }
}
