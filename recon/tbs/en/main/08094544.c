/* Draft, not exact (2026-09-24): candidate=488 reference=492 differing_halfwords=213
   (aligned, the loops match). Residual: the six BG offsets load in the order
   14,10,12,6,8,4 instead of 14..4, and 0xf02 is formed from the 0xf00 register
   where the reference loads it from the pool (and derives 0xf08 from 0xf10).
   2026-09-27 H1: reuse MAP_SCROLL.H's BgScroll pair and named shadow/work
   globals. SCROLL_ARM_HBLANK_DMA consumes three words per row, starting at
   BG1HOFS; the builder supplies shadow BG3, BG2, BG1 in that order. Test
   whether this pair interface restores the six initial loads and row cursor
   ownership. Result: 504/492 bytes, 231 differing halfwords, 116 aligned
   edits (baseline 488/492, 213/64). Pair-member stores introduce redundant
   zero extensions in both zero-step branches; frame becomes 20 instead of
   24 bytes and the vertical cursor stays at x with +2 stores, unlike ROM.
   Reject the pair-member cursor, retain the proven three-pair page layout.
   IWRAM_CALL.H unchanged. No exact credit.
   2026-09-29 (alchemy permute scorer): the draft scored 4533 (50
   register-only, 3 stack-only, 16 operand, 15 reordered, 17 inserted, 13
   deleted). This body
   is the permuter's best after a 300-second search (about 40,000 candidates):
   3472 (41 register-only, 2 stack-only, 15 operand, 12 reordered,
   13 inserted, 9 deleted). Its rewrites are search output, not a
   reading of the ROM; the literal ROM address still keeps it a draft.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "MAP_SCROLL.H"

/* Two pages of 160 scanline rows, consumed as BG1-BG3 HOFS/VOFS pairs. */
struct ScrollWork {
    struct BgScroll rows[2][160][3];
    u8 page;
    u8 mode;
    u16 frame;
    u8 unknown_f04[4];
    s32 freq_x;
    s32 freq_y;
    s32 step_x;
    s32 step_y;
    s32 amp_x;
    s32 amp_y;
};
extern struct ScrollWork *gHBlankScrollWork;

/* Build the idle page of per-scanline background offsets, bending each line
   by a sine wave, then make it the page the H-blank DMA reads next. */
void DisplayScroll_BuildAndSwapHBlankPage(void)
{
    register struct ScrollWork *work = gHBlankScrollWork;
    u16 y0 = (s16)Data_03001ad0[3].y;
    u16 y1 = (Data_03001ad0 + 2)->y;
    u16 x0 = (s16)Data_03001ad0[3].x;
    u16 x2 = (s16)Data_03001ad0[1].x;
    s32 step;
    register struct BgScroll *row;
    u16 x1 = (s16)Data_03001ad0[2].x;
    s32 phase;
    s32 amp;
    register u16 y2 = (s16)Data_03001ad0[1].y;
    s32 i;
    u16 d;

    row = work->rows[work->page ^ 1][0];
    step = work->step_x;
    phase = (work->frame + y0) * work->freq_x;
    if (step != 0 == 0) {
        i = 0;
        if (i != 160) {
            do {
                row[0].x = x0;
                row[1].x = x1;
                i++;
                row[2].x = x2;
                row += 3;
            } while (i != 160);
        }
    } else {
        amp = work->amp_x;
        i = 0;
        while (i != 160) {
            register s16 *sine = (s16 *)0x0809ed84;
            d = Iwram_MulQ16(sine[(phase >> 16) & 255], amp) / 256;
            row->x = d + x0;
            row++;
            row->x = x1 + d;
            row++;
            row->x = (s16)d + x2;
            row++;
            i++;
            phase += step;
        }
    }
    row = (struct BgScroll *)work->rows[work->page ^ 1][0];
    step = work->step_y;
    phase = (work->frame + y0) * work->freq_y;
    if (step == 0) {
        i = 0;
        if (160 != i) {
            do {
                row[0].y = y0;
                row[1].y = y1;
                row[2].y = y2;
                row += 3;
                i++;
            } while (i != 160);
        }
    } else {
        i = 0;
        amp = work->amp_y;
        if (160 != i) {
            do {
                s16 *sine = (s16 *)0x0809ed84;
                d = Iwram_MulQ16(sine[(phase >> 16) & 255], amp) / 256;
                row->y = y0 + d;
                row += 1;
                row->y = y1 + d;
                phase += step;
                row++;
                row->y = d + y2;
                row++;
                i++;
            } while (160 != i);
        }
    }
    work->frame++;
    work->page ^= 1;
}
