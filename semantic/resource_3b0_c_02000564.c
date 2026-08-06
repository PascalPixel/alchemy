#include "types.h"

/*
 * resource_3b0 scripted scene owner at 0x02000564, complete 588-byte span
 * through alignment and its seventeen-word pool before 0x020007b0.  No
 * reviewed skeleton homolog exists; all 53 calls were resolved independently.
 */
extern u8 *Data_03001ebc;






























extern void Func_0200178a();
extern void Func_020017a0();
extern void Func_02001736();
extern void Func_0200180e();
extern u8 * Func_020017c4();
extern void Func_020017e2();
extern void Func_020018a0();
extern void Func_020018b4();
extern void Func_020017ca();
extern void Func_0200183a();
extern u8 * Func_02001810();
extern void Func_02001838();
extern void Func_02001806();
extern void Func_0200190c();
extern void Func_0200180c();
extern void Func_0200185a();
extern void Func_020017c8();
extern void Func_020018cc();
extern void Func_020018c8();
extern void Func_020018be();
extern void Func_020018d0();
extern void Func_020018ac();
extern void Func_020018b8();
extern void Func_020018c4();
extern void Func_02001914();
extern void Func_0200191c();
extern void Func_02001904();
extern u8 * Func_020018ca();
extern u8 * Func_020018dc();
extern u8 * Func_020018e8();
extern void Func_02001874();
extern void Func_02001906();
extern void Func_02001910();
extern void Func_0200191a();
extern void Func_0200193c();
extern void Func_02001948();
extern void Func_02001954();
extern void Func_02001912();
extern void Func_0200198a();
extern void Func_02001976();
extern void Func_02001990();
extern void Func_0200199a();
extern void Func_020019aa();
extern void Func_020019d4();
extern void Func_02001a42();
extern void Func_02001a4e();
extern void Func_02001a2c();
extern void Func_02001978();
void Func_02000564(void)
{
    s32 *origin = *(s32 **)(*(u8 **)0x03001e70);
    u8 *actor;

    Func_0200178a();
    Func_020017a0((void *)0x020096f4);
    Func_02001736(1);
    Func_0200180e(0, 15);
    actor = Func_020017c4(0);
    *(s32 *)(Data_03001ebc + 448) = 0x203;
    Func_0200178a(actor, 0);
    Func_020017e2(8, (void *)0x0200939c);
    Func_020018a0();
    Func_020018b4();
    Func_020017ca(20);

    *(s32 *)0x02009930 = 0x00500000;
    Func_0200183a(9, 0x00500000, 0x00d20000, origin[1]);
    *(s32 *)0x02009938 = origin[0];
    *(s32 *)0x0200993c = origin[1];
    *(s32 *)0x02009934 = 0;
    actor = Func_02001810(9);
    actor[85] = 0;
    Func_02001838(9, (void *)0x02009314);
    Func_02001806(20);
    Func_0200190c(29);
    Func_0200180c(0x8f0);
    Func_0200185a(8);
    Func_020017c8(1);
    Func_020018cc(8, 0x100, 0);
    Func_020018c8(8, 0xb000, 0);
    Func_020018be(0x1e3e);
    Func_020018d0(8, 0, 10);

    Func_020018ac(10, 0x00500000, 0x00d20000);
    Func_020018b8(11, 0x00500000, 0x00d20000);
    Func_020018c4(12, 0x00500000, 0x00d20000);
    Func_0200190c(10, 3);
    Func_02001914(11, 3);
    Func_0200191c(12, 3);
    Func_02001904(10, 3);
    Func_0200190c(11, 3);
    Func_02001914(12, 3);

    actor = Func_020018ca(10);
    *(s32 *)(actor + 24) = 0x8000;
    *(u32 *)(actor + 108) = 0x020080a5;
    *(s32 *)(actor + 28) = 0x8000;
    *(s32 *)(actor + 24) = 0x8000;
    actor = Func_020018dc(11);
    *(u32 *)(actor + 108) = 0x020080a5;
    *(s32 *)(actor + 28) = 0x8000;
    actor = Func_020018e8(12);
    *(s32 *)(actor + 24) = 0x8000;
    *(s32 *)(actor + 28) = 0x8000;
    *(u32 *)(actor + 108) = 0x020080a5;
    Func_02001874(1);
    Func_02001906(10, 0x851e, 0x428f);
    Func_02001910(11, 0x7333, 0x3999);
    Func_0200191a(12, 0x9999, 0x4ccc);
    Func_0200193c(10, 0x100, 0x159);
    Func_02001948(11, 0x88, 0x14a);
    Func_02001954(12, 0x9c, 0x154);
    Func_02001912(60);
    Func_0200198a(8, 2);
    Func_02001976(8, 0xa4, 0x158);
    Func_02001990(8, 4, 10);
    Func_0200199a(8, 6, 40);
    Func_020019aa(8, 3);
    Func_020019d4(8, 0, 20);
    *(s32 *)(Data_03001ebc + 448) = 0x202;
    Func_02001a42();
    Func_02001a4e();
    Func_02001a2c(11);
    Func_02001978();
}
