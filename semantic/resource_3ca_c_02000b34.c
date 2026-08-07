#include "types.h"

/*
 * Resource 3ca cutscene script at 0x02000b34 (624 bytes,
 * 0x02000b34-0x02000da3).
 *
 * Complete owner.  Prologue at 0x02000b34 saves r5-r7 and lr, then r8/sl, and
 * opens an 8-byte frame; the matching unwind at 0x02000d7a restores all of
 * them and returns through `pop {r0} / bx r0`.  The popped register is r0, so
 * it holds the return address and the owner is `void`.
 *
 * Two literal pools sit inside the span; the map comes from walking the
 * owner's control flow, not from a referenced-words listing:
 *   0x02000caa-0x02000ceb, hopped by the bare `b.n 0x02000cec` at 0x02000ca8,
 *   0x02000d88-0x02000da3, after the epilogue.
 * The first hop is the case the pool guard exists for: r0 is loaded with
 * 144 << 1 = 288 at 0x02000c9e/0x02000ca6, *before* the branch, and is the
 * argument of the Func_080f9010 call at 0x02000cec on the far side.  Register
 * state carries across the pool.
 *
 * Pool contents worth naming:
 *   0x02000cb4 0x03001e70   IWRAM record-block pointer cell
 *   0x02000cc8 0x02008195   Func_02000194 + Thumb bit
 *   0x02000cd4 0x02008169   Func_02000168 + Thumb bit
 *   0x02000cdc 0x0059ffff   the sweep's end value
 *   0x02000da0 0x020080b1   Func_020000b0 + Thumb bit
 * The three odd in-image words are all passed to Func_080000d0/Func_080000d8,
 * so each is simultaneously a witness for the 0x02008000 link base and the
 * name of a per-frame task: 0x2195 - 0x8000 = file offset 0x194 (the scene
 * driver), 0x169 (a row already byte-exact in assets/code), 0xb1 (the palette
 * cycle).  This owner is therefore the scene's script: it installs each task,
 * runs a timed sequence, and tears the tasks down again.
 *
 * Calls.  34 `bl` sites, all resolved with tools/lib/overlay_call_targets.ts (the
 * row's `calls=31` is the usual undercount).  Placed calls per target, which
 * is the completeness proof — a total alone would not catch a permutation:
 *   Func_0808a010 x11, Func_080f9010 x6, Func_080091f0 x4, Func_080000d0 x3,
 *   Func_080000c0 x2, Func_080000d8 x1, Func_0808a018 x1, Func_0808a020 x1,
 *   Func_0808a0d0 x1, Func_0808a1b8 x1, Func_0808a460 x1, Func_080b0060 x1,
 *   Func_02000430 x1.  Total 34.
 * Five of the Func_0808a010 sites and one Func_080f9010 site sit inside loops,
 * so they are one C call expression each and must not be unrolled — doing so
 * would inject phantom calls into that multiset.
 *
 * UNCERTAINTIES.  (1) Func_0808a010 is read as "wait n frames" from the way
 * every call site separates two visible state changes; the name is not
 * otherwise pinned down.  (2) The halfword stores to sp+6 at 0x02000c80,
 * 0x02000c8e and 0x02000ca0 are never read back and the frame is discarded on
 * return; they are dead and are omitted.  (3) Func_0808a018, Func_0808a460,
 * Func_0808a020 and Func_080b0060 are reached with no register deliberately
 * set, so they are spelled with no arguments.
 */

extern s32 Data_020097e8;
extern s32 Data_020097ec;
extern s32 Data_020097f4;
extern s32 Data_020097f8;
extern s32 Data_020097fc;
extern s32 Data_02009804;
extern s32 Data_02009808;

/* Main-image imports reached through this overlay's veneer table.  Old-style
 * declarations are mandatory: import arity varies between call sites. */
void Func_0808a010();   /* wait n frames */
void Func_0808a018();
void Func_0808a020();
void Func_0808a0d0();
void Func_0808a1b8();
void Func_0808a460();
void Func_080f9010();
void Func_080091f0();
void Func_080000c0();   /* wait one frame */
void Func_080000d0();   /* install a per-frame task */
void Func_080000d8();   /* remove a per-frame task */
void Func_080b0060();

/* In-image callees. */
void Func_020000b0(void);
void Func_02000168(void);
void Func_02000194(void);
void Func_02000430(void);

void Func_02000b34(void)
{
    u8 *records;
    s32 i;
    s32 swept;

    records = *(u8 **)0x03001E70;

    Func_0808a018();
    Func_0808a460();
    Func_0808a0d0(0, 312, 232);           /* 156 << 1 = 312 */
    Func_0808a1b8(0, 0xc000, 0);          /* 192 << 8       */
    Func_0808a010(40);
    Func_080f9010(140);

    /* Ramp the backdrop colour up over 16 frames.  The packed value is
     * (i << 11) | (i << 5): the blue and green channels together, red left at
     * zero. */
    for (i = 0; i <= 15; i++) {
        *(volatile u16 *)0x05000000 = (u16)((i << 11) | (i << 5));
        Func_0808a010(10);
    }
    *(volatile u16 *)0x05000000 = 0x7e00;  /* 252 << 7 */

    /* Three alpha flashes: hold 0x1010 for three frames, 0x0810 for 65. */
    for (i = 2; i >= 0; i--) {
        Func_080f9010(212);
        *(volatile u16 *)0x04000052 = 0x1010;
        Func_0808a010(3);
        *(volatile u16 *)0x04000052 = 0x0810;
        Func_0808a010(65);
    }

    /* Enable the blend fade in the scene driver, then install it. */
    Data_020097e8 = 1;
    Data_020097ec = 0;
    Func_080000d0(Func_02000194, 3200);

    Data_020097f8 = 1;                    /* let the driver spawn objects */
    Func_0808a010(20);
    Func_080f9010(163);

    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(60);
    Data_020097f8 = 1;
    Func_080091f0(0x20000, 0x20000, 0x10000);
    Func_0808a010(60);
    Func_080091f0(0x30000, 0x30000, 0x10000);

    Data_020097f4 = 0;
    Func_080000d0(Func_02000168, 3200);

    /*
     * Sweep both coordinates by 0x3333 per frame until the accumulated shift
     * passes 0x0059ffff — 29 frames.  The accumulator lives in the frame
     * because Func_080000c0 clobbers the register; the assembly reloads it
     * from sp on every iteration for that reason.
     */
    swept = 0;
    do {
        *(s32 *)(records + 320) += 0x3333;
        *(s32 *)(records + 368) += 0x3333;
        swept += 0x3333;
        Func_080000c0(1);
    } while (swept <= 0x0059ffff);

    Func_080000d8(Func_02000168);

    /* Backgrounds back to priority 3, 3, 2 and both driver gates off. */
    Data_020097f8 = 0;
    *(volatile u16 *)0x0400000E = (u16)((*(volatile u16 *)0x0400000E & 0xfffc) | 3);
    *(volatile u16 *)0x0400000C = (u16)((*(volatile u16 *)0x0400000C & 0xfffc) | 3);
    Data_020097e8 = 0;
    *(volatile u16 *)0x0400000A = (u16)((*(volatile u16 *)0x0400000A & 0xfffc) | 2);

    /* r0 = 288 was set before the pool hop; this is its call site. */
    Func_080f9010(288);
    Func_080000c0(1);
    Func_080f9010(145);

    /* Fade to white over 17 frames, hold, then fade back over 17. */
    *(volatile u16 *)0x04000050 = 0xbf;
    for (i = 0; i <= 16; i++) {
        *(volatile u16 *)0x04000054 = (u16)i;
        Func_0808a010(1);
    }
    Func_0808a010(40);
    Func_080091f0(-1, -1, 0xe666);

    Data_02009804 = *(s32 *)(records + 320);
    Data_02009808 = *(s32 *)(records + 368);
    Data_020097fc = 1;                    /* hand the camera to the driver */

    for (i = 16; i >= 0; i--) {
        *(volatile u16 *)0x04000054 = (u16)i;
        Func_0808a010(8);
    }

    Func_080000d0(Func_020000b0, 3200);
    Func_080f9010(80);
    Func_080b0060();
    Func_0808a010(20);
    Func_0808a020();
    Func_02000430();
}
