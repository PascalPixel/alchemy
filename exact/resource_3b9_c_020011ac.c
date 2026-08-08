#include "types.h"

/*
 * resource_3b9 owner at 0x020011ac, 24 bytes: a two-call story-flag
 * hand-off. It clears one flag and sets its immediate neighbour.
 *
 * PUBLISHED, NOT CALLED. No `bl` in the image reaches it; its Thumb
 * pointer is written into a script record. Found by
 * `cargo run --release --manifest-path tools/overlay-published/Cargo.toml -- resource_3b9`. It is the smallest
 * published owner in this overlay and does nothing else -- no scene
 * bracket, no dialogue.
 *
 * THE TWO FLAG IDS ARE ADJACENT AND SPELLED DIFFERENTLY, which is the
 * only thing here that can mislead. The cleared flag is 0x235 (565),
 * loaded from the one-word literal pool. The set flag is built as
 * `movs r0,#141 / lsls r0,#2`, so it is 564 -- 0x234, one below the
 * one just cleared. Two consecutive ids in one function, one from a
 * pool and one computed, because the compiler picks the cheaper
 * encoding per value rather than per meaning. Transcribed as read; a
 * reader who assumed both came from the pool would get 565 twice.
 *
 * Complete owner: `push {lr}` at 0x020011ac through `pop {r0} / bx r0`
 * at 0x020011bc-0x020011be, then the one-word literal pool at
 * 0x020011c0 and a zero alignment halfword at 0x020011c2; the next
 * owner's prologue is at 0x020011c4.
 *
 * Both `bl` targets resolved through the overlay's import-veneer table
 * under the +2 rule (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --).
 *
 * Uncertainty: what the two flags gate is not established. The
 * clear-then-set-the-neighbour shape suggests a two-state selector
 * advancing by one, but this owner alone is no evidence for that.
 */

extern void Func_02003d7e();


void Func_020011ac(void)
{
    Func_02003d7e(0x235);
    Func_02003d7e(564);
}
