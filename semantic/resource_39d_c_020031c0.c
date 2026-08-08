#include "types.h"

/*
 * Resource 39d band selector at 0x020031c0 (68 bytes, 0 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020031c0, every branch
 * (0x020031de, 0x020031ec, 0x020031f0, 0x020031f4 forward to 0x020031f8, and
 * 0x020031fc back to 0x020031da) stays inside the body, and the walk stops at
 * the interworking return `pop {r5, r6} / pop {r0} / bx r0` at
 * 0x020031fe-0x02003202.  So the executable extent is exactly
 * 0x020031c0-0x02003203 (68 bytes).  The popped register is r0, so the popped
 * value is the return address and the owner is `void` — r0 is the advancing
 * output cursor and is not a result.
 *
 * Pool map — the walk never reaches this as an instruction:
 *   0x02003204  0x03001ebc   the workspace POINTER
 * It is even and far below the 0x02008000 link base, so it is an absolute
 * IWRAM address rather than an in-image offset, and it is loaded from
 * (`ldr r1,[r3,#0]`) rather than used directly: the workspace is the value
 * stored there, not the address itself.  The next prologue begins at
 * 0x02003208.
 *
 * There are no `bl`s at all: `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39d
 * 31c0 3204` reports `sites=0`, so the multiset is empty on both sides.
 *
 * `asrs #20` is the family's quantisation of a 16.16 coordinate to whole
 * tiles, applied to the argument and to both fields of each candidate.  The
 * two derived bounds are `64 - (y >> 20)` plus 8 and plus 11, so the accepted
 * band is three units wide and is anchored to the argument — `bgt` on the low
 * bound and `bge` on the high one make it half-open, [lo, hi).
 *
 * `subs r2,#4 / cmp r2,#4 / bhi` is the unsigned window idiom: it accepts
 * exactly 4..8 on the other axis.  Read without undoing the bias it looks like
 * a magnitude test against 4.
 *
 * The table is 66 pointer slots at workspace+20 and the loop runs the full
 * 0..65 with `cmp r4,#65 / bls`; a null slot is skipped but does NOT end the
 * scan, so the table is sparse rather than null-terminated.  What is appended
 * is the slot INDEX, not the pointer.
 *
 * STILL-OPEN residual, 16 bytes in the 7-instruction preamble
 * (0x020031c4-0x020031d2): the reference frees the anchor argument's
 * register (r1) early via a throwaway copy (`adds r2,r1,#0` / `asrs
 * r2,r2,#20`) so the very next load -- the workspace pointer dereference --
 * can land straight in r1, leaving one final `adds r1,r2,#0` needed only to
 * move that same value into r1 for the `table = workspace + 20` step.  This
 * source shifts the argument register in place (`asrs r1,r1,#20`, no copy),
 * which keeps r1 occupied through the base subtraction and forces the
 * workspace load into r2 instead; the eventual copy into r1 for `table`
 * still happens but one step later.  Restructuring statement/declaration
 * order (table-last, shift split into its own statement, base folded into
 * the shift result, workspace hoisted before/after the shift) closed 11 of
 * the original 27 differing bytes but could not reach the remaining 16:
 * candidate_explain.ts shows the sequence tied through class/priority for
 * most of the block but prints an explicit `?? model expects 171 to beat
 * 165 ... actual pick suggests an unmodeled tier` at the r6=base store --
 * an unmodeled register-pressure/dest-order heuristic decided gcc's real
 * pick, not source statement order (alchemist.ts confirms `exhausted` with
 * tiers [class,model-divergence,original-order,priority,unaligned] from
 * this improved baseline too, 24 compiles, no licensed move helps).  Per
 * the project's DEAD-END FINGERPRINT precedent, do not hand-permute this
 * further without a new compiler-level lever.
 */

extern u8 *Data_03001ebc;      /* workspace pointer */

void Func_020031c0(s32 *out, s32 anchor)
{
    s32 shifted = anchor;
    u8 *workspace = Data_03001ebc;
    s32 base;
    s32 low;
    s32 high;
    u32 index = 0;
    u8 **table;

    shifted >>= 20;
    base = 64 - shifted;
    low = base + 8;
    high = base + 11;
    table = (u8 **)(workspace + 20);

    for (; index <= 65; index++) {
        u8 *entry = table[index];
        s32 across;
        s32 along;

        if (entry == 0) continue;

        across = *(s32 *)(entry + 8) >> 20;
        along = *(s32 *)(entry + 16) >> 20;

        if ((u32)(across - 4) > 4) continue;
        if (low > along) continue;
        if (along >= high) continue;

        *out++ = index;
    }
}
