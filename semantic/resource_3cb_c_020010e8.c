#include "types.h"

/*
 * Resource 3cb dialogue step at 0x020010e8 (112 bytes,
 * 0x020010e8-0x02001157).
 *
 * Complete owner: `push {r5, r6, lr}` prologue at 0x020010e8 and the matching
 * interworking return `pop {r5, r6} / pop {r1} / bx r1` at 0x02001142.  The
 * popped return address goes to r1, so r0 survives and IS the result — the
 * value returned by the final Func_0808a020 call.
 *
 * The sixteen bytes at 0x02001148-0x02001157 are this owner's literal pool
 * (0x02000240, 0x0000298d, 0x0000298c, 0x0000298e).  Execution reaches `bx r1`
 * at 0x02001146 and never falls into them; the `cmp` pairs the disassembler
 * prints there are cue ids, not code.
 *
 * All nine call sites were resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * (true_target_offset = stored_displacement + 2), matching the inventory's
 * calls=9:
 *   0x020010ec -> veneer 0x02001844 -> Func_0808a018
 *   0x020010fe -> veneer 0x0200187c -> Func_0808a148
 *   0x02001106 -> veneer 0x0200180c -> Func_080770c0
 *   0x0200110e -> veneer 0x02001834 -> Func_08077148
 *   0x02001120 -> veneer 0x02001814 -> Func_080770c8
 *   0x0200112c -> veneer 0x0200181c -> Func_080770d0
 *   0x02001132 -> veneer 0x02001884 -> Func_0808a170
 *   0x0200113a -> veneer 0x0200188c -> Func_0808a178
 *   0x0200113e -> veneer 0x0200184c -> Func_0808a020
 *
 * This is one of a family of dialogue steps in this overlay (see also
 * Func_0200106c and Func_02001158) with the shape: barrier, publish the
 * speaker and the RAM record's word field through Func_0808a148, select a cue,
 * emit it through Func_0808a170, close with Func_0808a178(speaker, 0) and
 * Func_0808a020.
 *
 * resource_3cb is linked at 0x02008000 (the handler table at the tail of
 * assets/code/resource_3cb_overlay.s lists 0x02009051 = Func_02001050 + 1,
 * byte-exact in assets/code, among others).  0x02000240 is therefore below the
 * image: an ordinary RAM record, the same one the byte-exact
 * assets/code/resource_3cb_c_02000060.c reads.  The word used here is at
 * offset 500 (built as 250 << 1).
 *
 * The flag trio is Func_080770c0 = test, Func_080770c8 = set,
 * Func_080770d0 = clear; the polarity is forced by Func_020002d8's one-shot
 * timer and is consistent with the toggle below.
 *
 * Behaviour: flag 516 (built as 129 << 2) makes this a two-visit line.  On the
 * first visit the flag is clear, so the cue depends on the party size reported
 * by Func_08077148 (0x298c when it exceeds 3, otherwise 0x298d) and the flag is
 * set.  On a later visit the flag is set, so the cue is the fixed 0x298e and
 * the flag is cleared again.
 *
 * Uncertainties: the third argument 0 to Func_0808a148 is literal at every
 * site in this overlay and its meaning is not established; Func_0808a018 and
 * Func_0808a020 are reached with no argument register set by this owner.  The
 * Func_08077148 comparison at 0x02001112 is signed.
 */

extern u8 Data_02000240[];

/* Old-style declarations: arities vary per site across this overlay. */
void Func_02002932();
void Func_0200297c();
s32 Func_02002914();
void Func_02002944();
s32 Func_02002936();
void Func_0200294a();
void Func_020029b8();
void Func_020029c8();
s32 Func_0200298c();

                     

                    

                     

/* Used for their return values. */

s32 Func_020010e8(s32 speaker)
{
    u32 cue;

    Func_02002932();
    Func_0200297c(speaker, *(u32 *)&Data_02000240[500], 0);

    if (Func_02002914(516) != 0) {
        cue = 0x298e;
        Func_02002944(516);
    } else {
        if (Func_02002936() > 3) {
            cue = 0x298c;
        } else {
            cue = 0x298d;
        }
        Func_0200294a(516);
    }

    Func_020029b8(cue);
    Func_020029c8(speaker, 0);
    return Func_0200298c();
}
