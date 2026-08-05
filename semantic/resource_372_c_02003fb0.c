/* Linear actor-22 scene and camera setup. */
#include "types.h"
u8 *Func_0808a080();
void Func_0808a018(); void Func_0808a210(); void Func_0808a0a8();
void Func_080000d8(); void Func_0808a0d0(); void Func_0808a0f0();
void Func_0808a1b8(); void Func_080000c0(); void Func_0808a170();
void Func_0808a180(); void Func_0808a208(); void Func_0808a218();
void Func_0808a010(); void Func_0808a110(); void Func_0808a188();
void Func_0808a138(); void Func_0808a090(); void Func_080770c8();

void Func_02003fb0(void)
{
    u8 *actor;
    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_0808a0a8(22);
    Func_080000d8((void *)0x0200c5b9);
    Func_0808a0d0(0, 0x1e0, 0x570);
    Func_0808a0f0(0, 0, 0);
    Func_0808a1b8(22, 0x3000, 20);
    actor[35] |= 1;
    actor = Func_0808a080(22);
    Func_0808a0f0(22, 0x00f90000, 0x04d80000);
    Func_080000c0(1);
    Func_0808a170(0xed3);
    Func_0808a180(0x1016, 0);
    Func_0808a0f0(22, 0x00ac0000, 0x04fe0000);
    Func_080000c0(1);
    Func_0808a208(0x40000, 0x8000);
    Func_0808a210(0x00a20000, 0, 0x05050000, 1);
    Func_0808a218();
    Func_0808a010(40);
    Func_0808a110(22, 4);
    Func_0808a188(0x1016, 0, 10);
    Func_0808a1b8(22, 0xc000, 20);
    Func_0808a138(22, 2);
    Func_0808a188(0x1016, 0, 10);
    Func_0808a1b8(22, 0x1000, 20);
    Func_0808a110(22, 3);
    Func_0808a090(22, 0x20000, 0x10000);
    Func_0808a0d0(22, 0xa5, 0x514);
    Func_0808a0d0(22, 0xc3, 0x598);
    Func_080770c8(0x842);
}
