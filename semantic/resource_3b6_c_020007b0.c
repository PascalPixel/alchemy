#include "types.h"

/*
 * Resource 3b6 one-shot cutscene step at 0x020007b0.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020007b0 and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return at 0x02000840.  The
 * return address is popped into r0, so the owner is `void`.  r6 holds the
 * incoming argument throughout; r5 holds the first arm's message-id base.  An
 * alignment halfword and a five-word literal pool close the row at
 * 0x02000846-0x0200085b.
 *
 * Nineteen `bl` instructions, every one on a reachable path and every one
 * placed below; the inventory advertises `calls=18`, an undercount of one.
 * None of the nineteen falls inside the literal pool.
 *
 * All sites resolve with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- — an overlay `bl`
 * stores `target_offset - 2`, so the disassembler's labels are artefacts — and
 * are named for the main-image address in each veneer entry's trailing word.
 * The begin/message/act/end quartet is the one tracked byte-exact source
 * assets/code/resource_3b6_c_0200073c.c uses under its printed names.
 *
 * FLAG PAIR.  0x080770c0 and 0x080770c8 are adjacent veneer entries used here
 * on the same id 0x8be: the first as the guard, the second on the taken path
 * before the one-time body runs.  Read as query-then-set on a flag id, which
 * makes the whole second arm a once-only branch.  The reading is inferred from
 * this usage, not from the callees, and is recorded as an uncertainty.
 *
 * The first arm's body is the 0x020006ec / 0x02000760 / 0x020008cc triplet
 * verbatim, which corroborates the reading of that construct.  Its
 * 0x0808a170 / 0x0808a180 calls and the second arm's are distinct call sites
 * and are kept distinct.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_02001196();
s32 Func_02001184();
void Func_02001200();
void Func_02001210();
s32 Func_020011c8();
void Func_020011ba();
void Func_02001220();
void Func_02001228();
void Func_02001240();
void Func_020011da();
void Func_02001248();
void Func_02001260();
void Func_020011f6();
void Func_02001256();
void Func_02001204();
void Func_0200126a();
void Func_02001282();
void Func_02001226();
                       /* begin step */
                       /* query a numeric id; nonzero when selected */
                       /* set that same id */
                       /* present message by id */
                       /* act on the subject, variant used before a branch */
                       /* branch predicate; nonzero selects the second arm */
                       /* wait / delay by count */
                       /* act on the subject */
                     
                       /* end step */

void Func_020007b0(s32 subject)
{
    s32 message;

    Func_02001196();

    if (Func_02001184(0x8bd) == 0) {
        Func_02001200(message);
        message = 0x2399;
        Func_02001210(subject, 0);
        if (Func_020011c8(0, 0) == 0) {
            Func_020011ba(10);
            Func_02001220(message + 1);
        } else {
            Func_02001228(message + 2);
        }
        Func_02001240(subject, 0);
    } else {
        if (Func_020011c8(0x8be) == 0) {
            Func_020011da(0x8be);
            Func_02001248(0x239c);
            Func_02001260(subject, 0);
            Func_020011f6(10);
            Func_02001256(subject, 2);
            Func_02001204(20);
        }
        Func_0200126a(0x239d);
        Func_02001282(subject, 0);
    }

    Func_02001226();
}
