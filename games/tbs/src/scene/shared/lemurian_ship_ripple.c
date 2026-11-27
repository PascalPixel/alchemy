/* @file */
/* The Lemurian ship: the ripple of the water, a wave in each axis of the */
/* background's scrolling, changed line by line in the horizontal blank. */

#include "system.h"
#include "game/system_call.h"

static inline void WaitDma(volatile u32 *channel) {
	while(channel[2] & DMA_BUSY) {
	}
}

/* Starts the ripple, with the wave's frequency, its step each frame and its */
/* amplitude for x and for y: one task builds each frame's table of line */
/* offsets, and another sends it by DMA in the horizontal blank. */
void scLineScrollInitWave(s32 mode, FIXED freqX, s32 stepX, FIXED ampX,
						  FIXED freqY, s32 stepY, FIXED ampY) {
	LINE_SCROLL_STATE *state;

	state =
		syHeapSlotAllocEwram(HEAP_SLOT_LINE_SCROLL, sizeof(LINE_SCROLL_STATE));
	btDmaFill32(0, state, sizeof(LINE_SCROLL_STATE));
	WaitDma(REG_DMA3);
	state->mode = mode;
	state->freqX = freqX;
	state->freqY = freqY;
	state->ampX = ampX;
	state->ampY = ampY;
	state->stepX = stepX;
	state->stepY = stepY;
	syTaskAddFrame(fdLineScrollBuildPage, TASK_PASS_FRAME);
	syTaskAddFrame(fdLineScrollArmDma, TASK_PASS_VBLANK);
}
