#include "unit/menu/menu.h"
#include "unit/menu/item.h"
#include "unit/menu/psynergy.h"
#include "unit/menu/status.h"
#include "unit/menu/djinn.h"
#include "unit/menu/fourobj.h"
#include "unit/menu/compare.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/objvram.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/basepal.h"
#include "unit/runtime/heap.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "inventory_menu.h"
#include "io_reg.h"
#include "uiwindow.h"
#include "mem.h"
#include "character_menu.h"
#include "battle_unit.h"
#include "item.h"
#include "edition.h"
#include "btlsys.h"
#include "ui.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "vram_block.h"
#include "render_input.h"
#include "fixed_math.h"
#include "lib.h"
#include "psynergy_menu.h"
#include "animspr.h"
#include "field_sprite.h"
#include "scene.h"
#include "m7_interfaces.h"
#include "global_cells.h"
#include "keystate.h"
#include "window.h"
#include "menu_result.h"
#include "tbs_edition.h"
#include "game_state.h"
#include "sound_ids.h"
#include "iwram_call.h"
#include "heap_state.h"
#include "menu_list.h"
#include "irq.h"
#include "object_factory.h"
#include "far_runtime.h"
#include "equipment_menu.h"
#include "owner_state.h"
#include "inventory.h"
#include "layout_guard.h"
#include "btltype.h"
#include "btlcalc.h"
#include "eventwrk.h"
#include "djinn_menu.h"
#include "a9_motion.h"
#include "battle_efx.h"
#include "four_object_motion.h"

struct BtlUnit;

s32 Menu_ReservedStatusZero(void)
{
	return 0;
}

s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1)
{
	return (arg0 + arg1) % arg1;
}

s32 Menu_SetFirstObjectRowCoordinates(s32 arg0)
{
	struct DjinnMenuWork *menu;
	s32					  value;
	s32					  i;
	menu = gMenuWork;
	arg0 += 0x3D;
	value = 0x20;
	for (i = 0; i < 4; i++)
	{
		menu->owner_x[i] = value;
		menu->owner_y[i] = arg0;
		value += 0x38;
	}
	return arg0;
}

s32 ItemMenu_Run(void)
{
	struct DjinnMenuWork	 *menu;
	u32						  old_mode;
	s32						  state;
	struct DjinnMenuBackdrop *saved;
	s32						  zero;

	menu = Runtime_AllocateHeapBlock(55, 0xa70);
	old_mode = gGameState.message_speed;
	gGameState.message_speed = 2;
	state = gMenuCtrlWork->suspended = 1;
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	WaitFrames(1);
	UiWindow_InitializeWork(0);
	saved = Runtime_BumpAllocateAlternatePool(0x2130);
	menu->backdrop = saved;
	saved->tutorial_timer = 0;
	saved->tutorial_step = 0;
	if (GameFlag_TestFar(0x16e))
	{
		if (!GameFlag_TestFar(0x16f))
		{
			if (!GameFlag_TestFar(0x171))
				saved->tutorial_step = state;
			else
				saved->tutorial_step = 14;
		}
		else
		{
			if (!GameFlag_TestFar(0x171))
				saved->tutorial_step = 27;
			else
				saved->tutorial_step = 28;
		}
	}
	Scheduler_EnableOverlayCallbacksWithFlags();
	UiWork_SetAltFlagAndClearTableFar(1);
	Link_DrawShiftedTilePairFar((void *)0x06002500);
	menu->owner_count = Party_ListActiveOwnersFar(menu->owners);
	Resource_LoadPairedBlocksIfAvailable();
	ItemMenu_Init(0, 3, 0, 7);
	Menu_SetFirstObjectRowCoordinates(0);
	Palette_LightenBankHighlight(14);
	menu->help_window = UiWindow_CreateFar(13, 0, 17, 5, 2);
	menu->pair_entries[0] = 255;
	zero = 0;
	menu->selected_column[0] = zero;
	menu->selected_column[1] = zero;
	menu->selection[0] = 0;
	menu->selection[1] = 0;
	FourObjectMotion_InitializeBottomRow(menu->help_window, 0);
	DjinnMenu_RunScreen();
	FourObjectMotion_ClearSlotsAndScheduleAlt();
	Menu_ResetTwoResourceEntries();
	WaitFrames(1);
	ItemMenu_Close();
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	gMenuCtrlWork->suspended = 0;
	UiWindow_MarkVisibleTileAttributesFar();
	UiWork_SetAltFlagAndClearTableFar(0);
	Iwram_CopyWords((void *)0x06004000, saved->tiles, 0x2000);
	Iwram_CopyWords((void *)0x05000080, saved->palette, 128);
	WaitFrames(1);
	Scheduler_DisableOverlayCallbacksWithFlags();
	UiWindow_EraseBorderRectFar(0, 0, 30, 20);
	MemFree(menu->backdrop);
	Runtime_ReleaseHeapBlock(55);
	Event_ClearInvalidPackedValuesFar();
	gGameState.message_speed = old_mode;
	return 1;
}

s32 DjinnMenu_RunScreen(void)
{
	struct DjinnMenuWork *menu;
	struct RenderOutput	 *cursor;
	s32					  result;
	s32					  state;
	s32					  ret;
	s32					  done;
	s32					  i;

	done = 0;
	result = 0;
	ret = 0;
	menu = gMenuWork;
	cursor = menu->cursor;
	cursor->active = 13;
	cursor->anim_step = 0;
	Menu_OpenBackdropScreen();
	WaitFrames(1);
	state = 2;
	do
	{
		switch (state)
		{
		case 0:
			if (result < 0)
			{
				ret = -1;
				done = 1;
			}
			state = 2;
			break;
		case 1:
			break;
		case 2:
			Menu_SetFirstObjectRowCoordinates(0);
			FourObjectMotion_SetSlotPosition(1, 0, 200, 0);
			result = DjinnMenu_SelectDjinn(0);
			state = 15;
			if (result == 10)
				break;
			state = 0;
			if (result < 0)
				break;
			menu->selection[1] = menu->selected_column[0];
			state = 10;
			if (result == 7)
				break;
			state = 3;
			break;
		case 10:
			menu->owner = menu->owners[menu->selected_column[0]];
			menu->shown_owner[0] = menu->owners[menu->selected_column[0]];
			result = DjinnMenu_ShowOwnerStatus();
			if (result == -2)
				done = 1;
			state = 2;
			break;
		case 15:
			result = DjinnMenu_ShowHelp();
			if (result == -2)
				done = 1;
			state = 2;
			break;
		case 8:
			state = 0;
			if (menu->pair_count == 0)
				break;
			result = DjinnMenu_SelectDjinn(1);
			if (result == -2)
				done = 1;
			state = 4;
			if (result < 0)
				break;
			state = 9;
			break;
		case 3:
			DjinnMenu_DrawElementList(&menu->backdrop->list);
			Menu_SetFirstObjectRowCoordinates(-8);
			menu->owner = menu->owners[menu->selected_column[0]];
			menu->shown_owner[0] = menu->owners[menu->selected_column[0]];
			FourObjectMotion_SetSlotPosition(0, menu->selected_column[0] * 56 + 48, 54, 0);
			result = DjinnMenu_SelectDjinn(1);
			for (i = 0; i < menu->owner_count; i++)
				menu->owner_y[i] += 8;
			if (result == -2)
				done = 1;
			if (result < 0)
			{
				state = 2;
				break;
			}
			if (result == 3 || result == 4 || result == 8 || result == 9)
				menu->shown_owner[1] = menu->owners[menu->selected_column[1]];
			if (result < 0)
				state = 2;
			else if (result == 1)
				state = 5;
			else if (result == 2)
				state = 6;
			else if (result == 3)
			{
				menu->pending = 2;
				state = 7;
			}
			else if (result == 4)
			{
				menu->pending = 2;
				state = 9;
			}
			else if (result == 5)
				state = 11;
			else if (result == 6)
				state = 12;
			else if (result == 8)
			{
				menu->pending = 2;
				state = 13;
			}
			else if (result == 9)
			{
				menu->pending = 2;
				state = 14;
			}
			break;
		case 7:
			result = OwnerAction_RunCompareLoop(1);
			if (result == -2)
				done = 1;
			state = 3;
			if (result < 0)
				break;
		case 13:
			AudioCommand_PlayFar(126);
			result = Djinn_TransferFar(menu->shown_owner[0], menu->pair_element[0],
				menu->pair_index[0], menu->shown_owner[1]);
			UnitCalcFar(menu->shown_owner[0]);
			UnitCalcFar(menu->shown_owner[1]);
			menu->cursor->active = 13;
			RenderOutput_ClearListFar(menu->list_window);
			Menu_ComputeEntryValues(menu->backdrop);
			{
				s32 column;
				s32 found;
				u8	entry;

				column = (u16)((u32)menu->selection[1] % DJINN_LIST_ROWS);
				found = 0;
				entry = menu->pair_entries[0];
				for (i = 0; i < menu->backdrop->list.counts[column]; i++)
				{
					if (entry == (u8)menu->backdrop->list.ids[column][i])
					{
						found = i;
						break;
					}
				}
				/* FAKEMATCH: needed to match: a one-pass block builds the selection address before found * 10 */
				do
				{
					menu->selection[0] = column + found * DJINN_LIST_ROWS;
				} while (0);
				menu->cursor->active = 1;
			}
			state = 0;
			break;
		case 6:
			result = OwnerAction_RunCompareLoop(2);
			if (result == -2)
				done = 1;
			state = 3;
			if (result < 0)
				break;
		case 12:
			AudioCommand_PlayFar(175);
			Djinn_DeactivateFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
			result =
				Trade_AddOfferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
			UnitCalcFar(menu->shown_owner[0]);
			menu->cursor->active = 13;
			RenderOutput_ClearListFar(menu->list_window);
			menu->cursor->active = 1;
			state = 2;
			break;
		case 9:
			result = OwnerAction_RunCompareLoop(0);
			if (result == -2)
				done = 1;
			state = 3;
			if (result < 0)
				break;
		case 14:
			AudioCommand_PlayFar(126);
			Djinn_TransferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0],
				menu->shown_owner[1]);
			result = Djinn_TransferFar(menu->shown_owner[1], menu->pair_element[1],
				menu->pair_index[1], menu->shown_owner[0]);
			UnitCalcFar(menu->shown_owner[0]);
			UnitCalcFar(menu->shown_owner[1]);
			Menu_ComputeEntryValues(menu->backdrop);
			{
				s32 column;
				s32 found;
				u8	entry;

				column = (u16)((u32)menu->selection[1] % DJINN_LIST_ROWS);
				found = 0;
				entry = menu->pair_entries[0];
				for (i = 0; i < menu->backdrop->list.counts[column]; i++)
				{
					if (entry == (u8)menu->backdrop->list.ids[column][i])
					{
						found = i;
						break;
					}
				}
				/* FAKEMATCH: needed to match: a one-pass block builds the selection address before found * 10 */
				do
				{
					menu->selection[0] = column + found * DJINN_LIST_ROWS;
				} while (0);
				menu->cursor->active = 1;
			}
			state = 2;
			break;
		case 4:
			if (result == -1)
			{
				ret = result;
				state = 2;
			}
			else if (menu->pending & 1)
				state = 8;
			else if (menu->pending & 2)
				state = 7;
			break;
		case 5:
			result = OwnerAction_RunCompareLoop(3);
			if (result == -2)
				done = 1;
			state = 3;
			if (result < 0)
				break;
		case 11:
			AudioCommand_PlayFar(139);
			Djinn_ActivateFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
			result = Trade_RemoveOfferFar(
				menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
			UnitCalcFar(menu->shown_owner[0]);
			menu->cursor->active = 13;
			RenderOutput_ClearListFar(menu->list_window);
			menu->cursor->active = 1;
			state = 2;
			break;
		default:
			done = 1;
			break;
		}
	} while (done == 0);
	return ret;
}

//--------------------------------------------------------------------------
// adjust_palette_bank.c
// メニュー：BG パレットの明るさを変えた写しを作る
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// BG パレット 15・5・7 の 16 色の RGB に d を足し（0〜31 に収める）、
// それぞれ 1 つ前のパレット 14・4・6 に書く。7 は d でなく -12 を足す
//--------------------------------------------------------------------------
void Graphics_AdjustPaletteBank(s32 d)
{
	s32 n;
	s32 bank;
	s32 col;
	s32 mask;
	s32 idx;

	bank = 15;
	n = 0;
	mask = 31;
	do
	{
		for (col = 0; col <= 15; col++)
		{
			u32 color;
			s32 b;
			s32 g;
			s32 r;
			s32 v;

			idx = bank * 16 + col;
			color = ((u16 *)0x05000000)[idx];
			b = (color >> 10) & mask;
			g = (color >> 5) & mask;
			r = color & mask;
			b += d;
			g += d;
			r += d;
			if (b > 31)
				b = 31;
			if (g > 31)
				g = 31;
			if (r > 31)
				r = 31;
			if (b < 0)
				b = 0;
			if (g < 0)
				g = 0;
			if (r < 0)
				r = 0;
			v = b << 10;
			v |= g << 5;
			v |= r;
			((s16 *)0x04FFFFE0)[idx] = v; // 16 色前（1 つ前のパレット）
		}
		if (n == 0)
		{
			bank = 5;
		}
		else
		{
			bank = 7;
			d = -12;
		}
		n++;
	} while (n <= 2);
}

#define FRAME_TILE 150

#define FLAG_FIRST 0x8000
#define FLAG_SECOND 0x4000

s32 Menu_OpenBackdropScreen(void)
{
	struct DjinnMenuWork	 *work = gMenuWork;
	struct DjinnMenuBackdrop *backdrop = work->backdrop;

	UiWindow_UpdateOrCreate(&work->list_window, 0, 5, 30, 15, 2);
	WaitFrames(1);
	Iwram_CopyWords(backdrop->tiles, BG_CHAR_BLOCK(1), 0x2000);
	Iwram_CopyWords(backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0), 128);
	Iwram_FillWords(BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
	Iwram_FillWords((void *)&BG_PLTT_COLOR(4, 0), 128, 0x55555555);
	Graphics_ExpandVramTilesByColorTableFar(BG_CHAR_BLOCK(1) + 0x1000);
	Iwram_CopyWords(
		BG_CHAR_BLOCK(1) + FRAME_TILE * TILE_SIZE_4BPP, Menu_BackdropFrameTile, TILE_SIZE_4BPP);
	Dma_Set(Runtime_GetLowTableAddress(), (void *)&BG_PLTT_COLOR(5, 0), 0x80000010, REG_DMA3);
	BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
	Dma_Set((void *)&BG_PLTT_COLOR(15, 0), (void *)&BG_PLTT_COLOR(7, 0), 0x80000010, REG_DMA3);
	Graphics_AdjustPaletteBank(8);
	BG_PLTT_COLOR(7, 4) = BG_PLTT_COLOR(15, 4);
	BG_PLTT_COLOR(6, 4) = BG_PLTT_COLOR(15, 4);
	return DjinnMenu_DrawElementList(&backdrop->list);
}

s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b, u16 *out,
	s32 *first_count, s32 *second_count)
{
	s32 i;
	s32 j;
	s32 total;
	s32 first;
	s32 second;

	total = 0;
	first = 0;
	second = 0;
	for (i = 0; i < 32 && b[i].encoded_action != 0; i++)
	{
		out[total] = b[i].encoded_action & OWNER_ACTION_ID_MASK;
		total++;
		for (j = 0; j < 32; j++)
		{
			if (((b[i].encoded_action ^ a[j].encoded_action) & OWNER_ACTION_ID_MASK) == 0)
				break;
		}
		if (j == 32)
		{
			first++;
			out[total - 1] |= FLAG_FIRST;
		}
	}
	for (i = 0; i < 32 && a[i].encoded_action != 0; i++)
	{
		for (j = 0; j < 32; j++)
		{
			if (((a[i].encoded_action ^ b[j].encoded_action) & OWNER_ACTION_ID_MASK) == 0)
				break;
		}
		if (j == 32)
		{
			second++;
			out[total] = (a[i].encoded_action & OWNER_ACTION_ID_MASK) | FLAG_SECOND;
			total++;
		}
	}
	*first_count = first;
	*second_count = second;
	return total;
}

s32 Menu_ComputeEntryValues(void *tbl)
{
	struct DjinnMenuWork *state;
	s32					  i;
	u16					 *p;
	u16					 *src;
	s8					 *dst;
	s16					  v;
	s32					  cnt;

	state = gMenuWork;
	i = 0;
	if (i < state->owner_count)
	{
		dst = ((struct DjinnListTable *)tbl)->counts;
		src = state->owners;
		p = tbl;
		do
		{
			v = Djinn_ListOwnerEntries(p, *src, -1);
			cnt = state->owner_count;
			i += 1;
			*dst = v;
			src += 1;
			dst += 1;
			p += DJINN_LIST_ROWS;
		} while (i < cnt);
	}
}

#define ELEMENT_TILE 0x5001
#define ENTRY list[i]

void DjinnMenu_DrawElementList(struct DjinnListTable *tbl)
{
	struct DjinnMenuWork *menu;
	s32					  i;
	s32					  row;
	s32					  element;
	struct UiWork		 *ui;
	s32					  line;
	s32					  usable;
	u16					 *list;

	menu = WORK_SLOT(HEAP_SLOT_MENU);
	ui = WORK_SLOT(HEAP_SLOT_WINDOW);
	ui->menu_busy = 1;
	i = 0;
	if (menu->owner_count != 0)
	{
		do
		{
			tbl->counts[i] = Djinn_ListOwnerEntries(tbl->ids[i], menu->owners[i], -1);
			i++;
		} while (i < menu->owner_count);
	}
	RenderOutput_RedrawSavedRectFar(menu->list_window);
	UiText_DrawCharacterAtOffsetFar((s32)MsgUnleashEffect, menu->list_window, 0, 80);
	for (row = 0; row < menu->owner_count; row++)
	{
		list = tbl->ids[row];
		line = 0;
		for (element = 0; element < DJINN_ELEMENTS; element++)
		{
			for (i = 0; i < tbl->counts[row]; i++)
			{
				if (element != (ENTRY & DJINN_ENTRY_ELEMENT) >> 5)
					continue;
				if ((ENTRY & DJINN_ENTRY_IS_SET) == 0)
					UiWork_SetParamNibbleFar(2);
				usable = 0;
				if (Trade_CanOfferDjinnFar((ENTRY & DJINN_ENTRY_OWNER) >> 8,
						(ENTRY & DJINN_ENTRY_ELEMENT) >> 5, ENTRY & DJINN_ENTRY_INDEX)
					|| Djinn_IsActiveFar((ENTRY & DJINN_ENTRY_OWNER) >> 8,
						(ENTRY & DJINN_ENTRY_ELEMENT) >> 5, ENTRY & DJINN_ENTRY_INDEX))
					usable = 1;
				if (!usable)
					UiWork_SetParamNibbleFar(4);
				UiWindow_SetTilemapEntryFar(menu->list_window,
					((ENTRY & DJINN_ENTRY_ELEMENT) >> 5) + ELEMENT_TILE, row * 7 + 1, line + 2, 0);
				UiText_DrawCharacterAtOffsetFar(
					(s32)MsgDjinnName + ((ENTRY & DJINN_ENTRY_ELEMENT) >> 5) * DJINN_PER_ELEMENT
						+ (ENTRY & DJINN_ENTRY_INDEX),
					menu->list_window, row * 56 + 16, line * 8 + 16);
				line++;
				UiWork_SetParamNibbleFar(15);
			}
		}
	}
	UiWindow_DrawDividerLineFar(menu->list_window, 0, 10, 28, 10);
	((struct UiWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->dirty = 1;
	ui->menu_busy = 0;
}

s32 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
	UiWindow_SetPaletteBitRectFar(((struct UiWindow *)win)->x + arg1 + 1,
		((struct UiWindow *)win)->y + arg2 + 1, arg3, arg4, arg5);
}

void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette)
{
	struct UiRenderWork *work;
	u16					*p;
	s32					 col;
	u32					 tile;
	u32					 masked;

	work = gWindowWork[0];
	palette <<= 12;
	if (x < 0)
	{
		width += x;
		x = 0;
	}
	if (x + width > 29)
		width = 30 - x;
	if (y < 0)
	{
		height += y;
		y = 0;
	}
	if (y + height > 29)
		height = 20 - y;

	if (width > 0 && height > 0)
	{
		do
		{
			p = &((u16(*)[32])work->tilemap)[y][x];
			for (col = width; col != 0; col--)
			{
				tile = *p;
				if (((tile >> 12) & 15) == 15)
				{
					masked = tile & 0xffff0fff;
					*p = masked | palette;
				}
				p++;
			}
			UiRender_MarkRow(work, y);
			height--;
			y++;
		} while (height != 0);
	}
}

void UiWindow_ApplyRectAtObjectOrigin(void *obj, s32 x, s32 y, s32 width, s32 height, s32 palette)
{
	UiWindow_SetRectPalette(((struct UiWindow *)obj)->x + x + 1,
		((struct UiWindow *)obj)->y + y + 1, width, height, palette);
}

s32 DjinnMenu_ShowHelp(void)
{
	s32					  win_b;
	s32					  win_a;
	struct DjinnMenuWork *menu;
	struct UiRenderWork	 *work;
	s32					  result;
	s32					  previous;
	s32					  selection;
	s32					  list;
	struct UiChannelSlot *channel;
	s32					  message;
	s32					  cnt;
	s32					  list_message;

	menu = WORK_SLOT(HEAP_SLOT_MENU);
	work = WORK_SLOT(HEAP_SLOT_WINDOW);
	result = 0;
	previous = 0;
	selection = 0;
	RenderOutput_ClearListFar(menu->list_window);
	WaitFrames(1);
	RenderOutput_PrepareForRedrawFar(menu->help_window);
	message = (s32)MsgDjinnInfoPrompt;
	UiText_DrawCharacterAtOffsetFar(message, menu->help_window, 0, 0);
	message++;
	UiText_DrawCharacterAtOffsetFar(message, menu->help_window, 0, 16);
	UiWindow_SetRectPalette(1, 1, 11, 3, 6);
	UiWindow_ApplyRectAtObjectOrigin((void *)menu->list_window, 0, 0, 28, 10, 6);
	list = UiWindow_CreateFar(0, 9, 8, 10, 6);
	win_b = UiWindow_CreateFar(8, 12, 22, 7, 2);
	win_a = UiWindow_CreateFar(8, 9, 22, 3, 2);
	UiWindow_MarkVisibleTileAttributesFar();
	cnt = 0;
	list_message = (s32)MsgDjinnInfoTopic;
	do
	{
		UiText_DrawCharacterAtOffsetFar(cnt + list_message, list, 0, cnt * 8);
		cnt++;
	} while (cnt <= 6);
	do
	{
		RenderOutput_PrepareForRedrawFar(win_a);
		UiText_DrawResourceFar(selection + (s32)MsgDjinnInfoTopic, win_a, 0, 0);
		channel = (struct UiChannelSlot *)UiText_OpenEntryMessageFar(
			win_b, selection + (s32)MsgDjinnInfoText);
		Menu_DrawAtWindowOffset((void *)list, 0, previous, 6, 1, 15);
		Menu_DrawAtWindowOffset((void *)list, 0, selection, 6, 1, 14);
		previous = selection;
		for (;;)
		{
			UiMenu_PositionCursor(-12, (((struct UiWindow *)list)->y + selection) * 8 + 8);
			WaitFrames(1);
			if (gKeysRepeat & (KEY_RIGHT | KEY_DOWN))
			{
				selection++;
				selection = Menu_GetModuloOfSum(selection, 7);
				AudioCommand_PlayFar(111);
				break;
			}
			if (gKeysRepeat & (KEY_LEFT | KEY_UP))
			{
				selection--;
				selection = Menu_GetModuloOfSum(selection, 7);
				AudioCommand_PlayFar(111);
				break;
			}
			else if (gKeyState & KEY_START)
			{
				AudioCommand_PlayFar(113);
				result = -2;
				break;
			}
			else if (gKeyState & (KEY_B | KEY_SELECT))
			{
				AudioCommand_PlayFar(113);
				result = -1;
				break;
			}
			else if (gKeyState & KEY_A)
			{
				if (UiWork_IsCompleteFar())
				{
					selection++;
					selection = Menu_GetModuloOfSum(selection, 7);
					AudioCommand_PlayFar(112);
					break;
				}
				AudioCommand_PlayFar(111);
			}
		}
		if (work->glyph_resource != 99)
		{
			Resource_ResetEntry(work->glyph_resource);
			work->glyph_resource = 99;
		}
		((struct UiRenderWork *)gWindowWork[0])->page_arrow_shown = 0;
		RenderOutput_PrepareForRedrawFar(win_b);
		{
			struct UiWindow *window = channel->work;

			window->frame = 0;
			window->duration = 0;
			window->state = 0;
		}
		channel->work = NULL;
	} while (result == 0);
	((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
	RenderOutput_ClearListFar(win_a);
	RenderOutput_ClearListFar(win_b);
	WaitFrames(1);
	UiWork_FinalizeFar(win_a, 1);
	UiWork_FinalizeFar(list, 1);
	UiWork_FinalizeFar(win_b, 1);
	UiWindow_MarkVisibleTileAttributesFar();
	if (result == -2)
	{
		RenderOutput_PrepareForRedrawFar(menu->help_window);
		RenderOutput_PrepareForRedrawFar(menu->list_window);
		RenderOutput_PrepareForRedrawFar(menu->status_window);
		((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
	}
	Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
	return result;
}

#define DJINN_ENTRY_SET (-0x8000)

#define DJINN_OWNER(v) (((v) & 0xf00) >> 8)
#define DJINN_ELEMENT(v) (((v) & 0xe0) >> 5)
#define DJINN_NUMBER(v) ((v) & 0x1f)
#if EDITION_INTERNATIONAL
#define CURSOR_WIDTH 6
#else
#define CURSOR_WIDTH 5
#endif


s32 DjinnMenu_SelectDjinn(s32 mode)
{
	s8						  buf[8];
	u8						  balanced[16];
	struct DjinnMenuWork	 *state = gMenuWork;
	struct DjinnMenuBackdrop *lists = state->backdrop;
	struct BtlUnit			 *owner;
	s32						  window;
	s32						  redraw = 1;
	u32						  refresh;
	s32						  i;
	s32						  j;
	s32						  sel;
	s32						  x = (u16)(state->selection[mode] % 10);
	s32						  y = (u16)(state->selection[mode] / 10);
	s32						  savedY = 0;
	u32						  djinn = 0;
	s32						  groupMode = 0;
	s32						  setAll = 0;
	s32						  result;
	s32						  message;
	u32						  pressed;
	s32						  count;
	u32						  repeat;
	struct UiWindow			 *work;
	s32						  step;
	s8						 *status;

	sel = -1;
	status = buf;
	for (i = 7; i >= 0; i--)
		status[i] = 0;
	if (mode == 0)
	{
		DjinnMenu_DrawElementList(&lists->list);
		for (i = 0; i < state->owner_count; i++)
		{
			if (lists->list.counts[i] == 0)
				status[i] = 4;
		}
		savedY = y;
		for (i = 0; i < state->owner_count; i++)
		{
			if (status[x] == 4)
			{
				x++;
				x = Menu_GetModuloOfSum(x, state->owner_count);
			}
		}
	}
	else
	{
		Djinn_MarkBalancedEntries((s8 *)balanced, state->selected_column[0]);
		for (i = 0; i < state->owner_count; i++)
		{
			if (i == state->selected_column[0])
			{
				buf[i] = 7;
			}
			else if (balanced[i] != 0)
			{
				buf[i] = 0;
			}
			else
			{
				buf[i] = 3;
				if (lists->list.counts[i] == 0)
					status[i] = buf[i] | 4;
			}
		}
	}
	if (mode == 1)
	{
		s32 fromX;
		s32 fromY;
		u16 source_cursor;
		s32 n;
		source_cursor = state->selection[0];
		fromX = (u16)((u32)source_cursor % (u32)10);
		fromY = (u16)((u32)source_cursor / (u32)10);
		Menu_DrawAtWindowOffset(state->list_window, fromX * 7 + 1, fromY + 2, CURSOR_WIDTH, 1, 14);
		UiWindow_ApplyRectAtObjectOrigin(state->list_window, fromX * 7 + 1, 2, CURSOR_WIDTH, 7, 6);
		for (n = 0; n < state->owner_count; n++)
		{
			if (n == state->selected_column[0])
			{
				if (state->pair_entries[0] & 0x8000)
					message = (s32)&MsgDjinnStandbyAction;
				else
					message = (s32)&MsgDjinnSetAction;
			}
			else if (buf[n] & 2)
			{
				message = (s32)&MsgDjinnGiveAction;
			}
			else
			{
				message = (s32)&MsgDjinnTradeAction;
			}
			UiText_DrawCharacterAtOffsetFar(message, state->list_window, n * 56 + 8, 8);
		}
	}
	RenderOutput_RedrawSavedRectFar(state->help_window);
	state->cursor->active = 1;
	for (;;)
	{
		if (redraw != 0)
		{
			refresh = redraw;
			redraw = 0;
			sel = -1;
			if (!(buf[x] & 1))
				sel = y;
			window = state->status_window;
			owner = UnitGetFar(state->owners[x]);
			RenderOutput_RedrawSavedRectFar(window);
			UiText_DrawStringAtOffsetFar(owner->name, window, 0, 0);
			UiText_DrawCharacterAtOffsetFar(owner->class_index + (s32)MsgClassName, window, 0, 8);
			UiText_DrawStringAtOffsetFar(DjinnMenu_TextLevel, window, 48, 0);
			UiText_DrawNumberInWindowFar(owner->level, 2, window, 72, 0);
			if (mode == 0)
				UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnCharacterStatus, window, 0, 16);
			if (sel != -1)
				djinn = lists->list.ids[x][sel];
			RenderOutput_RedrawSavedRectFar(state->help_window);
			if (mode == 1)
			{
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
				UiWindow_SetTilemapEntryFar(
					state->help_window, DJINN_ELEMENT(state->pair_entries[0]) + 0x5001, 0, 0, 0);
				UiWork_PushValueSlotFar(state->shown_owner[0], 1);
				UiWork_PushValueSlotFar(DJINN_ELEMENT(state->pair_entries[0]) * 20
											+ DJINN_NUMBER(state->pair_entries[0]) + 300,
					4);
				UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel, state->help_window, 8, 0);
#else
				UiWork_PushValueSlotFar(state->shown_owner[0], 1);
				UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel, state->help_window, 0, 0);
				UiWork_PushValueSlotFar(DJINN_ELEMENT(state->pair_entries[0]) * 20
											+ DJINN_NUMBER(state->pair_entries[0]) + 300,
					4);
				UiWindow_SetTilemapEntryFar(
					state->help_window, DJINN_ELEMENT(state->pair_entries[0]) + 0x5001, 6, 0, 0);
				UiText_DrawCharacterAtOffsetFar(
					(s32)&MsgDjinnOwnerLabel + 1, state->help_window, 56, 0);
#endif
				UiText_DrawCharacterAtOffsetFar(
					(s32)&MsgDjinnOwnerLabel + 2, state->help_window, 0, 8);
			}
			if (sel == -1)
			{
				FourObjectMotion_SetSlotPosition(mode, 0, 200, 0);
			}
			else if (mode == 0)
			{
				if (groupMode)
				{
					if (setAll == 0)
					{
						UiText_DrawCharacterAtOffsetFar(
							(s32)&MsgDjinnSetAll, state->help_window, 0, 0);
					}
					else
					{
						UiText_DrawCharacterAtOffsetFar(
							(s32)&MsgDjinnAllStandby, state->help_window, 0, 0);
					}
					if (Trade_CanOfferDjinnFar(
							DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
						|| Djinn_IsActiveFar(
							DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
					{
						if (Trade_CanOfferDjinnFar(
								DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
							FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
						else
							FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
						FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
					}
					else
					{
						UiText_DrawCharacterAtOffsetFar(
							(s32)&MsgDjinnRecovering, state->help_window, 0, 16);
						FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
						FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
					}
					if (refresh & 2)
						FourObjectMotion_SetSlotPhase(mode, 0);
				}
				else
				{
					UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnChoose, state->help_window, 0, 0);
					if (Trade_CanOfferDjinnFar(
							DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
						|| Djinn_IsActiveFar(
							DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
					{
						if (Trade_CanOfferDjinnFar(
								DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
						{
							UiText_DrawCharacterAtOffsetFar(
								(s32)&MsgDjinnChoose + 3, state->help_window, 0, 16);
							FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
						}
						else
						{
							UiText_DrawCharacterAtOffsetFar(
								(s32)&MsgDjinnChoose + 2, state->help_window, 0, 16);
							FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
						}
						FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
					}
					else
					{
						UiText_DrawCharacterAtOffsetFar(
							(s32)&MsgDjinnChoose + 4, state->help_window, 0, 16);
						FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
						FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
					}
					if (refresh & 2)
						FourObjectMotion_SetSlotPhase(mode, 0);
				}
			}
			else
			{
				if (Trade_CanOfferDjinnFar(
						DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
					|| Djinn_IsActiveFar(
						DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
				{
					if (Trade_CanOfferDjinnFar(
							DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
						FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
					else
						FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
					FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 54, 0);
				}
				else
				{
					FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
					FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 54, 1);
				}
				if (refresh & 2)
					FourObjectMotion_SetSlotPhase(mode, 0);
			}
			RenderOutput_ClearListFar(state->list_window);
			if (sel != -1)
			{
				UiText_DrawCharacterAtOffsetFar((s32)&MsgUnleashEffect, state->list_window, 0, 80);
#if EDITION_INTERNATIONAL
				UiWindow_ClearInteriorTilesFar(state->list_window, 0, 96, 224, 104);
				UiText_DrawCharacterAtOffsetFar(
					DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + (s32)MsgDjinnEffect,
					state->list_window, 0, 96);
#else
				UiText_DrawResourceFar(
					DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + (s32)MsgDjinnEffect,
					state->list_window, 0, 96);
#endif
			}
			if (!(buf[x] & 1))
				Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 14);
			((struct UiWork *)gWindowWork[0])->dirty = 1;
		}
		if (buf[x] & 1)
			UiMenu_PositionCursor(x * 56 - 8, 52);
		else
			UiMenu_PositionCursor(x * 56 - 8, y * 8 + 60);
		WaitFrames(1);
		if (!(gKeysHeld & KEY_R) || (gKeysPressedLatch & 0x100))
		{
			if (groupMode)
				redraw = 1;
			groupMode = 0;
			setAll = 0;
		}
		repeat = gKeysRepeat;
		pressed = gKeyState;
		step = lists->tutorial_step;
		if (step != 0)
		{
			pressed = 0;
			lists->tutorial_timer++;
			repeat = 0;
			switch (step)
			{
			case 28:
				while (!(gKeyState & KEY_A))
				{
					UiMenu_PositionCursor(150, 26);
					WaitFrames(1);
				}
				pressed = 2;
				repeat = 2;
				break;
			case 27:
				if (lists->tutorial_timer == 60)
				{
					work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialRecovery, 9, 9, 1);
					gGameState.message_speed = 1;
					while (UiWork_IsCompleteFar() == 0)
						WaitFrames(1);
					UiWork_FinalizeFar(work, 1);
					DjinnMenu_DrawElementList(&lists->list);
					WaitFrames(1);
					work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialSetAgain, 9, 9, 1);
					gGameState.message_speed = 1;
					while (UiWork_IsCompleteFar() == 0)
						WaitFrames(1);
					UiWork_FinalizeFar(work, 1);
					DjinnMenu_DrawElementList(&lists->list);
					lists->tutorial_timer = 0;
					BattlePlacement_UpdateTimedEntriesFar();
					BattlePlacement_UpdateTimedEntriesFar();
					BattlePlacement_UpdateTimedEntriesFar();
					Djinn_DeactivateFar(0, 0, 0);
					Trade_AddOfferFar(0, 0, 0);
					UnitCalcFar(0);
					repeat = 2;
					pressed = 2;
				}
				break;
			case 1:
				if (lists->tutorial_timer == 60)
				{
					work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialGive, 9, 9, 1);
					gGameState.message_speed = 1;
					while (UiWork_IsCompleteFar() == 0)
						WaitFrames(1);
					UiWork_FinalizeFar(work, 1);
					DjinnMenu_DrawElementList(&lists->list);
					lists->tutorial_timer = 0;
					lists->tutorial_step = 2;
				}
				break;
			case 2:
			case 4:
			case 8:
			case 9:
			case 22:
				if (lists->tutorial_timer == 90)
				{
					repeat = 1;
					pressed = 1;
					lists->tutorial_timer = 0;
					lists->tutorial_step++;
				}
				break;
			case 3:
				if (lists->tutorial_timer == 90)
				{
					repeat = 16;
					pressed = 16;
					lists->tutorial_timer = 0;
					lists->tutorial_step = 4;
				}
				break;
			case 6:
			case 7:
				if (lists->tutorial_timer == 60)
				{
					work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialSetAfterGive, 9, 9, 1);
					gGameState.message_speed = 1;
					while (UiWork_IsCompleteFar() == 0)
						WaitFrames(1);
					UiWork_FinalizeFar(work, 1);
					DjinnMenu_DrawElementList(&lists->list);
					lists->tutorial_timer = 0;
					lists->tutorial_step = 8;
				}
				break;
			case 13:
				if (lists->tutorial_timer == 40)
				{
					lists->tutorial_timer = 0;
					repeat = 2;
					pressed = 2;
					lists->tutorial_step++;
				}
				break;
			case 14:
			case 16:
			case 17:
				if (lists->tutorial_timer == 40)
				{
					lists->tutorial_timer = 0;
					lists->tutorial_step++;
				}
				break;
			case 15:
				if (lists->tutorial_timer == 60)
				{
					gGameState.message_speed = 1;
					work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialPowers, 9, 9, 1);
					UiMenu_SlideCursor(2, 146);
					while (UiWork_IsCompleteFar() == 0)
						WaitFrames(1);
					while (!(gKeyState & KEY_A))
					{
						UiMenu_PositionCursor(2, 146);
						WaitFrames(1);
					}
					UiWork_FinalizeFar(work, 1);
					DjinnMenu_DrawElementList(&lists->list);
					WaitFrames(1);
					work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialUnleash, 9, 9, 1);
					while (UiWork_IsCompleteFar() == 0)
						WaitFrames(1);
					while (!(gKeyState & KEY_A))
					{
						UiMenu_PositionCursor(2, 146);
						WaitFrames(1);
					}
					UiWork_FinalizeFar(work, 1);
					DjinnMenu_DrawElementList(&lists->list);
					WaitFrames(1);
					work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialDemonstrate, 9, 9, 1);
					while (UiWork_IsCompleteFar() == 0)
						WaitFrames(1);
					while (!(gKeyState & KEY_A))
					{
						UiMenu_PositionCursor(2, 146);
						WaitFrames(1);
					}
					UiWork_FinalizeFar(work, 1);
					DjinnMenu_DrawElementList(&lists->list);
					WaitFrames(1);
					lists->tutorial_timer = 0;
					lists->tutorial_step = 16;
				}
				break;
			case 18:
			case 20:
				if (lists->tutorial_timer == 90)
				{
					repeat = 1;
					pressed = 1;
					lists->tutorial_timer = 0;
					lists->tutorial_step = 21;
				}
				break;
			case 21:
				if (lists->tutorial_timer == 90)
				{
					lists->tutorial_timer = 0;
					repeat = 32;
					pressed = 32;
					lists->tutorial_step = 22;
				}
				break;
			case 24:
				if (lists->tutorial_timer == 60)
				{
					lists->tutorial_timer = 0;
					lists->tutorial_step = 25;
					repeat = 2;
					pressed = 2;
				}
				break;
			}
		}
		if (mode == 0 && (pressed & 0x100))
		{
			if (sel == -1)
			{
				AudioCommand_PlayFar(114);
				continue;
			}
			i = 0;
			if (Trade_CanOfferDjinnFar(
					DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
				|| Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
				i = 1;
			groupMode = 1;
			gKeysPressedLatch = 0;
			if (i == 0)
			{
				AudioCommand_PlayFar(114);
				RenderOutput_ClearListFar(state->list_window);
				UiWindow_ClearInteriorTilesFar(state->list_window, 0, 80, 216, 104);
				UiText_DrawResourceFar((s32)&MsgDjinnNowRecovering, state->list_window, 0, 96);
				redraw = 1;
				continue;
			}
			if (djinn & 0x8000)
			{
				AudioCommand_PlayFar(175);
				Djinn_DeactivateFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
				Trade_AddOfferFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
			}
			else
			{
				AudioCommand_PlayFar(139);
				Djinn_ActivateFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
				Trade_RemoveOfferFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
			}
			UnitCalcFar(DJINN_OWNER(djinn));
			DjinnMenu_DrawElementList(&lists->list);
			redraw = 1;
		}
		if (mode == 0 && (pressed & 0x200))
		{
			result = 7;
			AudioCommand_PlayFar(112);
			break;
		}
		if ((pressed & 1) || (mode == 1 && (pressed & 0x100)))
		{
			i = 1;
			if (!(buf[x] & 1))
			{
				if (sel == -1)
				{
					AudioCommand_PlayFar(114);
					continue;
				}
				i = 0;
				if (Trade_CanOfferDjinnFar(
						DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
					|| Djinn_IsActiveFar(
						DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
					i = 1;
			}
			if (i == 0)
			{
				AudioCommand_PlayFar(114);
				RenderOutput_ClearListFar(state->list_window);
				UiWindow_ClearInteriorTilesFar(state->list_window, 0, 80, 216, 104);
				UiText_DrawResourceFar((s32)&MsgDjinnNowRecovering, state->list_window, 0, 96);
				continue;
			}
			if (mode == 1)
			{
				if (buf[x] & 1)
				{
					if (x == state->selected_column[0])
					{
						if (state->pair_entries[0] & 0x8000)
							result = 2;
						else
							result = 1;
					}
					else
					{
						result = 3;
					}
				}
				else
				{
					result = 4;
				}
			}
			else
			{
				result = 1;
			}
			AudioCommand_PlayFar(112);
			break;
		}
		if (pressed & 8)
		{
			AudioCommand_PlayFar(113);
			result = -2;
			break;
		}
		if (pressed & 2)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		if (mode == 0 && (pressed & 4))
		{
			if (groupMode)
			{
				setAll ^= 1;
				if (setAll)
					AudioCommand_PlayFar(139);
				else
					AudioCommand_PlayFar(175);
				for (i = 0; i < state->owner_count; i++)
				{
					for (j = 0; j < lists->list.counts[i]; j++)
					{
						u32 entry = lists->list.ids[i][j];
						s32 usable;
						usable = 0;
						if (Trade_CanOfferDjinnFar(
								DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry))
							|| Djinn_IsActiveFar(
								DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry)))
							usable = 1;
						if (usable)
						{
							if (setAll)
							{
								if (!(entry & 0x8000))
								{
									Djinn_ActivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
										DJINN_NUMBER(entry));
									Trade_RemoveOfferFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
										DJINN_NUMBER(entry));
									UnitCalcFar(DJINN_OWNER(entry));
								}
							}
							else if (entry & 0x8000)
							{
								Djinn_DeactivateFar(
									DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
								Trade_AddOfferFar(
									DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
								UnitCalcFar(DJINN_OWNER(entry));
							}
						}
					}
				}
				DjinnMenu_DrawElementList(&lists->list);
				redraw = 1;
			}
			else
			{
				result = 10;
				Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
				AudioCommand_PlayFar(112);
				break;
			}
		}
		if (repeat & 0x40)
		{
			AudioCommand_PlayFar(111);
			if (buf[x] & 4)
				continue;
			Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
			if (buf[x] & 4)
				continue;
			if (buf[x] & 1)
			{
				buf[x] &= ~1;
				y = 0;
			}
			else if (y == 0 && (buf[x] & 2))
			{
				buf[x] |= 1;
				redraw = 2;
				continue;
			}
			y--;
			count = lists->list.counts[x];
			if (count == 0)
				count = 1;
			y = Menu_GetModuloOfSum(y, count);
			savedY = y;
			redraw = 2;
			continue;
		}
		if (repeat & 0x80)
		{
			AudioCommand_PlayFar(111);
			if (buf[x] & 4)
				continue;
			Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
			y++;
			count = lists->list.counts[x];
			if (count == 0)
				count = 1;
			y = Menu_GetModuloOfSum(y, count);
			if ((buf[x] & 1) && !(buf[x] & 4))
			{
				buf[x] &= ~1;
				y = 0;
			}
			else if (y == 0 && (buf[x] & 2))
			{
				buf[x] |= 1;
			}
			savedY = y;
			redraw = 2;
			continue;
		}
		if (repeat & 0x20)
		{
			AudioCommand_PlayFar(111);
			if (!(buf[x] & 4))
				Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
			x--;
			x = Menu_GetModuloOfSum(x, state->owner_count);
			if (mode == 0)
			{
				for (i = 0; i < state->owner_count; i++)
				{
					if (buf[x] == 4)
					{
						x--;
						x = Menu_GetModuloOfSum(x, state->owner_count);
					}
				}
			}
			y = savedY;
			count = lists->list.counts[x];
			if (count == 0)
				count = 1;
			y = Menu_GetModuloOfSum(y, count);
			redraw = 2;
			continue;
		}
		if (repeat & 0x10)
		{
			AudioCommand_PlayFar(111);
			if (!(buf[x] & 4))
				Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
			x++;
			x = Menu_GetModuloOfSum(x, state->owner_count);
			if (mode == 0)
			{
				for (i = 0; i < state->owner_count; i++)
				{
					if (buf[x] == 4)
					{
						x++;
						x = Menu_GetModuloOfSum(x, state->owner_count);
					}
				}
			}
			y = savedY;
			count = lists->list.counts[x];
			if (count == 0)
				count = 1;
			y = Menu_GetModuloOfSum(y, count);
			redraw = 2;
		}
	}
	state->selected_column[mode] = x;
	if (sel != -1)
	{
		u32 entry;
		state->pair_entries[mode] = entry = lists->list.ids[x][sel];
		state->pair_index[mode] = entry % 32;
		state->pair_element[mode] = DJINN_ELEMENT(entry);
		state->pair_owner[mode] = DJINN_OWNER(entry);
		/* FAKEMATCH: keep entry live through owner extraction and pass result through the boundary; an input alone exchanges the final index shift and table-base copy (2 instructions differ), while no boundary ties the owner mask to entry (8 differ). 2026-10-11: plain, entry dies at the owner and, so reload ties the result to entry instead of the mask (ands r2, r3 for ands r3, r2: differ 8); an input-only barrier leaves a sched2 tie (differ 2); storing the fields in other orders differs 13-1106. */
		asm("" : "+r"(result) : "r"(entry));
	}
	state->selection[mode] = x + y * 10;
	return result;
}

s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element)
{
	struct BtlUnit *state = UnitGetFar(owner);
	s32				count = 0;
	s32				row;
	s32				bit;
	s32				entry;

	if (element == -1)
	{
		for (row = 0; row < 4; row++)
		{
			for (bit = 0; bit < 20; bit++)
			{
				if (state->djinn_active[row] & (1 << bit))
				{
					entry = (row << 5) | bit | DJINN_ENTRY_SET;
					entry |= owner << 8;
					out[count++] = entry;
				}
				else if (state->djinn_available[row] & (1 << bit))
				{
					out[count++] = (row << 5) | bit | (owner << 8);
				}
			}
		}
	}
	else
	{
		for (bit = 0; bit < 20; bit++)
		{
			if (state->djinn_active[element] & (1 << bit))
				out[count++] = (element << 5) | bit | DJINN_ENTRY_SET;
			else if (state->djinn_available[element] & (1 << bit))
				out[count++] = (element << 5) | bit;
		}
	}
	return count;
}

#if EDITION_INTERNATIONAL
#define ALT_PARAM 2
#else
#define ALT_PARAM 4
#endif

s32 Menu_RunPairedEntryAction(s32 mode, s32 param)
{
	s32					  sp14;
	struct DjinnMenuWork *menu;

	menu = gMenuWork;
	if (mode == 0)
	{
		sp14 = mode;
		DjinnMenu_DrawStatPreview(
			menu->second_window, 0, 0, menu->pair_owner[1], 1, mode, 2, param, 1);
		DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pair_owner[0], mode, 1, 2, param, mode);
	}
	else
	{
		DjinnMenu_DrawStatPreview(
			menu->second_window, 0, 0, menu->shown_owner[1], 1, 0, ALT_PARAM, param, 1);
		DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shown_owner[0], 0, 0, 1, param, 0);
	}
	return 1;
}

#if EDITION_INTERNATIONAL
#define PREVIEW_DIGITS 3
#define PREVIEW_CURRENT_X 48
#define PREVIEW_MAX_X 80
#define PREVIEW_SLASH_X 72
#define PREVIEW_LUCK_X 56
#define PREVIEW_ARROW_X icon_x
#define PREVIEW_PP_X 88
#else
#define PREVIEW_DIGITS 4
#define PREVIEW_CURRENT_X 32
#define PREVIEW_MAX_X 72
#define PREVIEW_SLASH_X 64
#define PREVIEW_LUCK_X 48
#define PREVIEW_ARROW_X (x * 8 + 64)
#define PREVIEW_PP_X 56
#endif
static __inline__ void Unit_Copy(struct BtlUnit *dst, struct BtlUnit *src)
{
	Iwram_CopyWords(dst, src, sizeof(struct BtlUnit));
}

s32 DjinnMenu_DrawStatPreview(
	s32 window, s32 x, s32 y, s32 owner, s32 give, s32 take, s32 mode, s32 page, s32 side)
{
	struct BtlUnit		 *state;
	struct BtlUnit		 *saved;
	s32					  give_element;
	s32					  give_index;
	s32					  give_set;
	s32					  take_element;
	s32					  take_index;
	s32					  take_set;
	s32					  lost;
	s32					  gained;
	u16					  actions[48];
	struct DjinnMenuWork *menu;

	state = UnitGetFar(owner);
	menu = gMenuWork;
	give_element = menu->pair_element[give];
	give_index = menu->pair_index[give];
	give_set = menu->pair_entries[give] & 0x8000;
	take_element = menu->pair_element[take];
	take_index = menu->pair_index[take];
	take_set = menu->pair_entries[take] & 0x8000;
	saved = MemAlloc(sizeof(struct BtlUnit));
	Unit_Copy(saved, state);

	if (page == 0)
	{
		if (mode == 3)
		{
			UiText_DrawNumberInWindowFar(
				state->max_hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_MAX_X, y * 8 + 56);
			UiText_DrawNumberInWindowFar(
				state->max_ep, PREVIEW_DIGITS, window, x * 8 + PREVIEW_MAX_X, y * 8 + 64);
			UiText_DrawNumberInWindowFar(
				state->hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
			UiText_DrawNumberInWindowFar(
				state->ep, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
			UiText_DrawStringAtOffsetFar(
				DjinnMenu_TextSlash, window, x * 8 + PREVIEW_SLASH_X, y * 8 + 56);
			UiText_DrawStringAtOffsetFar(
				DjinnMenu_TextSlash, window, x * 8 + PREVIEW_SLASH_X, y * 8 + 64);
		}
		else
		{
			UiText_DrawNumberInWindowFar(
				state->hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
			UiText_DrawNumberInWindowFar(
				state->ep, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
		}
		UiText_DrawNumberInWindowFar(
			state->atk, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 72);
		UiText_DrawNumberInWindowFar(
			state->def, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 80);
		UiText_DrawNumberInWindowFar(
			state->agi, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 88);
		UiText_DrawNumberInWindowFar(state->luck, 2, window, x * 8 + PREVIEW_LUCK_X, y * 8 + 96);
	}

	switch (mode)
	{
	case 0:
		Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
		Djinn_ActivateFar(owner, take_element, take_index & 31);
		break;
	case 1:
		give_index &= 31;
		Djinn_DeactivateFar(owner, give_element, give_index);
		break;
	case 2:
		if (give_set)
		{
			give_index &= 31;
			Djinn_DeactivateFar(owner, give_element, give_index);
		}
		Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
		if (take_set)
			Djinn_ActivateFar(owner, take_element, take_index & 31);
		break;
	case 4:
		Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
		if (take_set)
			Djinn_ActivateFar(owner, take_element, take_index & 31);
		break;
	}

	UnitCalcFar(owner);
	state = UnitGetFar(owner);

	if (page == 0)
	{
		s32 label;

		UiText_DrawStringAtOffsetFar(state->name, window, x * 8 + 40, y * 8);
		UiText_DrawStringAtOffsetFar(DjinnMenu_TextLevel, window, x * 8 + 40, y * 8 + 16);
		UiText_DrawNumberInWindowFar(state->level, 2, window, x * 8 + 88, y * 8 + 16);
		label = (s32)MsgPreviewStatLabel;
		UiText_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
		UiText_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
		UiText_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
		UiText_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
		UiText_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
		UiText_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
		UiText_DrawCharacterAtOffsetFar(
			(s32)MsgClassName + saved->class_index, window, x * 8, y * 8 + 32);
	}

	{
		s32 i = 0;

		if (page == 0)
		{
			s32 column;
#if EDITION_INTERNATIONAL
			s32 icon_x;
#endif
			s32 px;

			if (saved->class_index != state->class_index)
			{
				px = x * 8;
				UiText_DrawCharacterAtOffsetFar(
					(s32)MsgClassName + state->class_index, window, px, y * 8 + 48);
				UiWindow_SetTilemapEntryFar(window, 0xf296, x + 2, 5, 0);
			}
			column = x;
			if (saved->class_index != state->class_index)
				column += 5;
			for (i = 0; i < 4; i++)
			{
				UiWindow_SetTilemapEntryFar(window, 0x5001 + i, column + i * 2, y + 5, 0);
				UiWindow_SetTilemapEntryFar(
					window, state->djinn_active_counts[i] + 0xf030, column + i * 2 + 1, y + 5, 0);
			}
#if EDITION_INTERNATIONAL
			icon_x = x * 8 + 70;
#endif
			if (state->hp != saved->hp)
			{
				UiText_DrawNumberInWindowFar(state->hp, 4, window, x * 8 + 72, y * 8 + 56);
				if (state->hp > saved->hp)
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 56, 0);
				else
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 56, 1);
			}
			if (state->ep != saved->ep)
			{
				UiText_DrawNumberInWindowFar(state->ep, 4, window, x * 8 + 72, y * 8 + 64);
				if (state->ep > saved->ep)
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 64, 0);
				else
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 64, 1);
			}
			if (state->atk != saved->atk)
			{
				UiText_DrawNumberInWindowFar(state->atk, 4, window, x * 8 + 72, y * 8 + 72);
				if (state->atk > saved->atk)
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 72, 0);
				else
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 72, 1);
			}
			if (state->def != saved->def)
			{
				UiText_DrawNumberInWindowFar(state->def, 4, window, x * 8 + 72, y * 8 + 80);
				if (state->def > saved->def)
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 80, 0);
				else
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 80, 1);
			}
			if (state->agi != saved->agi)
			{
				UiText_DrawNumberInWindowFar(state->agi, 4, window, x * 8 + 72, y * 8 + 88);
				if (state->agi > saved->agi)
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 88, 0);
				else
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 88, 1);
			}
			if (state->luck != saved->luck)
			{
				UiText_DrawNumberInWindowFar(state->luck, 2, window, x * 8 + 88, y * 8 + 96);
				if (state->luck > saved->luck)
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 96, 0);
				else
					UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 96, 1);
			}
		}
	}

	if (page > 0)
	{
		s32 rows;
		s32 first;
		s8	count;
		s8	row;
		s8	line;

		rows = 6 - (mode != 3);
		first = rows * (page - 1);
		count = OwnerAction_DiffSlots(
			saved->action_slots, state->action_slots, actions, &gained, &lost);
		line = 0;
		for (row = 0; first < count && row < rows; row++, first++)
		{
			UiIcon_CreateWithLoadedResource(
				window, x * 8, (y + line * 2) * 8 + 4, actions[first] & OWNER_ACTION_ID_MASK);
			if (actions[first] & 0x8000)
				UiWork_SetParamNibbleFar(4);
			else if (actions[first] & 0x4000)
				UiWork_SetParamNibbleFar(2);
			else
				UiWork_SetParamNibbleFar(15);
			UiText_DrawCharacterAtOffsetFar(
				(s32)MsgAbilityName + (actions[first] & OWNER_ACTION_ID_MASK), window, x * 8 + 16,
				(y + line * 2) * 8 + 8);
			UiText_DrawNumberAtOffsetFar(
				BtlActGet(actions[first])->pp_cost, 2, window, x * 8 + 88, (y + line * 2) * 8 + 8);
			line++;
		}
		UiWork_SetParamNibbleFar(15);
		UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, x * 8 + PREVIEW_PP_X, y * 8);
		if (mode != 3)
		{
			s32 lines;

			lines = 0;
			if (gained)
			{
				UiWork_SetParamNibbleFar(4);
				UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyGained, window, x * 8, y * 8 + 88);
				lines = 1;
			}
			if (lost)
			{
				UiWork_SetParamNibbleFar(2);
				UiText_DrawCharacterAtOffsetFar(
					(s32)MsgPsynergyLost, window, x * 8, (y + lines) * 8 + 88);
				lines++;
			}
			if (lines == 0)
				UiText_DrawCharacterAtOffsetFar(
					(s32)MsgPsynergyUnchanged, window, x * 8, y * 8 + 88);
			UiWork_SetParamNibbleFar(15);
			UiWindow_DrawDividerLineFar(window, 0, 11, 13, 11);
		}
		((struct UiWork *)gWindowWork[0])->dirty = 1;
	}

	if (page == 0)
		SideObject_CreateFar(owner, 0, side, window, 0, 0);
	Unit_Copy(state, saved);
	MemFree(saved);
	return 1;
}
