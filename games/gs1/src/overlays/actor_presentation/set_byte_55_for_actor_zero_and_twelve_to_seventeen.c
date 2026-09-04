#include "types.h"

#define SceneActor_SetByte55ForActorZeroAnd12To17 Func_020006c0

u8 *Func_02006412(s32);
u8 *Func_0200641e(s32);
u8 *Func_0200642a(s32);
u8 *Func_02006434(s32);
u8 *Func_0200643e(s32);
u8 *Func_02006448(s32);
u8 *Func_02006452(s32);

void SceneActor_SetByte55ForActorZeroAnd12To17(void)
{
    s32 val;

    *(Func_02006412(0) + 0x55) = 3;
    val = 4;
    *(Func_0200641e(12) + 0x55) = val;
    *(Func_0200642a(13) + 0x55) = val;
    *(Func_02006434(14) + 0x55) = val;
    *(Func_0200643e(15) + 0x55) = val;
    *(Func_02006448(16) + 0x55) = val;
    *(Func_02006452(17) + 0x55) = val;
}
