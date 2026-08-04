#include "types.h"

extern u8 *Func_02004e04(s32);
extern u8 *Func_02004e14(s32);
extern u8 *Func_02004e3e(s32);
extern u8 *Func_02004e4e(s32);
extern void Func_02004f32(void);
extern void Func_02004d9e(s32, s32, s32, s32, s32, s32);
extern void Func_02004f6c(void);
extern void Func_02004dd8(s32, s32, s32, s32, s32, s32);

void Func_02000080(void)
{
    s32 x;
    s32 z;

    if (*(u16 *)(Func_02004e04(0) + 6) >= 0xa000
        && *(u16 *)(Func_02004e14(0) + 6) <= 0xe000) {
        Func_02004f32();
        x = 42;
        z = 85;
        Func_02004d9e(41, 85, 1, 1, x, z);
    } else if (*(u16 *)(Func_02004e3e(0) + 6) >= 0x2000
               && *(u16 *)(Func_02004e4e(0) + 6) <= 0x6000) {
        Func_02004f6c();
        x = 42;
        z = 85;
        Func_02004dd8(43, 85, 1, 1, x, z);
    }
}
