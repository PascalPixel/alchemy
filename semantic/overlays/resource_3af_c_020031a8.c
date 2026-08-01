typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

/*
 * resource_3af owner at 0x020031a8, 220 bytes: the overlay's per-frame
 * drift task -- it nudges a position pair along a trig heading, walks
 * two 16-bit phase accumulators forward by random steps, and advances a
 * wrapped 2-axis offset that it also publishes into the map context.
 *
 * NOT IN ANY INVENTORY, but its installer IS drafted: the 0x02001444
 * entry driver calls `Func_080000d0((void *)((s32)&Func_020031a8 | 1),
 * 3200)` on two of its arms, through the pool word 0x0200b1a9. So this
 * is the one member of the overlay's published-callback set whose
 * publication site is code rather than a script-record table, and the
 * driver draft is what named it in the first place.
 *
 * The trig step. `Func_08000120` is the sine lobe and `Func_08000118`
 * the cosine lobe of the ROM trig pair (dispatch slots 12 and 11).
 * Both are called with the CURRENT value of an accumulator, and the
 * results are added to consecutive words of a pointer held in the map
 * context's first field:
 *
 *     p[0] += Func_08000120(*phaseA) >> 1;        (arithmetic shift)
 *     p[1] += Func_08000118(*phaseB);
 *
 * The `stmia r6!, {r3}` is a store-and-bump of that pointer, not a
 * block move -- one register in the list.
 *
 * The two accumulators then advance by scaled random draws, both built
 * from shift/add chains and both masked back to 16 bits:
 *
 *     *phaseA += (draw * 384) >> 16;    (r*2 + r = r*3, then <<7)
 *     *phaseA  = (u16)*phaseA;      (mask spelled as a ldrh/str pair)
 *     *phaseB  = (*phaseB + ((draw2 * 512) >> 16)) & 0xffff;
 *
 * The first mask is easy to miss: it is spelled `ldrh r2,[r5,#0]` then
 * `str r2,[r5,#0]`, reading the low halfword back and storing it as a
 * word. That is a truncation, not a redundant reload.
 *
 * The offset step publishes BEFORE it updates -- `context[268]` and
 * `context[272]` receive the offsets as they were on entry, and only
 * then are they decremented by the per-frame delta and wrapped. The
 * two axes are NOT treated the same:
 *
 *     axis 0: subtract delta; if negative add 0x200000; if now above
 *             0x200000 subtract 0x200000 (spelled as adding
 *             0xffe00000, and the compare is `ble`, so exactly
 *             0x200000 is left alone)
 *     axis 1: subtract delta; if negative add 0x200000. No upper wrap.
 *
 * The missing upper wrap on axis 1 is transcribed as compiled. It may
 * be deliberate (the axis-1 delta may never be negative) or an
 * asymmetry in the original; nothing in this function settles it, so it
 * is recorded rather than "fixed".
 *
 * Complete owner: `push {r5, r6, lr}` plus the sl/r9 and r8 spills
 * (`mov r6,sl / mov r5,r9 / push {r5, r6}` then `mov r6,r8 /
 * push {r6}`) at 0x020031a8, through the matching unwind
 * `pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * pop {r5, r6} / pop {r0} / bx r0` at 0x02003258-0x02003264; an
 * alignment halfword and seven pool words (0x03001e70, 0x0200db58,
 * 0x0200db38, 0x0000ffff, 0x0200db50, 0x0200db60, 0xffe00000) end at
 * 0x02003283, and the next owner (0x02003284, already banked
 * byte-exact) begins at 0x02003284.
 *
 * Cross-link: 0x0200db50 and 0x0200db60 are the same two blocks the
 * driver and 0x02003a0c write directly -- the driver at offset +4,
 * 0x02003a0c at offset +0, and this task reads and updates both words
 * of each. Three owners sharing one pair of records.
 *
 * Uncertainty: the scale constants 384 and 512, the wrap bound
 * 0x200000 and the context displacements 268/272 are exact; what the
 * quantities represent is not established.
 */

extern u8 *Data_03001e70;       /* map context pointer cell, IWRAM */
extern s32 Data_0200db38;       /* phase accumulator B */
extern s32 Data_0200db50[];     /* wrapped 2-axis offset */
extern s32 Data_0200db58;       /* phase accumulator A */
extern s32 Data_0200db60[];     /* per-frame delta for the offset pair */

extern s32 Func_08000118(s32 angle);    /* trig, cosine lobe */
extern s32 Func_08000120(s32 angle);    /* trig, sine lobe */
extern s32 Func_080000f8(void);

void Func_020031a8(void)
{
    u8 *context;
    s32 *cursor;
    s32 axis;

    context = Data_03001e70;
    cursor = *(s32 **)context;

    cursor[0] += Func_08000120(Data_0200db58) >> 1;
    cursor[1] += Func_08000118(Data_0200db38);

    Data_0200db58 += (s32)(((u32)Func_080000f8() * 384) >> 16);
    Data_0200db58 = *(u16 *)&Data_0200db58;
    Data_0200db38 = (Data_0200db38 + (s32)(((u32)Func_080000f8() * 512) >> 16)) & 0xffff;

    *(s32 *)(context + 268) = Data_0200db50[0];
    axis = Data_0200db50[0] - Data_0200db60[0];
    Data_0200db50[0] = axis;
    if (axis < 0) {
        Data_0200db50[0] = axis + (128 << 14);
    }
    if (Data_0200db50[0] > (128 << 14)) {
        Data_0200db50[0] += (s32)0xffe00000;
    }

    *(s32 *)(context + 272) = Data_0200db50[1];
    axis = Data_0200db50[1] - Data_0200db60[1];
    Data_0200db50[1] = axis;
    if (axis < 0) {
        Data_0200db50[1] = axis + (128 << 14);
    }
}
