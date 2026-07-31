typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x0200486c (18 bytes, 179 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population, found via
 * overlay_unindexed.ts's reference resolution: every overlay `bl` target
 * whose first halfword is `push {..,lr}` and has no index entry). `push
 * {lr}` at 0x0200486c, epilogue `pop {r0} / bx r0` at
 * 0x0200487a-0x0200487c returns r0 unmodified (void). The next owner starts
 * at 0x02004880 on a 4-byte boundary; the halfword at 0x0200487e is the
 * alignment pad, not part of this span.
 *
 * Raw callee naming (this overlay's sibling convention, e.g.
 * resource_3b1_c_0200012c.c): both calls resolve to addresses inside this
 * same unindexed cluster (0x0200ae2a, 0x0200ad38), neither yet named or
 * covered elsewhere.
 */

void Func_0200ae2a();
void Func_0200ad38();

void Func_0200486c(s32 a)
{
    Func_0200ae2a(a, 0);
    Func_0200ad38(10);
}
