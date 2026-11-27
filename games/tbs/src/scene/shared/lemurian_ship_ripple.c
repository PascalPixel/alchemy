/* The Lemurian ship: the ripple of the water, a wave in each axis of the
   background's scrolling, changed line by line in the horizontal blank. */

#include "system.h"
#include "game/system_call.h"

static inline void scWaitDMA(volatile Uint32 *channel)
{
	while(channel[2] & DMA_BUSY) {
	}
}

/* Starts the ripple, with the wave's frequency, its step each tick and its
   amplitude for x and for y: one event builds each tick's table of line
   offsets, and another sends it by DMA in the horizontal blank. */
void scLineScrollInitWave(Sint32 mode, FIXED freqX, Sint32 stepX, FIXED ampX,
	FIXED freqY, Sint32 stepY, FIXED ampY)
{
	LINE_SCROLL_STATE *state;

	state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_LINE_SCROLL, sizeof(LINE_SCROLL_STATE));
	DMA_FILL32(0, state, sizeof(LINE_SCROLL_STATE));
	scWaitDMA(REG_DMA3);
	state->mode = mode;
	state->freqX = freqX;
	state->freqY = freqY;
	state->ampX = ampX;
	state->ampY = ampY;
	state->stepX = stepX;
	state->stepY = stepY;
	sySetTickEvent(fdLineScrollBuildPage, EVENT_PASS_TICK);
	sySetTickEvent(fdLineScrollArmDMA, EVENT_PASS_VBLANK);
}
