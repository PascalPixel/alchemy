/* A great whirlwind's sway (Air's Rock, Shaman Village): each screen line of
   BG0 is pushed side to side by a jittered sine wave, set line by line by
   HBlank DMA 0. Each overlay that has it includes this body after naming the
   function (WAVE_SCROLL) and its table of a scroll for each screen line
   (WAVE_SCROLL_LINES). */

#include "system.h"
#include "resource_ids.h"

extern Sint16 WAVE_SCROLL_LINES[];

void WAVE_SCROLL(void)
{
	Sint16			*line = WAVE_SCROLL_LINES;
	Sint32			 i;
	Sint16			*src;
	volatile Uint16 *scroll;
	volatile Uint16 *dma;

	for(i = 0; i != DISPLAY_HEIGHT; i++, line++) {
		Sint32 range = (DISPLAY_HEIGHT - i) >> 2;

		/* A sine of the line and the tick, (WAVE_SCROLL_CALM_LINE - i) / 2
		   pixels either way, less 4, jittered by up to a quarter of the lines
		   below. */
		*line = (syMulFX(sySin((i + (Uint8)TickCount) * (TRIG_TURN / 256)),
					 (WAVE_SCROLL_CALM_LINE - i) * (toFIXED(1) / 64)) >>
					11) -
			4;
		*line += syRandom() % (range * 2) - range;
		if(*line > WAVE_SCROLL_MAX)
			*line -= WAVE_SCROLL_MAX;
		else if(*line < WAVE_SCROLL_MIN)
			*line -= WAVE_SCROLL_MIN;
	}
	line = WAVE_SCROLL_LINES;
	scroll = (volatile Uint16 *)REG_BG0HOFS;
	*scroll = *line;
	dma = REG_DMA0;
	dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
	dma[5] &= ~DMA_ENABLE;
	src = line + 1;
	(void)dma[5];
	syDMAXCopy(src, (void *)scroll, DMA_HBLANK_HALFWORD_PER_LINE,
		(volatile Uint32 *)dma);
}
