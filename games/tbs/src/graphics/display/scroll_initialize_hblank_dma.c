#include "mem.h"
#include "dma.h"
#include "system.h"
#include "callback_scheduler.h"
#include "scroll.h"
#include "io_reg.h"

static __inline__ void WaitDma(volatile u32 *channel)
{
	while (channel[2] & 0x80000000)
	{
	}
}

void DisplayScroll_InitializeHBlankDma(
	s32 mode, s32 freq_x, s32 step_x, s32 amp_x, s32 freq_y, s32 step_y, s32 amp_y)
{
	struct DisplayScrollWork *work;
	volatile u32			  zero;

	work = Runtime_AllocateBlock(34, 0xf20);
	zero = 0;
	Dma_Set(&zero, work, 0x850003c8, REG_DMA3);
	WaitDma(REG_DMA3);
	work->mode = mode;
	work->freq_x = freq_x;
	work->freq_y = freq_y;
	work->amp_x = amp_x;
	work->amp_y = amp_y;
	work->step_x = step_x;
	work->step_y = step_y;
	Scheduler_AddOrUpdateCallback(DisplayScroll_BuildAndSwapHBlankPage, 3200);
	Scheduler_AddOrUpdateCallback(DisplayScroll_ArmHBlankDma, 1152);
}
