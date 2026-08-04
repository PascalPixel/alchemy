#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_020017aa(s32);

void Func_02000964(void)
{
    s16 *counter = (s16 *)(Data_03001ebc + 0x16C);

    Func_020017aa(*counter);
}
