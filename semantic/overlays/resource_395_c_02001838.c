typedef unsigned int u32;
typedef volatile unsigned int vu32;

/*
 * resource_395 owner at 0x02001838, 32 bytes: 16 of code, a four-word pool.
 *
 * TAIL LEAF. Found 2026-08-01 by the return-shape rule in `ruleTail`, and by
 * nothing else. It lies PAST the last recorded owner, so sweep D's gap loop
 * never reached it; it opens with no `push`, so sweep C is blind; no `bl` in
 * the image targets it (`overlay_call_targets`, sites=756, a live run); and no
 * 4-aligned word in the image holds its published spelling under base+0x8000
 * with or without the Thumb bit. Invisible to all four sweeps at once, which
 * HANDOVER had recorded only as a hypothetical.
 *
 * The ROM, verbatim:
 *
 *   ldr r2, [pc, #12]   -> 0x03001ed0
 *   ldr r3, [pc, #16]   -> 0x040000d4   (DMA3SAD)
 *   ldr r0, [r2]
 *   ldr r1, [pc, #16]   -> 0x02009de0
 *   ldr r2, [pc, #16]   -> 0x840000e0
 *   stmia r3!, {r0, r1, r2}
 *   subs r3, #12
 *   bx lr
 *
 * SPAN IS 32 BYTES, NOT 16. All four pool words at 0x2001848-0x2001857 are
 * reached by this row's own pc-relative loads -- each offset re-derived from
 * the aligned pc rather than read off a listing -- so they belong to this
 * owner. The bound at the far end comes from the ROM: the halfword at
 * 0x2001878 is `b500`, the next function's prologue.
 *
 * WHAT IT DOES. `stmia r3!` with r3 = 0x040000d4 writes DMA3SAD, DMA3DAD and
 * DMA3CNT in one instruction: source `*(u32 *)0x03001ed0`, destination
 * 0x02009de0, control 0x840000e0 -- CNT_L 0x00e0 = 224 units, CNT_H 0x8400 =
 * enable plus 32-bit width. A DMA3 word copy of 224 words, launched by writing
 * the register triple.
 *
 * `subs r3, #12` restores a register that is dead at the `bx lr`. It is
 * compiler residue from the `!` writeback, not a fifth effect, and it is not
 * reconstructed here because nothing observes it.
 *
 * THE TWIN AT 0x02001858 IS NOT THIS ROW. Its sixteen code bytes are
 * byte-identical and three of its four pool words match; the destination is
 * 0x0200a4e0 against this row's 0x02009de0. Shared shape is not permission to
 * carry an answer across -- the one word that differs is the one word the
 * function exists to supply.
 *
 * 0 of 0 callees.
 */

void Func_02001838(void)
{
    vu32 *dma3 = (vu32 *)0x040000d4;

    dma3[0] = *(u32 *)0x03001ed0;
    dma3[1] = (u32)0x02009de0;
    dma3[2] = (u32)0x840000e0;
}
