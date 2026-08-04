#include "types.h"

extern s32 Func_020047e2(void);

s32 Func_02000570(u8 *a)
{
    u32 v = ((u32)(Func_020047e2() << 5)) >> 16;

    if (v == 6) {
        s32 t = 0xD0;
        *(u16 *)(a + 6) = t << 8;
    } else if (v == 9) {
        s32 t = 0xB0;
        *(u16 *)(a + 6) = t << 8;
    }
    return 1;
}
