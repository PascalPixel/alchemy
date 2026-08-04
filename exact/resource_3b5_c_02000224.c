#include "types.h"

extern u8 *Func_02000fa6();
extern u8 *Func_02000fb8(s32);
extern void Func_02000f66(u8 *, s32);
extern void Func_02000f7e(s32, s32, s32, s32);

void Func_02000224(void)
{
    u8 *actor = Func_02000fa6(8);

    if (actor != 0) {
        u8 *slot = actor + 89;

        *slot = 0;
    }
    Func_02000f66(Func_02000fb8(8), 0);
    Func_02000f7e(0, 0x2200000, 0x1200000, 0xFD);
    Func_02000fa6(0x200);
}
