/*
 * resource_3a7 per-frame scene task at 0x020009c0, 224 bytes.
 *
 * This is the task the owner at 0x02000c50 installs: its pool word 0x020089c1
 * is Func_020009c0 + the Thumb bit under the proven 0x02008000 link base.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020009c0 through `pop {r5, r6,
 * r7} / pop {r0} / bx r0` at 0x02000a82.  The return address is popped into r0,
 * so the owner is `void`, as a per-frame task must be.  Six literal-pool words
 * sit past the epilogue inside the span (0x02000a88 .. 0x02000a9c); the
 * control-flow walk reaches none of them.
 *
 * FREE LAYOUT WITNESS for the in-image data.  The three pointer pool words are
 * 0x0200a214, 0x0200a224 and 0x0200a228 - file offsets 0x2214, 0x2224 and
 * 0x2228.  The first is indexed `[cue << 2]` with `cue` in 0..3, so it is four
 * s32 entries, and 0x2214 + 4*4 = 0x2224 is exactly the frame counter the same
 * owner drives.  That single identity fixes the element count, the stride and
 * the link base at once (the documented `array_base + count*stride ==
 * scalar_base` witness), with 0x2228 following as the cue cursor.
 *
 * Shape, per frame:
 *   - bail out entirely while byte +91 of record 10 is non-zero;
 *   - bump the frame counter, wrapping to 0 past 190;
 *   - when the counter reaches the current cue's scheduled frame, poke record
 *     (cue + 11) with 0x0a3d at +72 and advance the cue cursor 0..3 cyclically;
 *   - sweep the four records 11..14: any whose +40 is non-negative and whose
 *     +12 has not yet passed 0xffff gets an effect burst (0x02000b8c), is reset
 *     to 0xff0000 / 0 / 0 / 0, and the task waits 106 frames;
 *   - finally run the tile-claim predicate 0x02000944 for subjects 10 and 9,
 *     and on a claim by subject 10 raise the one-shot flag 0x207 the first time.
 *
 * The 0x207 flag is the same one the triggers at 0x0200048c and 0x020004d0 test
 * as a precondition - this task is its writer.  That is the documented "a gate
 * flag's setter is often in a DIFFERENT owner" shape, and it cross-validates
 * both files.
 *
 * Call targets resolved with
 *   bun tools/overlay/overlay_show.ts resource_3a7 09c0 -n 224 |
 *   bun tools/lib/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 13 sites, matching the inventory's calls=13:
 *   Func_0808a080 x3 <- veneer 0x02001130
 *   Func_02000b8c x1  (this overlay's effect-burst spawner)
 *   Func_080f9010 x4 <- veneer 0x02001150  (106, 106, 204, 106)
 *   Func_02000944 x2  (this overlay's tile-claim predicate)
 *   Func_0808a100 x1 <- veneer 0x02001140
 *   Func_080770c0 x1 <- veneer 0x02001100
 *   Func_080770c8 x1 <- veneer 0x02001108
 * The two 106-frame waits at 0x02000a6e and 0x02000a7e are distinct sites on
 * distinct paths and are written out separately rather than merged.
 *
 * Uncertainties: the record fields (+12, +40, +72, +91) are transcribed, not
 * interpreted - they are the same fields 0x02000944, 0x02000aa0 and 0x02000c50
 * write, with the same 0xff0000 and 0x0a3d constants, so the layout is
 * cross-checked but its meaning is not established.  The zero written to the
 * counter and to +72/+40/+91 comes from registers the assembly proves are zero
 * (the +91 byte read on entry, and a `movs r7,#0`).
 */

/* Old-style declarations: overlay imports vary their argument count between
 * call sites in this overlay.  Those used in conditions need a return type. */
#include "types.h"

u8 *Func_0808a080();    /* scene-entity record by selector */
void Func_02000b8c();   /* spawn an effect burst at a record's position */
void Func_080f9010();   /* scripted delay, in frames */
s32 Func_02000944();    /* claim the entity standing on the subject's tile */
void Func_0808a100();
s32 Func_080770c0();    /* test a scene completion flag */
void Func_080770c8();   /* raise a scene completion flag */

/* In-image data under the proven 0x02008000 link base.  The overlay image is
 * writable EWRAM, so these are mutable cells, not constants. */
extern s32 Data_0200a214[4];    /* scheduled frame per cue */
extern s32 Data_0200a224;       /* frame counter, 0..190 */
extern s32 Data_0200a228;       /* cue cursor, 0..3 */

void Func_020009c0(void)
{
    s32 cue;
    s32 index;

    if (Func_0808a080(10)[91] != 0) {
        return;
    }

    Data_0200a224 = Data_0200a224 + 1;
    if (Data_0200a224 > 190) {
        Data_0200a224 = 0;
    }

    cue = Data_0200a228;
    if (Data_0200a214[cue] == Data_0200a224) {
        *(s32 *)(Func_0808a080(cue + 11) + 72) = 0x0a3d;

        Data_0200a228 = Data_0200a228 + 1;
        if (Data_0200a228 > 3) {
            Data_0200a228 = 0;
        }
    }

    index = 0;
    do {
        u8 *entry = Func_0808a080(index + 11);

        if (*(s32 *)(entry + 40) >= 0 && *(s32 *)(entry + 12) <= 0x0000ffff) {
            *(s32 *)(entry + 40) = 0;
            Func_02000b8c(entry);
            *(s32 *)(entry + 12) = 0x00ff0000;
            *(s32 *)(entry + 72) = 0;
            entry[91] = 0;
            Func_080f9010(106);
        }

        index++;
    } while (index <= 3);

    if (Func_02000944(10) != 0) {
        Func_0808a100(10, 1);

        if (Func_080770c0(0x207) == 0) {
            Func_080770c8(0x207);
            Func_080f9010(204);
        } else {
            Func_080f9010(106);
        }
    }

    if (Func_02000944(9) != 0) {
        Func_080f9010(106);
    }
}
