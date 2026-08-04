#include "types.h"

/*
 * Resource 3ca scene entry point at 0x020012ac (104 bytes,
 * 0x020012ac-0x02001313).
 *
 * Complete owner: `push {r5, r6, lr}` / `sub sp, #4` at 0x020012ac, matching
 * `add sp, #4 / pop {r5, r6} / pop {r0} / bx r0` at 0x020012f0.  The popped
 * register is r0, so it holds the return address and the owner is `void`.
 * Everything from 0x020012f8 is this owner's literal pool and is data:
 *
 *   0x020012f8  0x00000030   the value written to the +0x1c18 halfword
 *   0x020012fc  0x02009c1a   in-image halfword, file offset 0x1c1a
 *   0x02001300  0x11111111   the DMA fill pattern
 *   0x02001304  0x040000d4   DMA3 source register
 *   0x02001308  0x85000040   DMA3 control: enable, 32-bit, source fixed, 64
 *   0x0200130c  0x02009c18   in-image halfword, file offset 0x1c18
 *   0x02001310  0x020091c5   Func_020011c4 + Thumb bit — the installed task
 *
 * That last word is the cheapest available witness for the 0x02008000 link
 * base and it pays twice: it is an odd in-image pool word passed to
 * Func_080000d0, so it both proves the base (0x020091c5 - 1 - 0x8000 = file
 * offset 0x11c4, an ordinary `push` prologue and a strict-queue row of this
 * overlay) and names the per-frame task this entry point installs.
 *
 * All four call sites resolved with tools/overlay_call_targets.ts; the
 * disassembler's own `bl` annotations are wrong for overlays by construction:
 *
 *   0x020012b4 -> veneer 0x0200134c -> Func_08000170
 *   0x020012bc -> veneer 0x0200135c -> Func_080001d0
 *   0x020012dc -> veneer 0x02001354 -> Func_080001c8
 *   0x020012ec -> veneer 0x02001324 -> Func_080000d0
 *
 * Placed calls per target: Func_08000170 x1, Func_080001d0 x1, Func_080001c8
 * x1, Func_080000d0 x1 — matching the four resolved sites exactly.
 */

extern u16 Data_02009c18;
extern u16 Data_02009c1a;

/* Main-image imports reached through this overlay's veneer table.  Old-style
 * declarations are mandatory here: one import name is reached with different
 * argument counts at different sites in this overlay. */
void *Func_08000170();   /* allocator: returns a buffer of the requested size */
s32 Func_080001d0();
void Func_080001c8();
void Func_080000d0();    /* installs a per-frame task */

/* The task this entry point publishes; its body is the 0x020011c4 row. */
void Func_020011c4(void);

void Func_020012ac(void)
{
    u32 fill;
    u32 *buffer;
    volatile u32 *dma3;

    buffer = (u32 *)Func_08000170(256);

    /*
     * r0 is not reloaded between the two branches, so the buffer pointer is
     * still in r0 when Func_080001d0 is entered.  Whether that callee reads it
     * is unverified; the dataflow is preserved as written.
     */
    Data_02009c1a = (u16)Func_080001d0(buffer);

    /*
     * Clear the buffer by DMA from a single stack word: control 0x85000040 is
     * enable + 32-bit units + fixed source, 64 units — exactly the 256 bytes
     * just allocated.
     */
    fill = 0x11111111;
    dma3 = (volatile u32 *)0x040000D4;
    dma3[0] = (u32)&fill;
    dma3[1] = (u32)buffer;
    dma3[2] = 0x85000040;
    /* The `subs r3, #12` after the `stmia r3!` restores the DMA base and is
     * then dead; it has no observable effect. */

    Func_080001c8((s16)Data_02009c1a, 256, buffer);

    Data_02009c18 = 0x30;
    Func_080000d0(Func_020011c4, 3200);
}
