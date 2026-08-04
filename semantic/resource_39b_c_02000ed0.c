#include "types.h"

extern u8 *Func_0808a080();
extern void Func_0808a018();
extern void Func_080f9010();
extern void Func_0808a100();
extern void Func_0808a0e0();
extern void Func_0808a0e8();
extern void Func_0808a158();
extern void Func_080091e0();
extern void Func_0808a010();
extern void Func_0808a368();
extern void Func_0808a370();
extern void Func_0808a248();
extern void Func_0808a020();

/* Run actor 0's launch-and-settle sequence, then return to the selected page. */
void Func_02000ed0(s32 page)
{
    u8 *actor = Func_0808a080(0);
    Func_0808a018();
    Func_080f9010(228);
    *(void **)(actor + 108) = (void *)0x02008cc1;
    *(s32 *)(actor + 48) = 0x3333;
    Func_0808a100(0, 2);
    Func_0808a0e0(0, 0, -6);
    Func_0808a0e8(0);
    Func_0808a158(0, 15);
    actor = Func_0808a080(0);
    Func_080091e0(actor, 0);
    *(void **)(actor + 108) = 0;
    Func_0808a010(30);
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(page);
    Func_0808a020();
}
