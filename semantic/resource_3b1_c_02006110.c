#include "types.h"

























extern u8 *Data_03001ebc;

/*
 * resource_3b1 owner at 0x02006110, 668 bytes including its seven-word
 * trailing pool. It stages four scene actors, switches the scene state at
 * +448 from 0x201 to 0x202, then closes with two paired flag-array scans.
 * The sole return is at 0x0200638e. All 63 BL sites across 23 resolved targets
 * appear below in machine order.
 */
extern int Func_0200b0cc();
extern int Func_0200b0d6();
extern int Func_0200b0e0();
extern int Func_0200b0ea();
extern void Func_0200c610();
extern void Func_0200aa3a();
extern void Func_0200a9f2();
extern void Func_0200aa06();
extern void Func_0200c7b8();
extern void Func_0200c7cc();
extern void Func_0200c652();
extern void Func_0200c71a();
extern void Func_0200c748();
extern void Func_0200aa12();
extern void Func_0200c726();
extern void Func_0200c72e();
extern void Func_0200c736();
extern void Func_0200c746();
extern void Func_0200c68c();
extern void Func_0200c7a6();
extern void Func_0200c7b2();
extern void Func_0200c7bc();
extern void Func_0200c7c8();
extern void Func_0200c6fe();
extern void Func_0200c70c();
extern void Func_0200c728();
extern void Func_0200c764();
extern void Func_0200c770();
extern void Func_0200c77c();
extern void Func_0200c790();
extern void Func_0200c7c0();
extern void Func_0200c854();
extern void Func_0200c860();
extern void Func_0200c86c();
extern void Func_0200c878();
extern void Func_0200c830();
extern void Func_0200ab22();
extern void Func_0200c80e();
extern void Func_0200c816();
extern void Func_0200c81e();
extern void Func_0200c836();
extern void Func_0200ab48();
extern void Func_0200c834();
extern void Func_0200c83c();
extern void Func_0200c844();
extern void Func_0200c85e();
extern void Func_0200c8da();
extern void Func_0200ab92();
extern void Func_0200c87a();
extern void Func_0200c882();
extern void Func_0200c826();
extern void Func_0200c868();
extern void Func_0200c874();
extern void Func_0200c896();
extern void Func_0200c992();
extern void Func_0200c99e();
extern void Func_0200c712(u8 *, u8 *);
extern void Func_0200c71c(u8 *, u8 *);
extern void Func_0200c828();
extern void Func_0200c996();
void Func_02006110(void)
{
    int actor0;
    int actor1;
    int actor2;
    int actor3;

    actor0 = Func_0200b0cc(0, 0);
    actor1 = Func_0200b0d6(1, 0);
    actor2 = Func_0200b0e0(2, 0);
    actor3 = Func_0200b0ea(3, 0);
    Func_0200c610();
    Func_0200aa3a(10, 0, 0);
    Func_0200a9f2((void *)8, 472, 0x90, 0x5000);
    *(s32 *)(Data_03001ebc + 448) = 0x201;
    Func_0200aa06((void *)27, 0x198, 0x8e, 0x3000);
    Func_0200c7b8();
    Func_0200c7cc();
    Func_0200c652(40);
    Func_0200c71a(27, 1);
    Func_0200c748(0x1f78);
    Func_0200aa12(27);
    Func_0200c726(actor0, 2);
    Func_0200c72e(actor1, 2);
    Func_0200c736(actor2, 2);
    Func_0200c746(actor3, 2);
    Func_0200c68c(20);
    Func_0200c7a6(actor0, 0, 0);
    Func_0200c7b2(actor1, 0x8000, 0);
    Func_0200c7bc(actor2, 0, 0);
    Func_0200c7c8(actor3, 0x8000, 40);
    Func_0200c6fe(actor0, 0x10000, 0x8000);
    Func_0200c70c(actor1, 0x10000, 0x8000);
    Func_0200c71a(actor2, 0x10000, 0x8000);
    Func_0200c728(actor3, 0x10000, 0x8000);
    Func_0200c764(actor0, 0x1d6, 0xac);
    Func_0200c770(actor1, 0x19a, 0xac);
    Func_0200c77c(actor2, 0x1d6, 0xcc);
    Func_0200c790(actor3, 0x19a, 0xcc);
    Func_0200c7b8(actor0, 1);
    Func_0200c7c0(actor1, 1);
    Func_0200c7c8(actor2, 1);
    Func_0200c854(actor1, 0xd000, 0);
    Func_0200c860(actor0, 0xb000, 0);
    Func_0200c86c(actor3, 0xd000, 0);
    Func_0200c878(actor2, 0xb000, 20);
    Func_0200c830(27, 1);
    Func_0200ab22(27);
    Func_0200c80e(actor0, 3);
    Func_0200c816(actor1, 3);
    Func_0200c81e(actor2, 3);
    Func_0200c836(actor3, 3);
    Func_0200ab48(27);
    Func_0200c834(actor0, 3);
    Func_0200c83c(actor1, 3);
    Func_0200c844(actor2, 3);
    Func_0200c85e(actor3, 3);
    Func_0200c8da(27, 0, 0);
    Func_0200ab92(0, 0x8000);
    Func_0200c87a(0, 3);
    Func_0200c882(27, 3);
    Func_0200c826(27, 0x10000, 0x8000);
    Func_0200c868(27, 0x198, 0x84);
    Func_0200c874(27, 0x1bc, 0x84);
    *(s32 *)(Data_03001ebc + 448) = 0x202;
    Func_0200c896(27, 0, 0);
    Func_0200c992();
    Func_0200c99e();
    Func_0200c712((u8 *)0x92c, (u8 *)0x935);
    Func_0200c71c((u8 *)0x917, (u8 *)0x990);
    Func_0200c828(0x8a0);
    Func_0200c996(10);
}
