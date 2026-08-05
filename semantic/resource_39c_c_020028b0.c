#include "types.h"

/*
 * Complete scene choreography for the actor-10 trigger at fixed-point tile
 * 51.  The script stages actors 0..3, advances dialogue/pose beats, performs
 * two runtime-dependent branches that update the shared skip counter, then
 * runs the palette, placement, and closing story-flag sequence.  The 128
 * calls below are kept in the exact independently resolved machine order.
 */
extern u8 *Data_03001ebc;

extern u8 *Func_0808a080();
extern void Func_0808a018();
extern void Func_0808a090();
extern void Func_0808a010();
extern void Func_0808a138();
extern void Func_0808a1b8();
extern void Func_0808a170();
extern void Func_0808a178();
extern s32 Func_0808a070();
extern void Func_0808a110();
extern void Func_0808a188();
extern void Func_0808a1e8();
extern void Func_02000030();
extern void Func_0808a0f0();
extern void Func_0808a0c8();
extern void Func_0808a0d0();
extern void Func_0808a0e8();
extern void Func_0808a100();
extern void Func_02004f44();
extern void Func_080f9010();
extern void Func_0808a338();
extern void Func_0808a330();
extern void Func_0808a348();
extern void Func_080000c0();
extern void Func_02002844();
extern void Func_08009180();
extern void Func_08009178();
extern void Func_080000d8();
extern void Func_0808a128();
extern void Func_080091c0();
extern void Func_02000048();
extern void Func_0808a0b8();
extern void Func_080770c8();
extern void Func_0808a020();

static void AdvanceSkipBeat(void)
{
    u16 *counter = (u16 *)(Data_03001ebc + 472);
    *counter = (u16)(*counter + 1);
}

void Func_020028b0(void)
{
    u8 *actor;
    s32 position;

    actor = Func_0808a080(10);
    if (position < 0)
        position += 0x000fffff;
    position = *(s32 *)(actor + 8);
    position >>= 20;

    Func_0808a018();
    if (position == 51) {
        Func_0808a090(3, 0xcccc, 0x6666);
        Func_0808a010(20);
        Func_0808a138(3, 2);
        Func_0808a010(20);
        Func_0808a1b8(3, 0xd000, 0);
        Func_0808a1b8(0, 0x5000, 10);
        Func_0808a170(0x157f);
        Func_0808a178(3, 0);

        if (Func_0808a070(0, 0) == 0) {
            AdvanceSkipBeat();
            Func_0808a010(20);
            Func_0808a110(3, 3);
            Func_0808a188(3, 0, 20);
        } else {
            Func_0808a010(20);
            Func_0808a110(3, 4);
            Func_0808a188(3, 0, 20);
            AdvanceSkipBeat();
        }

        Func_0808a1e8(0, 0x100, 60);
        actor = Func_0808a080(1);
        Func_02000030(actor, 1);
        actor = Func_0808a080(2);
        Func_02000030(actor, 1);
        Func_0808a090(1, 0xcccc, 0x6666);
        Func_0808a090(2, 0xcccc, 0x6666);
        Func_0808a0f0(1, 0x3680000, 0x2580000);
        Func_0808a0f0(2, 0x3680000, 0x2580000);
        Func_0808a0c8(2, 0x378, 0x278);
        Func_0808a0d0(1, 0x370, 0x268);
        Func_0808a1b8(1, 0x5000, 0);
        Func_0808a0e8(2);
        Func_0808a1b8(2, 0x8000, 0);
        Func_0808a138(1, 1);
        Func_0808a010(20);
        Func_0808a188(1, 0, 20);
        Func_0808a1e8(3, 0x101, 60);
        Func_0808a188(3, 0, 20);
        Func_0808a1b8(1, 0x3000, 0);
        Func_0808a1b8(2, 0xb000, 20);
        Func_0808a100(1, 3);
        Func_0808a110(2, 3);
        Func_0808a010(30);
        Func_0808a1b8(1, 0x5000, 0);
        Func_0808a1b8(2, 0x8000, 20);
        Func_0808a188(2, 0, 20);
        Func_0808a110(3, 3);
        Func_0808a010(20);
        Func_0808a188(3, 0, 20);
        Func_0808a0c8(3, 0x348, 0x290);
        Func_0808a010(5);
        Func_0808a1b8(2, 0x5000, 0);
        Func_0808a010(10);
        Func_0808a1b8(0, 0x4000, 0);
        Func_0808a0e8(3);
        Func_0808a010(10);
        Func_0808a188(3, 0, 20);
        Func_0808a1b8(3, 0xd000, 20);
        Func_0808a188(3, 0, 20);
        Func_0808a138(1, 1);
        Func_0808a010(20);
        Func_0808a1b8(1, 0xc000, 20);
        Func_0808a178(1, 0);

        if (Func_0808a070(0, 0) == 0) {
            Func_0808a010(20);
            Func_0808a110(1, 3);
            Func_0808a188(1, 0, 20);
            AdvanceSkipBeat();
        } else {
            AdvanceSkipBeat();
            Func_0808a010(20);
            Func_0808a110(1, 4);
            Func_0808a188(1, 0, 20);
        }

        Func_0808a100(3, 16);
        Func_0808a010(30);
        Func_0808a138(3, 1);
        Func_0808a010(20);
        Func_0808a188(3, 0, 20);
        Func_0808a0d0(3, 0x348, 0x278);
        Func_0808a1b8(0, 0x5000, 0);
        Func_0808a1b8(1, 0x5000, 0);
        Func_0808a1b8(2, 0x8000, 0);
        Func_0808a010(30);
        Func_02004f44();
        Func_0808a010(50);
        Func_080f9010(131);
        Func_0808a338(0x10000, 0);
        Func_0808a330(0x207e9f, 0);
        Func_0808a348(10);
        Func_080000c0(1);
        Func_080f9010(220);
        Func_080000c0(40);
        Func_0808a330(0x10000, 0);
        Func_0808a348(60);
        Func_080000c0(60);
        Func_080f9010(209);
        Func_02002844();
        Func_08009180(126, 35, 116, 35, 1, 2);
        Func_08009178((void *)0x0200e0b8, 116, 35);
        Func_080000d8((void *)0x0200a649);
        Func_0808a010(20);
        Func_0808a110(3, 3);
        Func_0808a010(20);
        Func_0808a090(3, 0x30000, 0x18000);
        Func_0808a128(3, 4, 0);
        Func_0808a0c8(3, 0x348, 0x258);
        Func_0808a0e8(3);
        Func_080091c0(116, 36, 3, 4, 52, 36);
        Func_02000048(0x3480000, 0x380000, 0x2600000, 223);
        Func_0808a090(3, 0xcccc, 0x6666);
        Func_0808a0d0(3, 0x348, 0x230);
        Func_0808a0f0(3, 0, 0);
        Func_0808a010(20);
        Func_0808a1b8(0, 0x4000, 0);
        Func_0808a1b8(1, 0x8000, 0);
        Func_0808a1b8(2, 0xb000, 0);
        Func_0808a010(10);
        Func_0808a100(0, 3);
        Func_0808a100(1, 3);
        Func_0808a110(2, 3);
        Func_0808a010(20);
        Func_0808a100(1, 2);

        actor = Func_0808a080(0);
        if (actor != 0)
            Func_0808a0b8(1, *(s16 *)(actor + 10), *(s16 *)(actor + 18));

        Func_0808a100(2, 2);
        actor = Func_0808a080(0);
        if (actor != 0)
            Func_0808a0b8(2, *(s16 *)(actor + 10), *(s16 *)(actor + 18));

        Func_0808a0e8(1);
        Func_0808a0f0(1, 0, 0);
        Func_0808a0e8(2);
        Func_0808a0f0(2, 0, 0);
        Func_080770c8(0x871);
    }

    Func_0808a020();
}
