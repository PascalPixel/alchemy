typedef unsigned char u8;
typedef unsigned int u32;

/*
 * resource_3c9 owner at 0x020059f0, 56 bytes: gated by a byte flag at
 * +99 on its single object argument -- when set, recomputes a
 * fixed-point offset from a byte counter at +98 into a u32 field at
 * +12, calls a leaf helper, then advances that same counter while it
 * stays in (0, 31].
 *
 * Complete owner: `push {r5, lr}` at 0x020059f0 through `pop {r5} / pop
 * {r0} / bx r0` at 0x02005a20-0x02005a24, no literal pool; the next
 * owner's prologue is exactly at 0x02005a28, no gap. `arg0` is read
 * (the +99 byte test) before being overwritten, so it is a real
 * argument; void return.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts resource_3c9
 * 59f0 5a28`'s `+2` rule: one call site, target 0x02005688. That target
 * is NOT itself a `push {..,lr}`-prologue function -- it is a leaf
 * routine reached only by `bl` and returning with a bare `bx lr` (never
 * touches lr), so the classifier's prologue heuristic misses it and it
 * would otherwise print as `unknown`. Confirmed by reading the bytes at
 * 0x02005688 directly: it zeroes/sets several fields on the pointer in
 * r0 (the same object this owner was called with) and returns via
 * `bx lr`. Declared here rather than drafted; worth flagging for
 * whoever indexes leaf (no-push) callees in this overlay.
 *
 * Uncertainty: none of +12/+76/+98/+99's layout is established beyond
 * these raw offsets.
 */

extern void Func_02005688(void *record);

void Func_020059f0(u8 *arg0)
{
    if (*(u8 *)(arg0 + 99) != 0) {
        u8 counter = *(u8 *)(arg0 + 98);

        *(u32 *)(arg0 + 12) = *(u32 *)(arg0 + 76) + ((u32)(counter >> 2) << 16);

        Func_02005688(arg0);

        {
            u32 c = *(u8 *)(arg0 + 98);
            if (c != 0) {
                if (c <= 31) {
                    *(u8 *)(arg0 + 98) = (u8)(c + 1);
                }
            }
        }
    }
}
