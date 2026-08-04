#include "resource_397.h"

typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;

extern void Func_02000496(s32);
extern void Func_0200048c(s32);

void Func_02000138(void)
{
    u8 *state = Data_03001ebc;
    s16 *counter;

    Func_02000496(0x7B);
    counter = (s16 *)(state + 0x16C);
    Func_0200048c(*counter);
}
