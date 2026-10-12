#include "unit/graphics/font/mark_codes.h"
#include "unit/graphics/window/scales.h"
#include "unit/graphics/window/sideobjt.h"
#include "unit/graphics/window/spranim.h"
#include "unit/graphics/window/textspd.h"
#include "unit/menu/optdata.h"
#include "unit/graphics/window/window.h"
#include "unit/graphics/window/window11.h"
#include "unit/graphics/window/window12.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/graphics/text/moji.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "types.h"
#include "edition.h"
#include "window.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "global_cells.h"
#include "menu_list.h"
#include "render_input.h"
#include "resource.h"
#include "output.h"
#include "glyph.h"
#include "game_state.h"
#include "btlwork.h"
#include "heap_state.h"
#include "text_font.h"
#include "text_render_runtime.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "resource_ids.h"
#include "ramcode.h"
#include "affine.h"
#include "btlpty.h"
#include "scene.h"
#include "text_reader.h"
#include "lib.h"
#include "battle_unit.h"
#include "btlsys.h"
#include "item.h"
#include "vram_block.h"
#include "select.h"
#include "node_chain.h"
#include "text/msg_ids.h"
#include "sound_ids.h"
#include "layout_guard.h"
#include "field_event.h"
#include "b5_context.h"
#include "btltype.h"
#include "uiflow.h"
#include "owner_state.h"
#include "workspace_options.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "save_state.h"
#include "irq.h"
#include "character_menu.h"
#include "animspr.h"
#include "four_object_motion.h"
#include "field_sprite.h"
#include "uiwindow.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "vram_tab.h"
#include "djinn_preview.h"
#include "ui.h"
#include "motobj.h"
#include "effect_step.h"
#include "resmenu.h"
#include "btlsmn.h"
#include "serial_runtime.h"
#include "battle_effect_work.h"
#include "shop.h"
#include "disptran.h"
#include "frame.h"

struct WindowLine
{
	u16 left;
	u16 span;
};

struct WindowBuffer
{
	u8				  unknown_000[4];
	struct WindowLine lines[160];
};

void UiWork_CloseAndRelease(void);

void AffineEffect_UpdateFrame(void)
{
	struct ResourceMenuWork	 *work;
	struct ResourceMenuEntry *sprite;
	struct AffineTransform	  effect;
	s32						  scale;
	s32						  matrix;
	s32						  edge;

	s32							   y;
	u32							   end;
	u32							   i;
	u16							  *line;
	struct DisplayTransitionState *buffers;

	work = gMenuSelectWork;
	sprite = work->entries;
	scale = Ui_BounceScales[(gFrameCount * 2) & 31];
	scale = (scale - 256) / 4 + 304;
	effect.scale_x = scale;
	effect.scale_y = scale;
	effect.angle = 0;
	matrix = AffineMatrix_BuildForEffect(&effect);
	for (i = 0; i < work->count; i++, sprite++)
	{
		u32 *attr;
		s32	 slot;
		s32	 px;

		px = sprite->x;
		if (px == 0)
			continue;
		if (i == work->selection)
		{
			s32 left = px + scale * 7 / 512 - 20;

			if (sprite->y != 0)
				y = sprite->y + scale * 3 / 256 - 20;
			else
				y = (scale * 15 / 256 - 30) & 255;
			attr = &sprite->link;
			*attr++ = 0;
			*attr++ = (matrix << 25) | y | (left << 16) | 0x80002300;
			*attr = gVramBlockCache[sprite->slot].offset >> 5;
			slot = 246;
		}
		else
		{
			u32 base;
			u32 value;

			attr = &sprite->link;
			/* FAKEMATCH: needed to match: reading the y halfword through attr
             * keeps the attribute-pointer copy before its load; sprite->y
             * moves the copy two instructions later. */
			y = ((s16 *)attr)[7];
			*attr++ = 0;
			value = y | (px << 16) | 0x80002000;
			base = gVramBlockCache[sprite->slot].offset;
			*attr++ = value;
			*attr = base >> 5;
			slot = 245;
		}
		Runtime_PushSlotEntry(sprite, slot);
	}
	if (work->row == 0)
	{
		struct WindowBuffer *window;

		s32 span;

		buffers = (struct DisplayTransitionState *)gDisplayTransition;
		if (buffers == 0 || work->count == 0)
			return;
		window = (struct WindowBuffer *)buffers->pages[buffers->page];
		edge = work->entries[work->selection].x;
		span = (scale * 12 - 2817) / 256;
		edge = ((edge - span) << 8) + (edge + span) + 23;
		end = work->entries[work->selection].y + 24;
		end += (scale * 32 - 7937) / 512 + 1;
		line = &window->lines[24].span;
		i = 24;
		if (i < end)
		{
			do
			{
				*line = (*line & 0xff) | edge;
				i++;
				line += 2;
			} while (i < end);
		}
		edge = work->entries[0].x;
		if (work->selection == 0)
			edge -= (scale * 12 - 2817) / 256;
		edge <<= 8;
		line = &window->lines[0].span;
		for (i = 0; i <= 23; i++)
		{
			*line = (*line & 0xff) | edge;
			line += 2;
		}
	}
	else
	{
		struct WindowBuffer *window;
		s32					 start;

		s32 span;

		buffers = (struct DisplayTransitionState *)gDisplayTransition;
		if (buffers == 0 || work->count == 0)
			return;
		window = (struct WindowBuffer *)buffers->pages[buffers->page];
		edge = work->entries[work->selection].x;
		span = (scale * 12 - 2817) / 256;
		edge = ((edge - span) << 8) + (edge + span) + 23;
		start = work->entries[work->selection].y;
		start -= (scale * 32 - 7937) / 512 + 1;
		line = &window->lines[start].span;
		i = start;
		if (i <= 135)
		{
			do
			{
				*line = (*line & 0xff) | edge;
				i++;
				line += 2;
			} while (i <= 135);
		}
		edge = work->entries[0].x;
		if (work->selection == 0)
			edge -= (scale * 12 - 2817) / 256;
		edge <<= 8;
		line = &window->lines[136].span;
		for (i = 136; i <= 159; i++)
		{
			*line = (*line & 0xff) | edge;
			line += 2;
		}
	}
}

void *AffineEffect_InitializeWork(void)
{
	void		*work;
	volatile u32 zero;
	work = Runtime_AllocateBlock(58, 152);
	zero = 0;
	Dma_Set(&zero, work, 0x85000026, REG_DMA3);
	Scheduler_AddOrUpdateCallback(AffineEffect_UpdateFrame, 0xc76);
	return work;
}

void Menu_EndResourceSelection(void)
{
	struct UiWindow			*child;
	s32						 i;
	struct ResourceMenuWork *work;

	work = gMenuSelectWork;
	Scheduler_RemoveCallback(AffineEffect_UpdateFrame);
	child = work->window;
	if (child != 0)
	{
		UiWork_Finalize(child, 2);
	}
	i = 0;
	while (i < work->count)
	{
		Resource_ResetEntry(work->entries[i].slot);
		i += 1;
	}
	Runtime_ReleaseHeapBlock(0x3A);
	WaitFrames(1U);
}

s32 Menu_RunResourceSelectionLoop(s32 initial)
{
	struct ResourceMenuWork *work = gMenuSelectWork;
	s32						 resource;

	work->selection = initial;
redraw:
	RenderOutput_PrepareForRedraw(work->window);
	if (work->resource_base != 0)
		resource = work->resource_base + work->selection;
	else
		resource = work->resource_ids[work->selection] + (s32)MsgCommandName;
	UiText_DrawCharacterAtOffset(resource, work->window, 0, 0);
	for (;;)
	{
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			AudioCommand_PlayFar(112);
			return work->selection;
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			return -1;
		}
		if (gKeyState & KEY_START)
		{
			AudioCommand_PlayFar(113);
			return -1;
		}
		if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP))
		{
			AudioCommand_PlayFar(111);
			work->selection--;
			if (work->selection < 0)
				work->selection = work->count - 1;
			goto redraw;
		}
		if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN))
		{
			AudioCommand_PlayFar(111);
			work->selection++;
			if (work->selection >= work->count)
				work->selection = 0;
			goto redraw;
		}
	}
}

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs)
{
	if (diff >= 0)
		return diff;
	return rhs - lhs;
}

s32 Menu_SelectResource(s32 start, s32 goal)
{
	s16						 resource_base;
	s16						 cur;
	s32						 diff;
	s32						 resource_id;
	s32						 dist;
	s32						 pos;
	s32						 step;
	s32						 delay;
	const u8				*tbl;
	struct ResourceMenuWork *state;

	state = gMenuSelectWork;
	step = 1;
	delay = 12;
	state->selection = start;
	if (goal < start)
		step = -1;
	pos = start;

	for (;;)
	{
		RenderOutput_PrepareForRedraw(state->window);
		resource_base = state->resource_base;
		if (resource_base != 0)
		{
			resource_id = resource_base + state->selection;
		}
		else
		{
			resource_id = state->resource_ids[state->selection] + (s32)MsgCommandName;
		}
		UiText_DrawCharacterAtOffset(resource_id, state->window, 0, 0);

		cur = state->selection;
		tbl = Menu_SelectionStepDelays;
		diff = cur - goal;
		dist = AbsoluteDifference(diff, cur, goal);
		WaitFrames(tbl[dist] + delay);

		if (pos == goal)
			break;

		state->selection += step;
		AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
		delay = 0;
		pos += step;
	}

	WaitFrames(48);
	AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
	return goal;
}

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
	s32	  size = 1024;
	void *buffer = MemAlloc(size);
	u16	 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

	Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
	VramBlock_LoadCached(slot, size, buffer);
	MemFree(buffer);
}

void Menu_AppendResourceEntry(s32 no)
{
	struct ResourceMenuWork	 *work;
	struct ResourceMenuEntry *entry;
	s16						  index;
	s32						  slot;
	s32						  flags;

	work = gMenuSelectWork;
	index = work->count;
	if (index <= 5)
	{
		work->count++;
		entry = &work->entries[index];
		slot = Resource_FindFreeEntry();
		Menu_LoadResourceSlot(slot, no);
		entry->x = index * 24 + 32;
		flags = 136;
		entry->y = flags;
		entry->slot = slot;
		work->resource_ids[index] = no;
	}
}

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resource_base)
{
	struct ResourceMenuWork *menu = gMenuSelectWork;
	s32						 x;
	s32						 i;
	s32						 count;

	menu->width = width + 2;
	menu->resource_base = resource_base;
	menu->row = row;
	count = menu->count;
	x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
	for (i = 0; i < menu->count; i++)
	{
		struct ResourceMenuEntry *entry = &menu->entries[i];

		entry->x = x * 8;
		entry->y = row * 8;
		x += 3;
	}
	menu->window = UiWindow_Create(x, row, menu->width, 3, 2);
}

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resource_base)
{
	struct ResourceMenuWork	 *work;
	s32						  i;
	struct ResourceMenuEntry *entry;

	work = gMenuSelectWork;
	work->width = width + 2;
	work->resource_base = resource_base;
	work->row = y;
	for (i = 0; i < work->count; i++)
	{
		entry = &work->entries[i];
		entry->y = y << 3;
		entry->x = x << 3;
		x += 3;
	}
	work->window = UiWindow_Create(x, y, work->width, 3, 2);
}

static __inline__ s32 TblGet(s8 *tbl, s32 index)
{
	return tbl[index];
}

s32 Menu_SelectTopEntry(s32 sel)
{
	s32 group = 0;
	s32 ofs;
	s32 triple;
	s32 ret;
	s8 *tbl;

	if (Party_SumDjinnCountsFar(-1) == 0)
	{
		group = 1;
	}

	triple = group * 3;
	tbl = Menu_TopEntryPositionByCommand;
	ofs = triple << 1;
	sel = TblGet(tbl, sel + ofs) - 1;
	if (sel < 0)
	{
		sel = 0;
	}

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(1);
	if (group == 0)
	{
		Menu_AppendResourceEntry(15);
	}
	Menu_AppendResourceEntry(2);
	Menu_AppendResourceEntry(7);
	Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();

	if (ret >= 0)
	{
		ret = Menu_TopEntryCommandByPosition[ret + ofs + 1];
	}

	return ret;
}

#if EDITION_INTERNATIONAL
#define ANIMATE_ENTRIES_ROW 7
#else
#define ANIMATE_ENTRIES_ROW 5
#endif

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1)
{
	UiWindow_OpenMode1AndWaitFrame();
	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(1);
	Menu_AppendResourceEntry(0xF);
	Menu_AppendResourceEntry(2);
	Menu_AppendResourceEntry(7);
	Menu_CenterResourceEntries(0x11, ANIMATE_ENTRIES_ROW, 0);
	arg1 = Menu_SelectResource(arg0, arg1 - 1);
	Menu_EndResourceSelection();
	UiWork_CloseAndRelease();
	return arg1;
}

s32 Menu_SelectSaveSlotAction(void)
{
	s32 type;
	s32 ret;
	s32 initial;
	u32 group;

	group = 0;
	initial = 0;
	type = SaveState_ScanRecordFlags();
	if (type < 0)
	{
		return -1;
	}
	if (type == 0)
	{
		return 0;
	}
	if (type == 3)
	{
		group = 1;
	}
	else if (type == 0x67)
	{
		group = 2;
	}
	else if (type > 0x64)
	{
		group = 3;
	}
	else
	{
		initial = 1;
	}
	AffineEffect_InitializeWork();
	if ((group == 0) || (group == 3))
	{
		Menu_AppendResourceEntry(0x15);
	}
	if (group <= 1U)
	{
		Menu_AppendResourceEntry(0x16);
	}
	if ((group == 0) || (group == 3))
	{
		Menu_AppendResourceEntry(0x17);
	}
	Menu_AppendResourceEntry(0x18);
	if ((*(s16 *)gTitleExtraOptionEnabled) != 0)
	{
		Menu_AppendResourceEntry(0x1D);
	}
	if (gTitleSendOptionEnabled != 0)
	{
		Menu_AppendResourceEntry(0x1E);
	}
	Menu_CenterResourceEntries(0x11, TYPE_MENU_WIDTH, 0);
	ret = Menu_RunResourceSelectionLoop(initial);
	Menu_EndResourceSelection();
	if (ret >= 0)
	{
		ret = Menu_SaveSlotActionByPosition[ret + (group * 6)];
	}
	return ret;
}

#if EDITION_INTERNATIONAL
#define MODE_LABEL_X 18
#define MODE_LABEL_RIGHT 144
#else
#define MODE_LABEL_X 8
#define MODE_LABEL_RIGHT 80
#endif

void Menu_DrawModeLabel(void)
{
	struct ResourceMenuWork *state = gMenuSelectWork;

	if (state->previous_mode != state->selection)
	{
		state->previous_mode = state->selection;
		UiWindow_ClearInteriorTiles(state->lower_window, 8, 40, MODE_LABEL_RIGHT, 80);

		if (state->selection != 1)
		{
			if (state->selection > 1)
				goto mode_other;
			if (state->selection != 0)
				goto mode_other;

			{
				s32 text = (s32)MsgPasswordLevel;

				UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 40);
				UiText_DrawCharacterAtOffset(text + 1, state->lower_window, MODE_LABEL_X, 48);
				UiText_DrawCharacterAtOffset(text + 2, state->lower_window, MODE_LABEL_X, 56);
				UiText_DrawCharacterAtOffset(text + 3, state->lower_window, MODE_LABEL_X, 64);
				text += 4;
				UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 72);
				goto done;
			}
		}

		{
			s32 text = (s32)MsgPasswordLevel;

			UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 40);
			UiText_DrawCharacterAtOffset(text + 1, state->lower_window, MODE_LABEL_X, 48);
			text += 2;
			UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 56);
			goto done;
		}

	mode_other:
		{
			s32 text = (s32)MsgPasswordLevel;

			UiText_DrawCharacterAtOffset(text++, state->lower_window, MODE_LABEL_X, 40);
			UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 48);
		}
	done:;
	}
}

#if EDITION_INTERNATIONAL
#define PASSWORD_HELP_X 16
#else
#define PASSWORD_HELP_X 0
#endif

#if defined(TBS_EDITION_FR)

void Menu_DrawModeIndicator(void)
{
	struct ResourceMenuWork *state = gMenuSelectWork;

	if (state->previous_mode != state->selection)
	{
		if (state->selection == 0)
		{
			RenderOutput_ClearList(state->lower_window);
			UiText_DrawResource((s32)MsgPasswordTransferHelp, (s32)state->lower_window, 16, 4);
			UiText_DrawResource((s32)MsgPasswordTransferHelp + 1, (s32)state->lower_window, 16, 16);
		}
		else if (state->previous_mode == 0)
		{
			RenderOutput_ClearList(state->lower_window);
			UiText_DrawResource((s32)MsgCableTransferHelp, (s32)state->lower_window, 0, 4);
			UiText_DrawResource((s32)MsgCableTransferHelp + 1, (s32)state->lower_window, 0, 16);
			UiText_DrawResource((s32)MsgCableTransferHelp + 2, (s32)state->lower_window, 0, 28);
		}
		state->previous_mode = state->selection;
	}
}
#else
void Menu_DrawModeIndicator(void)
{
	struct ResourceMenuWork *state = gMenuSelectWork;
	s16						*shown = &state->previous_mode;
	s16						*current = &state->selection;

	if (*shown != *current)
	{
		*shown = *current;
		RenderOutput_PrepareForRedraw(state->lower_window);
		if (*current == 0)
		{
#if defined(TBS_EDITION_DE)
			UiText_DrawCharacterAtOffset((s32)MsgPasswordTransferHelp, state->lower_window, 32, 8);
			UiText_DrawCharacterAtOffset(
				(s32)MsgPasswordTransferHelp + 1, state->lower_window, 32, 24);
#else
			UiText_DrawResource(
				(s32)MsgPasswordTransferHelp, state->lower_window, PASSWORD_HELP_X, 4);
			UiText_DrawResource(
				(s32)MsgPasswordTransferHelp + 1, state->lower_window, PASSWORD_HELP_X, 16);
#endif
		}
		else
		{
#if defined(TBS_EDITION_DE)
			UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp, state->lower_window, 0, 0);
			UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp + 1, state->lower_window, 0, 16);
			UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp + 2, state->lower_window, 0, 32);
#else
			UiText_DrawResource((s32)MsgCableTransferHelp, state->lower_window, 0, 4);
			UiText_DrawResource((s32)MsgCableTransferHelp + 1, state->lower_window, 0, 16);
			UiText_DrawResource((s32)MsgCableTransferHelp + 2, state->lower_window, 0, 28);
#endif
		}
	}
}
#endif

s32 Menu_SelectResourceLayout(s32 mode)
{
	struct ResourceMenuWork *work;
	struct UiWindow			*window;
	s32						 msg;
	struct UiWindow		   **upper;
	s32						 result;

	AffineEffect_InitializeWork();
	work = gMenuSelectWork;
	if (mode == 0)
	{
		Menu_AppendResourceEntry(44);
		Menu_AppendResourceEntry(45);
	}
	else
	{
		Menu_AppendResourceEntry(46);
		Menu_AppendResourceEntry(47);
		Menu_AppendResourceEntry(48);
#if defined(TBS_EDITION_ES)
		Menu_CenterResourceEntries(17, 5, 0);
#endif
	}
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
	Menu_CenterResourceEntries(17, 7, 0);
#elif defined(TBS_EDITION_ES)
#elif defined(TBS_EDITION_FR)
	Menu_CenterResourceEntries(17, 9, 0);
#else
	Menu_CenterResourceEntries(17, 5, 0);
#endif
	if (mode != 0)
	{
		Scheduler_AddOrUpdateCallback(Menu_DrawModeLabel, 0xc76);
		work->previous_mode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
		window = UiWindow_Create(7, 0, 17, 4, 2);
#elif defined(TBS_EDITION_ES)
		window = UiWindow_Create(5, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR)
		window = UiWindow_Create(5, 0, 22, 4, 2);
#else
		window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
		msg = (s32)MsgPasswordSelection;
		upper = &work->upper_window;
		*upper = window;
		UiText_DrawResource(msg, (s32)window, 0, 4);
#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_DE)
		window = UiWindow_Create(1, 4, 28, 12, 2);
#else
		window = UiWindow_Create(3, 4, 25, 12, 2);
#endif
		work->lower_window = window;
		UiText_DrawResource(msg + 1, (s32)window, 8, 0);
		UiText_DrawResource(msg + 2, (s32)work->lower_window, 8, 11);
		msg += 3;
		UiText_DrawResource(msg, (s32)work->lower_window, 8, 22);
#else
		window = UiWindow_Create(5, 4, 21, 12, 2);
		work->lower_window = window;
		UiText_DrawResource(msg + 1, (s32)window, 0, 4);
		msg += 2;
		UiText_DrawResource(msg, (s32)work->lower_window, 0, 16);
#endif
	}
	else
	{
		Scheduler_AddOrUpdateCallback(Menu_DrawModeIndicator, 0xc76);
		work->previous_mode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
		window = UiWindow_Create(6, 0, 18, 4, 2);
#elif defined(TBS_EDITION_ES)
		window = UiWindow_Create(4, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
		window = UiWindow_Create(5, 0, 20, 4, 2);
#else
		window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
		upper = &work->upper_window;
		*upper = window;
#if EDITION_INTERNATIONAL
		UiText_DrawResource((s32)MsgTransferMethod, (s32)window, 2, 4);
#else
		UiText_DrawResource((s32)MsgTransferMethod, (s32)window, 0, 4);
#endif
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
		work->lower_window = UiWindow_Create(1, 5, 28, 7, 2);
#elif defined(TBS_EDITION_DE)
		work->lower_window = UiWindow_Create(0, 5, 30, 7, 2);
#else
		work->lower_window = UiWindow_Create(5, 5, 21, 7, 2);
#endif
	}
	result = Menu_RunResourceSelectionLoop(0);
	if (mode != 0)
		Scheduler_RemoveCallback(Menu_DrawModeLabel);
	else
		Scheduler_RemoveCallback(Menu_DrawModeIndicator);
	RenderOutput_PrepareForRedraw(*upper);
	RenderOutput_PrepareForRedraw(work->lower_window);
	UiWork_Finalize(*upper, 2);
	UiWork_Finalize(work->lower_window, 2);
	WaitFrames(1);
	Menu_EndResourceSelection();
	return result;
}

//--------------------------------------------------------------------------
// confirm_selection.c
// メニュー：コマンドのアイコンを並べてえらばせる（はい・いいえ など）
//--------------------------------------------------------------------------
// アイコンは ResourceId_CommandIcons の番号。
// えらんでいるコマンドの名前をウィンドウに出す
//--------------------------------------------------------------------------
// アイコン 0x11〜0x14 を画面の中央に並べてえらぶ（行 17）
// sel : はじめにえらんでいる位置。戻り値はえらんだ位置
//--------------------------------------------------------------------------
s32 Menu_SelectEntry11To14(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x11);
	Menu_AppendResourceEntry(0x12);
	Menu_AppendResourceEntry(0x13);
	Menu_AppendResourceEntry(0x14);
#if defined(TBS_EDITION_DE) || defined(TLA_EDITION_DE)
	Menu_CenterResourceEntries(17, 8, 0); // 名前のウィンドウの幅
#elif EDITION_INTERNATIONAL
	Menu_CenterResourceEntries(17, 7, 0);
#else
	Menu_CenterResourceEntries(17, 6, 0);
#endif
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}

//--------------------------------------------------------------------------
// アイコン 0x19〜0x1c を画面の中央に並べてえらぶ（行 17）
//--------------------------------------------------------------------------
s32 Menu_SelectEntry19To1c(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x19);
	Menu_AppendResourceEntry(0x1A);
	Menu_AppendResourceEntry(0x1B);
	Menu_AppendResourceEntry(0x1C);
#if EDITION_INTERNATIONAL
	Menu_CenterResourceEntries(17, 10, 0);
#else
	Menu_CenterResourceEntries(17, 8, 0);
#endif
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}

//--------------------------------------------------------------------------
// はい・いいえ（アイコン 5・6）をえらぶ。キャンセルは 1（いいえ）
// low  : 0 なら行 0、0 でなければ行 17 に出す
// mes  : 名前のメッセージの先頭（0 ならコマンド名）
// w    : 名前のウィンドウの幅（0 なら 3）
// sel  : はじめにえらんでいる位置
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelection(s32 low, s32 mes, s32 w, s32 sel)
{
	s32 row;

	row = 0;

	AffineEffect_InitializeWork();

	if (w == 0)
	{
		w = 3;
	}
	if (low != 0)
	{
		row = 17;
	}

	Menu_AppendResourceEntry(5);
	Menu_AppendResourceEntry(6);
	Menu_CenterResourceEntries(row, w, mes);

	sel = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	if (sel == -1)
	{
		sel = 1;
	}
	return sel;
}

//--------------------------------------------------------------------------
// はい・いいえを (x, y) に出してえらぶ。キャンセルは 1（いいえ）
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 sel)
{
	s32 mes = (s32)MsgCommandYes;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(5);
	Menu_AppendResourceEntry(6);
	Menu_LayoutResourceEntries(x, y, 3, mes);
	sel = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	if (sel == -1)
	{
		sel = 1;
	}
	return sel;
}

//--------------------------------------------------------------------------
// select_entry_20_to_21.c
// メニュー：コマンドのアイコン 0x20・0x21 の 2 つからえらばせる
//--------------------------------------------------------------------------
// 版ごとの名前のウィンドウの幅
#if !EDITION_INTERNATIONAL
#define PAIR_WIDTH 7
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) || defined(TLA_EDITION_FR) \
	|| defined(TLA_EDITION_IT)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

//--------------------------------------------------------------------------
// アイコン 0x20・0x21 を画面の中央に並べてえらぶ（行 17）
// sel : はじめにえらんでいる位置。戻り値はえらんだ位置
//--------------------------------------------------------------------------
s32 Menu_SelectEntry20To21(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x20);
	Menu_AppendResourceEntry(0x21);
	Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}
