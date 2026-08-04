#include "types.h"

extern void Func_02003ae4(void);
extern s32 Func_02003b02(s32);
extern void Func_02003ab0(s32, s32);
extern void Func_02003b84(void);
extern void Func_02003b20(s32, s32, s32);
extern void Func_02003b34(s32, s32, s32);
extern void Func_02003b18(void);

void Func_020017a8(void)
{
    Func_02003ae4();
    Func_02003ab0(Func_02003b02(8), 0);
    Func_02003b84();
    Func_02003b20(0, 0x20000, 0x1999);
    Func_02003b34(0, 0x108, 0xC4);
    Func_02003b18();
}
