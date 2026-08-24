#include "types.h"
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))

extern u8 *Func_020033c4(s32);
extern void Func_020033ba(void);
extern void Func_020034f8(s32);
extern void Func_02003428(s32, s32);
extern void Func_0200341c(s32, s32, s32);
extern void Func_0200342a(s32);
extern void Func_02003462(s32, s32);
extern s32 Func_02003400(s32);
extern void Func_020033a6(s32, s32);
extern void Func_020033f8(s32);
extern void Func_02003504(void);
extern void Func_02003510(void);
extern void Func_020034d6(s32);
extern void Func_0200341a(void);
extern u8 Value_02008cc1;

void Func_02000ed0(s32 a)
{
    u8 *v = Func_020033c4(0);
    Func_020033ba();
    Func_020034f8(0xe4);
    F(v, s32, 0x6c) = (s32)&Value_02008cc1;
    F(v, s32, 0x30) = 0x3333;
    Func_02003428(0, 2);
    Func_0200341c(0, 0, -6);
    Func_0200342a(0);
    Func_02003462(0, 15);
    Func_020033a6(Func_02003400(0), 0);
    F(v, s32, 0x6c) = 0;
    Func_020033f8(30);
    Func_02003504();
    Func_02003510();
    Func_020034d6(a);
    Func_0200341a();
}
