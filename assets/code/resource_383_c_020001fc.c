#include "types.h"

extern u8 *Func_02004f7e(s32);
extern void Func_02004f4c(void);
extern void Func_0200512e(s32, s32);
extern s32 Func_02004f36(s32);
extern void Func_02005040(s32);
extern void Func_02005048(s32);
extern void Func_02005060(s32, s32);
extern void Func_02004f8c(void);

void Func_020001fc(void)
{
    s32 value = *(u16 *)(Func_02004f7e(0) + 6);

    Func_02004f4c();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_0200512e(1, 23);
    } else {
        if (Func_02004f36(0x855) == 0) {
            Func_02005040(0x128d);
        } else {
            Func_02005048(0x137b);
        }
        Func_02005060(23, 0);
    }
    Func_02004f8c();
}
