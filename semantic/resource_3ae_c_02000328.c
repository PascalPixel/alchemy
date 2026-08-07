#include "types.h"

/*
 * Resource 3ae, owner at 0x02000328 (400 bytes; TWO literal pools sit inside
 * the body — 0x02000368..0x02000376 and 0x020004a0..0x020004b6 — and the first
 * is branched over by the `b.n 0x02000496` at 0x02000366, so the owner runs on
 * past it.  Found by a control-flow walk, not by reading straight through.)
 *
 * Role known in advance from the call graph: the in-image handler table holds
 * the odd word 0x02008329 == Func_02000328 + 1 as the payload of TWO entries,
 * both keyed 0x08a9000c but under different condition words (0x00000400 and
 * 0x00008400).  So this is a table-dispatched interaction handler invoked with
 * no arguments from two table slots.
 *
 * Prologue `push {r5,r6,lr}` at 0x02000328; epilogue `pop {r5,r6} / pop {r0} /
 * bx r0` at 0x0200049a.  The return address is popped into r0, so r0 does not
 * survive and the owner is void.  No frame.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong).  36 call
 * sites against the inventory's calls=32; the inventory count is documented as
 * systematically low and every `bl` in the span is transcribed below in order,
 * so the set is complete.  Distinct callees: Func_0808a080, Func_0808a018,
 * Func_080770c0, Func_0808a170, Func_0808a178, Func_0808a070, Func_0808a010,
 * Func_0808a180, Func_0808a0d0, Func_0808a1b8, Func_080770c8, Func_08077040,
 * Func_08077038, Func_0808a110, Func_080772b0, Func_0808a020.
 *
 * Facing quantisation at the head: `ldrh r3,[r0,#6] / adds r3,r3,#0x2000 /
 * ands r3,#0xffffc000 / lsls #16 / asrs #16` rounds the player's 16-bit angle
 * to the nearest quadrant and keeps it as a SIGNED halfword, so the four
 * possible values are 0, 0x4000, -0x8000 and -0x4000.  The later test
 * `lsls r3,r5,#16 / cmp r3,#0x80000000` is therefore "the rounded facing is
 * 0x8000", i.e. the player is facing the one direction this handler accepts.
 *
 * NOTE — asymmetric bracket: when that facing test fails the owner branches
 * straight to the epilogue at 0x0200049a and never calls Func_0808a020, even
 * though Func_0808a018 has already run.  That is what the code does and it is
 * preserved; it is either deliberate (the scene stays open for the caller) or
 * an original-source bug, and nothing in this overlay distinguishes the two.
 *
 * `Data_03001ebc` is the RAM progress block established by the byte-exact
 * sibling `assets/code/resource_3ae_c_02000ad4.c`; the unsigned halfword at
 * byte offset 472 is the same counter `Func_02000260` bumps.
 *
 * The five Func_0808a180(12, 0) calls and the four Func_0808a0d0 calls are
 * DISTINCT sites on distinct paths and are not merged.
 *
 * Uncertainties: 0x8a5 / 0x8a7 / 0x8a9 are read as event-flag ids and 0x1d16
 * and 0x1d20..0x1d23 as cue ids, from their argument positions (0x1d23 is
 * already recorded as a cue id by `resource_3ae_c_020002dc.c`).  235 is passed
 * to both Func_08077040 and Func_08077038 and the two results are then handed
 * to Func_080772b0 as a pair, which reads as "look up two properties of item
 * 235 and hand them over"; the identities of those imports are not otherwise
 * witnessed here.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();
void Func_0808a018();
void Func_0808a020();
s32 Func_080770c0();
void Func_080770c8();
s32 Func_0808a070();
void Func_0808a010();
void Func_0808a0d0();
void Func_0808a110();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
s32 Func_08077040();
s32 Func_08077038();
void Func_080772b0();

extern u8 *Data_03001ebc;

void Func_02000328(void)
{
    u8 *player;
    s16 facing;
    s32 a;
    s32 b;

    player = Func_0808a080(0);
    facing = (s16)((*(u16 *)(player + 6) + 0x2000) & 0xc000);

    Func_0808a018();

    if (Func_080770c0(0x8a7) != 0) {
        if (Func_080770c0(0x8a9) != 0) {
            Func_0808a170(0x1d23);
            Func_0808a178(12, 0);
        } else {
            Func_0808a170(0x1d20);
            Func_0808a178(12, 0);

            if (Func_0808a070(0, 0) == 0) {
                Func_0808a010(10);
                Func_0808a170(0x1d21);
                Func_0808a180(12, 0);
                /* movs r2,#161 / lsls r2,r2,#3 == 1288 */
                Func_0808a0d0(12, 88, 1288);
                /* movs r1,#128 / lsls r1,r1,#7 == 0x4000 */
                Func_0808a1b8(12, 0x4000, 0);
                Func_0808a010(20);
                Func_080770c8(0x8a9);
            } else {
                Func_0808a170(0x1d22);
                Func_0808a180(12, 0);
            }
        }
    } else {
        if (facing != (s16)0x8000) {
            /* Deliberate early return: Func_0808a020 is NOT called here. */
            return;
        }

        Func_0808a170(0x1d16);
        Func_0808a180(12, 0);

        if (Func_080770c0(0x8a5) != 0) {
            a = Func_08077040(235);
            b = Func_08077038(235);

            Func_0808a110(12, 3);
            Func_0808a0d0(12, 88, 1288);
            Func_0808a1b8(12, 0x4000, 0);

            *(u16 *)(Data_03001ebc + 472) += 1;

            Func_0808a180(12, 0);
            Func_080772b0(a, b);
            Func_080770c8(0x8a7);

            player = Func_0808a080(0);
            /* movs r2,#163 / lsls r2,r2,#3 == 1304 */
            Func_0808a0d0(0, *(s16 *)(player + 10), 1304);
            Func_0808a0d0(0, 72, 1304);
            Func_0808a0d0(12, 88, 1304);
            Func_0808a1b8(12, 0, 0);
        } else {
            Func_0808a180(12, 0);
        }
    }

    Func_0808a020();
}
