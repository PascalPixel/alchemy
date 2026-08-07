#include "types.h"

/*
 * Resource 3c3, dialogue bracket at 0x02000730 (124 bytes, 11 call sites).
 *
 * Complete owner: `push {lr}` at 0x02000730, `pop {r0} / bx r0` at 0x02000798.
 * The popped branch register IS r0, so it holds the return address and the
 * owner is `void`.  Bytes 0x0200079c-0x020007ab are the four-word literal
 * pool: 0x0000089f (the story flag 0x020003c4 sets on its way out),
 * 0x00002668 and 0x0000264e (two dialogue line ids), and 0x03001ebc (the
 * workspace pointer cell).
 *
 * SHARED TAIL, spelled with a label rather than per-arm copies.  The
 * `Func_0808a180(9, 0)` at 0x02000772 is reached from two paths — the
 * already-seen arm branches to it over the whole body (`b.n 0x02000772` at
 * 0x02000746), and the unseen arm falls into it after Func_0808a110.  There
 * are exactly THREE Func_0808a180 sites in the assembly; writing the tail once
 * per arm would inflate the multiset to four.  The third site sits in the
 * skip-beat arm at 0x02000790 and is genuinely distinct.
 *
 * The skip-beat counter is the documented idiom: `movs r3,#236 / lsls r3,#1`
 * (byte offset 472) off the 0x03001ebc workspace pointer, bumped by 2 here
 * against the sibling 0x020003c4's 1.  `Data_03001ebc` is a pointer CELL, so
 * `ldr r3,[pc] / ldr r2,[r3]` is one dereference.
 *
 * The Func_0808a070 guard is tested for `!= 0` at this site (`bne`) — the
 * prompt idiom appears with both polarities, so the comparison is read per
 * call site rather than assumed.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`; all 11 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_0808a180 x3, Func_0808a170 x2, and one each of Func_080770c0,
 *   Func_0808a018, Func_0808a020, Func_0808a070, Func_0808a110,
 *   Func_0808a178.
 */

/* Old-style declarations: overlay import arities vary per call site. */
s32 Func_080770c0();    /* test a story flag (used in a condition) */
void Func_0808a018();   /* open a scripted scene */
void Func_0808a020();   /* close a scripted scene */
s32 Func_0808a070();    /* dialogue prompt; result selects the branch */
void Func_0808a110();   /* scene-presentation request */
void Func_0808a170();   /* show a dialogue line by id */
void Func_0808a178();   /* dialogue-line variant with a mode word */
void Func_0808a180();   /* wait for the slot's action to finish */

extern u8 *Data_03001ebc;

void Func_02000730(void)
{
    u8 *workspace;

    Func_0808a018();

    if (Func_080770c0(0x89f) != 0) {
        Func_0808a170(0x2668);
        goto close;
    }

    Func_0808a170(0x264e);
    Func_0808a178(9, 0);

    if (Func_0808a070(0, 0) != 0) {
        /* Skip-beat counter, two beats' worth. */
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 2;
        Func_0808a180(9, 0);
        goto done;
    }

    Func_0808a110(9, 4);
    Func_0808a180(9, 0);

close:
    Func_0808a180(9, 0);

done:
    Func_0808a020();
}
