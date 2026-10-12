#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime9.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "ramcode.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "irq.h"
#include "types.h"
#include "scene.h"
#include "resource.h"
#include "resource_ids.h"
#include "global_cells.h"
#include "cartboot.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "frame.h"
#include "keystate.h"
#include "input.h"
#include "oam.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "irqlock.h"
#include "iowrite.h"
#include "lib.h"
#include "fixed_math.h"
#include "affine.h"
#include "iwram_call.h"
#include "vram_block.h"
#include "string.h"
#include "low_runtime.h"
#include "vram_tab.h"
#include "layout_guard.h"
#include "edition.h"
#include "transform.h"
#include "project.h"
#include "save_state.h"
#include "runtime_interfaces.h"
#include "flash.h"
#include "audio_engine_symbols.h"

void BlendTransition_Update(void)
{
	if (gBlendDuration != 0)
	{
		{
			volatile u16 *blend_control;
			u32			  control;

			if (gBlendBrighten != 0)
			{
				control = gBlendLayers | 0x80;
				blend_control = &REG_BLDCNT;
			}
			else
			{
				control = gBlendLayers | 0xc0;
				blend_control = &REG_BLDCNT;
			}
			*blend_control = control;
		}
		{
			u8 *remaining = &gBlendFramesLeft;
			s32 delta;
			s32 level;
			s32 step;

			(*remaining)--;
			level = gBlendTargetLevel;
			delta = gBlendStartLevel - gBlendTargetLevel;
			step = *remaining;
			level += __divsi3(delta * step, gBlendDuration);
			REG_BLDY = level;
			if (*remaining == 0)
				gBlendDuration = 0;
		}
	}
}

// フェードは VBlank ごとに BlendTransition_Update が進める。
// 明るさ（BLDY）は 0 で元のまま、16 で真っ白または真っ黒。
//   gBlendBrighten   : 0 黒へ / 1 白へ
//   gBlendLayers     : BLDCNT の対象レイヤー（下の 4 つは 0x3e で BG1〜3・OBJ・背景色）
//   gBlendFramesLeft : 残りフレーム数
//--------------------------------------------------------------------------
// BG1〜3・OBJ・背景色を、今の目標の明るさから level まで frames フレームで
// フェードする
// brighten : 0 黒へ / 1 白へ
//--------------------------------------------------------------------------
#define DEFINE_BLEND_FADE(name, brighten, level) \
	void name(s32 frames)                        \
	{                                            \
		gBlendBrighten = brighten;               \
		gBlendLayers = 0x3e;                     \
		gBlendStartLevel = gBlendTargetLevel;    \
		gBlendTargetLevel = level;               \
		gBlendDuration = frames;                 \
		gBlendFramesLeft = gBlendDuration;       \
	}

// 黒へフェードアウト
DEFINE_BLEND_FADE(Blend_SetDarkenTarget16, 0, 16)

// 黒からフェードイン
DEFINE_BLEND_FADE(Blend_SetDarkenTarget0, 0, 0)

// 白へフェードアウト
DEFINE_BLEND_FADE(Blend_SetBrightenTarget16, 1, 16)

// 白からフェードイン
DEFINE_BLEND_FADE(Blend_SetBrightenTarget0, 1, 0)

//--------------------------------------------------------------------------
// フェードを指定する
// bright : 0 黒へ / 1 白へ
// layer  : BLDCNT の対象レイヤー（下位 6 ビット）
// start  : はじめの明るさ（16 より大きければ今の目標の明るさから）
// target : 目標の明るさ
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void Blend_ConfigureTransition(s8 bright, s32 layer, u32 start, s32 target, s32 frames)
{
	gBlendBrighten = bright;
	gBlendLayers = layer & 0x3f;
	if (start > 16U)
	{
		gBlendStartLevel = gBlendTargetLevel;
	}
	else
	{
		gBlendStartLevel = start;
	}
	gBlendTargetLevel = target;
	gBlendFramesLeft = (gBlendDuration = frames);
}

//--------------------------------------------------------------------------
// フェードが終わるまで待つ
//--------------------------------------------------------------------------
void Blend_WaitForTransition(void)
{
	if (gBlendDuration != 0)
	{
		do
		{
			WaitFrames(1);
		} while (gBlendDuration != 0);
	}
}
