#include "types.h"

extern void Func_02002c44(s32, s32, s32, s32, s32, s32);
extern void Func_02002c76(s32, s32, s32, s32, s32, s32);

void Func_020013b4(void)
{
    {
        s32 extent = 2;

        Func_02002c44(22, 85, 25, 85, extent, extent);
    }
    {
        s32 extent = 25;

        Func_02002c76(25, 15, 2, 2, extent, extent);
    }
}
