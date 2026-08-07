#include "types.h"
extern u8 *Func_0808a080(int); extern void Func_0808a018(void); extern void Func_0808a170(int);
extern void Func_0808a100(int,int); extern void Func_0200173c(int,int,int); extern void Func_02001724(int,int);
extern void Func_080000c0(int); extern void Func_0808a020(void);

void Func_0200085c(void)
{
    u8 *actor = Func_0808a080(15);
    s16 facing = *(s16 *)(actor + 6);
    u16 *state = (u16 *)(actor + 100);

    *state |= 2;
    Func_0808a018();
    Func_0808a170(0x122d);
    Func_0808a100(15, 0);
    Func_0200173c(15, 0, 2);
    Func_02001724(15, 10);
    *(s16 *)(actor + 6) = facing;
    Func_080000c0(1);
    Func_0808a020();
    *state &= 1;
}
