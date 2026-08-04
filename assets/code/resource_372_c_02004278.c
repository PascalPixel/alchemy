#include "types.h"

extern void Func_02008978(void);
extern void Func_0200897c(s32);
extern s32 Func_02008ae4(s32, s32);
extern void Func_0200898c(s32);
extern void Func_020089b0(void);

void Func_02004278(void)
{
    u8 *state;

    Func_02008978();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    Func_0200897c(0x87f);
    Func_02008ae4(12, 3);
    Func_0200898c(0x900);
    Func_020089b0();
}
