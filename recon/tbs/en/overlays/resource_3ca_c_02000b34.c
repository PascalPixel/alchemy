/* NONMATCHING: 624 bytes, candidate 620, 253 differing halfwords, 153
 * halfword edits (2026-09-25). Scene_ClosePresentationSequence, meant for
 * FIELD/BABI_FUNE/F_00B34.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: Resolved each synthetic call to its real service and used
 * inline wrappers for repeated constants; palette ramp structure and final
 * stores remain nonmatching.
 * WALL: structural-topology: palette ramp and scene handoff */
#include "TYPES.H"

/* AUDITED GENERATED PRESENTATION FINALE for Scene_ClosePresentationSequence:
 * 35 calls, palette ramps, blend-register setup, and runtime handoff. */

void Main_0808a018(void);
void Main_0808a460(void);
void Main_0808a0d0(void);
void Main_0808a1b8(void);
void Main_0808a010(void);
void Main_080f9010(void);
void Main_080000d0(void);
void Main_080091f0(void);
void Main_080000c0(void);
void Main_080000d8(void);
void Main_080b0060(void);
void Main_0808a020(void);
void Scene_RunExtendedPresentationSequence(void);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Scene_ClosePresentationSequence(void)
{
    u8 *runtime;
    s32 base;
    s32 i1;
    s32 i2;
    s32 i3;
    s32 i4;
    s32 phase;

    runtime = *(u8 **)0x03001e70;

    Main_0808a018();
    Main_0808a460();
    Call3((void (*)())Main_0808a0d0, 0, 312, 232);
    Call3((void (*)())Main_0808a1b8, 0, 49152, 0);
    Call1((void (*)())Main_0808a010, 40);
    Call1((void (*)())Main_080f9010, 140);
    for (i1 = 0; i1 <= 15; i1++) {
        *(volatile u16 *)0x05000000 = (i1 << 11) | (i1 << 5);
        Call1((void (*)())Main_0808a010, 10);
    }
    *(volatile u16 *)0x05000000 = 0x7e00;
    for (i2 = 2; i2 >= 0; i2--) {
        Call1((void (*)())Main_080f9010, 212);
        *(volatile u16 *)0x04000050 = 3;
        Call1((void (*)())Main_0808a010, 3);
        *(volatile u16 *)0x04000050 = 0x810;
        Call1((void (*)())Main_0808a010, 65);
    }
    *(u32 *)0x020097e8 = 1;
    *(u32 *)0x020097ec = 0;
    Call4((void (*)())Main_080000d0, 33587605, 3200, 0, 33593324);
    *(u32 *)0x02009804 = 1;
    Call1((void (*)())Main_0808a010, 20);
    Call1((void (*)())Main_080f9010, 163);
    Call3((void (*)())Main_080091f0, 65536, 65536, 65536);
    Call1((void (*)())Main_0808a010, 60);
    Call3((void (*)())Main_080091f0, 131072, 131072, 65536);
    Call1((void (*)())Main_0808a010, 60);
    Call3((void (*)())Main_080091f0, 196608, 196608, 65536);
    *(u32 *)0x02009808 = 0;
    Call4((void (*)())Main_080000d0, 33587561, 3200, 0, 33593332);
    phase = 0;
    do {
        *(s32 *)(runtime + 320) += 0x3333;
        *(s32 *)(runtime + 368) += 0x3333;
        phase += 0x3333;
        Call1((void (*)())Main_080000c0, 1);
    } while (phase <= 0xe666);
    Call1((void (*)())Main_080000d8, 33587561);
    *(u32 *)0x020097f8 = 0;
    *(volatile u16 *)0x04000052 = (*(volatile u16 *)0x04000052 & 0xfffc) | 0x0a;
    *(volatile u16 *)0x04000050 = (*(volatile u16 *)0x04000050 & 0xfffc) | 0x0a;
    *(volatile u16 *)0x0400000e = (*(volatile u16 *)0x0400000e & 0xfffc) | 0x0400;
    *(u32 *)0x020097f4 = 0;
    Call1((void (*)())Main_080f9010, 288);
    Call1((void (*)())Main_080000c0, 1);
    Call1((void (*)())Main_080f9010, 145);
    *(volatile u16 *)0x04000054 = 191;
    for (i3 = 0; i3 <= 16; i3++) {
        *(volatile u16 *)0x05000000 = i3;
        Call1((void (*)())Main_0808a010, 1);
    }
    Call1((void (*)())Main_0808a010, 40);
    Call3((void (*)())Main_080091f0, -1, -1, 58982);
    *(u32 *)0x02009804 = *(u32 *)(runtime + 320);
    *(u32 *)0x02009808 = *(u32 *)(runtime + 368);
    *(u32 *)0x020097fc = 1;
    for (i4 = 16; i4 >= 0; i4--) {
        *(volatile u16 *)0x05000000 = i4;
        Call1((void (*)())Main_0808a010, 8);
    }
    Call2((void (*)())Main_080000d0, 33587377, 3200);
    Call1((void (*)())Main_080f9010, 80);
    Main_080b0060();
    Call1((void (*)())Main_0808a010, 20);
    Main_0808a020();
    Scene_RunExtendedPresentationSequence();
}
