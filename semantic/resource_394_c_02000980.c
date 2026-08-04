#include "types.h"

/*
 * Resource 394 overlay initialiser at 0x02000980.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus `mov r7,sl / mov r6,r8 /
 * push {r6, r7}` at 0x02000980, and the matching `add sp,#8 / pop {r3, r5} /
 * mov r8,r3 / mov sl,r5 / pop {r5, r6, r7} / pop {r1} / bx r1` at 0x02000a78.
 * 272-byte row: 226 bytes of code and 46 bytes of pool.
 *
 * POOL MAP, from a control-flow walk — this row is the reason to do the walk
 * rather than guess.  There is an INTERIOR pool at 0x02000a30-0x02000a53
 * (36 bytes) which the body hops with a bare `b.n 0x02000a54` at 0x02000a2e,
 * and r0 and r3 are BOTH live across it: r0 is the record from
 * Func_0808a080(10) and r3 is the 0xc000 written at +0x18 before the hop and
 * at +0x1c after it.  Ending the row at the pool, or restarting register state
 * at it, drops that store.  The trailing pool is the alignment halfword at
 * 0x02000a86 plus 0x02000a88-0x02000a8f (10 bytes), and the next owner's
 * prologue is at 0x02000a90.  36 + 10 = 46, which is exactly span - code.
 *
 * ROOT.  This is entry 0 of the exported-entry veneer table at image offset 0:
 * `ldr r4,[pc,#0] / bx r4 / .word 0x02008981`, which is 0x0200_0980 plus the
 * Thumb bit under the 0x02008000 link base.  The other five table entries name
 * the constant-loader leaves at 0x30/0x38/0x3c/0x44/0x4c.  Working the call
 * graph root-down from here reaches 0x02000194, 0x02000a90, 0x02000b3c and the
 * tracked byte-exact 0x02000e64.
 *
 * Signature.  The return address is popped into r1, not r0, so r0 survives and
 * is the result — and `movs r0,#0` immediately precedes the pop, so the owner
 * returns 0.
 *
 * Cells and buffers.  0x020092c0, 0x020092c4 and 0x020092c8 are in-image
 * pointer cells (even pool words under the proven base, so data), initialised
 * here to three addresses two bytes apart inside a plain EWRAM scratch buffer
 * at 0x02001000 — below the link band, so a RAM address rather than an
 * in-image offset.  tracked byte-exact assets/code/resource_394_c_020008b0.c
 * already declares the middle one `extern u16 *Data_020092c4` and writes a
 * halfword through it, which is exactly the shape used below.
 *
 * The guarded block is a DMA3 transfer: source Data_0200911c (in-image data at
 * file offset 0x111c), destination the scratch buffer, control 0x84000012.
 *
 * The workspace store is the additive displacement/value pair: r2 is 448 while
 * the address is formed and only then becomes 516, so it writes 516 at
 * Data_03001ebc + 448 — the same opening resource_398's variant setups have.
 *
 * Twelve call sites, matching the row's advertised count.
 */

extern u8 *Data_03001ebc;
extern u8 Data_0200911c[];

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_080091b8();  /* place a fixture, first bank: (x, y, w, h, sx, sy) */
void Func_080091c0();  /* place a fixture, second bank */
s32 Func_080770c0();   /* read event flag */
u8 *Func_0808a080();   /* actor record by slot id */
void Func_0808a100();  /* set actor visibility/state (slot, state) */

void Func_02000194(void);
void Func_02000a90(s16 *records);
void Func_02000b3c(s16 *records, s32 value);
void Func_02000e64(s32 level);

s32 Func_02000980(void)
{
    u8 *actor;
    s32 shade;

    *(u8 **)0x020092c4 = (u8 *)0x02001000;
    *(u8 **)0x020092c8 = (u8 *)0x02001002;
    *(u8 **)0x020092c0 = (u8 *)0x02001004;

    Func_080091b8(32, 0, 64, 32, 0, 64);
    Func_080091c0(0, 0, 32, 32, 0, 64);
    Func_080091c0(32, 0, 32, 32, 0, 32);

    if (Func_080770c0(0x109) == 0) {
        *(u32 *)0x040000d4 = (u32)Data_0200911c;
        *(u32 *)0x040000d8 = *(u32 *)0x020092c0;
        *(u32 *)0x040000dc = 0x84000012;

        **(s16 **)0x020092c4 = 0;
        **(s16 **)0x020092c8 = 1;
    }

    Func_02000a90(*(s16 **)0x020092c0);
    Func_02000b3c((s16 *)Data_0200911c, 0xff);
    Func_02000194();

    Func_0808a100(9, 0);

    actor = Func_0808a080(9);
    actor[0x55] = 0;

    actor = Func_0808a080(10);
    *(u16 *)(actor + 32) = 8;
    shade = 0xc000;
    *(s32 *)(actor + 24) = shade;
    /* the literal pool sits here in the row; r0 and r3 carry across it */
    *(s32 *)(actor + 28) = shade;

    *(s32 *)(Data_03001ebc + 448) = 516;

    if (Func_080770c0(0x845) == 0) {
        Func_02000e64(4);
    }
    return 0;
}
