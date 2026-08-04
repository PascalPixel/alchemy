#include "types.h"
extern void Func_0808a018(void); extern void Func_0808a1e8(s32,s32,s32); extern void Func_0808a010(s32);
extern void Func_0808a0c8(s32,s32,s32); extern void Func_0808a0e8(s32); extern void Func_0808a0a8(s32);
extern void Func_0808a100(s32,s32); extern void Func_0808a1b8(s32,s32,s32); extern void Func_0808a170(s32);
extern void Func_0808a180(s32,s32); extern void Func_080770c8(s32); extern void Func_080091c0(s32,s32,s32,s32,s32,s32);
extern void Func_0808a020(void);
void Func_02000eb0(void)
{
    if (*(s16 *)(0x02000240 + 588) != 0)
        return;
    Func_0808a018();
    Func_0808a1e8(8, 0x100, 2);
    Func_0808a1e8(9, 0x100, 15);
    Func_0808a010(30);
    Func_0808a0c8(8, 152, 168);
    Func_0808a0c8(9, 168, 168);
    Func_0808a0e8(8);
    Func_0808a0e8(9);
    Func_0808a0a8(8);
    Func_0808a100(8, 0);
    Func_0808a1b8(8, 0x3000, 0);
    Func_0808a0a8(9);
    Func_0808a100(9, 0);
    Func_0808a1b8(9, 0x5000, 0);
    Func_0808a170(0x24da);
    Func_0808a180(8, 0);
    Func_080770c8(0x240);
    Func_080091c0(6, 11, 1, 1, 7, 11);
    Func_080091c0(6, 11, 1, 1, 8, 11);
    Func_080091c0(6, 11, 1, 1, 9, 11);
    Func_0808a020();
}
