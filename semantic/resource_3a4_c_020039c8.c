typedef unsigned short u16;
typedef signed char s8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x020039c8, 124 bytes: a palette-cycling tick.
 * Count a hold counter down each frame; when it runs out, read the next
 * (entry, hold) byte pair from an in-image script, rewinding to the start
 * on a -1 terminator, then kick DMA3 to copy nine halfwords from the entry's
 * slot in the table at Data_03001ed0 into palette RAM at 0x05000006.
 *
 * A PUBLISHED CALLBACK (sweep B): no `bl` in the image reaches it. No
 * arguments, void.
 *
 * measureSpan IS WRONG HERE, AND IN THE OTHER DIRECTION. It reports 356
 * bytes, ending 0x02003b2c -- it runs straight through the next owner and
 * on into the import-veneer bank at 0x02003a70. Every previous row on this
 * overlay ran PAST measureSpan, so "treat it as a lower bound" had been the
 * working rule; this row breaks that rule the other way and would have been
 * drafted 232 bytes too long if trusted. The true end is
 * fixed by the already-recorded owner at 0x02003a44 (44 bytes, per
 * semantic/regions.json), so the span is 0x02003a44 - 0x020039c8 = 124.
 * measureSpan is not a bound in either direction; it is a hint.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020039c8 through the
 * interworking return `pop {r5, r6} / pop {r0} / bx r0` at 0x02003a2e-
 * 0x02003a32, then the four-word literal pool 0x02003a34-0x02003a43. There
 * is a SECOND, INTERIOR pool at 0x020039f6-0x02003a0b, sitting between the
 * rewind branch and the code that resumes at 0x02003a0c -- the pool trap in
 * its usual place, mid-function rather than at the end.
 *
 * ZERO call sites, and that claim is checked rather than assumed.
 * `bun tools/overlay_call_targets.ts resource_3a4 0x39c8 0x3a44` returns
 * sites=0, which on this overlay is exactly what a silently mis-invoked
 * tool also returns, so it was confirmed independently by scanning every
 * halfword pair in the span for a BL prefix/suffix encoding: none. The row
 * really does make no calls; it drives the hardware directly.
 *
 * A worked example of why coverage is not a certificate. Run the guard
 * without an explicit end and it reports `0/1 SHORTFALL -- absent from the
 * draft: Func_080000d0`. That callee is real, but it belongs to the NEXT
 * owner at 0x02003a44, which the guard's 356-byte span swallowed. Given
 * the correct bounds (`m2c_guard resource_3a4 39c8 3a44`) it reports 0/0.
 * A coverage number is only as good as the span it was computed over, and
 * on this row the default span is wrong.
 *
 * The DMA kick is one `stmia r3!, {r0, r1, r2}` at 0x02003a24 writing
 * DMA3SAD, DMA3DAD and DMA3CNT in one instruction, with the pointless
 * `subs r3, #12` after it restoring the register the write-back moved.
 * Expressed as three volatile word stores, which is the tree's existing
 * spelling for this at other sites. 0x80000009 is enable plus nine 16-bit
 * units; 0x05000006 is palette RAM entry 3.
 *
 * Globals, all read directly, none passed in:
 *   Data_03001ed0  a base pointer word in the workspace block; the DMA
 *                  source is this pointer plus (entry << 1)
 *   Data_0200d25c  image offset 0x525c, the hold counter -- read signed at
 *                  the top, written signed from the script, decremented as
 *                  a halfword at the bottom
 *   Data_0200d260  image offset 0x5260, the script cursor
 *   Data_0200bd28  image offset 0x3d28, the s8 script itself, read with a
 *                  SIGN-EXTENDING ldrsb so the -1 terminator is a value,
 *                  not a large positive index
 *
 * One anomaly recorded without a theory: the prologue is `push {r5, r6,
 * lr}` yet the body loads r4 twice (0x020039cc, 0x02003a16) and never
 * restores it. r4 is callee-saved, so this owner clobbers a register it
 * did not preserve. Observed from the encodings, not explained.
 */

extern s32 Data_03001ed0;
extern s16 Data_0200d25c;      /* image offset 0x525c */
extern u16 Data_0200d260;      /* image offset 0x5260 */
extern s8 Data_0200bd28[];     /* image offset 0x3d28 */

void Func_020039c8(void)
{
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
    u16 cursor;
    s8 entry;

    if (Data_0200d25c <= 0) {
        for (;;) {
            cursor = Data_0200d260;
            entry = Data_0200bd28[(s16)cursor];
            Data_0200d260 = (u16)(cursor + 1);
            if (entry != -1) {
                break;
            }
            Data_0200d260 = 0;
        }

        Data_0200d260 = (u16)(cursor + 2);
        Data_0200d25c = Data_0200bd28[(s16)(cursor + 1)];

        dma3[0] = (u32)(Data_03001ed0 + (entry << 1));
        dma3[1] = 0x05000006;
        dma3[2] = 0x80000009;
    }

    Data_0200d25c = (s16)(Data_0200d25c - 1);
}
