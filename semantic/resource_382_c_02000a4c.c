#include "types.h"
extern u8 *Func_0808a080(int); extern void Func_0808a018(void); extern void Func_0808a170(int); extern int Func_080770c0(int);
extern void Func_0808a100(int,int); extern void Func_0200173c(int,int,int); extern void Func_02001724(int,int);
extern void Func_080000c0(int); extern void Func_0808a020(void);
void Func_02000a4c(void)
{
    u8 *actor = Func_0808a080(14); s16 facing = *(s16 *)(actor + 6); u16 *state = (u16 *)(actor + 100);
    *state |= 2; Func_0808a018(); Func_0808a170(0x1339);
    if (Func_080770c0(2) != 0) { u8 *workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Func_0808a100(14, 0); Func_0200173c(14, 0, 2); Func_02001724(14, 10);
    *(s16 *)(actor + 6) = facing; Func_080000c0(1); Func_0808a020(); *state &= 1;
}
