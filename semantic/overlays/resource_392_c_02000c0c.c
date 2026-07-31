typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_392 owner at 0x02000c0c, 64 bytes: install one of two palettes and
 * publish the result.
 *
 * Complete owner: `push {lr}` at 0x02000c0c through `pop {r0} / bx r0` at
 * 0x02000c34-0x02000c36.  The return address is popped into r0, so the owner
 * is `void` (HANDOVER section 0, epilogue rule).  r0 is tested before it is
 * written, so it takes one argument.  The 24-byte literal pool at
 * 0x02000c38-0x02000c4f is inside the advertised span and after the epilogue.
 *
 * Two call sites, matching the inventory's calls=2, resolved with
 * `bun tools/overlay_call_targets.ts resource_392 0c0c`:
 *   0x02000c2c -> veneer 0x0e64 -> Func_0808a330    (import)
 *   0x02000c30 -> in-image prologue 0x02000bcc      (this overlay's own)
 * The printed annotations (0x02001a92, 0x020017fe) are the usual `bl` decoding
 * artefact; the stored displacement is the target's image offset minus two.
 * 0x02000bcc is outside this owner's span and is already converted as
 * semantic/overlays/resource_392_c_02000bcc.c, so it is a genuine call and not
 * a `goto`.
 *
 * Pool words, against this overlay's proven 0x02008000 link base:
 *   0x03001ed0  IWRAM pointer slot -- holds the destination buffer.
 *   0x040000d4  DMA3SAD; the owner writes SAD/DAD/CNT with one
 *               `stmia r3!, {r0, r1, r2}`, the established DMA3 spelling.
 *   0x020097b0  in-image data at file offset 0x17b0 (the "set" palette).
 *   0x020090b0  in-image data at file offset 0x10b0 (the "clear" palette).
 *   0x840000e0  the DMA3 control word: enable, 32-bit, 224 words = 896 bytes.
 * The `subs r3,#12` after the `stmia` merely rewinds the auto-incremented
 * register and is dead, exactly as documented for resource_381:330c.
 *
 * The two arms differ ONLY in which source word they load -- both load
 * 0x040000d4 into r3 and both fall into the same `stmia`.  They are written
 * here as one transfer with a selected source, which is what the code does;
 * there is no call inside either arm, so nothing in the multiset depends on
 * the choice.
 *
 * Uncertainties.  Which of the two palettes is the "on" state is taken from
 * the polarity of the test (a non-zero argument selects 0x020097b0) and not
 * from any consumer.  Func_0808a330's arguments here are (0x00010000, 0); the
 * tree's other sites pass (0x00010000, 1) and read it as "publish the
 * completed work buffer", so the second argument looks like a flag whose two
 * values are both in use, but this owner does not settle it.
 */

/* Overlay import (via the veneer table).  Old-style declaration: one import
 * name can be reached with different argument counts at different sites. */
void Func_0808a330();       /* publish the completed work buffer */

/* This overlay's own follow-up, already converted. */
void Func_02000bcc(void);

/* In-image palettes at `pool word - 0x8000`.  The overlay image is writable
 * EWRAM, so these are deliberately not `const`. */
extern u8 Data_020090b0[];
extern u8 Data_020097b0[];

static void Dma3_02000c0c(const void *source, void *destination, u32 control)
{
    /* One `stmia r3!, {r0, r1, r2}` from 0x040000d4 in the original. */
    *(volatile u32 *)0x040000d4 = (u32)source;
    *(volatile u32 *)0x040000d8 = (u32)destination;
    *(volatile u32 *)0x040000dc = control;
}

void Func_02000c0c(s32 selected)
{
    void *destination = *(void **)0x03001ed0;
    const void *source = selected != 0 ? Data_020097b0 : Data_020090b0;

    Dma3_02000c0c(source, destination, 0x840000e0);   /* 224 words */

    Func_0808a330(0x00010000, 0);
    Func_02000bcc();
}
