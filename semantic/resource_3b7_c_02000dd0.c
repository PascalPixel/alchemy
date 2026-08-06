/*
 * resource_3b7 owner at 0x02000dd0, 116 bytes (0x02000dd0-0x02000e43).
 *
 * Loads a slot record with a position triple and a heading, then retags every
 * attachment hanging off the record's sub-object list.
 *
 * Code runs 0x02000dd0-0x02000e42.  One literal pool sits INSIDE the span:
 * 0x02000e1e-0x02000e23 (an alignment halfword plus the word 0x00000000 at
 * 0x02000e20).  It is branched over by the `b.n 0x02000e24` at 0x02000e1c and
 * is read only by the `ldr r2, [pc, #40]` at 0x02000df6, so its bytes are data.
 * Register state is carried straight across it: r4 = 255, r0 and r1 are set at
 * 0x02000e16-0x02000e1a and are live in the loop that begins at 0x02000e24.
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts` (an overlay
 * `bl` stores `target_offset - 2`).  In program order:
 *   0x02000ddc -> veneer 0x0200193c -> Func_0808a080
 *   0x02000e06 -> veneer 0x02001894 -> Func_08009088
 * Completeness: 2 sites over 2 distinct targets, matching the tool's
 * `sites=2 distinct_targets=2` and the inventory row's calls=2.
 *
 * Func_0808a080 is the established slot-record lookup: it takes a slot id and
 * returns the record or 0, and its +8 / +12 / +16 words are the 16.16 position
 * triple (see semantic/overlays/resource_373_c_02002f14.c and
 * semantic/overlays/resource_39f_c_02001150.c).  The byte-exact sibling
 * `assets/code/resource_3b7_c_02000e44.c` writes the adjacent byte field +0x54
 * after the same null test, which corroborates the record layout used here.
 *
 * The fifth argument arrives on the stack.  The prologue pushes {r5, r6, r7, lr}
 * and then {r7} (holding r8), i.e. 20 bytes, so the `ldr r1, [sp, #20]` at
 * 0x02000e04 is the first incoming stack word.
 *
 * The epilogue is `pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0`,
 * so r0 holds the popped return address and the owner returns nothing: void.
 *
 * Uncertainties, both deliberate rather than cleaned up:
 *  - The `record == 0` branch skips only the initialisation block and falls
 *    into `*(s32 *)(record + 0x50)`, so a missing record is dereferenced at
 *    address 0x50.  The bytes are not in doubt (`beq.n 0x02000e0a`, and
 *    0x02000e0a is `ldr r0, [r5, #80]`); it reads as a latent null
 *    dereference in the original, and is preserved rather than guarded.
 *  - r4 is written without being saved, although the prologue saves only
 *    r5-r7, lr and r8.  Nothing is called after it is set, so nothing
 *    observable depends on it; this is the same unsaved-r4 idiom already
 *    recorded for resource_371 and resource_372.
 *  - `*(u8 *)(item + 22) |= 0xff` is spelled as the assembly has it
 *    (`ldrb / orrs r3, r4 / strb`) even though the result is always 0xff.
 *  - Func_08009088's interface is unknown; r0 still holds the record from the
 *    lookup and is not reloaded before the branch.
 */

#include "types.h"




extern u8 * Func_0200271a();
extern void Func_0200269c();
void Func_02000dd0(s32 slot, s32 *position, s32 heading_word, s32 tag, s32 arg4)
{
    u8 *record;
    u8 *list;
    s32 remaining;

    record = Func_0200271a(slot);

    if (record != 0) {
        *(s32 *)(record + 8) = position[0];
        *(s32 *)(record + 12) = position[1];
        *(s32 *)(record + 16) = position[2];
        *(u16 *)(record + 6) = (u16)heading_word;

        /* r2 is the pooled constant 0, stored to both byte fields. */
        *(u8 *)(record + 0x55) = 0;
        *(u8 *)(*(s32 *)(record + 0x50) + 0x26) = 0;

        /* r0 still holds the record returned by the lookup. */
        Func_0200269c(record, arg4);
    }

    /* Reached whether or not the record existed - see the note above. */
    list = *(u8 **)(record + 0x50);
    remaining = *(u8 *)(list + 0x27);

    if (remaining != 0) {
        u8 **item = (u8 **)(list + 0x28);

        do {
            u8 *entry = *item++;

            if (*(u8 *)(entry + 5) != tag) {
                u8 flags = *(u8 *)(entry + 22);

                *(u8 *)(entry + 5) = (u8)tag;
                *(u8 *)(entry + 22) = flags | 0xff;
            }
            remaining--;
        } while (remaining != 0);
    }
}
