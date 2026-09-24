/* Draft, not exact (2026-09-24): candidate=488 reference=492 differing_halfwords=213
   (aligned, the loops match). Residual: the six BG offsets load in the order
   14,10,12,6,8,4 instead of 14..4, and 0xf02 is formed from the 0xf00 register
   where the reference loads it from the pool (and derives 0xf08 from 0xf10). */
#include "TYPES.H"
#include "IWRAM_CALL.H"

/* Two pages of 160 scanline rows, each the HOFS/VOFS pairs of BG0-BG2. */
struct ScrollWork {
    u16 rows[2][160][6];
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

/* Build the idle page of per-scanline background offsets, bending each line
   by a sine wave, then make it the page the H-blank DMA reads next. */
void DisplayScroll_BuildAndSwapHBlankPage(void)
{
    struct ScrollWork *work = *(struct ScrollWork **)0x03001ed8;
    s16 *ofs = (s16 *)0x03001ad0;
    u16 y0 = ofs[7];
    u16 x0 = ofs[6];
    u16 y1 = ofs[5];
    u16 x1 = ofs[4];
    u16 y2 = ofs[3];
    u16 x2 = ofs[2];
    u16 *row;
    s32 step;
    s32 phase;
    s32 amp;
    s32 i;
    u16 d;

    row = work->rows[work->page ^ 1][0];
    step = work->step_x;
    phase = (work->frame + y0) * work->freq_x;
    if (step == 0) {
        for (i = 0; i != 160; i++) {
            row[0] = x0;
            row[2] = x1;
            row[4] = x2;
            row += 6;
        }
    } else {
        amp = work->amp_x;
        for (i = 0; i != 160; i++) {
            s16 *sine = (s16 *)0x0809ed84;
            d = Iwram_MulQ16(sine[(phase >> 16) & 255], amp) / 256;
            *row = x0 + d;
            row += 2;
            *row = x1 + d;
            row += 2;
            *row = x2 + d;
            row += 2;
            phase += step;
        }
    }

    row = &work->rows[work->page ^ 1][0][1];
    step = work->step_y;
    phase = (work->frame + y0) * work->freq_y;
    if (step == 0) {
        for (i = 0; i != 160; i++) {
            row[0] = y0;
            row[2] = y1;
            row[4] = y2;
            row += 6;
        }
    } else {
        amp = work->amp_y;
        for (i = 0; i != 160; i++) {
            s16 *sine = (s16 *)0x0809ed84;
            d = Iwram_MulQ16(sine[(phase >> 16) & 255], amp) / 256;
            *row = y0 + d;
            row += 2;
            *row = y1 + d;
            row += 2;
            *row = y2 + d;
            row += 2;
            phase += step;
        }
    }

    work->frame++;
    work->page ^= 1;
}
