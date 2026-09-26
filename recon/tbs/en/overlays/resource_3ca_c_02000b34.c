/* NONMATCHING: 624 bytes, candidate 624, 24 differing halfwords, 23
 * halfword edits (2026-09-26). Scene_ClosePresentationSequence, meant for
 * FIELD/BABI_FUNE/F_00B34.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Complete extent 02000b34..02000da4: pool 0cac..0ce8, return 0d86,
 * final pool 0d88..0da0. Current pools and complete layout match.
 * Three structural trials, starting from 620 / 253 / 153:
 * 1. Reconstructed the actual handoff: BLDALPHA 1010/0810, flag f8 set
 *    twice, flag f4 cleared, two-argument task callbacks, ramp through
 *    59ffff, BG3/BG2/BG1 priorities 3/3/2 via volatile stack halfword,
 *    flag e8 cleared, BLDCNT bf and BLDY ramps. Produced 616 / 242 / 112.
 * 2. Separate extern globals instead of neighbouring absolute casts restored
 *    independent loads and the retained f8 pointer: 628 / 182 / 88.
 * 3. Separate bright/dim loop constants and initialize priority two only at
 *    its phase: 624 / 25 / 23, with all literal pools matching.
 * Reopened bounded structural trials (2026-09-26):
 * 1. Inline priority-store helper: 620 / 172 / 131; added volatile stack
 *    reloads, changed priority constants and moved pools. Rejected.
 * 2. Bright/dim constants outside an explicit countdown loop: 624 / 24 / 23.
 *    Retained: counter initialization now follows constant setup as in ROM.
 * 3. One changing priority halfword: 624 / 91 / 40; extended its lifetime,
 *    displaced the retained zero and removed a later counter reset. Rejected.
 * Remaining: first palette shifts reversed, blink-loop argument setup and
 * decrement scheduled early, priority-three versus stack-address r5/r4
 * swap, priority-two temporary, and final BLDCNT/BLDY setup scheduling.
 * Stop at three structural hypotheses. No credit until every byte matches. */
#include "TYPES.H"

extern u32 Data_020097e8;
extern u32 Data_020097ec;
extern u32 Data_020097f4;
extern u32 Data_020097f8;
extern u32 Data_020097fc;
extern u32 Data_02009804;
extern u32 Data_02009808;

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
    volatile u16 cnt;

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
    {
        s32 color = 0x7e00;

        *(volatile u16 *)0x05000000 = color;
    }
    {
        s32 bright = 0x1010;
        s32 dim = 0x810;

        i2 = 2;
        do {
            Call1((void (*)())Main_080f9010, 212);
            *(volatile u16 *)0x04000052 = bright;
            Call1((void (*)())Main_0808a010, 3);
            *(volatile u16 *)0x04000052 = dim;
            i2--;
            Call1((void (*)())Main_0808a010, 65);
        } while (i2 >= 0);
    }
    Data_020097e8 = 1;
    Data_020097ec = 0;
    Call2((void (*)())Main_080000d0, 33587605, 3200);
    Data_020097f8 = 1;
    Call1((void (*)())Main_0808a010, 20);
    Call1((void (*)())Main_080f9010, 163);
    Call3((void (*)())Main_080091f0, 65536, 65536, 65536);
    Call1((void (*)())Main_0808a010, 60);
    Data_020097f8 = 1;
    Call3((void (*)())Main_080091f0, 131072, 131072, 65536);
    Call1((void (*)())Main_0808a010, 60);
    Call3((void (*)())Main_080091f0, 196608, 196608, 65536);
    Data_020097f4 = 0;
    Call2((void (*)())Main_080000d0, 33587561, 3200);
    phase = 0;
    do {
        *(s32 *)(runtime + 320) += 0x3333;
        *(s32 *)(runtime + 368) += 0x3333;
        phase += 0x3333;
        Call1((void (*)())Main_080000c0, 1);
    } while (phase <= 0x59ffff);
    Call1((void (*)())Main_080000d8, 33587561);
    Data_020097f8 = 0;
    {
        /* FAKEMATCH: halfword priority locals retain the short pool reach. */
        struct Half { u16 v; } three, two;

        three.v = 3;
        cnt = (*(volatile u16 *)0x0400000e & 0xfffc) | three.v;
        *(volatile u16 *)0x0400000e = cnt;
        cnt = (*(volatile u16 *)0x0400000c & 0xfffc) | three.v;
        *(volatile u16 *)0x0400000c = cnt;
        two.v = 2;
        cnt = (*(volatile u16 *)0x0400000a & 0xfffc) | two.v;
        *(volatile u16 *)0x0400000a = cnt;
    }
    Data_020097e8 = 0;
    Call1((void (*)())Main_080f9010, 288);
    Call1((void (*)())Main_080000c0, 1);
    Call1((void (*)())Main_080f9010, 145);
    {
        s32 blend = 191;

        *(volatile u16 *)0x04000050 = blend;
    }
    for (i3 = 0; i3 <= 16; i3++) {
        *(volatile u16 *)0x04000054 = i3;
        Call1((void (*)())Main_0808a010, 1);
    }
    Call1((void (*)())Main_0808a010, 40);
    Call3((void (*)())Main_080091f0, -1, -1, 58982);
    Data_02009804 = *(u32 *)(runtime + 320);
    Data_02009808 = *(u32 *)(runtime + 368);
    Data_020097fc = 1;
    for (i4 = 16; i4 >= 0; i4--) {
        *(volatile u16 *)0x04000054 = i4;
        Call1((void (*)())Main_0808a010, 8);
    }
    Call2((void (*)())Main_080000d0, 33587377, 3200);
    Call1((void (*)())Main_080f9010, 80);
    Main_080b0060();
    Call1((void (*)())Main_0808a010, 20);
    Main_0808a020();
    Scene_RunExtendedPresentationSequence();
}
