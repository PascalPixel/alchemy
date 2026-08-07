#include "types.h"

/*
 * Resource 3cb party-roster membership test at 0x02000f30 (88 bytes,
 * 0x02000f30-0x02000f87).
 *
 * Complete owner: `push {r5, r6, r7, lr}` prologue at 0x02000f30 and the
 * matching interworking return `pop {r5, r6, r7} / pop {r1} / bx r1` at
 * 0x02000f7c.  The popped return address goes to r1, so r0 survives and IS the
 * result: the owner returns 1 on a hit and 0 otherwise.
 *
 * The eight bytes at 0x02000f80-0x02000f87 cover the `bx r1` tail, two
 * alignment bytes and this owner's single pool word 0x02000240 at 0x02000f84.
 * That word is data — the `lsls` pair the disassembler prints there is the
 * pool, not code.
 *
 * Both call sites were resolved with tools/lib/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2):
 *   0x02000f34 -> veneer 0x02001834 -> Func_08077148
 *   0x02000f40 -> veneer 0x0200180c -> Func_080770c0
 * sites=2 against the inventory's calls=2.
 *
 * Data_02000240 is the overlay workspace record also used by the byte-exact
 * assets/code/resource_3cb_c_02000060.c (as `s16 Data_02000240[]`, index 225).
 * The byte run at offset 504 read here is the same roster run that
 * Func_0200053c copies out and terminates with 0xff, which is why the scan
 * below stops on 0xff as well as on the count.
 *
 * Behaviour: take the roster length from Func_08077148, clamp it to 3 — or to
 * 4 when Func_080770c0(370) reports zero — then scan that many roster bytes
 * for a byte equal to the argument, stopping early at a 0xff terminator.
 *
 * Faithfulness notes: the assembly keeps two cursors over the same run (r0 for
 * the terminator test with post-increment, r2 for the comparison), but they
 * are initialised to the same address and advance in lockstep, so both read
 * the same byte each iteration; that is written as one cursor here.  The count
 * comparisons at 0x02000f4a/0x02000f52/0x02000f76 are signed, so a
 * non-positive length returns 0 without touching the run.
 */

extern u8 Data_02000240[];

/* Old-style declarations; both are used for their return values. */
s32 Func_0200276a();
s32 Func_0200274e();

s32 Func_02000f30(s32 member)
{
    s32 count = Func_0200276a();
    s32 limit = 3;
    s32 i;
    const u8 *roster;

    if (Func_0200274e(370) == 0) {
        limit = 4;
    }
    if (count > limit) {
        count = limit;
    }

    roster = &Data_02000240[504];
    for (i = 0; i < count; i++) {
        u8 entry = roster[i];

        if (entry == 0xff) {
            return 0;
        }
        if ((s32)entry == member) {
            return 1;
        }
    }

    return 0;
}
