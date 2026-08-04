#include "types.h"

/*
 * resource_3b0 scripted scene owner at 0x02000564, complete 588-byte span
 * through alignment and its seventeen-word pool before 0x020007b0.  No
 * reviewed skeleton homolog exists; all 53 calls were resolved independently.
 */
extern u8 *Data_03001ebc;
extern void Func_080000c0();
extern void Func_08009150();
extern void Func_080091e0();
extern void Func_080770c8();
extern void Func_0808a010();
extern void Func_0808a018();
extern void Func_0808a020();
extern void Func_0808a030();
extern u8 *Func_0808a080();
extern void Func_0808a090();
extern void Func_0808a098();
extern void Func_0808a0a8();
extern void Func_0808a0b8();
extern void Func_0808a0d0();
extern void Func_0808a0f0();
extern void Func_0808a128();
extern void Func_0808a130();
extern void Func_0808a138();
extern void Func_0808a158();
extern void Func_0808a170();
extern void Func_0808a188();
extern void Func_0808a1b8();
extern void Func_0808a1e0();
extern void Func_0808a1e8();
extern void Func_0808a248();
extern void Func_0808a360();
extern void Func_0808a368();
extern void Func_0808a370();
extern void Func_080f9010();

void Func_02000564(void)
{
    s32 *origin = *(s32 **)(*(u8 **)0x03001e70);
    u8 *actor;

    Func_0808a018();
    Func_0808a030((void *)0x020096f4);
    Func_080000c0(1);
    Func_0808a158(0, 15);
    actor = Func_0808a080(0);
    Func_080091e0(actor, 0);
    Func_0808a098(8, (void *)0x0200939c);
    *(s32 *)(Data_03001ebc + 448) = 0x203;
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);

    *(s32 *)0x02009938 = origin[0];
    *(s32 *)0x0200993c = origin[1];
    Func_0808a0f0(9, 0x00500000, 0x00d20000, origin[1]);
    actor = Func_0808a080(9);
    actor[85] = 0;
    *(s32 *)0x02009930 = 0x00500000;
    *(s32 *)0x02009934 = 0;
    Func_0808a098(9, (void *)0x02009314);
    Func_0808a010(20);
    Func_080f9010(29);
    Func_080770c8(0x8f0);
    Func_0808a0a8(8);
    Func_080000c0(1);
    Func_0808a1e8(8, 0x100, 0);
    Func_0808a1b8(8, 0xb000, 0);
    Func_0808a170(0x1e3e);
    Func_0808a188(8, 0, 10);

    Func_0808a0f0(10, 0x00500000, 0x00d20000);
    Func_0808a0f0(11, 0x00500000, 0x00d20000);
    Func_0808a0f0(12, 0x00500000, 0x00d20000);
    Func_0808a1e0(10, 3);
    Func_0808a1e0(11, 3);
    Func_0808a1e0(12, 3);
    Func_0808a158(10, 3);
    Func_0808a158(11, 3);
    Func_0808a158(12, 3);

    actor = Func_0808a080(10);
    *(s32 *)(actor + 24) = 0x8000;
    *(s32 *)(actor + 28) = 0x8000;
    *(u32 *)(actor + 108) = 0x020080a5;
    actor = Func_0808a080(11);
    *(s32 *)(actor + 24) = 0x8000;
    *(s32 *)(actor + 28) = 0x8000;
    *(u32 *)(actor + 108) = 0x020080a5;
    actor = Func_0808a080(12);
    *(s32 *)(actor + 24) = 0x8000;
    *(s32 *)(actor + 28) = 0x8000;
    *(u32 *)(actor + 108) = 0x020080a5;
    Func_080000c0(1);
    Func_0808a090(10, 0x851e, 0x428f);
    Func_0808a090(11, 0x7333, 0x3999);
    Func_0808a090(12, 0x9999, 0x4ccc);
    Func_0808a0b8(10, 0x100, 0x159);
    Func_0808a0b8(11, 0x88, 0x14a);
    Func_0808a0b8(12, 0x9c, 0x154);
    Func_0808a010(60);
    Func_0808a138(8, 2);
    Func_0808a0d0(8, 0xa4, 0x158);
    Func_0808a128(8, 4, 10);
    Func_0808a128(8, 6, 40);
    Func_0808a130(8, 3);
    Func_0808a188(8, 0, 20);
    *(s32 *)(Data_03001ebc + 448) = 0x202;
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(11);
    Func_0808a020();
}
