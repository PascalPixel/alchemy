#include "types.h"

extern void Func_0200863e(s32, s32, s32, s32);
extern void Func_020084b2(void);
extern void Func_020085a8(s32, s32, s32);
extern u8 *Func_0200854e(s32);
extern void Func_020084aa(s32);

void Func_02004218(void)
{
    Func_0200863e(0xE80000, -1, 0x2A40000, 0);
    Func_020084b2();
    Func_020085a8(0, 0xE80000, 0x2A40000);
    {
        s32 x = 0x80;

        *(u16 *)(Func_0200854e(0) + 6) = x << 7;
    }
    Func_020084aa(1);
}
