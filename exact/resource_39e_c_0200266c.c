#include "types.h"

/*
 * resource_39e owner at 0x0200266c, 16 bytes: one of three thin direction
 * wrappers around this overlay's own Func_02004d00. This one passes
 * (0, 32).
 *
 * PUBLISHED, NOT CALLED. No `bl` in the image reaches it; its Thumb
 * pointer is written into a script record, so neither the call-graph
 * sweep nor `tools/overlay-unindexed` lists it. Found by
 * `cargo run --release --manifest-path tools/overlay-published/Cargo.toml -- resource_39e`.
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
 * This is the only one of the three with no negation, which is why it
 * is two halfwords shorter in code and carries an alignment halfword at
 * 0x0200267a instead.
 *
 * Complete owner: `push {lr}` at 0x0200266c through `pop {r0} / bx r0`
 * at 0x02002676-0x02002678; no literal pool, one alignment halfword at
 * 0x0200267a, and the next owner prologue (its sibling 0x0200267c)
 * begins at 0x0200267c.
 *
 * Uncertainty: the two arguments are a displacement pair by shape, from
 * the sibling comparison above; their units are unknown.
 */

extern void Func_02004d00(s32 arg0, s32 arg1);

void Func_0200266c(void)
{
    Func_02004d00(0, 32);
}
