#include "types.h"

extern u8 *Data_03001ebc;
extern s32 Data_02000240[];

extern s32 Func_0200565e(s32);

void Func_020019e0(void)
{
    u8 *state = Data_03001ebc;
    s32 v = Data_02000240[125];

    if (v != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == v
        && Func_0200565e(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 t = 99;

        *p = t;
    }
}
