#include "types.h"

extern u8 *Func_020009ea(s32);
extern void Func_020009c8(void);
extern void Func_02000a6a(s32, s32);
extern void Func_02000a3a(s32);
extern void Func_02000a52(s32, s32);
extern void Func_020009f6(void);

void Func_02000380(void)
{
    u32 place;

    place = *(u16 *) (Func_020009ea(0) + 6);
    Func_020009c8();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000a6a(4, 19);
    } else {
        Func_02000a3a(0x1CE2);
        Func_02000a52(19, 0);
    }

    Func_020009f6();
}
