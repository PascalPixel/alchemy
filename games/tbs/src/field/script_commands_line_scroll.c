/* The line scroll: per-line scroll offsets fed by HBlank DMA. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* VBlank-pass event for the line scroll: restarts DMA 0 each VBlank to copy the
   current page's BG1 to BG3 scroll offsets into the scroll registers one
   scanline at a time. */
void fdLineScrollArmDMA(void)
{
	volatile Uint16	  *dma;
	LINE_SCROLL_STATE *state;
	Uint32			  *src;
	Uint16			   stat;
	Uint32			   control;
	volatile Uint32	  *dst;

	state = LineScroll;
	src = (Uint32 *)state->rows[state->page];

	dma = REG_DMA0;
	dst = (volatile Uint32 *)&REG_BG1HOFS;
	stat = dma[DMA_CNT_H_INDEX];
	control = stat & (Uint16)~DMA_START_TIMING_DRQ_REPEAT;
	dma[DMA_CNT_H_INDEX] = control;
	stat = dma[DMA_CNT_H_INDEX];
	control = stat & (Uint16)~DMA_ENABLE;
	dma[DMA_CNT_H_INDEX] = control;
	(void)dma[DMA_CNT_H_INDEX];

	*dst = *src++;
	*dst = *src++;
	*dst = *src++;

	control = DMA_HBLANK_WORDS_PER_LINE(3);
	syDMAXCopy(src, (void *)dst, control, (volatile Uint32 *)dma);
}

/* Event for the line scroll: fills the hidden page with each scanline's BG3,
   BG2 and BG1 offsets, the scroll plus a sine wave for x and for y (an axis
   with no step stays flat), then shows that page. */
void fdLineScrollBuildPage(void)
{
	LINE_SCROLL_STATE *state = LineScroll;
	Sint32			   x3 = (Sint16)BgScrollWork[3].x;
	Sint32			   y3 = (Sint16)BgScrollWork[3].y;
	Sint32			   x2 = (Sint16)BgScrollWork[2].x;
	Sint32			   y2 = (Sint16)BgScrollWork[2].y;
	Sint32			   x1 = (Sint16)BgScrollWork[1].x;
	Sint32			   y1 = (Sint16)BgScrollWork[1].y;
	Uint16			  *row;
	Sint32			   step;
	FIXED			   phase;
	FIXED			   amplitude;
	Sint32			   scanline;
	Uint16			   wave;

	row = &state->rows[state->page ^ 1][0][0].x;
	step = state->stepX;
	phase = (state->frm + (Uint16)y3) * state->freqX;
	if(step == 0) {
		for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
			row[scanline * 6] = x3;
			row[scanline * 6 + 2] = x2;
			row[scanline * 6 + 4] = x1;
		}
	} else {
		Uint16 scroll3 = x3;
		Uint16 scroll2 = x2;
		Uint16 scroll1 = x1;

		amplitude = state->ampX;
		for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
			wave = syMulFX(LineScrollWaveSine[FIXEDtoINT(phase) &
							   (LINE_SCROLL_WAVE_ENTRIES - 1)],
					   amplitude) /
				LINE_SCROLL_WAVE_SCALE;
			*row = scroll3 + wave;
			row += 2;
			*row = scroll2 + wave;
			row += 2;
			*row = scroll1 + wave;
			row += 2;
			phase += step;
		}
	}
	row = &state->rows[state->page ^ 1][0][0].y;
	step = state->stepY;
	phase = (state->frm + (Uint16)y3) * state->freqY;
	if(step == 0) {
		for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
			row[scanline * 6] = y3;
			row[scanline * 6 + 2] = y2;
			row[scanline * 6 + 4] = y1;
		}
	} else {
		Uint16 scroll3 = y3;
		Uint16 scroll2 = y2;
		Uint16 scroll1 = y1;

		amplitude = state->ampY;
		for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
			wave = syMulFX(LineScrollWaveSine[FIXEDtoINT(phase) &
							   (LINE_SCROLL_WAVE_ENTRIES - 1)],
					   amplitude) /
				LINE_SCROLL_WAVE_SCALE;
			*row = scroll3 + wave;
			row += 2;
			*row = scroll2 + wave;
			row += 2;
			*row = scroll1 + wave;
			row += 2;
			phase += step;
		}
	}
	state->frm++;
	state->page ^= 1;
}

/* The shared source that starts the line scroll, at its place in the ROM. */
#include "../scene/shared/lemurian_ship_ripple.c"

/* Stops the line scroll: removes its two events and stops DMA 0. */
void fdLineScrollStop(void)
{
	volatile Uint16 *dma0;

	syCloseEvent(fdLineScrollArmDMA);
	syCloseEvent(fdLineScrollBuildPage);
	dma0 = REG_DMA0;
	dma0[DMA_CNT_H_INDEX] =
		(Uint16)~DMA_START_TIMING_DRQ_REPEAT & dma0[DMA_CNT_H_INDEX];
	dma0[DMA_CNT_H_INDEX] = (Uint16)~DMA_ENABLE & dma0[DMA_CNT_H_INDEX];
	(void)dma0[DMA_CNT_H_INDEX];
}
