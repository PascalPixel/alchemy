#include "types.h"

/*
 * resource_3a6 owner at 0x02001748, complete 40-byte span through its one-word
 * pool: play cue 123, then dispatch the signed scene value at workspace +364.
 */

extern u8 *Data_03001ebc;



extern void Func_020034b8(s32 cue);
extern void Func_020034a6(s32 value);
void Func_02001748(void)
{
    s32 value = *(s16 *)(Data_03001ebc + 364);

    Func_020034b8(123);
    Func_020034a6(value);
}
