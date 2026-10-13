#include "vram_tab.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/marker.h"
#include "unit/object/sparkle.h"
#include "unit/object/disptran.h"
#include "unit/object/palbuf.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/math.h"
#include "unit/runtime/heap.h"
#include "types.h"
#include "scene.h"
#include "btltype.h"
#include "game_state.h"
#include "fieldrun.h"
#include "btlsys.h"
#include "battle.h"
#include "battle_unit.h"
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
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flag_ids.h"
#include "iwram_call.h"
#include "eventwrk.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "fieldobj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_sprite.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "ownerval.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "itemgift.h"
#include "btlcalc.h"
#include "disptran.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "effect_runtime.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "layout_guard.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "btlpty.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "fieldcoo.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_efx.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"

//--------------------------------------------------------------------------
// ウィンドウ 0/1 を有効にし、ページの値を WININ, WINOUT, WIN0H, WIN1H に入れてから
// 残りを HBlank ごとに DMA0 で WIN0H と WIN1H へ 1 ワードずつ送る
//--------------------------------------------------------------------------
void BattleFx_StartWindowHBlankDma(void)
{
	struct DisplayTransitionState *work = gDisplayTransition;
	u16							  *src = work->pages[work->page];
	volatile u16				  *channel = REG_DMA0;

	channel[5] &= 0xc5ff;
	channel[5] &= 0x7fff;
	(void)channel[5];
	REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
	Io_Write16(*src++, &REG_WININ);
	Io_Write16(*src++, &REG_WINOUT);
	Io_Write16(*src++, &REG_WIN0H);
	Io_Write16(*src++, &REG_WIN1H);
	Io_Write16(160, &REG_WIN0V);
	Io_Write16(160, &REG_WIN1V);
	Dma_Set(src, (void *)&REG_WIN0H, 0xa6600001, (volatile u32 *)channel);
}

struct TransitionSpan
{
	u16 cnt;
	u16 val;
	u8	x0;
	u8	x1;
	u8	y0;
	u8	y1;
};

struct TransitionShape
{
	u16					  first;
	u16					  second;
	struct TransitionSpan spans[3];
};

extern struct TransitionShape DisplayTransition_Shapes[];

void DisplayTransition_UpdateScanlineTable(void)
{
	struct DisplayTransitionState *state;
	struct TransitionSpan		  *span;
	struct TransitionShape		  *shape;
	struct MapScrollWork		  *map;
	u16							  *dst;
	s32							  *view7;
	s32							  *view8;
	s32							  *view9;
	s32							  *view77;
	struct ObjectRuntime		  *obj;
	u32							   i;
	s32							   k;
	s32							   cnt;
	s32							   v;
	s32							   rr;
	s32							   d;
	u32							   h;
	s32							   lo;
	s32							   hi;
	s32							   cx;
	s32							   cy;

	s32 ay;
	s32 bx;
	s32 by;
	u32 w;
	s32 e;

	map = gMapWork[0];
	state = gMapWork[23];

	if (state->duration != 0)
	{
		if (state->step >= state->duration)
		{
			state->duration = 0;
			if (state->next_mode == 0)
			{
				if (state->end == 64)
				{
					REG_DISPCNT &= 0x81ff;
				}
				else
				{
					REG_DISPCNT &= 0x9fff;
				}
				Scheduler_RemoveCallback((SchedulerCallback)BattleFx_StartWindowHBlankDma);
				Scheduler_RemoveCallback((SchedulerCallback)DisplayTransition_UpdateScanlineTable);
				{
					volatile u16 *dma0 = REG_DMA0;

					dma0[5] &= 0xc5ff;
					dma0[5] &= 0x7fff;
					dma0[5];
				}
				return;
			}
			state->mode = state->next_mode;
		}
		else
		{
			state->value =
				state->start
				+ Iwram_SignedDivide((state->end - state->start) * ++state->step, state->duration);
		}
	}

	dst = state->pages[1 ^ state->page];
	UiWindow_FillFromSceneFar(dst + 2);

	switch (state->mode)
	{
	case 0:
		*dst++ = 0x7f7f;
		*dst++ = 1;
		v = state->value;
		if (v & 32)
		{
			v = 32 - (v & 31);
		}
		else
		{
			v &= 31;
		}
		v = DisplayTransition_NoiseWidths[v];
		for (i = 0; i < 160; i++)
		{
			w = (Random16() * (241 - v)) >> 16;
			dst[0] = ((w << 8) | (w + v));
			dst += 2;
		}
		break;

	case 1:
		v = state->value;
		shape = &DisplayTransition_Shapes[v & 31];
		if (v & 32)
		{
			*dst++ = shape->first;
			*dst++ = shape->second;
		}
		else
		{
			*dst++ = shape->second;
			*dst++ = shape->first;
		}
		for (i = 0; i < 3; i++)
		{
			span = &shape->spans[i];
			cnt = span->cnt;
			hi = span->val;
			if (cnt != 0)
			{
				if (hi != 0)
				{
					for (k = 0; k < cnt; k++)
					{
						dst[0] = hi;
						dst += 2;
					}
				}
				else
				{
					rr = span->x0;
					lo = span->y0;
					d = span->x1;
					hi = span->y1;
					for (k = 0; k < cnt; k++)
					{
						dst[0] = (((rr + (d - rr) * k / cnt) << 8) + (lo + (hi - lo) * k / cnt));
						dst += 2;
					}
				}
			}
		}
		break;

	case 2:
		v = state->value - 1;
		if (v & 32)
		{
			*dst++ = 1;
			*dst++ = 0x7f7f;
		}
		else
		{
			*dst++ = 0x7f7f;
			*dst++ = 1;
		}
		v &= 31;
		for (i = 0; i < 160; i++)
		{
			h = (v << 4) + ((16 * Random16()) >> 16);
			if (h > 255)
			{
				h = 255;
			}
			dst[0] = h;
			dst += 2;
		}
		break;

	case 3:
		v = state->value;
		*dst++ = 0x7f7f;
		*dst++ = 1;
		if ((u32)v > 32)
		{
			*dst++ = 1;
			*dst++ = 0x7f7f;
			v = 64 - v;
		}
		v *= 5;
		rr = (v * v) << 16;
		for (i = 0; i < 160; i++)
		{
			v = i - 80;
			h = Iwram_Sqrt(rr - ((v * v) << 16)) >> 8;
			lo = 120 - h;
			hi = h + 120;
			if (lo < 0)
			{
				lo = 0;
			}
			if (hi < 0)
			{
				hi = 0;
			}
			if (lo > 240)
			{
				lo = 240;
			}
			if (hi > 240)
			{
				hi = 240;
			}
			dst[0] = ((lo << 8) + hi);
			dst += 2;
		}
		break;

	case 4:
		v = state->value;
		if (v & 32)
		{
			*dst++ = 1;
			*dst++ = 0x7f7f;
		}
		else
		{
			*dst++ = 0x7f7f;
			*dst++ = 1;
		}
		v = (u32)(240 * (v & 31)) >> 5;
		for (i = 0; i < 160; i += 2)
		{
			e = ((240 - v) << 8) + 240;
			dst[0] = v;
			dst[2] = e;
			dst += 4;
		}
		break;

	case 7:
	{
		s32 ax;

		view7 = &map->view_x;
		ax = view7[0] & 0xffff0000;
		ay = view7[1] & 0xffff0000;
		obj = ObjectTable_Get(gGameState.selected_actor);
		cx = (obj->x - ax) / 0x10000;
		cy = ((obj->z - obj->y) - ay) / 0x10000 - 16;
		// the frame counter is read here, where mode 8 forms its index, and the value is not used
		gFrameCount;
		*dst++ = state->active;
		*dst++ = state->mask;
		v = state->value;
		if ((v & 32) == 0)
		{
			v = 32 - (v & 31);
		}
		else
		{
			v &= 31;
		}
		if (gFrameCount & 1)
		{
			v = 0;
		}
		v *= 5;
		rr = (v * v) << 16;
		for (i = 0; i < 160; i++)
		{
			v = i - cy;
			h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
			lo = cx - h;
			hi = cx + h;
			if (lo < 0)
			{
				lo = 0;
			}
			if (hi < 0)
			{
				hi = 0;
			}
			if (lo > 240)
			{
				lo = 240;
			}
			if (hi > 240)
			{
				hi = 240;
			}
			dst[0] = ((lo << 8) + hi);
			dst += 2;
		}
		break;
	}
	case 8:
		view8 = &map->view_x;
		bx = view8[0] & 0xffff0000;
		by = view8[1] & 0xffff0000;
		cx = (state->centre_x - bx) / 0x10000;
		cy = (state->centre_y - by) / 0x10000 - 16;
		k = -cy * 2 + gFrameCount;
		*dst++ = state->mask;
		*dst++ = state->active;
		v = state->value;
		if (v & 32)
		{
			v = 32 - (v & 31);
		}
		else
		{
			v &= 31;
		}
		v *= 5;
		rr = (v * v) << 16;
		for (i = 0; i < 160; i++)
		{
			v = i - cy;
			h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
			lo = cx - h;
			hi = cx + h;
			if (lo < hi)
			{
				d = DisplayTransition_EdgeOffsets[k & 31];
				lo -= d;
				hi += d;
				if (lo >= hi)
				{
					lo = 240;
					hi = 240;
				}
			}
			if (lo < 0)
			{
				lo = 0;
			}
			if (hi < 0)
			{
				hi = 0;
			}
			if (lo > 240)
			{
				lo = 240;
			}
			if (hi > 240)
			{
				hi = 240;
			}
			dst[0] = ((lo << 8) + hi);
			dst += 2;
			k += 2;
		}
		break;

	case 9:
	{
		s32 ax;

		view9 = &map->view_x;
		ax = view9[0] & 0xffff0000;
		ay = view9[1] & 0xffff0000;
		obj = ObjectTable_Get(gGameState.selected_actor);
		cx = (obj->x - ax) / 0x10000;
		cy = ((obj->z - obj->y) - ay) / 0x10000 - 16;
		// the frame counter is read here, where mode 8 forms its index, and the value is not used
		gFrameCount;
		*dst++ = state->mask;
		*dst++ = state->active;
		v = state->value;
		if (v & 32)
		{
			v = 32 - (v & 31);
		}
		else
		{
			v &= 31;
		}
		v *= 5;
		rr = (v * v) << 16;
		for (i = 0; i < 160; i++)
		{
			v = i - cy;
			h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
			lo = cx - h;
			hi = cx + h;
			if (lo < 0)
			{
				lo = 0;
			}
			if (hi < 0)
			{
				hi = 0;
			}
			if (lo > 240)
			{
				lo = 240;
			}
			if (hi > 240)
			{
				hi = 240;
			}
			dst[0] = ((lo << 8) + hi);
			dst += 2;
		}
		break;
	}
	case 77:
	{
		s32 ax;

		view77 = &map->view_x;
		ax = view77[0] & 0xffff0000;
		ay = view77[1] & 0xffff0000;
		obj = ObjectTable_Get(gGameState.selected_actor);
		cx = (obj->x - ax) / 0x10000;
		cy = ((obj->z - obj->y) - ay) / 0x10000 - 8;
		// the frame counter is read here, where mode 8 forms its index, and the value is not used
		gFrameCount;
		*dst++ = state->mask;
		*dst++ = state->active;
		v = state->value;
		if (v & 32)
		{
			v = 32 - (v & 31);
		}
		else
		{
			v &= 31;
		}
		v *= 5;
		rr = (v * v) << 16;
		for (i = 0; i < 160; i++)
		{
			v = i - cy;
			h = Iwram_Sqrt(rr - ((v * v * 3) << 14)) >> 8;
			lo = cx - h;
			hi = cx + h;
			if (lo < 0)
			{
				lo = 0;
			}
			if (hi < 0)
			{
				hi = 0;
			}
			if (lo > 240)
			{
				lo = 240;
			}
			if (hi > 240)
			{
				hi = 240;
			}
			dst[0] = ((lo << 8) + hi);
			dst += 2;
		}
		break;
	}
	}

	state->page ^= 1;
}

void DisplayTransition_InitializeBattleEffectState(s32 mode)
{
	struct DisplayTransitionState *state = Runtime_AllocateBlock(31, sizeof(*state));

	Dma_Fill32(0, state, sizeof(*state));
	state->mode = mode;
	state->value = 0;
	state->mask = 0x3f3f;
	state->active = 1;
	Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
	Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
}

void BattleFx_EnableTwoCallbacks(void)
{
	Scheduler_EnableCallbacks(&DisplayTransition_UpdateScanlineTable);
	Scheduler_EnableCallbacks(&BattleFx_StartWindowHBlankDma);
}

struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void)
{
	struct DisplayTransitionState *state;

	state = Runtime_AllocateBlock(31, sizeof(*state));
	Dma_Fill32(0, state, sizeof(*state));
	return state;
}

void DisplayTransition_Start(s32 mode, s32 frames)
{
	struct DisplayTransitionWindow *display;
	s32								value;
	s32								kind;

	kind = (mode >> 8) & 0xff;
	display = gMapWork[0];
	value = mode & 0xff;
	switch (kind)
	{
	case 0:
		Blend_SetDarkenTarget16(0);
		Blend_SetDarkenTarget0(frames);
		WaitFrames(1);
		break;
	case 1:
		BattleFx_ApplyColorToSourceBuffer(0x8000, PLTT_BACKDROP);
		BattleFx_StartBufferInterpolation(frames);
		WaitFrames(1);
		IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
		BattleFx_SetPrimaryBufferValue(0);
		return;
	case 2:
	{
		struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
		state->mode = value;
		state->value = 0;
		state->mask = 63;
		state->active = 1;
		Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
		Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
		WaitFrames(1);
		IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
		state->start = 0;
		state->end = 32;
		state->duration = frames;
		state->step = 0;
		return;
	}
	case 3:
	{
		struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
		state->mode = value;
		state->value = 32;
		DisplayTransition_FillTilemapAndSolidTile(15);
		WaitFrames(1);
		Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
		IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
		state->start = 0;
		state->end = 32;
		state->duration = frames;
		state->step = 0;
		return;
	}
	case 4:
	{
		struct DisplayTransitionState *state;

		display = gMapWork[0];
		state = DisplayTransition_AllocateAndClearState();
		display->first_line = 80;
		display->second_line = 80;
		WaitFrames(1);
		if (value == 0)
		{
			Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
			Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
			state->start = 80;
			state->end = 0;
			state->duration = frames;
			state->step = 0;
		}
		else
		{
			Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
			Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
			state->start = 80;
			state->end = 0;
			state->duration = frames;
			state->step = 0;
		}
		break;
	}
	}
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, 0x20000);
}

void DisplayTransition_Finish(s32 mode, s32 frames)
{
	s32 value;
	s32 kind;

	kind = (mode >> 8) & 0xff;
	value = mode & 0xff;
	switch (kind)
	{
	case 0:
		Blend_SetDarkenTarget0(0);
		Blend_SetDarkenTarget16(frames);
		break;
	case 1:
		BattleFx_ApplyColorToTargetBuffer(0x8000, 0);
		BattleFx_StartBufferInterpolation(frames);
		break;
	case 2:
	{
		struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
		state->mode = value;
		state->value = 32;
		state->mask = 63;
		state->active = 1;
		Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
		Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
		WaitFrames(1);
		state->start = 32;
		state->end = 64;
		state->duration = frames;
		state->step = 0;
		break;
	}
	case 3:
	{
		struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
		state->mode = value;
		state->value = 32;
		DisplayTransition_FillTilemapAndSolidTile(0);
		WaitFrames(1);
		Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
		state->start = 32;
		state->end = 64;
		state->duration = frames;
		state->step = 0;
		break;
	}
	case 4:
	{
		struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
		if (value == 0)
		{
			Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
			Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
			state->start = 0;
			state->end = 80;
			state->duration = frames;
			state->step = 0;
		}
		else
		{
			Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
			Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
			state->start = 0;
			state->end = 80;
			state->duration = frames;
			state->step = 0;
		}
		break;
	}
	}
}

void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200)
{
	struct DisplayTransitionWindow *state;

	state = gMapWork[0];
	if (state != NULL)
	{
		if (clear_0200 != 0)
		{
			state->dispcnt &= 0xFDFF;
		}
		if (clear_0400 != 0)
		{
			state->dispcnt &= 0xFBFF;
		}
		if (clear_0800 != 0)
		{
			state->dispcnt &= 0xF7FF;
		}
	}
}

void DisplayTransition_Update(void)
{
	struct DisplayTransitionFrame  *state = *Ram_DisplayWork;
	struct DisplayTransitionWindow *display = *(struct DisplayTransitionWindow **)Ram_MapWork;
	s8							   *duration = &state->duration;
	u32								display_value;

	if (*duration != 0)
	{
		s8 *step = &state->step;

		if (*step >= *duration)
		{
			*duration = 0;
			Scheduler_RemoveCallback(DisplayTransition_Update);
			Runtime_SetIrqHandler(1, 0, 0);
			return;
		}
		else
		{
			s32 delta = state->end - state->start;
			s32 value;

			(*step)++;
			value = Iwram_SignedDivide(delta * *step, *duration);
			state->value = state->start + value;
		}
	}

	display_value = state->value;
	if (display_value > 79)
	{
		display->first_line = 200;
		display->second_line = 250;
	}
	else
	{
		display->first_line = display_value;
		display->second_line = 159 - display_value;
	}
}

void DisplayTransition_UpdateFromCentre(void)
{
	struct DisplayTransitionFrame  *state = *Ram_DisplayWork;
	struct DisplayTransitionWindow *window = *(struct DisplayTransitionWindow **)Ram_MapWork;
	s8							   *duration = &state->duration;
	u16								value;

	if (*duration != 0)
	{
		s8 *step = &state->step;

		if (*step >= *duration)
		{
			*duration = 0;
			Scheduler_RemoveCallback(DisplayTransition_UpdateFromCentre);
			Runtime_SetIrqHandler(1, 0, 0);
			return;
		}
		else
		{
			s32 delta = state->end - state->start;
			s32 offset;

			(*step)++;
			offset = Iwram_SignedDivide(delta * *step, *duration);
			state->value = state->start + offset;
		}
	}

	value = state->value;
	if (value > 79)
	{
		window->first_line = 200;
		window->second_line = 250;
	}
	else if (value != 0 && (gFrameCount & 1))
	{
		window->first_line = value + 80;
		window->second_line = 80 - value;
	}
	else
	{
		window->first_line = 0;
		window->second_line = 159;
	}
}

void DisplayTransition_UpdateScanline(void)
{
	u32								line;
	struct DisplayTransitionWindow *state;

	line = REG_VCOUNT;
	state = gMapWork[0];

again:
	switch (state->irq_phase)
	{
	case 3:
		if (line >= state->irq_first_line)
		{
			REG_DISPCNT = (REG_DISPCNT & 0xFFF8) | 2;
			state->irq_phase = 9;
		}
		break;
	case 2:
		if (line >= state->irq_second_line)
		{
			REG_DISPCNT &= 0xFFF8;
			state->irq_phase = 9;
		}
		break;
	case 1:
		if (line >= state->irq_first_line)
		{
			REG_DISPCNT = (REG_DISPCNT & 0xFFF8) | 2;
			state->irq_phase++;
			goto again;
		}
		if (line >= state->irq_second_line)
		{
			REG_DISPCNT &= 0xFFF8;
			state->irq_phase = 3;
			goto again;
		}
		break;
	case 0:
		if (line <= 158)
		{
			state->irq_phase = 1;
			goto again;
		}
		break;
	}
}

void DisplayTransition_UpdateFrame(void)
{
	struct DisplayTransitionFrame *state = *Ram_DisplayWork;
	s32							   value;
	s32							   blend;
	u32							   i;

	if (state->duration != 0)
	{
		if (state->step >= state->duration)
		{
			volatile u16 *dma0;
			state->duration = 0;
			Scheduler_RemoveCallback(DisplayTransition_UpdateFrame);
			dma0 = REG_DMA0;
			dma0[5] &= 0xc5ff;
			dma0[5] &= 0x7fff;
			(void)dma0[5];
			return;
		}
		else
		{
			s32 delta = state->end - state->start;
			state->step++;
			state->value = state->start + Iwram_SignedDivide(delta * state->step, state->duration);
		}
	}

	value = state->value - 1;
	state->dither_toggle ^= 1;
	blend = 0;
	if (value & 32)
		blend = 15;
	value = (value & 31) * 2;
	for (i = 0; i <= 1; i++)
	{
		u8	entry = DisplayTransition_DitherTable[value & 63];
		u8 *p = &state->palette_nibbles[entry >> 1];
		if (entry & 1)
			*p = (*p & 0x0f) | (blend << 4);
		else
			*p = (*p & 0xf0) | blend;
		value++;
	}

	IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->palette_nibbles, 0x84000008);
}

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

void DisplayTransition_FillTilemapAndSolidTile(s32 color)
{
	struct DisplayTransitionState *work = gDisplayTransition;
	Dma_Fill32(0xf000f000, BG_SCREEN_BLOCK(4), 0x500);
	if (color != -1)
	{
		u32	 pattern = 0;
		s32	 cnt;
		u32 *tile;
		for (cnt = 7; cnt >= 0; --cnt)
			pattern = (pattern << 4) | color;
		tile = work->solid_tile;
		for (cnt = 7; cnt >= 0; --cnt)
			*tile++ = pattern;
		Dma_Set(work->solid_tile, BG_CHAR_BLOCK(0), 0x84000008, REG_DMA3);
	}
}

void DisplayTransition_InitializeState(s32 value)
{
	struct DisplayTransitionState *state;

	state = Runtime_AllocateBlock(0x1f, sizeof(*state));
	Dma_Fill32(0, state, sizeof(*state));
	DisplayTransition_FillTilemapAndSolidTile(0);
	state->mode = value;
	state->value = 0;
	Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
	WaitFrames(0x78);
}
