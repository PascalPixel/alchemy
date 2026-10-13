#include "unit/window/sideobjt.h"
#include "unit/window/window.h"
#include "unit/window/icon.h"
#include "unit/window/select.h"
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

void Scheduler_ScheduleCallbackCAfterFrames(void)
{
	Scheduler_AddOrUpdateCallback((SchedulerCallback)Menu_RunSelection, 0xC80);
}

void Menu_DrawSelectionRow(struct UiWindow *work, s32 first, const s16 *second)
{
	s16 selected = first;
	s32 label = Scene_GetAreaNameIndexFar(selected, *second) + (s32)MsgDebugEntryName;
	RenderOutput_PrepareForRedraw(work);
	UiText_DrawNumber(selected, 3, (s32)work, 0, 14);
	UiText_DrawNumber(*second, 3, (s32)work, MENU_LABEL_X, 14);
	UiText_DrawString(Menu_ColonString, (s32)work, MENU_TEXT_X, 0);
	UiText_DrawResource(selected + (s32)MsgDebugCategoryName, (s32)work, 0, 0);
	UiText_DrawString(Menu_ColonString, (s32)work, MENU_TEXT_X, 14);
	UiText_DrawResource(label, (s32)work, MENU_LABEL_X, 0);
}

s16 Menu_RunSelection(void)
{
	s32				  resource;
	s16				  mode;
	s16				  secondary;
	struct MenuSprite object;
	s16				  primary;
	struct UiWindow	 *work;
	s16				  result;

	work = 0;
	mode = 0;
	primary = gGameState.default_scene;
	secondary = gGameState.default_entrance;
	work = UiWindow_Create(0, 7, 30, 5, 2);
	Menu_DrawSelectionRow(work, primary, &secondary);
	UiTextResource_Initialize(&object, &resource);

	while (gKeysHeld != 0)
		WaitFrames(1);

	for (;;)
	{
		result = Menu_HandleSelectionRowInput(work, primary, &secondary, &mode);
		if (result == -1)
		{
			UiTextResource_Release(resource);
			UiWork_Finalize(work, 2);
			Event_SetDestinationFar(primary, secondary);
			return result;
		}
		if (result == -2)
		{
			UiTextResource_Release(resource);
			UiWork_Finalize(work, 2);
			return result;
		}

		UiTextResource_SetPosition(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
		primary = result;
		WaitFrames(1);
	}
}

s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode)
{
	if (gKeysRepeat & KEY_A)
		return -1;
	if (gKeysRepeat & KEY_B)
		return -2;
	if (gKeysRepeat & KEY_DOWN)
	{
		*mode ^= 1;
	}
	else if (gKeysRepeat & KEY_UP)
	{
		*mode ^= 1;
	}
	else if (gKeysRepeat & KEY_RIGHT)
	{
		if (*mode == 0)
		{
			value++;
		}
		else
		{
			(*sub)++;
			if (*sub > 99)
				*sub = 0;
		}
		if (value > 200)
			value = 0;
		Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
	}
	else if (gKeysRepeat & KEY_LEFT)
	{
		if (*mode == 0)
		{
			value--;
		}
		else
		{
			(*sub)--;
			if (*sub < 0)
				*sub = 99;
		}
		if (value < 0)
			value = 200;
		Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
	}
	else if (gKeysRepeat & KEY_R)
	{
		if (*mode == 0)
		{
			*sub = 0;
			value += 10;
		}
		else
		{
			*sub += 10;
			if (*sub > 99)
				*sub -= 99;
		}
		if (value > 200)
			value = 0;
		Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
	}
	else if (gKeysRepeat & KEY_L)
	{
		if (*mode == 0)
		{
			*sub = 0;
			value -= 10;
		}
		else
		{
			*sub -= 10;
			if (*sub < 0)
				*sub += 99;
		}
		if (value < 0)
			value = 200;
		Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
	}
	return value;
}

//--------------------------------------------------------------------------
// run_selection_with_cursor_object.c
// メニュー：フラグの一覧画面（カーソルで選んで A で切りかえる）
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 1 ページ 16x16 のフラグを表示し、閉じるまで入力を受ける
// ページ 8 から始める。戻り値はいつも 0
//--------------------------------------------------------------------------
s32 Menu_RunSelectionWithCursorObject(void)
{
	struct MenuSprite obj;	  // カーソルのオブジェクト
	s32				  cur[2]; // カーソルの列・行
	s32				  res;
	s32				  page;
	s32				  win;
	s32				  ret;

	page = 8;
	win = 0;
	cur[0] = win;
	cur[1] = win;
	win = UiWindow_Create(1, 0, 28, 20, 2);
	Menu_DrawFlagBitTable(win, page);
	UiTextResource_Initialize(&obj, &res);
	for (;;)
	{
		WaitFrames(1);
		ret = Menu_HandleFlagGridInput(win, &page, cur);
		if (ret == -1)
			break; // 閉じる
		if (ret == 1)
			Menu_DrawFlagBitTable(win, page); // フラグかページが変わった
		UiTextResource_SetPosition(&obj, cur[0] * 8 + 58, cur[1] * 8 + 20);
	}
	UiTextResource_Release(res);
	UiWork_Finalize(win, 2);
	return 0;
}

//--------------------------------------------------------------------------
// format_hex.c
// 文字：数を 16 進の文字列にする
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// value の下から count けた（5 けたまで）を、上のけたから大文字の
// 16 進で out に書く（終わりの 0 は書かない）
//--------------------------------------------------------------------------
void Text_FormatHex(u32 value, u32 count, u8 *out)
{
	u8	buf[8];
	u32 i;

	if (count > 5)
		count = 5;

	for (i = 0; i != count; i++)
	{
		u32 d = value & 15;
		if (d <= 9)
			buf[i] = d + '0';
		else
			buf[i] = d + 'A' - 10;
		value >>= 4;
	}

	for (i = count - 1; (s32)i >= 0; i--)
		*out++ = buf[i];
}

void Menu_DrawFlagBitTable(s32 window, s32 start_flag)
{
	s32	 row;
	s32	 y;
	s32	 flag;
	char label[5];
	char bits[17];

	RenderOutput_PrepareForRedraw((struct UiWindow *)window);
	UiText_DrawStringInWindow((u8 *)Menu_HexDigitsString, window, 48, 0);

	flag = start_flag << 8;
	for (row = 0; row != 16; row++)
	{
		s32 i;

		y = row * 8 + 16;

		for (i = 0; i != 5; i++)
		{
			label[i] = 0;
		}
		Text_FormatHex(flag, 3, (s32)label);
		UiText_DrawStringInWindow((u8 *)label, window, 0, y);
		UiText_DrawStringInWindow((u8 *)Menu_ColonString, window, 32, y);

		for (i = 0; i < 16; i++)
		{
			s32 val = GameFlag_TestFar(flag);
			bits[i] = (val != 0) + 48;
			flag++;
		}
		bits[i] = 0;
		UiText_DrawStringInWindow((u8 *)bits, window, 48, y);
	}
}

s32 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor)
{
	s32 *row = &cursor[1];

	if (gKeysRepeat & KEY_A)
	{
		s32 flag = ((*page << 4) + *row << 4) + cursor[0];

		if (GameFlag_TestFar(flag))
			GameFlag_ClearBitFar(flag);
		else
			GameFlag_SetBitFar(flag);
		return 1;
	}
	if ((gKeyState & KEY_B) || (gKeysRepeat & KEY_SELECT))
		return -1;
	if (gKeysRepeat & KEY_UP)
	{
		if (--*row < 0)
			*row = 15;
	}
	else if (gKeysRepeat & KEY_DOWN)
	{
		if (++*row > 15)
			*row = 0;
	}
	else if (gKeysRepeat & KEY_LEFT)
	{
		if (--cursor[0] < 0)
			cursor[0] = 15;
	}
	else if (gKeysRepeat & KEY_RIGHT)
	{
		if (++cursor[0] > 15)
			cursor[0] = 0;
	}
	else if ((gKeysRepeat & KEY_L) && (gKeysRepeat & KEY_START))
	{
		*page -= 10;
		if (*page < 0)
			*page = 15;
		return 1;
	}
	else if ((gKeysRepeat & KEY_R) && (gKeysRepeat & KEY_START))
	{
		*page += 10;
		if (*page > 15)
			*page = 0;
		return 1;
	}
	else if (gKeysRepeat & KEY_L)
	{
		if (--*page < 0)
			*page = 15;
		return 1;
	}
	else if (gKeysRepeat & KEY_R)
	{
		if (++*page > 15)
			*page = 0;
		return 1;
	}
	return 0;
}

void Menu_ReservedNoOp(void)
{
}

void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index)
{
	GameFlag_SetBitFar((element * 0x14) + index + 0x30);
	Djinn_AddToOwnerFar(owner, element, index);
}

s32 Menu_RunWorkspaceResultLoop(void)
{
	s32 result;

retry:
	result = Menu_RunWorkspaceSelectionLoop();
	if (result == -1)
	{
		return -1;
	}
	if (result == 0)
	{
		if (Save_WriteSelectedSlot() == -1)
		{
			goto retry;
		}
	}
	else if (result == 1)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgSleepMode, 1);
		gSleepRequested = result;
	}
	else if (result == 2)
	{
		if (Menu_RunWorkspaceOptions() == -1)
		{
			goto retry;
		}
	}
	return 0;
}

s32 DebugMenu_BrowseIcons(void)
{
	s32					redraw;
	s32					tile;
	s32					page;
	s32					mode;
	struct RenderInput *window;
	s32					slot;
	s32					base;
	s32					i;
	s32					row;
	s32					x;
	s32					y;

	window = NULL;
	page = 0;
	mode = 0;
	redraw = 1;
	gMenuCtrlWork->suspended = redraw;
	WaitFrames(1);

	while (1)
	{
		if (gKeysRepeat & KEY_LEFT)
		{
			page--;
			redraw = 1;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			redraw = 1;
			page++;
		}
		if (gKeysRepeat & KEY_L)
		{
			mode--;
			redraw = 1;
		}
		if (gKeysRepeat & KEY_R)
		{
			redraw = 1;
			mode++;
		}
		if (gKeysRepeat & KEY_A)
			break;
		if (gKeysRepeat & KEY_B)
			break;
		if (redraw != 0)
		{
			redraw = 0;
			page = (page + 8) % 8;
			mode = (mode + 3) % 3;
			UiWork_Finalize(window, 2);
			window = UiWindow_Create(10, 0, 18, 12, 2);
			if (mode == 0)
				UiText_DrawStringInWindow(Workspace_ItemTitle, window, 0, 0);
			else if (mode == 1)
				UiText_DrawStringInWindow(Workspace_EnergyTitle, window, 0, 0);
			else
				UiText_DrawStringInWindow(Workspace_StatusTitle, window, 0, 0);
			UiText_DrawStringInWindow(Workspace_PageLabel, window, 0, 8);
			UiText_DrawNumberInWindow(page, 0, window, 40, 8);
			base = page << 5;
			UiText_DrawNumberInWindow(base, 3, window, 64, 8);
			UiText_DrawStringInWindow(Workspace_RangeDash, window, 88, 8);
			UiText_DrawNumberInWindow(base + 31, 3, window, 96, 8);
			for (i = 0; i <= 31; i++)
			{
				slot = -1;
				row = i / 8;
				x = (i - row * 8) * 16;
				y = row * 16 + 16;
				if (mode == 0)
				{
					UiIcon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
					RenderOutput_Create(slot, 0x40000000, window, x, y);
				}
				else if (mode == 1)
				{
					UiIcon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
					RenderOutput_Create(slot, 0x40000000, window, x, y);
				}
				else
				{
					slot = Resource_FindFreeEntry();
					Ui_BuildPatternToSlot(i, 0, slot);
					RenderOutput_Create(slot, 0x40000000, window, x, y);
				}
			}
		}
		WaitFrames(1);
	}
	UiWork_Finalize(window, 2);
	gMenuCtrlWork->suspended = 0;
	return 0;
}

#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
#define DEBUG_GLYPH_NAME ((s32)MsgDebugGlyphName)
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define DEBUG_GLYPH_NAME ((s32)MsgEnd)
#else
#define DEBUG_GLYPH_NAME ((s32)MsgDebugSecondForm)
#endif

s32 DebugMenu_BrowseEntryGlyphs(void)
{
	struct UiRenderWork *work;
	struct RenderInput	*portrait;
	s32					 redraw;
	struct RenderInput	*window;
	s32					 count;
	s32					 total;
	s32					 index;
	s32					 i;
	s32					 glyph;
	s32					 tile;
	s32					 slot;
	s32					 cancel;
	u32					 slot_offset;

	work = UI_WORK;
	redraw = 1;
	portrait = UiWindow_CreateWithSideObject(0, 0, 10, 5);
	window = UiWindow_Create(10, 10, 14, 3, 2);
	index = 0;
	for (i = 0; Portrait_CharacterMap[i * 2] != -1; i++)
	{
	}
	count = i;
	for (i = 0; Portrait_SpriteMap[i * 2] != -1; i++)
	{
	}
	total = count + i;

	/* FAKEMATCH: the count-derived mask is needed to match the native mask
     * in r9 and work in fp; a direct constant swaps them (four instructions). */
	cancel = ((u32)(u16)count >> 16) | KEY_B;
	/* FAKEMATCH: defining the offset before the loop keeps its load in r1; defining it inside redraw uses r3 (4 instructions differ in all six editions). */
	slot_offset = RENDER_SLOT_VALUE_OFS + 2;
	while (1)
	{
		if (gKeysRepeat & KEY_LEFT)
		{
			redraw = 1;
			index--;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			redraw = 1;
			index++;
		}
		if (gKeysRepeat & KEY_L)
		{
			redraw = 1;
			index -= 10;
		}
		if (gKeysRepeat & KEY_R)
		{
			redraw = 1;
			index += 10;
		}
		if (gKeysRepeat & KEY_A)
			break;
		if (gKeysRepeat & cancel)
			break;
		if (redraw)
		{
			redraw = 0;
			index = (index + total) % total;
			RenderOutput_PrepareForRedraw(window);
			if (index < count)
				glyph = Portrait_CharacterMap[index * 2 + 1];
			else
				glyph = Portrait_SpriteMap[(index - count) * 2 + 1] + 128;
			{
				/* FAKEMATCH: the word temporary is needed to match palette's
                 * r2 through the slot store; direct assignment chooses r3. */
				s32 pal;
				s32 loaded = *(u16 *)((u8 *)work + slot_offset);
				pal = 15;
				slot = loaded;
				UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, pal, 1);
			}
			UiText_DrawNumberInWindow(index, 2, window, 0, 0);
			UiText_DrawCharacterAtOffset(index + DEBUG_GLYPH_NAME, window, 24, 0);
		}
		WaitFrames(1);
	}
	UiWork_Finalize(window, 2);
	UiWork_Finalize(portrait, 2);
	WaitFrames(1);
	return 0;
}
