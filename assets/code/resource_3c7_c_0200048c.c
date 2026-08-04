#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_0200175a(s32);
extern void Func_020017b0(s32);

void Func_0200048c(void)
{
    u8 *state = Data_03001ebc;
    s16 *counter = (s16 *)(state + 0x16C);

    Func_0200175a(*counter);
    Func_020017b0(0x7B);
}
