#include "types.h"

int Func_02004fa8();
void Func_020037b4();
void Func_02004670();
void Func_0200486c();
void Func_02004880();
void Func_020048ac();
void Func_020048e8();
void Func_080091e0();
int Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
int Func_0808a080();
void Func_0808a088();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0b0();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a158();
void Func_0808a170();
void Func_0808a1b8();
void Func_0808a1e8();

extern u8 *Data_03001ebc;

/*
 * resource_3b1 owner at 0x02005d10, 1,024 bytes including its fourteen-word
 * trailing pool. Four scene slots are arranged around actor 27, and two
 * successive flag groups encode the outcome in the scene counter at +472.
 * The sole return is at 0x020060d6. All 87 BL sites across 29 resolved targets
 * appear below in machine order.
 */
void Func_02005d10(void)
{
    int actor0;
    int actor1;
    int actor2;
    int actor3;
    int outcome;
    int record;

    actor0 = Func_02004fa8(0, 0);
    actor1 = Func_02004fa8(1, 0);
    actor2 = Func_02004fa8(2, 0);
    actor3 = Func_02004fa8(3, 0);
    Func_0808a018();
    Func_020037b4();
    Func_020048e8(10, 0, 0);
    Func_020048e8(17, 0, 0);
    Func_0808a0f0(8, 0x01d80000, 0x00980000);
    Func_0808a0f0(27, 0x01b80000, 0x00860000);
    Func_0808a158(27, 15);
    record = Func_0808a080(27);
    Func_080091e0(record, 0);
    Func_02004670(16);
    Func_0808a100(9, 5);
    Func_020048ac(0x01b60000, -1, 0x00ae0000, 0x01000001);
    Func_020048e8(8, 1, 20);
    Func_0808a158(27, 0);
    record = Func_0808a080(27);
    Func_080091e0(record, 1);
    Func_0808a090(27, 0x10000, 0x8000);
    Func_0808a0d0(27, 0x198, 0x84);
    Func_0808a0d0(27, 0x198, 0x8e);
    Func_0808a1b8(27, 0x3000, 20);
    Func_0808a130(27, 2);
    Func_0808a170(0x1f29);
    Func_0200486c(27);
    Func_0808a010(120);
    Func_020048e8(12, actor0, 0);
    Func_020048e8(12, actor1, 1);
    Func_020048e8(12, actor2, 0);
    Func_020048e8(12, actor3, 1);
    Func_020048e8(11, 0, 0);
    Func_0808a1b8(actor0, 0xd000, 0);
    Func_0808a1b8(actor1, 0xb000, 0);
    Func_0808a1b8(actor2, 0xd000, 0);
    Func_0808a1b8(actor3, 0xb000, 60);

    outcome = 0;
    if (Func_080770c0(0x934) != 0)
        outcome = 2;
    else if (Func_080770c0(0x933) != 0 || Func_080770c0(0x92f) != 0)
        outcome = 1;

    Func_0808a138(actor0, 1);
    if (outcome == 1)
        *(u16 *)(Data_03001ebc + 472) += 1;
    else if (outcome == 2)
        *(u16 *)(Data_03001ebc + 472) += 2;
    Func_0808a130(actor0, 2);
    Func_0200486c(actor0);
    Func_0808a170(0x1f2d);
    Func_0808a110(27, 4);
    Func_0200486c(27);
    Func_0808a1e8(actor0, 0x102, 0);
    Func_0808a1e8(actor1, 0x102, 0);
    Func_0808a1e8(actor2, 0x102, 0);
    Func_0808a1e8(actor3, 0x102, 60);
    Func_0200486c(27);
    Func_0808a0d0(27, 0x198, 0x84);
    Func_0808a0d0(27, 0x1bc, 0x84);
    Func_0808a088(27);
    Func_0808a010(40);

    if (outcome == 0) {
        if (Func_080770c0(0x92c) != 0 || Func_080770c0(0x92d) != 0)
            outcome = 3;
    }
    if (outcome == 0)
        *(u16 *)(Data_03001ebc + 472) += 1;
    else if (outcome == 1)
        *(u16 *)(Data_03001ebc + 472) += 2;
    else if (outcome == 2)
        *(u16 *)(Data_03001ebc + 472) += 3;

    Func_02004880(actor0, 0);
    Func_0200486c(actor0);
    Func_0808a090(actor0, 0x10000, 0x8000);
    Func_0808a0b0(actor0, 0x0200e904);
    Func_0808a1b8(actor1, 0x5000, 0);
    Func_0808a1b8(actor2, 0, 0);
    Func_0808a1b8(actor3, 0x8000, 40);
    Func_0808a1b8(actor1, 0xd000, 0);
    Func_0808a1b8(actor2, 0xb000, 0);
    Func_0808a1b8(actor3, 0x5000, 20);
    Func_0808a090(actor1, 0x10000, 0x8000);
    Func_0808a090(actor2, 0x10000, 0x8000);
    Func_0808a090(actor3, 0x10000, 0x8000);
    Func_0808a098(actor2, 0x0200e904);
    Func_0808a010(40);
    Func_0808a0b0(actor1, 0x0200e938);
    Func_0808a098(actor1, 0x0200e904);
    Func_0808a0b0(actor3, 0x0200e938);
    Func_0808a0b0(actor3, 0x0200e904);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a090(3, 0x10000, 0x8000);
    Func_0808a098(1, 0x0200e7c8);
    Func_0808a098(2, 0x0200e7c8);
    Func_0808a0b0(3, 0x0200e7c8);
    Func_020048e8(23, 0, 0);
    Func_080770d0(0x927);
    Func_080770c8(0x8a0);
    Func_080770d0(0x12f);
    Func_0808a020();
}
