#include "types.h"

extern u8 *Func_02000a32(s32);
extern void Func_02000a10(void);
extern void Func_02000ab2(s32, s32);
extern void Func_02000a82(s32);
extern void Func_02000a9a(s32, s32);
extern void Func_02000a3e(void);

void Func_020003c8(void)
{
    u32 place;

    place = *(u16 *) (Func_02000a32(0) + 6);
    Func_02000a10();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000ab2(5, 20);
    } else {
        Func_02000a82(0x1CE4);
        Func_02000a9a(20, 0);
    }

    Func_02000a3e();
}
