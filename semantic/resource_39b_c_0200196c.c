#include "types.h"

extern int Func_080770c0();
extern void Func_080770c8();
extern void Func_0808a018();
extern u8 *Func_0808a080();
extern void Func_0808a0f0();
extern void Func_0808a1b8();
extern void Func_0808a360();
extern void Func_0808a370();
extern void Func_0808a010();
extern void Func_0808a148();
extern void Func_0808a110();
extern void Func_0808a0c8();
extern void Func_0808a138();
extern void Func_0808a0e8();
extern void Func_0808a020();

/* One-time positioning bridge that hands actor 3 from flag 0x250 to 0x872. */
void Func_0200196c(void)
{
    u8 *actor;
    u8 *workspace;

    s32 permuted_11;
    if (Func_080770c0(0x250) != 0) return;
    Func_080770c8(0x250);
    Func_0808a018();
    actor = Func_0808a080(12); *(int *)(actor + 24) = -0x10000;
    actor = Func_0808a080(13); *(int *)(actor + 24) = -0x10000;
    actor = Func_0808a080(14); *(int *)(actor + 24) = -0x10000;
    Func_0808a0f0(3, 0x880000, 0x900000);
    Func_0808a1b8(3, 0x10000, 10);
    permuted_11 = *(u8 **)0x03001ebc;
    *(int *)(workspace + 448) = 0x201;
    workspace  = permuted_11;
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(60);
    Func_0808a148(3, 0, 0);
    Func_0808a110(3, 3);
    Func_0808a010(30);
    Func_0808a0c8(3, 136, 72);
    Func_0808a010(40);
    Func_0808a138(0, 1);
    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);
    Func_080770c8(0x872);
    *(int *)(workspace + 448) = 0x204;
    Func_0808a020();
}
