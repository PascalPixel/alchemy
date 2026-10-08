#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "canvas.h"
#include "motobj.h"
#include "mem.h"
#include "resource.h"
#include "types.h"
#include "heap_state.h"
#include "iwram_call.h"
#include "battle_efx.h"
#include "callback_scheduler.h"
#include "system.h"
#include "fixed_math.h"
#include "resource_ids.h"
#include "ram_buffer.h"
#include "btlwork.h"
#include "battle_effect_work.h"
#include "io_reg.h"
#include "btldisp.h"
void Audio_PlayCue(s32 cue);

extern u8 gMapCellBuffer[];


typedef s32 (*WordCopy)(void *, const void *, s32);

static __inline__ void CopyWords(WordCopy copy, void *destination, const void *source, s32 size)
{
	/* FAKEMATCH: needed to match: a direct call loads the destination and source before the routine, changing pool order */
	copy(destination, source, size);
}

#define PIXEL_BUFFER Ram_MapCellBuffer

void BattleFx_RunCounterReveal(struct BattleEffectArgument *object)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	u8						*sheet;
	void					*cells;
	DrawRectangleFn			 rectangle[2];
	struct MotionObject		*member_object;
	/* FAKEMATCH: needed to match: the second position is never used: it gives the frame its native 76 bytes */
	struct EffectPosition pos[2];
	s32					  curve[2];
	s32					  draw_enabled;
	s32					  idx_a;
	s32					  idx_b;
	s32					  zero_val;
	s32					  frame;

	heap_cache = (void **)gBattleFxWork;
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	/* FAKEMATCH: needed to match: the zero is held in sl through the setup calls */
	zero_val = 0;
	work->effect = object;
	BattleFx_BeginTiledCanvas(0);
	REG_BG2PA = 0x100;
	REG_BLDALPHA = 0x1010;
	sheet = Resource_GetTableEntry((s32)&ResourceId_CounterRevealSheet);
	Iwram_CopyWords(BG_PLTT, sheet, 128);
	sheet = sheet + 128;
	Resource_DecodeType01(sheet, work);
	cells = Ram_MapCellBuffer;
	sheet = Resource_GetTableEntry((s32)&ResourceId_RedCrescentSheetA);
	sheet = sheet + 128;
	Resource_DecodeType01(sheet, cells);
	BattleEffect_LoadWork(46, 7, 7, 3, 1);
	rectangle[0] = (DrawRectangleFn)heap_cache[7];
	BattleEffect_LoadWork(47, 7, 7, 7, 1);
	rectangle[1] = (DrawRectangleFn)heap_cache[8];
	Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0x480);
	work->transfer_mode = 1;
	work->transfer_value = zero_val;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	draw_enabled = 1;
	if (work->effect->side == 1)
	{
		curve[0] = -0x500000;
	}
	else
	{
		curve[0] = 0x700000;
	}
	curve[1] = -0x200000;
	for (frame = 0; frame != 132; frame++)
	{
		s32	 spin;
		s32	 screen_x;
		s32	 screen_y;
		s32	 amp;
		s32	 row_base;
		s32	 angle;
		s32	 i;
		s32 *scanline;

		spin = frame << 9;
		screen_x = (curve[0] >> 16) + ((Trig_Sin(spin) << 4) >> 16) + 48;
		screen_y = (curve[1] >> 16) + ((Trig_Cos(spin) << 2) >> 16) + 16;
		if (frame == 88)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 32)
		{
			if (work->effect->side == 1)
			{
				curve[0] = -0x200000;
			}
			else
			{
				curve[0] = 0x480000;
			}
			curve[1] = 0x180000;
			draw_enabled = 0;
		}
		if (frame == 33)
		{
			REG_BLDALPHA = 0x1010;
			draw_enabled = 1;
		}
		if (frame == 64)
		{
			EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], pos);
			if (work->effect->side == 1)
			{
				curve[0] = (pos[0].x - 128) << 16;
			}
			else
			{
				curve[0] = (pos[0].x - 64) << 16;
			}
			curve[1] = 0;
			draw_enabled = 0;
		}
		if (frame == 65)
		{
			REG_BLDALPHA = 0x1010;
			draw_enabled = 1;
		}
		scanline = work->bg2_x;
		amp = 0;
		if (frame <= 31)
		{
			if (frame > 15)
			{
				amp = (frame * 2) - 32;
				REG_BLDALPHA = (31 - frame) | 0x1000;
			}
		}
		else if (frame <= 63)
		{
			if (frame > 47)
			{
				amp = (frame * 2) - 96;
				REG_BLDALPHA = (63 - frame) | 0x1000;
			}
		}
		if (amp < 0)
		{
			amp = 0;
		}

		i = 0;
		row_base = (6 - screen_x) << 8;
		angle = frame << 11;
		for (; i != 160; i++)
		{
			*scanline++ = row_base - ((Trig_Sin(angle) * amp) >> 10);
			angle += 0x800;
		}
		if (draw_enabled != 0)
		{
			if (work->effect->side == 0)
			{
				idx_a = 0;
				idx_b = 0;
			}
			else
			{
				idx_a = 1;
				/* FAKEMATCH: needed to match: i is 160 here, so this is 0: a literal 0 drops a stack slot and shrinks the frame */
				idx_b = i >> 31;
			}
			if (frame <= 87)
			{
				rectangle[work->effect->side](canvas, work, CounterReveal_PanelX[idx_a * 7],
					CounterReveal_PanelY[idx_b * 7] + screen_y, 57, 98);
			}
			else
			{
				if (frame <= 91)
				{
					rectangle[work->effect->side](canvas, work, CounterReveal_PanelX[idx_a * 7],
						CounterReveal_PanelY[idx_b * 7] + screen_y, 57, 98);
				}
				rectangle[work->effect->side](canvas, work->sheet + 0x15d2,
					CounterReveal_PanelX[idx_a * 7 + 1],
					CounterReveal_PanelY[idx_b * 7 + 1] + screen_y, 99, 69);
				if ((frame - 88) <= 1U)
				{
					Iwram_FillWords(canvas, 0x4000, 0x3F3F3F3F);
				}
				if ((frame - 90) <= 1U)
				{
					rectangle[work->effect->side](canvas, work->sheet + 0x3081,
						CounterReveal_PanelX[idx_a * 7 + 2],
						CounterReveal_PanelY[idx_b * 7 + 2] + screen_y, 128, 91);
				}
				if ((frame - 92) <= 1U)
				{
					rectangle[work->effect->side](canvas, (void *)gMapCellBuffer,
						CounterReveal_PanelX[idx_a * 7 + 3],
						CounterReveal_PanelY[idx_b * 7 + 3] + screen_y, 128, 91);
				}
				if ((frame - 94) <= 1U)
				{
					rectangle[work->effect->side](canvas, gMapCellBuffer + 0x2d80,
						CounterReveal_PanelX[idx_a * 7 + 4],
						CounterReveal_PanelY[idx_b * 7 + 4] + screen_y, 128, 59);
				}
				if ((frame - 96) <= 1U)
				{
					rectangle[work->effect->side](canvas, gMapCellBuffer + 0x4b00,
						CounterReveal_PanelX[idx_a * 7 + 5],
						CounterReveal_PanelY[idx_b * 7 + 5] + screen_y, 122, 29);
				}
				if ((frame - 98) <= 1U)
				{
					rectangle[work->effect->side](canvas, gMapCellBuffer + 0x58d2,
						CounterReveal_PanelX[idx_a * 7 + 6],
						CounterReveal_PanelY[idx_b * 7 + 6] + screen_y, 76, 25);
				}
			}
		}
		if (frame == 88)
		{
			member_object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
			member_object->velocity_y = 0x10000;
			member_object->acceleration = 0x20000;
			member_object->speed_limit = 0x20000;
			member_object->vertical_motion_strength = 0;
			member_object->auto_face_motion = 0;
			member_object->snap_to_target = 0;
			Object_SetMoveTargetFar(member_object, member_object->x << 1, 0, member_object->z);
			ObjectGroup_UpdateMembers(work->effect->actors[0], -1, 5, -1, 0);
		}
		if (frame == 120)
		{
			GetBattleObjectSlotFar(work->effect->actors[0])->object->vertical_motion_strength =
				0xAB85;
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
	BattleBackground_LoadFar(1, BtlWk->background, 24);
	Runtime_ReleaseHeapBlock(47);
	Runtime_ReleaseHeapBlock(46);
	BattleFx_EndCanvasLayer();
}

void Graphics_ConvertBackgroundToBlueRamp(void)
{
	s32	 i, x, y, row, end, speed;
	s32	 red, green, blue;
	u16	 color;
	u8	*delays, *p, *pixel;
	u16 *palette;
	u32	 delay_mask;
	palette = (u16 *)0x05000040;
	i = 0;
	do
	{
		s32 half;
		half = i / 2;
		*palette++ = (i << 10) | (half << 5) | half;
		i++;
	} while (i != 32);
	delays = *(u8 **)&gWorkSlot[41];
	end = 0;
	speed = 16;
	CopyWords(Iwram_CopyWords, PIXEL_BUFFER, (void *)0x06008000, 0x7800);
	delay_mask = 63;
	p = delays;
	do
	{
		*p++ = Random16() & delay_mask;
	} while (p != delays + 256);
	row = 0;
	i = 0;
	do
	{
		end += speed / 4;
		speed++;
		while (row != end)
		{
			x = 0;
			do
			{
				y = row - delays[x];
				if (y < 0)
					goto next_pixel;
				if (y > 119)
					goto next_pixel;
				{
					pixel = PIXEL_BUFFER
							+ ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) + ((y / 8) << 11));
					color = ((s16 *)0x05000000)[*pixel];
					red = color & 31;
					green = (color >> 5) & 31;
					blue = (color >> 10) & 31;
					if (red < green)
						red = green;
					if (red < blue)
						red = blue;
					*pixel = 63 - red;
				}
			next_pixel:
				x++;
			} while (x != 256);
			row++;
		}
		Iwram_CopyWords((void *)0x06008000, PIXEL_BUFFER, 0x7800);
		WaitFrames(1);
		if (end > 248)
			break;
		i++;
	} while (i != 27);
	palette = (u16 *)0x050000c0;
	i = 0;
	do
	{
		s32 half;
		half = i / 2;
		*palette++ = (i << 10) | (half << 5) | half;
		i++;
	} while (i != 32);
	i = 0;
	do
	{
		PIXEL_BUFFER[i] += 64;
		i++;
	} while (i != 0x7800);
	CopyWords(Iwram_CopyWords, (void *)0x06008000, PIXEL_BUFFER, 0x7800);
	WaitFrames(1);
}
