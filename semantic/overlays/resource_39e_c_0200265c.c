typedef int s32;

/*
 * resource_39e owner at 0x0200265c, 16 bytes: one of three thin direction
 * wrappers around this overlay's own Func_0200268c. This one passes
 * (0, -32).
 *
 * PUBLISHED, NOT CALLED. No `bl` in the image reaches it; its Thumb
 * pointer is written into a script record, so neither the call-graph
 * sweep nor `tools/overlay_unindexed.ts` lists it. Found by
 * `bun tools/overlay_published.ts resource_39e`.
 *
 * THIS CLOSES A LOOP ON AN EARLIER DRAFT. The header of
 * resource_39e_c_0200268c.c records "Called 3x within this overlay"
 * without being able to say by whom, because all three callers are
 * data-installed. They are 0x0200265c, 0x0200266c and 0x0200267c, and
 * lined up they are plainly a direction bank forwarding a displacement
 * pair into Func_0808a0e0:
 *
 *     0x0200265c   (0, -32)
 *     0x0200266c   (0,  32)
 *     0x0200267c   (-32, 0)
 *
 * The fourth quadrant (32, 0) is absent from this overlay; whether the
 * scene never needs it or it lives elsewhere is not established.
 *
 * The negation is `movs r1,#32 / negs r1,r1`, so the argument is a
 * genuine -32 rather than a large unsigned constant.
 *
 * Complete owner: `push {lr}` at 0x0200265c through `pop {r0} / bx r0`
 * at 0x02002668-0x0200266a; no literal pool, and the next owner
 * prologue (its sibling 0x0200266c) begins immediately at 0x0200266c.
 *
 * Uncertainty: the two arguments are a displacement pair by shape, from
 * the sibling comparison above; their units are unknown.
 */

extern void Func_0200268c(s32 arg0, s32 arg1);

void Func_0200265c(void)
{
    Func_0200268c(0, -32);
}
