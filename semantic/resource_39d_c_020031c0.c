typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

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
 * There are no `bl`s at all: `bun tools/overlay_call_targets.ts resource_39d
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
 */

extern u8 *Data_03001ebc;      /* workspace pointer */

void Func_020031c0(s32 *out, s32 anchor)
{
    u8 **table = (u8 **)(Data_03001ebc + 20);
    s32 base = 64 - (anchor >> 20);
    s32 low = base + 8;
    s32 high = base + 11;
    s32 index;

    for (index = 0; index <= 65; index++) {
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
