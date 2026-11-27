/* The line scroll: per-line scroll offsets fed by HBlank DMA. Part of
   script_commands.c. */

/* LINE_SCROLL_STATE.mode: 0 waves BG1, BG2 and BG3 alike; any other mode waves
   BG3 alone (Mars Lighthouse's rain ledges). */
#define LINE_SCROLL_ALL_LAYERS	 0
/* LineScrollWaveSine's entries, a whole cycle: a phase is 16.16 entries. */
#define LINE_SCROLL_WAVE_ENTRIES 256
/* The sine's units in a pixel: its peak, 4096, is 16 pixels at an amplitude of
   toFIXED(1). */
#define LINE_SCROLL_WAVE_UNITS	 256
/* The wave's offset in pixels at phase, for a 16.16 amplitude. */
#define LINE_SCROLL_WAVE(phase, amplitude) \
	(syMulFX(LineScrollWaveSine[FIXEDtoINT(phase) & \
				 (LINE_SCROLL_WAVE_ENTRIES - 1)], \
		 (amplitude)) / \
		LINE_SCROLL_WAVE_UNITS)

/* VBlank-pass event for the line scroll: restarts DMA 0 each VBlank to copy the
   current page's BG1 to BG3 scroll offsets into the scroll registers one
   scanline at a time, but not while Reveal shows or a battle is due. */
void fdLineScrollArmDMA(void)
{
	LINE_SCROLL_STATE *state = Ram_HeapSlots->lineScroll;
	Uint32			  *line = (Uint32 *)state->rows[state->page];
	volatile Uint32	  *dest = (volatile Uint32 *)&REG_BG1HOFS;
	FIELD_STATE		  *event = Ram_HeapSlots->fieldState;
	volatile Uint32	  *tgt;

	if(event != NULL) {
		if(event->revealActive != 0)
			return;
		if(event->encounter != 0)
			return;
	}
	{
		volatile Uint16 *dma = REG_DMA0;
		Uint16			 control;

		control = dma[DMA0_CONTROL];
		dma[DMA0_CONTROL] = control & (Uint16)~DMA_START_TIMING_DRQ_REPEAT;
		dma[DMA0_CONTROL] &= ~DMA_ENABLE;
		tgt = dest;
		(void)dma[DMA0_CONTROL];
	}
	*dest = *line++;
	*dest = *line++;
	*dest = *line++;
	syDMAXCopy(line, (void *)tgt,
		(DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD |
			DMA_32BIT)
				<< DMA_CONTROL_SHIFT |
			sizeof(state->rows[0][0]) / 4,
		REG_DMA0);
}

/* Event for the line scroll: fills the hidden page with each scanline's BG1,
   BG2 and BG3 offsets, the scroll plus a sine wave for x and for y (an axis
   with no step stays flat; outside LINE_SCROLL_ALL_LAYERS only BG3 waves), its
   phase following BG1's y, then shows that page. */
void fdLineScrollBuildPage(void)
{
	LINE_SCROLL_STATE *state = Ram_HeapSlots->lineScroll;
	Uint8			   mode = state->mode;
	Sint32			   x1 = (Sint16)BgScrollWork[1].x;
	Sint32			   y1 = (Sint16)BgScrollWork[1].y;
	Sint32			   x2 = (Sint16)BgScrollWork[2].x;
	Sint32			   y2 = (Sint16)BgScrollWork[2].y;
	Sint32			   x3 = (Sint16)BgScrollWork[3].x;
	Sint32			   y3 = (Sint16)BgScrollWork[3].y;
	FIELD_STATE		  *event = Ram_HeapSlots->fieldState;
	Uint16			  *row;
	Sint32			   step;
	Sint32			   phase;
	FIXED			   amplitude;
	Sint32			   scanline;
	Uint16			   wave;
	Uint16			   waveX3;
	Uint16			   waveY3;

	if(event != NULL && event->revealActive != 0)
		return;
	if(mode == LINE_SCROLL_ALL_LAYERS) {
		row = &state->rows[state->page ^ 1][0][0].x;
		step = state->stepX;
		phase = (state->frm + (Uint16)y1) * state->freqX;
		if(step == 0) {
			for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
				row[scanline * 6] = x1;
				row[scanline * 6 + 2] = x2;
				row[scanline * 6 + 4] = x3;
			}
		} else {
			Uint16 bg1 = x1;
			Uint16 bg2 = x2;

			waveX3 = x3;
			amplitude = state->ampX;
			for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
				wave = LINE_SCROLL_WAVE(phase, amplitude);
				row[0] = bg1 + wave;
				row[2] = bg2 + wave;
				row[4] = waveX3 + wave;
				row += 6;
				phase += step;
			}
		}
		row = &state->rows[state->page ^ 1][0][0].y;
		step = state->stepY;
		phase = (state->frm + (Uint16)y1) * state->freqY;
		if(step == 0) {
			for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
				row[scanline * 6] = y1;
				row[scanline * 6 + 2] = y2;
				row[scanline * 6 + 4] = y3;
			}
		} else {
			Uint16 bg1 = y1;
			Uint16 bg2 = y2;
			waveY3 = y3;
			amplitude = state->ampY;
			for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
				wave = LINE_SCROLL_WAVE(phase, amplitude);
				*row = bg1 + wave;
				row += 2;
				*row = bg2 + wave;
				row += 2;
				*row = waveY3 + wave;
				row += 2;
				phase += step;
			}
		}
	} else {
		row = &state->rows[state->page ^ 1][0][0].x;
		step = state->stepX;
		phase = (state->frm + (Uint16)y1) * state->freqX;
		if(step == 0) {
			for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
				row[scanline * 6] = x1;
				row[scanline * 6 + 2] = x2;
				row[scanline * 6 + 4] = x3;
			}
		} else {
			waveX3 = x3;
			amplitude = state->ampX;
			for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
				wave = LINE_SCROLL_WAVE(phase, amplitude);
				row[0] = x1;
				row[2] = x2;
				row[4] = waveX3 + wave;
				row += 6;
				phase += step;
			}
		}
		row = &state->rows[state->page ^ 1][0][0].y;
		step = state->stepY;
		phase = (state->frm + (Uint16)y1) * state->freqY;
		if(step == 0) {
			for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
				row[scanline * 6] = y1;
				row[scanline * 6 + 2] = y2;
				row[scanline * 6 + 4] = y3;
			}
		} else {
			waveY3 = y3;
			amplitude = state->ampY;
			for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
				wave = LINE_SCROLL_WAVE(phase, amplitude);
				row[0] = y1;
				row[2] = y2;
				row[4] = waveY3 + wave;
				row += 6;
				phase += step;
			}
		}
	}
	state->frm++;
	state->page ^= 1;
}

/* Waits for a DMA channel to finish. */
static inline void fdWaitDMA(volatile DMA_REGISTERS *channel)
{
	while(channel->ctrl & DMA_ENABLE << DMA_CONTROL_SHIFT) {
	}
}

/* Starts the line scroll: its state (heap slot HEAP_SLOT_LINE_SCROLL) holds the
   mode and, for x and y, how fast the wave runs from tick to tick, its step
   from one scanline to the next and its amplitude; two events, one in the
   vertical blank pass and one in the tick pass, build and send each tick's
   line offsets. */
void fdLineScrollInitWave(Sint32 mode, Sint32 freqX, Sint32 stepX, Sint32 ampX,
	Sint32 freqY, Sint32 stepY, Sint32 ampY)
{
	LINE_SCROLL_STATE *state;

	state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_LINE_SCROLL, sizeof(LINE_SCROLL_STATE));
	DMA_FILL32(0, state, sizeof(LINE_SCROLL_STATE));
	fdWaitDMA((volatile DMA_REGISTERS *)&REG_DMA3SAD);
	state->mode = mode;
	state->freqX = freqX;
	state->freqY = freqY;
	state->ampX = ampX;
	state->ampY = ampY;
	state->stepX = stepX;
	state->stepY = stepY;
	sySetEvent(fdLineScrollBuildPage, EVENT_PASS_TICK);
	sySetEvent(fdLineScrollArmDMA, EVENT_PASS_VBLANK);
}

/* Stops the line scroll: removes its two events and stops DMA 0. */
void fdLineScrollStop(void)
{
	syCloseEvent(fdLineScrollArmDMA);
	syCloseEvent(fdLineScrollBuildPage);
	DMA0_STOP();
}

#undef LINE_SCROLL_ALL_LAYERS
#undef LINE_SCROLL_WAVE_ENTRIES
#undef LINE_SCROLL_WAVE_UNITS
#undef LINE_SCROLL_WAVE
