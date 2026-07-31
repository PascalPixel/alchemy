#include "types.h"

/*
 * Main-image core-band gap at 0x08002dd8, 40 bytes (census kind
 * `shared_literal_thumb_helper_module`; manifest evidence confirms
 * "two_callable_thumb_helpers, cross_function_literal_pool,
 * former_08002df0_boundary_merged" -- one row, two functions sharing
 * a trailing pool word).
 *
 * Both functions index the same IWRAM word array at 0x03001e50 that
 * src/080048b0.c already establishes byte-exact as a two-slot bump
 * allocator base (`base[0]`/`base[1]`, one cursor per allocation
 * "kind"). Reused here rather than re-deriving a new symbol.
 *
 * `bun tools/main_xref.ts 08002dd8 08002df0` confirms both are
 * genuine, heavily-called standalone entries (333 and 72 call sites
 * respectively, each also published as a Thumb pointer), not interior
 * fragments of anything else.
 *
 * Func_08002dd8(index): if `base[index]`'s bits 22+ are all clear,
 * no-op. Otherwise clear `base[index]` to 0 and re-file its old value
 * into `base[0]` or `base[1]` -- selected by bit 24 of that value
 * (isolated as `(value >> 22) & 4`, i.e. a byte offset of 0 or 4 added
 * to `base`) -- moving a pending transfer record into whichever of
 * the two allocator slots the record's own flag bit designates.
 *
 * Func_08002df0(value): stores `value` directly into `base[0]` or
 * `base[1]`, selected the same way by its own bit 24.
 */

void Func_08002dd8(s32 index)
{
    u32 *base = (u32 *)0x03001e50;
    u32 value = base[index];
    u32 selector;

    if ((value >> 22) == 0) {
        return;
    }

    base[index] = 0;
    selector = (value >> 22) & 4;
    *(u32 *)((u8 *)base + selector) = value;
}

void Func_08002df0(u32 value)
{
    u32 *base = (u32 *)0x03001e50;
    u32 selector = (value >> 22) & 4;

    *(u32 *)((u8 *)base + selector) = value;
}
