#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_02000b16(void);

void Func_0200009c(void)
{
    u8 *state = Data_03001ebc;

    *(s32 *)(state + 0x1C0) = 0x201;
    *(s32 *)(state + 0x1C8) = 24;
    Func_02000b16();
}
