#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCROLL.H"

/* Build the idle page of per-scanline background offsets, bending each line
   by a sine wave, then make it the page the H-blank DMA reads next. */
void DisplayScroll_BuildAndSwapHBlankPage(void)
{
    struct DisplayScrollWork *work = gHBlankScrollWork;
    s32 x3 = (s16)gBgScroll[3].x;
    s32 y3 = (s16)gBgScroll[3].y;
    s32 x2 = (s16)gBgScroll[2].x;
    s32 y2 = (s16)gBgScroll[2].y;
    s32 x1 = (s16)gBgScroll[1].x;
    s32 y1 = (s16)gBgScroll[1].y;
    u16 *row;
    s32 step;
    s32 phase;
    s32 amp;
    s32 i;
    u16 d;

    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->step_x;
    phase = (work->frame + (u16)y3) * work->freq_x;
    if (step == 0) {
        for (i = 0; i != 160; i++) {
            row[i * 6] = x3;
            row[i * 6 + 2] = x2;
            row[i * 6 + 4] = x1;
        }
    } else {
        u16 a = x3;
        u16 b = x2;
        u16 c = x1;

        amp = work->amp_x;
        for (i = 0; i != 160; i++) {
            d = Iwram_MulQ16(DisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
            *row = a + d;
            row += 2;
            *row = b + d;
            row += 2;
            *row = c + d;
            row += 2;
            phase += step;
        }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->step_y;
    phase = (work->frame + (u16)y3) * work->freq_y;
    if (step == 0) {
        for (i = 0; i != 160; i++) {
            row[i * 6] = y3;
            row[i * 6 + 2] = y2;
            row[i * 6 + 4] = y1;
        }
    } else {
        u16 a = y3;
        u16 b = y2;
        u16 c = y1;

        amp = work->amp_y;
        for (i = 0; i != 160; i++) {
            d = Iwram_MulQ16(DisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
            *row = a + d;
            row += 2;
            *row = b + d;
            row += 2;
            *row = c + d;
            row += 2;
            phase += step;
        }
    }
    work->frame++;
    work->page ^= 1;
}
