#include "types.h"

void Func_0200486c();
void Func_02004880();
void Func_020048e8();
void Func_080770c8();
void Func_080772e8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
int Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a170();
void Func_0808a178();
void Func_0808a188();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a460();

extern u8 *Data_03001ebc;

/*
 * resource_3b1 owner at 0x020027d8, 2,072 bytes: the second half of the large
 * multi-actor scene.  Three answer branches update the scene counter at +472;
 * nullable record copies and the final teardown are preserved explicitly.
 * The sole return is followed by fifteen owner-local pool words through
 * 0x02002fef.  All 203 BL sites across 30 targets appear in machine order.
 */
void Func_020027d8(void)
{
    u8 *record;

    Func_0808a018();
    Func_0808a460();
    Func_0808a148(8, 0, 0);
    Func_0808a1e8(8, 0x100, 40);
    Func_0808a130(8, 3);
    Func_0808a170(0x1ddb);
    Func_0200486c(0x1008);
    Func_0808a130(9, 1);
    Func_0808a130(12, 1);
    Func_0808a130(11, 1);
    Func_0808a130(13, 1);
    Func_0808a138(10, 1);
    Func_0808a1b8(9, 0xd000, 0);
    Func_0808a1b8(12, 0xd000, 0);
    Func_0808a1b8(11, 0xd000, 0);
    Func_0808a1b8(13, 0xd000, 0);
    Func_0808a1b8(10, 0xb000, 20);
    Func_0808a138(8, 1);
    Func_0808a178(0x1008, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a138(9, 2);
        Func_0200486c(0x9009);
        Func_0808a1e8(8, 0x108, 40);
        *(u16 *)(Data_03001ebc + 472) += 2;
        Func_0200486c(0x1008);
    } else {
        *(u16 *)(Data_03001ebc + 472) += 2;
        Func_0808a138(9, 1);
        Func_0200486c(0x9009);
        Func_0808a130(8, 2);
        Func_0200486c(0x9008);
    }

    Func_0808a1e8(13, 0x105, 40);
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(0x01d80000, -1, 0x027c0000, 1);
    Func_0808a090(13, 0x10000, 0x8000);
    Func_0808a0d0(13, 472, 0x296);
    Func_02004880(13, 0xb000);
    Func_0200486c(13);
    Func_02004880(8, 0x5000);
    Func_0808a110(8, 3);
    Func_0808a110(9, 3);
    Func_0808a1b8(11, 0, 0);
    Func_0808a1b8(13, 0x8000, 20);
    Func_0808a100(11, 3);
    Func_0808a110(13, 3);
    Func_0808a010(20);
    Func_0808a138(12, 1);
    Func_02004880(12, 0x3000);
    Func_0808a188(0x100c, 0, 20);
    Func_0808a1b8(11, 0xb000, 20);
    Func_0808a1e8(11, 0x101, 40);
    Func_0200486c(11);
    Func_02004880(12, 0xd000);
    Func_0808a100(12, 4);
    Func_0200486c(0x900c);
    Func_02004880(13, 0xb000);
    Func_0808a138(13, 1);
    Func_0200486c(13);
    Func_0808a1e8(9, 0x100, 20);
    Func_02004880(9, 0x3000);
    Func_0808a138(9, 1);
    Func_0200486c(9);
    Func_0808a110(12, 3);
    Func_0200486c(0x900c);
    Func_0808a138(8, 2);
    Func_0200486c(8);
    Func_02004880(12, 0xd000);
    Func_0808a110(12, 3);
    Func_0200486c(0x900c);
    Func_0808a138(11, 2);
    Func_02004880(11, 0xb000);
    Func_0200486c(11);
    Func_02004880(12, 0);
    Func_0200486c(0x900c);
    Func_0808a1b8(8, 0x3000, 0);
    Func_0808a1b8(9, 0, 0);
    Func_0808a1b8(11, 0xd000, 0);
    Func_0808a1b8(13, 0xd000, 0);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a0d0(0, 486, 608);
    Func_0808a1b8(0, 0x8000, 0);

    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a0d0(1, 486, 624);
    Func_0808a1b8(1, 0x8000, 0);
    record = Func_0808a080(1);
    if (record != 0)
        Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a0d0(2, 486, 640);
    Func_0808a1b8(2, 0x8000, 0);
    record = Func_0808a080(2);
    if (record != 0)
        Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    Func_0808a090(3, 0x10000, 0x8000);
    Func_0808a0d0(3, 486, 656);
    Func_0808a1b8(3, 0x8000, 20);

    Func_0808a1e8(12, 0x108, 40);
    Func_0200486c(0x900c);
    Func_0808a138(9, 1);
    Func_0200486c(0x1009);
    Func_0808a110(8, 3);
    Func_02004880(8, 0x5000);
    Func_0200486c(8);
    Func_02004880(8, 0x3000);
    Func_0808a178(8, 0);

    if (Func_0808a070(0, 0) == 1) {
        Func_0808a130(8, 2);
        Func_0200486c(8);
        Func_0808a110(12, 3);
        Func_0200486c(0x900c);
        Func_0808a130(9, 1);
        Func_0808a188(0x9009, 0, 40);
        *(u16 *)(Data_03001ebc + 472) += 1;
    } else {
        *(u16 *)(Data_03001ebc + 472) += 3;
        Func_0808a130(8, 3);
        Func_0808a188(8, 0, 40);
    }

    Func_0808a138(13, 1);
    Func_0200486c(13);
    Func_0808a138(8, 1);
    Func_02004880(8, 0x5000);
    Func_0200486c(8);
    Func_0808a138(13, 1);
    Func_02004880(13, 0xb000);
    Func_0808a188(13, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a090(8, 0xcccc, 0x6666);
    Func_0808a0d0(8, 472, 632);
    Func_0200486c(0x4008);
    Func_0808a1e8(13, 0x103, 40);
    Func_0808a130(13, 2);
    Func_0200486c(13);
    Func_0808a100(8, 4);
    Func_0808a188(0x4008, 0, 40);
    Func_0808a138(11, 1);
    Func_02004880(11, 0xb000);
    Func_0200486c(0x100b);
    Func_0808a1e8(10, 0x102, 20);
    Func_0808a090(10, 0x26666, 0x13333);
    Func_0808a128(10, 2, 0);
    Func_0808a0d0(10, 462, 0x2a2);
    Func_02004880(10, 0xb000);
    Func_0808a130(10, 2);
    Func_0200486c(10);
    Func_02004880(9, 0x5000);
    Func_0808a110(9, 4);
    Func_0200486c(9);
    Func_0808a110(8, 3);
    Func_0200486c(0x4008);
    Func_0808a1e8(13, 0x102, 40);
    Func_0808a188(13, 0, 40);
    Func_02004880(9, 0x3000);
    Func_0808a130(9, 2);
    Func_0200486c(0x1009);
    Func_02004880(12, 0);
    Func_0808a1b8(8, 0x8000, 0);
    Func_0808a1b8(9, 0x5000, 0);
    Func_0808a1b8(11, 0xb000, 0);
    Func_0808a1b8(13, 0xb000, 0);
    Func_0808a1b8(10, 0xb000, 20);
    Func_0808a138(12, 1);
    Func_0808a188(0x100c, 0, 20);
    Func_0808a1e8(8, 0x101, 40);
    Func_02004880(8, 0xd000);
    Func_0808a178(0x1008, 0);

    if (Func_0808a070(0, 0) == 0) {
        *(u16 *)(Data_03001ebc + 472) += 1;
        Func_0808a110(8, 3);
        Func_0200486c(0x1008);
    } else {
        *(u16 *)(Data_03001ebc + 472) += 1;
        Func_0200486c(0x1008);
    }

    Func_0808a110(0, 3);
    Func_0808a110(8, 3);
    Func_0200486c(0x1008);
    Func_02004880(8, 0x8000);
    Func_0200486c(0x4008);
    Func_020048e8(2, 0, 0);
    Func_0808a100(12, 3);
    Func_0808a100(11, 3);
    Func_0808a100(9, 3);
    Func_0808a130(10, 2);
    Func_0808a138(13, 2);
    Func_0808a010(20);
    Func_0808a098(10, (void *)0x0200e6e4);
    Func_0808a010(4);
    Func_0808a098(11, (void *)0x0200e6e4);
    Func_0808a010(4);
    Func_0808a098(12, (void *)0x0200e6e4);
    Func_0808a010(4);
    Func_0808a098(9, (void *)0x0200e6e4);
    Func_0808a100(3, 2);

    record = Func_0808a080(2);
    if (record != 0)
        Func_0808a0b8(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);
    Func_0808a100(2, 2);
    record = Func_0808a080(1);
    if (record != 0)
        Func_0808a0b8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);
    Func_0808a100(1, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a098(13, (void *)0x0200e6e4);
    Func_0808a0d0(8, 456, 648);
    Func_02004880(8, 0);
    Func_080772e8(232);
    Func_080770c8(0x925);
    Func_0808a020();
}
