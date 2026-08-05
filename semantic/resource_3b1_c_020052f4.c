#include "types.h"

void Func_0200486c();
void Func_02004880();
void Func_02004890();
void Func_020048ac();
void Func_020048e8();
void Func_080770d0();
void Func_0808a010();
int Func_0808a070();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0b0();
void Func_0808a0c0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a188();
void Func_0808a1b8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a248();
void Func_0808a360();
void Func_0808a370();

extern u8 *Data_03001ebc;

/*
 * resource_3b1 owner at 0x020052f4, 912 bytes including its thirteen-word
 * trailing pool. It stages five actors and actor 27, preserves the answer
 * branch's scene-counter increment, and finishes by publishing state 0x209.
 * The sole return is at 0x0200564e. All 80 BL sites across 27 resolved targets
 * appear below in machine order.
 */
void Func_020052f4(void)
{
    Func_02004890((void *)0, 0x1bc, 0x12c, 0);
    Func_02004890((void *)1, 0x1ca, 0x136, 0);
    Func_02004890((void *)2, 0x1bc, 0x14a, 0);
    Func_02004890((void *)3, 0x1b0, 0x136, 0);
    Func_02004890((void *)27, 0x1b8, 0x86, 0x8000);
    Func_02004890((void *)10, 0x1c6, 0xf8, 0x3000);
    Func_0808a100(10, 6);
    Func_020048ac(0x01b80000, -1, 0x01340000, 0x01000001);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a1b8(1, 0xa000, 0);
    Func_0808a1b8(2, 0xa000, 0);
    Func_0808a1b8(3, 0x2000, 40);
    Func_020048e8(2, 1, 20);
    Func_0808a170(0x1e6e);
    Func_0200486c(27);
    Func_020048e8(1, 0xc000, 0);
    Func_0808a208(0x26666, 0x4ccc);
    Func_0808a210(0x01b80000, -1, 0x00b00000, 1);
    Func_0808a090(27, 0x19999, 0xcccc);
    Func_0808a0d0(27, 0x198, 0x86);
    Func_0808a0d0(27, 0x198, 0x98);
    Func_0808a0d0(27, 0x1a8, 0xa4);
    Func_0808a208(0x19999, 0x3333);
    Func_0808a210(0x01b80000, -1, 0x012c0000, 1);
    Func_0808a0d0(27, 0x1a8, 0xde);
    Func_0808a0d0(27, 0x1a8, 0x106);
    Func_0808a1b8(27, 0x3000, 20);
    Func_0808a138(27, 1);
    Func_0200486c(27);
    Func_020048e8(2, 1, 20);
    Func_0808a110(27, 3);
    Func_0808a138(27, 1);
    Func_0200486c(27);
    Func_020048e8(3, 2, 60);
    Func_020048e8(1, 0xe000, 60);
    Func_0808a1b8(27, 0, 40);
    Func_0808a138(27, 1);
    Func_0808a100(27, 2);
    Func_0808a0c0(27, 0x1b0, 0x10c);
    Func_0808a0c0(27, 0x1c4, 0x10c);
    Func_0808a100(27, 1);
    Func_02004880(27, 0xd000);
    Func_0808a130(27, 2);
    Func_0808a188(27, 0, 20);
    Func_020048e8(1, 0xc000, 20);
    Func_0808a110(27, 4);
    Func_0808a010(40);
    Func_0808a188(27, 0, 80);
    Func_0808a138(27, 1);
    Func_0808a010(20);
    Func_0808a110(27, 3);
    Func_0808a010(10);
    Func_02004880(27, 0x5000);
    Func_0808a178(27, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a110(27, 3);
        Func_0200486c(27);
    } else {
        Func_0808a110(27, 4);
        Func_0200486c(27);
        *(u16 *)(Data_03001ebc + 472) += 1;
        Func_020048e8(3, 2, 40);
        Func_0808a138(27, 1);
        Func_0808a100(27, 3);
        Func_0200486c(27);
    }

    Func_020048e8(2, 1, 20);
    Func_0808a098(1, 0x0200e7f0);
    Func_0808a098(2, 0x0200e7f0);
    Func_0808a0b0(3, 0x0200e7f0);
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0x01b80000, -1, 0x00b00000, 1);
    Func_0808a090(0, 0x10000, 0x8000);
    *(s32 *)(Data_03001ebc + 448) = 0x209;
    Func_0808a0d0(0, 0x1a8, 0x110);
    Func_0808a0c8(0, 0x1a8, 0xa4);
    Func_0808a010(60);
    Func_020048e8(9, 0, 0);
    Func_080770d0(0x301);
    Func_080770d0(0x927);
    Func_0808a248(4);
}
