#include "types.h"

/*
 * Resource 3cb dialogue step at 0x02001158 (92 bytes,
 * 0x02001158-0x020011b3).
 *
 * Complete owner: `push {r5, r6, lr}` prologue at 0x02001158 and the matching
 * interworking return `pop {r5, r6} / pop {r1} / bx r1` at 0x020011a0.  The
 * popped return address goes to r1, so r0 survives and IS the result — the
 * value returned by the final Func_0808a020 call.
 *
 * The twelve bytes at 0x020011a8-0x020011b3 are this owner's literal pool
 * (0x02000240, 0x0000298a, 0x0000298b), with two alignment bytes at
 * 0x020011a6.  Execution reaches `bx r1` at 0x020011a4 and never falls into
 * them.  The `ldr r0,[pc,#0] / bx lr / .word 0x02009e14` at 0x020011b4 that
 * follows is a separate constant-loader leaf, not part of this owner.
 *
 * All seven call sites were resolved with tools/lib/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the inventory reports
 * calls=7, which matches:
 *   0x0200115c -> veneer 0x02001844 -> Func_0808a018
 *   0x0200116e -> veneer 0x0200187c -> Func_0808a148
 *   0x02001182 -> veneer 0x020017ec -> Func_08015120
 *   0x02001188 -> veneer 0x02001884 -> Func_0808a170
 *   0x02001190 -> veneer 0x02001884 -> Func_0808a170
 *   0x02001198 -> veneer 0x0200188c -> Func_0808a178
 *   0x0200119c -> veneer 0x0200184c -> Func_0808a020
 * Note the two Func_0808a170 sites are the two arms of one if/else, so the
 * distinct-target count (6) is lower than the site count (7).
 *
 * resource_3cb is linked at 0x02008000 (proven by the handler table at the
 * tail of assets/code/resource_3cb_overlay.s, whose entries 0x02009051 and
 * 0x02008861 are Func_02001050 — byte-exact in assets/code — and
 * Func_02000860 plus the Thumb bit).  0x02000240 is therefore below the image
 * and is an ordinary RAM record, the same one the byte-exact
 * assets/code/resource_3cb_c_02000060.c reads.  This owner uses two fields of
 * it: a word at +500 (built as 250 << 1) and a halfword at +680 (170 << 2).
 *
 * Behaviour: open the dialogue barrier, hand the caller's handle and the
 * record's word field to Func_0808a148, then select one of two cues by the
 * record's halfword field — a nonzero value is first released through
 * Func_08015120(value, 5) and selects cue 0x298a, zero selects cue 0x298b.
 * Close with Func_0808a178(handle, 0) and Func_0808a020.
 *
 * Uncertainties: the third argument 0 to Func_0808a148 and the second
 * argument 5 to Func_08015120 are literal here and their meanings are not
 * established; Func_0808a018 and Func_0808a020 are reached with no argument
 * register set by this owner.  The halfword at +680 is read zero-extended.
 */

extern u8 Data_02000240[];

/* Old-style declarations: arities vary per site across this overlay. */
void Func_08015120();
void Func_0808a018();
s32 Func_0808a020();
void Func_0808a148();
void Func_0808a170();
void Func_0808a178();

/* Used for its return value. */

s32 Func_02001158(s32 handle)
{
    u32 word;
    u16 cueHandle;

    Func_0808a018();

    word = *(u32 *)&Data_02000240[500];
    Func_0808a148(handle, word, 0);

    cueHandle = *(u16 *)&Data_02000240[680];
    if (cueHandle != 0) {
        Func_08015120((s32)cueHandle, 5);
        Func_0808a170(0x298a);
    } else {
        Func_0808a170(0x298b);
    }

    Func_0808a178(handle, 0);
    return Func_0808a020();
}
