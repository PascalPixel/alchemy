#include "types.h"

extern const u8 Data_020044b8[];
extern const u8 Data_020045b8[];

s32 Func_02002a58(s32 id, s32 a, s32 b, s32 angle, const void *path);

void Func_020014f4(void)
{
    Func_02002a58(9, 18, 20, 0, Data_020044b8);
    Func_02002a58(9, 17, 19, 0x8000, Data_020045b8);
}
