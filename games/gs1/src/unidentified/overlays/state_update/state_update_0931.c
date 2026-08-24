#include "types.h"

extern s32 Func_0200b9e2(s32);
extern u8 *Func_0200b9ee(s32);
extern u8 *Func_0200ba42(s32);
extern void Func_0200ba14(s32);
extern s32 Func_0200ba0e(s32);
extern u8 *Func_0200ba60(s32);
extern void Func_0200ba2c(s32);
extern u8 Value_00000241;

void Func_02005a40(void)
{
    if (Func_0200b9e2(0x241) != 0) {
        u8 *p = Func_0200b9ee(0x106);
        if (p == 0) {
            u8 *q = Func_0200ba42(22);
            q[0x5b] = 0;
            Func_0200ba14(0x241);
        }
    } else {
        if (Func_0200ba0e(0x106) != 0) {
            u8 *q = Func_0200ba60(22);
            q[0x5b] = 1;
            Func_0200ba2c(0x241);
        }
    }
}
