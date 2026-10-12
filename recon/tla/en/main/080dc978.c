/* Near miss of Func_080dc978, differ 6 of 92, written from the listing with the wave work as a
 * struct (with byte offsets it is differ 47). Left: the native loads the halfword zero for
 * unknown_295 (ldr r1, =0) before the offset's shift (lsls r2, r2, #1), here after it; and
 * the native's literal pool sits two instructions earlier (b before movs r3, #0; ldrsb), which
 * is the four estimated bytes of one empty asm. */
#include "types.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "dma.h"
#include "ram_buffer.h"

#define WAVE_FX_SLOT 88

struct WaveFxWork
{
	u8	unknown_000[0x28b];
	s8	red;
	s8	green;
	s8	blue;
	u16 phase;
	u16 id;
	u16 value;
	u8	steps;
	u8	unknown_295;
	u8	unknown_296[2];
};

extern u32 gFrameCount;

void BattleEffect_InitializeSharedScene(void);
void Func_080dce60(void);
void BattleFx_ApplyColorToTargetBuffer(s32 colors, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void Func_080dca84(void);
void Func_080dcb44(void);

void Func_080dc978(s32 id, s32 value)
{
	struct WaveFxWork *work;

	work = Runtime_AllocateBlock(WAVE_FX_SLOT, sizeof(*work));
	Dma_Fill32(0, work, sizeof(*work));
	work->phase = (gFrameCount * 2) % 360;
	work->id = id;
	work->value = value;
	work->steps = 8;
	work->unknown_295 = 0;
	BattleEffect_InitializeSharedScene();
	Func_080dce60();
	BattleFx_ApplyColorToTargetBuffer(
		(work->blue << 10) | (work->green << 5) | work->red | 0x200000, 1);
	BattleFx_StartBufferInterpolation(8);
	Func_080dca84();
	Scheduler_AddOrUpdateCallback((SchedulerCallback)Func_080dcb44, 0x480);
}
