#include "types.h"
extern u8 *Func_0808a080(s32);
extern void Func_0808a0f0(s32, s32, s32);
extern void Func_080091b8(s32, s32, s32, s32, s32, s32);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_0808a3c0(s32, s32, s32);
extern void Func_080770d0(s32);
extern void Func_0808a100(s32, s32);
extern void Func_08009240(u8 *, s32);
extern void Func_080000d8(u8 *);
extern void Func_080770c8(s32);
void Func_020005f0(void)
{
    u8 *actor;
    Func_0808a080(0);
    Func_0808a0f0(19, 0, 0);
    Func_0808a0f0(8, 0, 0);
    Func_080091b8(38, 38, 1, 1, 46, 4);
    Func_080091c0(37, 37, 3, 3, 13, 3);
    Func_080091c0(37, 37, 1, 1, 14, 2);
    Func_080091c0(8, 16, 1, 1, 7, 16);
    Func_0808a3c0(102, 0, 0);
    Func_080091b8(32, 42, 3, 2, 1, 15);
    Func_080770d0(0x200);
    Func_0808a100(8, 1);
    *(s32 *)(Func_0808a080(8) + 108) = 0;
    actor = Func_0808a080(8);
    Func_08009240(actor, 0);
    Func_080000d8((u8 *)0x02008469);
    Func_080770c8(0x201);
}
