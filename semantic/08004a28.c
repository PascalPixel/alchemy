/*
 * The store-merge blocker, worked here because this is its smallest instance:
 * 28 bytes, three word stores and a return.
 *
 * The reference writes the three words as one `stmia r3!, {r0, r1, r2}` and
 * then `subs r3, #12`, and we emit three `str`. Reading it as the allocator
 * refusing to put the values in ascending registers is the wrong diagnosis --
 * the sequence is not reachable from C at all:
 *
 *   - The `store_multiple` peepholes in arm.md are TARGET_ARM only, so Thumb
 *     has no consecutive-store merging.
 *   - Thumb's only multi-register store is the block move through
 *     `arm_gen_store_multiple`, and that always pairs `stmia` with an `ldmia`
 *     from memory. Measured on this compiler: a by-value struct already in
 *     r1-r3 is spilled to the stack and reloaded before the `stmia`, and a
 *     `*p++ =` walk emits one-register `stmia rB!, {rN}` per store, never a
 *     merged three.
 *   - old_agbcc does the same, so it is not the other compiler either.
 *
 * The `subs r3, #12` is the tell. It undoes the writeback into a register that
 * is dead at the `bx lr` on the next instruction. No compiler emits a dead
 * compensation; a hand-written kick macro that restores its pointer does. The
 * retained-assembly classes already carry `deliberate_dma_kick_macro`, and no
 * byte-exact owner in the corpus writes 0x040000d4.
 *
 * 53 main-image owners carry this exact residual, 11,282 bytes, all of them
 * `stmia r3!, {r0, r1, r2}` followed by `subs r3, #12`. The 30 whose residual
 * the merge dominates are registered in semantic/unmatchable.json. The C below
 * is the correct reading of what the hardware is told to do; it is the encoding
 * that is out of reach, so no further source reading will close it.
 */
#include "types.h"

void Func_08004a28(u32 destination)
{
    u32 *dma = (u32 *)0x040000d4;

    dma[0] = 0x03000350;
    dma[1] = destination;
    dma[2] = 0x8400000c;
}
