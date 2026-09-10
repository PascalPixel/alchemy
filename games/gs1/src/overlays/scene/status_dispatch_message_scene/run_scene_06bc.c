#include "types.h"

#define FieldScene_RunScene3b9_020006bc Func_020006bc

extern u8 Data_03001ebc[];
extern u8 Value_00002125[];

void Func_020032b4();
s32 Func_0200334e();
void Func_02003364();
s32 Func_02003374();
s32 Func_020032e4();
void Func_020033a0();
void Func_020032fc();

static __inline__ __attribute__((always_inline)) void bump_step(s32 amount)
{
    u8 *scene = *(u8 **)Data_03001ebc;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}

void FieldScene_RunScene3b9_020006bc(void)
{
    Func_020032b4();
    Func_0200334e(8, 0, 20);
    Func_02003364((s32)&Value_00002125);
    Func_02003374(8, 0);
    if (Func_020032e4(0, 0) != 0)
        bump_step(1);
    Func_020033a0(8, 0);
    Func_020032fc();
}
