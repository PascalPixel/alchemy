#include "types.h"

extern u8 *Func_020051d6(s32);
extern void Func_020051a4(void);
extern void Func_0200537e(s32, s32);
extern s32 Func_0200518e(s32);
extern void Func_02005298(s32);
extern void Func_02000846(s32);
extern void Func_020052a6(s32);
extern void Func_020052be(s32, s32);
extern void Func_020051ea(void);

void Func_02000454(void)
{
    s32 value = *(u16 *)(Func_020051d6(0) + 6);

    Func_020051a4();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_0200537e(6, 21);
    } else if (Func_0200518e(0x855) == 0) {
        Func_02005298(0x1284);
        Func_02000846(21);
    } else {
        Func_020052a6(0x1374);
        Func_020052be(21, 0);
    }
    Func_020051ea();
}
