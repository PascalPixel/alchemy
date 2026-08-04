typedef unsigned int u32;
typedef volatile unsigned int vu32;

/*
 * resource_395 owner at 0x02001858, 32 bytes: 16 of code, a four-word pool.
 *
 * TAIL LEAF, the twin of 0x02001838. Same provenance: past the last recorded
 * owner so sweep D's gap loop never reached it, no `push` for sweep C, absent
 * from `overlay_call_targets` (sites=756, a live run), and no published word
 * anywhere in the image under base+0x8000 with or without the Thumb bit.
 *
 * The ROM, verbatim:
 *
 *   ldr r2, [pc, #12]   -> 0x03001ed0
 *   ldr r3, [pc, #16]   -> 0x040000d4   (DMA3SAD)
 *   ldr r0, [r2]
 *   ldr r1, [pc, #16]   -> 0x0200a4e0
 *   ldr r2, [pc, #16]   -> 0x840000e0
 *   stmia r3!, {r0, r1, r2}
 *   subs r3, #12
 *   bx lr
 *
 * SPAN IS 32 BYTES, NOT 16, for the same reason as its twin: the four pool
 * words at 0x2001868-0x2001877 are each reached by this row's own pc-relative
 * loads, re-derived from the aligned pc. The far bound is the ROM's: `b500` at
 * 0x2001878, the next function's prologue.
 *
 * READ INDEPENDENTLY OF ITS TWIN, and the difference is the whole point. The
 * sixteen code bytes are byte-identical to 0x02001838 and three of the four
 * pool words match. The destination differs -- 0x0200a4e0 here against
 * 0x02009de0 there -- so the two rows launch DMA3 copies of the same 224 words
 * from the same source pointer into DIFFERENT destinations. Copying either
 * body onto the other would verify green and write the wrong buffer, which is
 * the 3c9/3a1 hazard at a smaller scale.
 *
 * 0 of 0 callees.
 */

void Func_02001858(void)
{
    vu32 *dma3 = (vu32 *)0x040000d4;

    dma3[0] = *(u32 *)0x03001ed0;
    dma3[1] = (u32)0x0200a4e0;
    dma3[2] = (u32)0x840000e0;
}
