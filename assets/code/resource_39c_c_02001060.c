#include "types.h"

extern u8 *Data_03001ebc;
extern void Func_02001ff8(s32);

void Func_02001060(void)
{
    Func_02001ff8(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}
