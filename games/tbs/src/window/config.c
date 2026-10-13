#include "unit/window/scales.h"
#include "unit/window/optdata.h"
#include "unit/window/window.h"
#include "unit/window/config.h"
#include "unit/window/wintile.h"
#include "unit/window/output.h"
#include "unit/window/ptystat.h"
#include "unit/window/save.h"
#include "unit/window/btlcmd.h"
#include "unit/window/debug.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/evmes.h"
#include "unit/window/moji.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/objvram.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/heap.h"
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

struct SelectionWorkspace
{
	u8				 unknown_000[0x46];
	u16				 first_active;
	u16				 first_resource;
	u8				 unknown_04a[0x352 - 0x4a];
	u16				 second_active;
	u16				 second_resource;
	u8				 unknown_356[0xff4 - 0x356];
	struct UiWindow *window;
	u8				 unknown_ff8[12];
};

void Menu_AllocateSelectionWorkspace(void)
{
	struct SelectionWorkspace *work;
	u32						   zero;

	work = Runtime_AllocateBlock(0x13, 0x1004);
	zero = 0;
	work->first_active = zero;
	work->second_active = zero;
}

void Ui_FinalizeWorkAndReleaseHeap13(void)
{
	struct SelectionWorkspace *state;

	state = *(struct SelectionWorkspace **)gSelectionWorkspace;
	UiWork_Finalize(state->window, 0);
	while (UiWork_IsIdle(state->window) == 0)
	{
		WaitFrames(1);
	}
	if (state->first_active != 0)
	{
		Resource_ResetEntry(state->first_resource);
	}
	if (state->second_active != 0)
	{
		Resource_ResetEntry(state->second_resource);
	}
	Runtime_ReleaseHeapBlock(0x13);
}

void GraphicsPalette_ReservedNoOpC9BC(void)
{
}

void GraphicsPalette_ReservedNoOpC9C0(void)
{
}

void GraphicsPalette_ReservedNoOpC9C4(void)
{
}

struct MenuWorkspaceEntry
{
	u8	unknown_00[10];
	u16 state;
	u8	unknown_0c[40];
};

void Menu_ResetWorkspaceSelection(struct WorkspaceWork *work)
{
	struct MenuWorkspaceEntry *entry = (struct MenuWorkspaceEntry *)((u8 *)work + 0x400);
	u16						   zero = 0;

	entry->state = zero;
	entry++;
	entry->state = zero;
	entry++;
	entry->state = zero;
	entry++;
	entry->state = zero;
	entry++;
	entry->state = zero;
	entry++;
	entry->state = zero;
	entry++;
	entry->state = zero;
	work->page = zero;
	work->tint[0] = zero;
	work->tint[1] = zero;
	work->tint[2] = zero;
	work->cursor_frame = zero;
}

void GraphicsPalette_SetTintChannelsFromCounters(struct WorkspaceWork *work)
{
	s16 phase;
	s32 bias;
	s32 blue;
	s32 red;
	s32 green;

	phase = (gGameState.palette_glow[0] + 12) % 24 * 4;
	bias = gGameState.palette_glow[1] - 7;
	red = PaletteGlow_WaveTable[(phase % 96)];
	green = PaletteGlow_WaveTable[(phase + 32) % 96];
	blue = PaletteGlow_WaveTable[(phase + 64) % 96];
	red += bias;
	green += bias;
	blue += bias;
	if (red < 0)
		red = 0;
	if (green < 0)
		green = 0;
	if (blue < 0)
		blue = 0;
	if (red > 31)
		red = 31;
	if (green > 31)
		green = 31;
	if (blue > 31)
		blue = 31;
	work->tint[0] = red;
	work->tint[1] = green;
	work->tint[2] = blue;
}

void Palette_WriteBlendedBank15Entries(struct WorkspaceWork *work)
{
	*(s16 *)0x050001E8 = Palette_ScaleTintChannels(work, 0xEEEE, 0xCCCC, 0x11110);
	*(s16 *)0x050001EA = Palette_ScaleTintChannels(work, 0xD555, 0xBBBB, 0xEEEE);
	*(s16 *)0x050001EC = Palette_ScaleTintChannels(work, 0xBBBB, 0xAAAA, 0xCCCC);
	*(s16 *)0x050001EE = Palette_ScaleTintChannels(work, 0xA221, 0x9999, 0xAAAA);
	*(s16 *)0x050001F0 = Palette_ScaleTintChannels(work, 0x10888, 0xDDDD, 0x13333);
	*(s16 *)0x050001F2 = Palette_ScaleTintChannels(work, 0x12221, 0xEEEE, 0x15555);
	*(s16 *)0x050001F4 = Palette_ScaleTintChannels(work, 0x13BBB, 0x10000, 0x17777);
}

u16 Palette_ScaleTintChannels(struct WorkspaceWork *work, s32 scale_r, s32 scale_g, s32 scale_b)
{
	s32 r;
	s32 g;
	s32 b;

	r = Iwram_MulQ16(work->tint[0] << 16, scale_r) >> 16;
	g = Iwram_MulQ16(work->tint[1] << 16, scale_g) >> 16;
	b = Iwram_MulQ16(work->tint[2] << 16, scale_b) >> 16;
	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
	if (r > 31)
		r = 31;
	if (g > 31)
		g = 31;
	if (b > 31)
		b = 31;
	return r + ((b << 10) + (g << 5));
}

u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale)
{
	s32 red;
	s32 green;
	s32 blue;

	red = Iwram_MulQ16(rgb[0] << 16, red_scale) >> 16;
	green = Iwram_MulQ16(rgb[1] << 16, green_scale) >> 16;
	blue = Iwram_MulQ16(rgb[2] << 16, blue_scale) >> 16;
	if (red < 0)
		red = 0;
	if (red > 31)
		red = 31;
	if (green < 0)
		green = 0;
	if (green > 31)
		green = 31;
	if (blue < 0)
		blue = 0;
	if (blue > 31)
		blue = 31;
	return red + ((blue << 10) + (green << 5));
}

void PaletteGlow_Update(s32 phase, s32 brightness)
{
	s16 rgb[3];
	s16 step;
	s16 red;
	s16 green;
	s16 blue;
	s16 offset;

	step = ((phase + 12) % 24) * 4;
	offset = brightness - 7;
	red = PaletteGlow_WaveTable[(step % 96)] + offset;
	green = PaletteGlow_WaveTable[(step + 32) % 96] + offset;
	blue = PaletteGlow_WaveTable[(step + 64) % 96] + offset;
	if (red < 0)
		red = 0;
	if (red > 31)
		red = 31;
	if (green < 0)
		green = 0;
	if (green > 31)
		green = 31;
	if (blue < 0)
		blue = 0;
	if (blue > 31)
		blue = 31;
	rgb[0] = red;
	rgb[1] = green;
	rgb[2] = blue;
	BG_PLTT_COLOR(15, 4) = Color_ScaleComponents(rgb, 0xeeee, 0xcccc, 0x11110);
	BG_PLTT_COLOR(15, 5) = Color_ScaleComponents(rgb, 0xd555, 0xbbbb, 0xeeee);
	BG_PLTT_COLOR(15, 6) = Color_ScaleComponents(rgb, 0xbbbb, 0xaaaa, 0xcccc);
	BG_PLTT_COLOR(15, 7) = Color_ScaleComponents(rgb, 0xa221, 0x9999, 0xaaaa);
	BG_PLTT_COLOR(15, 8) = Color_ScaleComponents(rgb, 0x10888, 0xdddd, 0x13333);
	BG_PLTT_COLOR(15, 9) = Color_ScaleComponents(rgb, 0x12221, 0xeeee, 0x15555);
	BG_PLTT_COLOR(15, 10) = Color_ScaleComponents(rgb, 0x13bbb, 0x10000, 0x17777);
}

void GraphicsPalette_DecrementSelectionWrap(struct WorkspaceWork *work)
{
	if (work->page == 0)
		work->page = 2;
	else
		work->page--;
}

void Menu_AdvanceWorkspaceIndexModulo3(struct WorkspaceWork *work)
{
	if (++work->page > 2)
		work->page = 0;
}

void GraphicsPalette_DecrementSelectedCounter(struct WorkspaceWork *work)
{
	switch (work->page)
	{
	case 0:
		if (gGameState.message_speed != 0)
			gGameState.message_speed--;
		break;
	case 1:
		if (gGameState.palette_glow[0] != 0)
			gGameState.palette_glow[0]--;
		break;
	case 2:
		if (gGameState.palette_glow[1] != 0)
			gGameState.palette_glow[1]--;
		break;
	}
}

void GraphicsPalette_AdjustSelectionCounter(struct WorkspaceWork *work)
{
	switch (work->page)
	{
	case 0:
		if (gGameState.message_speed <= 1)
			gGameState.message_speed++;
		break;
	case 1:
		if (gGameState.palette_glow[0] <= 23)
			gGameState.palette_glow[0]++;
		break;
	case 2:
		if (gGameState.palette_glow[1] <= 14)
			gGameState.palette_glow[1]++;
		break;
	}
}

void GraphicsPalette_SelectionNoOp(void)
{
}

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
	s32					  src0;
	s32					  src1;
	s32					  sel;
	struct WorkspaceWork *work;

	work = gSelectionWork;
	sel = work->page;
	ShopCursor_AdvanceFar(&work->cursor.cursor);
	ShopCursor_MoveTowardTargetFar(&work->marker[0].cursor);
	ShopCursor_MoveTowardTargetFar(&work->marker[1].cursor);

	if (sel == 0)
	{
		src0 = (work->cursor_frame & 7) + (s32)&ResourceId_ShopCursorFrames;
	}
	else
	{
		src0 = (s32)&ResourceId_ShopCursorFrames;
	}
	VramBlock_LoadCached((u8)work->marker[0].output->index, 0x100, Resource_GetTableEntry(src0));

	if (sel == 1)
	{
		src1 = (work->cursor_frame & 7) + (s32)&ResourceId_ShopCursorFrames;
	}
	else
	{
		src1 = (s32)&ResourceId_ShopCursorFrames;
	}
	VramBlock_LoadCached((u8)work->marker[1].output->index, 0x100, Resource_GetTableEntry(src1));

	if (sel > 1)
		Ui_ApplyTableScaleToObject(work->frame[sel - 2][work->option[sel]]);
	work->cursor_frame++;
}

void OptionMenu_InitializeWork(void)
{
	struct WorkspaceWork *work;
	volatile u32		  zero;

	work = Runtime_AllocateBlock(20, 0x628);
	zero = 0;
	Dma_Set((const void *)&zero, work, 0x8500018a, REG_DMA3);
	work->option[0] = gGameState.palette_glow[0];
	work->option_count[0] = 24;
	work->option[1] = gGameState.palette_glow[1];
	work->option_count[1] = 15;
	work->option[2] = gGameState.message_speed;
	work->option_count[2] = 3;
	work->option[3] = gGameState.speech;
	work->option_count[3] = 2;
	work->option[4] = gGameState.saved_options;
	work->option_count[4] = 2;
	Scheduler_AddOrUpdateCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance, 0xc80);
}

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
	Scheduler_RemoveCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance);
	Runtime_ReleaseHeapBlock(0x14);
}

#define SLIDER_PALETTE 14
#define RESOURCE_SLOTS 96

struct RenderInput *Menu_OpenWorkspaceOptions(void)
{
	/* FAKEMATCH: the shared cursor union keeps pointer stores before window reads; an equal-size struct reorders these moves in all six editions. */
	struct WorkspaceWork *work;
	struct RenderInput	 *win;
	struct RenderOutput	 *out;
	s32					  x;
	s32					  y;
	s32					  msg;

	work = gSelectionWork;
	win = UiWindow_Create(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14, 2);
	UiWindow_DrawDividerLine(win, 0, 2, OPTION_WINDOW_WIDTH - 1, 2);
	UiWindow_DrawDividerLine(win, 0, 4, OPTION_WINDOW_WIDTH - 1, 4);
	UiWindow_DrawDividerLine(win, 0, 7, OPTION_WINDOW_WIDTH - 1, 7);
	UiWindow_DrawDividerLine(win, 0, 10, OPTION_WINDOW_WIDTH - 1, 10);
	msg = (s32)MsgWindowColorLabel;
	UiText_DrawCharacterAtOffset(msg, win, 8, 0);
	msg++;
	UiText_DrawCharacterAtOffset(msg, win, 8, 16);
#if EDITION_INTERNATIONAL
	msg = (s32)MsgMessageSpeedLabel;
	UiText_DrawCharacterAtOffset(msg, win, 8, 32);
	msg++;
	UiText_DrawCharacterAtOffset(msg, win, 32, 40);
#else
	UiText_DrawCharacterAtOffset((s32)MsgMessageSpeedLabel, win, 8, 40);
#endif
	UiText_DrawCharacterAtOffset((s32)MsgSpeechLabel, win, 8, 64);
	UiText_DrawCharacterAtOffset((s32)MsgAutoSleepLabel, win, 8, 88);

	x = Resource_FindFreeEntry();
	if (x < RESOURCE_SLOTS)
	{
		VramBlock_LoadCached(x, 128, Resource_FixedBlockBTiles);
		out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
		work->cursor.output = out;
		x = win->x * 8;
		y = win->y * 8 + 12;
		ShopCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
	}

	x = Resource_FindFreeEntry();
	if (x < RESOURCE_SLOTS)
	{
		Dma_Set(WorkspaceOptions_SliderPalette, (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0),
			0x80000020, REG_DMA3);
		VramBlock_LoadCached(x, 256, WorkspaceOptions_SliderTiles);
		y = 0;
		out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
		out->oam.palette = SLIDER_PALETTE;
		out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
		out->oam.index += 4;
		out->oam.palette = SLIDER_PALETTE;
		y = 16;
		out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
		out->oam.palette = 15;
		out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
		out->oam.index += 4;
		out->oam.palette = 15;
	}

	x = Resource_FindFreeEntry();
	if (x < RESOURCE_SLOTS)
	{
		VramBlock_LoadCached(x, 256, 0);
		out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
		out->oam.palette_256 = 1;
		work->marker[0].output = out;
		x = win->x * 8 + OPTION_SLIDER_X;
		x += work->option[0] * 60 / work->option_count[0];
		y = win->y * 8 + 4;
		ShopCursor_SetPositionImmediateFar(&work->marker[0].cursor, x, y);
	}

	x = Resource_FindFreeEntry();
	if (x < RESOURCE_SLOTS)
	{
		VramBlock_LoadCached(x, 256, 0);
		out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
		out->oam.palette_256 = 1;
		work->marker[1].output = out;
		x = win->x * 8 + OPTION_SLIDER_X;
		x += work->option[1] * 60 / work->option_count[1];
		y = win->y * 8 + 20;
		ShopCursor_SetPositionImmediateFar(&work->marker[1].cursor, x, y);
	}

	y = 28;
	work->frame[0][0] =
		RenderResource_CreateFrame(OptionMenu_SpeedFrames[0], 0, win, OPTION_FRAME_X, y);
	work->frame[0][1] =
		RenderResource_CreateFrame(OptionMenu_SpeedFrames[1], 0, win, OPTION_FRAME_X + 24, y);
	work->frame[0][2] =
		RenderResource_CreateFrame(OptionMenu_SpeedFrames[2], 0, win, OPTION_FRAME_X + 48, y);
	y = 52;
	work->frame[1][0] = RenderResource_CreateFrame(OptionMenu_SpeechFrames[0], 0, win, 100, y);
	work->frame[1][1] = RenderResource_CreateFrame(OptionMenu_SpeechFrames[1], 0, win, 124, y);
	y = 76;
	work->frame[2][0] = RenderResource_CreateFrame(OptionMenu_SleepFrames[0], 0, win, 100, y);
	work->frame[2][1] = RenderResource_CreateFrame(OptionMenu_SleepFrames[1], 0, win, 124, y);
	return win;
}

s32 Menu_RunWorkspaceOptions(void)
{
	struct WorkspaceWork *work;
	struct RenderInput	 *icon;
	struct RenderInput	 *win;
	struct RenderOutput	 *pair;
	s8					 *pA;
	s8					 *pB;
	s32					  redraw;
	s32					  page;
	s32					  i;
	s32					  dim;
	s32					  index;
	s32					  x;
	s32					  y;
	s32					  result;
	struct RenderOutput	 *frame;

	redraw = 1;
	page = 0;
	OptionMenu_InitializeWork();
	work = gSelectionWork;
	win = UiWindow_Create(OPTION_WINDOW_X, 2, OPTION_WINDOW_WIDTH, 3, 2);
	icon = Menu_OpenWorkspaceOptions();
	pair = RenderResource_CreatePair(7, icon, 64, -48);
	WaitFrames(1);
	pA = &work->option[0];
	pB = &work->option[1];

	for (;;)
	{
		if (redraw != 0)
		{
			redraw = 0;
			page = (page + 5) % 5;
			work->option[page] =
				(work->option[page] + work->option_count[page]) % work->option_count[page];
			work->page = page;
			if (gSleepDisabled != 0)
				work->option[4] = 0;

			for (i = 0; i <= 2; i++)
			{
				frame = work->frame[0][i];
				frame->sentinel = 0xfb;
				UiIcon_PrepareObjectFar(frame);
				index = (u8)work->frame[0][i]->index;
				dim = 0;
				if (i != work->option[2])
					dim = 1;
				RenderResource_LoadFrame(OptionMenu_SpeedFrames[i], index, dim);
			}
			for (i = 0; i <= 1; i++)
			{
				frame = work->frame[1][i];
				frame->sentinel = 0xfb;
				UiIcon_PrepareObjectFar(frame);
				index = (u8)work->frame[1][i]->index;
				dim = 0;
				if (i != work->option[3])
					dim = 1;
				RenderResource_LoadFrame(OptionMenu_SpeechFrames[i], index, dim);
			}
			for (i = 0; i <= 1; i++)
			{
				frame = work->frame[2][i];
				frame->sentinel = 0xfb;
				UiIcon_PrepareObjectFar(frame);
				index = (u8)work->frame[2][i]->index;
				dim = 0;
				if (i != work->option[4])
					dim = 1;
				RenderResource_LoadFrame(OptionMenu_SleepFrames[i], index, dim);
			}

			x = icon->x * 8 + OPTION_SLIDER_X;
			x += (*pA * 60) / work->option_count[0];
			y = icon->y * 8 + 4;
			Shop_SetCursorFar(&work->marker[0].cursor, x, y, 1);
			x = icon->x * 8 + OPTION_SLIDER_X;
			x += (*pB * 60) / work->option_count[1];
			y = icon->y * 8 + 20;
			Shop_SetCursorFar(&work->marker[1].cursor, x, y, 1);

			x = work->option[2] + (s32)MsgMessageSpeedSetting;
			UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 40, OPTION_SPEED_END, 48);
			UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 40);
			x = work->option[3] + (s32)MsgSpeechSetting;
			UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 64, OPTION_SETTING_END, 72);
			UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 64);
			x = work->option[4] + (s32)MsgAutoSleepSetting;
			UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 88, OPTION_SETTING_END, 96);
			UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 88);
			PaletteGlow_Update(*pA, *pB);

			x = icon->x * 8;
			y = (page * 3 + icon->y) * 8 + 4;
			if (page == 0)
				y += 8;
			Shop_SetCursorFar(&work->cursor.cursor, x, y, 3);
			RenderOutput_ClearList(win);
			UiText_DrawResource(page + (s32)MsgOptionHelp, win, 0, 0);
		}
		Ui_ApplyTableOffsetToPair(pair);
		WaitFrames(1);

		if (gKeyState & KEY_SELECT)
		{
			AudioCommand_PlayFar(112);
			work->preset++;
			redraw = 1;
			if (work->preset > 5)
				work->preset = 0;
			*pA = OptionMenu_FirstColorPresets[work->preset];
			*pB = OptionMenu_SecondColorPresets[work->preset];
			continue;
		}
		if (gKeyState & (KEY_A | KEY_START))
		{
			result = 0;
			AudioCommand_PlayFar(112);
			break;
		}
		if (gKeyState & KEY_B)
		{
			result = -1;
			AudioCommand_PlayFar(113);
			break;
		}
		if (gKeysRepeat & KEY_UP)
		{
			AudioCommand_PlayFar(111);
			page--;
			redraw = 1;
		}
		else if (gKeysRepeat & KEY_DOWN)
		{
			AudioCommand_PlayFar(111);
			page++;
			redraw = 1;
		}
		else
		{
			if (gKeysRepeat & KEY_LEFT)
			{
				AudioCommand_PlayFar(111);
				work->option[page]--;
				redraw = 1;
			}
			if (gKeysRepeat & KEY_RIGHT)
			{
				AudioCommand_PlayFar(111);
				work->option[page]++;
				redraw = 1;
			}
		}
	}

	UiWork_Finalize(win, 2);
	UiWork_Finalize(icon, 2);
	if (result == 0)
	{
		gGameState.palette_glow[0] = work->option[0];
		gGameState.palette_glow[1] = work->option[1];
		gGameState.message_speed = work->option[2];
		gGameState.speech = work->option[3];
		gGameState.saved_options = work->option[4];
		gOptionMirror = gGameState.saved_options;
	}
	else
	{
		PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	}
	Runtime_ScheduleCallbackAndReleaseBlock20B();
	WaitFrames(1);
	return result;
}

#define FLAG_WORKSPACE_SETTINGS_ONLY 0x17e
#define WORKSPACE_ROWS 3
#define WORKSPACE_DEBUG_ROWS 3
#define WORKSPACE_ROW_HEIGHT 24
#define WORKSPACE_TEXT_X 48

void Menu_RunSelectedWorkspaceEntry(void)
{
	struct WorkspaceWork *work = gSelectionWork;
	u32					  index;

	ShopCursor_AdvanceFar(&work->cursor.cursor);
	index = work->page;
	Ui_ApplyTableScaleToObject(work->icon[index]);
}

void Menu_InitializeSelectedWorkspace(void)
{
	void		*work;
	volatile u32 zero;
	work = Runtime_AllocateBlock(20, 0x628);
	zero = 0;
	Dma_Set(&zero, work, 0x8500018a, REG_DMA3);
	Scheduler_AddOrUpdateCallback(Menu_RunSelectedWorkspaceEntry, 3200);
}

void Runtime_ScheduleCallbackAndReleaseBlock20A(void)
{
	Scheduler_RemoveCallback(&Menu_RunSelectedWorkspaceEntry);
	Runtime_ReleaseHeapBlock(0x14);
}

struct RenderInput *Menu_CreateWorkspaceWindows(void)
{
	struct WorkspaceWork *work;
	s32					  rows;
	s32					  first;
	s32					  settings_only;
	s32					  top;
	s32					  height;
	struct RenderInput	 *win;
	s32					  y;
	s32					  slot;
	s32					  msg;
	s32					  i;
	s32					  x;

	rows = WORKSPACE_ROWS;
	work = gSelectionWork;
	settings_only = GameFlag_TestFar(FLAG_WORKSPACE_SETTINGS_ONLY);
	first = 0;
	if (settings_only != 0)
	{
		rows = 1;
		first = 2;
	}
	if (gDebugMode != 0)
		rows += WORKSPACE_DEBUG_ROWS;
	top = 8 - rows;
	height = rows * 3 + 1;
	if (top + height > 19)
	{
		top = 1;
		height = 19;
	}
	win = UiWindow_Create(5, top, 20, height, 2);
	for (i = 1; i < rows; i++)
		UiWindow_DrawDividerLine(win, 0, i * 3, 19, i * 3);
	y = 4;
	if (settings_only == 0)
	{
		msg = (s32)MsgWorkspaceSaveQuest;
		UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4);
		msg++;
		UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4 + WORKSPACE_ROW_HEIGHT);
		y = 4 + 2 * WORKSPACE_ROW_HEIGHT;
	}
	UiText_DrawResource((s32)MsgWorkspaceChangeSettings, win, WORKSPACE_TEXT_X, y);
	y += WORKSPACE_ROW_HEIGHT;
	if (gDebugMode != 0)
	{
		msg = (s32)MsgWorkspaceRoughMenu;
		UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, y);
		y += WORKSPACE_ROW_HEIGHT;
		UiText_DrawResource(msg + 1, win, WORKSPACE_TEXT_X, y);
		y += WORKSPACE_ROW_HEIGHT;
		msg += 2;
		UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, y);
	}
	slot = Resource_FindFreeEntry();
	if (slot < RESOURCE_SLOTS)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
		work->cursor.output = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
		x = win->x * 8;
		y = win->y * 8 + 16;
		ShopCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
	}
	y = -4;
	for (i = 0; i < rows; i++)
	{
		work->icon[i] =
			RenderResource_CreateFrame(Menu_WorkspaceIconFrames[first + i], 0, win, 12, y);
		y += WORKSPACE_ROW_HEIGHT;
	}
	return win;
}

s32 Menu_RunWorkspaceSelectionLoop(void)
{
	struct RenderInput	 *window;
	s32					  redraw;
	struct RenderOutput	 *pair;
	s32					  first;
	s32					  i;
	s32					  j;
	s32					  rows;
	s32					  settings_only;
	s32					  dim;
	s32					  y;
	s32					  result;
	s32					  page;
	s32					  index;
	s32					  x;
	s32					  top;
	s8					 *tbl;
	struct RenderOutput	 *icon;
	struct WorkspaceWork *work;

	redraw = 1;
	rows = 3;
	settings_only = GameFlag_TestFar(0x17E);
	first = 0;
	Menu_InitializeSelectedWorkspace();
	work = gSelectionWork;
	window = Menu_CreateWorkspaceWindows();
#if EDITION_INTERNATIONAL
	y = -0x18;
	if (gDebugMode != 0)
	{
		y = -0x10;
	}
	pair = RenderResource_CreatePair(6, window, 0x28, y);
#else
	pair = RenderResource_CreatePair(6, window, 0x28, -0x18);
#endif
	WaitFrames(1);
	page = work->page;
	if (settings_only != 0)
	{
		rows = 1;
		first = 2;
	}
	if (gDebugMode != 0)
	{
		rows += 3;
	}
	for (;;)
	{
		if (redraw != 0)
		{
			redraw = 0;
			page = (page + rows) % rows;
			work->page = page;
			i = 0;
			if (i < rows)
			{
				j = first;
				tbl = Menu_WorkspaceIconFrames;
				do
				{
					icon = work->icon[i];
					icon->sentinel = 0xFB;
					UiIcon_PrepareObjectFar(icon);
					index = (u8)icon->index;
					dim = 0;
					if (i != work->page)
					{
						dim = 1;
					}
					RenderResource_LoadFrame(*(s8 *)(j + (s32)tbl), index, dim);
					i++;
					j++;
				} while (i < rows);
			}
			top = window->y;
			x = window->x * 8;
			y = (((page * 3) + top) * 8) + 0x10;
			Shop_SetCursorFar(&work->cursor.cursor, x, y, 3);
		}
		Ui_ApplyTableOffsetToPair(pair);
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			result = page;
			AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
		}
		else if (gKeyState & (KEY_B | KEY_START))
		{
			result = -1;
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
		}
		else
		{
			if (gKeysRepeat & KEY_UP)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				page -= 1;
				redraw = 1;
			}
			else if (gKeysRepeat & KEY_DOWN)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				page += 1;
				redraw = 1;
			}
			continue;
		}
		break;
	}
	UiWork_Finalize(window, 2);
	Runtime_ScheduleCallbackAndReleaseBlock20A();
	WaitFrames(1);
	if (result >= 0)
	{
		result += first;
	}
	return result;
}
