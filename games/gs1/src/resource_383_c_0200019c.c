#include "types.h"

extern u8 *Func_02004f1e(s32);
extern void Func_02004eec(void);
extern void Func_020050c6(s32, s32);
extern s32 Func_02004ed6(s32);
extern void Func_02004fe0(s32);
extern void Func_02004fe8(s32);
extern void Func_02005000(s32, s32);
extern void Func_02004f2c(void);

void Func_0200019c(void)
{
    s32 value = *(u16 *)(Func_02004f1e(0) + 6);

    Func_02004eec();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_020050c6(5, 20);
    } else {
        if (Func_02004ed6(0x855) == 0) {
            Func_02004fe0(0x1282);
        } else {
            Func_02004fe8(0x1372);
        }
        Func_02005000(20, 0);
    }
    Func_02004f2c();
}
