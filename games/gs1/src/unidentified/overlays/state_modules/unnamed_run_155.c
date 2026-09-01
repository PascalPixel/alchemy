/* Contiguous unnamed state-owner run for resource_3c2. */

#include "types.h"

extern u8 Value_00002624;
extern void Func_02000da2(s32);
extern s32 Func_02000db2(s32, s32);
extern s32 Func_02000d4a(s32, s32);
extern void Func_02000d3c(s32);
extern void Func_02000dc2(s32);
extern void Func_02000dca(s32);
extern void Func_02000de2(s32, s32);

void Func_020001d4(s32 object)
{
    s32 cue = (s32) &Value_00002624;
    Func_02000da2(cue);
    Func_02000db2(object, 0);
    if (Func_02000d4a(0, 0) == 0) {
        Func_02000d3c(10);
        Func_02000dc2(cue + 1);
    } else {
        Func_02000dca(cue + 2);
    }
    Func_02000de2(object, 0);
}

#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_02000e2a(s32);
extern void Func_02000e68(s32);

void Func_0200021c(void)
{
    u8 *state = Data_03001ebc;
    s16 *counter = (s16 *)(state + 0x16C);

    Func_02000e2a(*counter);
    Func_02000e68(0x7B);
}

