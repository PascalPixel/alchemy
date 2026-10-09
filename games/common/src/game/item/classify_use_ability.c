//--------------------------------------------------------------------------
// classify_use_ability.c
// アイテム：召喚えらびのメニューと、戦闘でアイテムを使えるかの判定
//--------------------------------------------------------------------------

#include "btlsys.h"
#include "item.h"
#include "types.h"
#include "scene.h"
#include "edition.h"
#include "graphics.h"

#define SENTOU_OK 0x80 // tgt_flg：戦闘で使える

#if !GAME_TLA
#include "callback_scheduler.h"
#include "resource.h"
#include "btlsmn.h"
#include "menu_list.h"
#include "io_reg.h"
#include "sound_ids.h"

#if EDITION_INTERNATIONAL
#define SUMMON_COST_X 13 // 必要ジン数の列
#else
#define SUMMON_COST_X 10
#endif

enum
{
	SUMMON_ELEMENT_TILE = 0x5001,		// 属性アイコン
	SUMMON_PAGE_TILE = 0xf301,			// ページ番号
	SUMMON_SELECTED_PAGE_TILE = 0xf30b, // 表示中のページ番号
	SUMMON_LEFT_ARROW_TILE = 0xf334,	// 左矢印
	SUMMON_RIGHT_ARROW_TILE = 0xf335	// 右矢印
};

#define SUMMON_END 32 // 召喚リストの終わり

extern u8 MsgAbilityName;
extern u8 MsgAbilityDescription;

void UiWindow_PutGlyph(struct UiWindow *, u32, u32, u32, s32);
s32	 Trade_ListFlaggedEntriesFar(u8 *ids);
struct ItemDefinition *Item_GetDirectFar(s32 item);

//--------------------------------------------------------------------------
// 召喚えらび
// 覚えた召喚を 1 ページ 4 つずつ並べ、えらんだ召喚の番号を返す
// （キャンセルは -1）
// standby : 属性ごとのスタンバイ中のジンの数
//--------------------------------------------------------------------------
s32 SummonMenu_SelectSummon(s32 unused0, s32 unused1, const u8 *standby)
{
	struct MenuSprite			   cursor;
	struct MenuSprite			   sprites[4];
	u8							   available[33]; // 覚えた召喚
	u8							   ordered[33];	  // 呼べるものを先にした並び
	u16							   text[64];
	s32							   handles[4];
	u8							   visible[4];
	s32							   glyph;
	s32							   count;
	struct UiWindow				  *window;
	struct UiRenderWork			  *render;
	s32							   drawn_row, drawn_page, row, page;
	s32							   cursor_handle;
	struct UiWindow				  *description, *counts;
	s32							   mask, preferred;
	struct MenuPoint			   cursor_position;
	const struct SummonDefinition *summon;
	struct MenuCell				  *nav;
	struct MenuSprite			  *sprite, *cur;
	s32							   used;
	s32							   id, tile, affordable;
	s32							   pressed, repeated, result;
	s32							   index;

	/* FAKEMATCH: register choice (spill slots) only: a dead store. GCSE
	   sizes its hash table from the function's length and PRE numbers its
	   reaching registers in bucket order; with this store the table has
	   379 buckets and &ordered and &sprites take their spill slots before
	   count - 1, as in the native frame (377 buckets without it: 80 rows
	   differ, every one a spill slot's offset). */
	result = 0;
	render = (struct UiRenderWork *)gWindowWork[0];
	drawn_row = -1;
	drawn_page = -1;
	cursor_handle = (s16)Resource_LoadIntoFreeSlot(128); // 16x16 スプライト 1 枚分
	description = UiWindow_Create(0, 4, 30, 4, 42);
	counts = UiWindow_Create(20, 8, 10, 3, 6);
	mask = 0;
	/* FAKEMATCH: instruction order only: the one-pass block's loop notes
	   are scheduling barriers, so counts is stored before mask and the
	   window call's stack argument before its register arguments (differ
	   10 without it, the table held at 379 buckets). */
	do
	{
		page = ((struct MenuCell *)gWindowWork[42])->page;
		row = ((struct MenuCell *)gWindowWork[42])->row;
		preferred = ((struct MenuCell *)gWindowWork[42])->preferred_row;
	} while (0);
#if EDITION_INTERNATIONAL
	window = UiWindow_Create(13, 11, 17, 9, 6);
#else
	window = UiWindow_Create(16, 11, 14, 9, 6);
#endif

	// 行ごとのアイコンのスプライト
	for (index = 0; index < 4; index++)
	{
		s32 y = index * 2;

		sprite = &sprites[index];
		/* FAKEMATCH: instruction order only: the one-pass block keeps
		   y = index * 2 first, as in the twins' setup loops (differ 4). */
		do
		{
			sprites[index].oam.word.attr01 = MENU_SPRITE_SIZE_16;
			sprites[index].oam.word.attr23 = 0;
		} while (0);
		sprite->oam.f.x = window->x * 8 + 8;
		sprite->oam.f.y = (y + window->y) * 8 + 4;
	}
	for (index = 0; index < 4; index++)
	{
		s32 slot = Resource_LoadIntoFreeSlot(128);
		handles[index] = slot;
		sprites[index].oam.f.tile = Resource_GetBuffer(slot, -1);
	}

	// 今のスタンバイ数で呼べる召喚を後ろから順に先頭へ、残りをもとの順で後ろへ
	count = Trade_ListFlaggedEntriesFar(available);
	/* FAKEMATCH: instruction order only (block placement and schedule):
	   with the first scan's exit inside this one-pass block, loop.c's
	   find_and_verify_loops finds no barrier at depth 0 and leaves the
	   main loop's select block in place, as the native has it (without
	   the block it moves after the scan: differ 224, 6 branches fewer);
	   inside it, count and used are stored before count - 1 is built
	   (differ 2 with used = 0 before the block). */
	do
	{
		s32 i;

		used = 0;
		for (i = count - 1; i >= 0; i--)
		{
			const struct SummonDefinition *s;
			s32							   n, k;

			k = available[i];
			s = SummonDefinition_Get(k);
			for (n = 0; n <= 3; n++)
			{
				if (s->djinn_required[n] > standby[n])
					break;
			}
			if (n == 4)
			{
				ordered[used++] = k;
				available[i] = SUMMON_END;
			}
		}
	} while (0);
	{
		s32 i, k;

		for (i = 0; i < count; i++)
		{
			k = available[i];
			if (k != SUMMON_END)
				ordered[used++] = k;
		}
	}
	ordered[used] = SUMMON_END;

	for (;;)
	{
		cur = &cursor;

		//------------------------------------------------------------------
		// ページか行が変わったら描きなおす
		//------------------------------------------------------------------
		if (page != drawn_page || row != drawn_row)
		{
			render->menu_busy = 1;
			Ui_SetRectHighlight(
				window->x + 1, window->y + drawn_row * 2 + 1, window->width - 2, 1, 15);
			Ui_FillVramBlockPattern();

			// 説明文と、必要なジンの属性
			{
				const struct SummonDefinition *s;
				s32							   element;

				s = SummonDefinition_Get(ordered[page + row]);
#if EDITION_INTERNATIONAL
				UiText_CopyMessageString(
					s->name_message_id + (s32)&MsgAbilityDescription, text, 52);
#else
				UiText_DecodeMessage(s->name_message_id + (s32)&MsgAbilityDescription, text, 32);
#endif
				UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0, 4);
				drawn_row = row;
				mask = 0;
				for (element = 0; element <= 3; element++)
				{
					if (s->djinn_required[element] != 0)
						mask |= 1 << element;
				}
			}

			// スタンバイ数と、ページの召喚 4 つ
			if (page != drawn_page)
			{
				RenderOutput_RedrawSavedRect(window);
				{
					s32 element;

					for (element = 0; element <= 3; element++)
					{
						UiWindow_SetTilemapEntry(
							counts, element + SUMMON_ELEMENT_TILE, element * 2, 0, 0);
						UiWindow_PutGlyph(counts, standby[element] + '0', element * 2 + 1, 0, 0);
					}
				}
				{
					s32 element, col, y;

					index = 0;
					if ((id = ordered[page + index]) != SUMMON_END)
					{
					next:
						summon = SummonDefinition_Get(id);
						col = 0;
						for (element = 0; element <= 3; element++)
						{
							if (summon->djinn_required[element] > standby[element])
								break;
						}
						affordable = element == 4;
						Ability_LoadGlyph(
							summon->name_message_id & 0x3fff, 0, &handles[index], &glyph, 1);
						y = index * 2;
						sprites[index].oam.f.tile = glyph;
						if (!affordable)
							UiWork_SetParamNibble(2); // 呼べない召喚は色 2
						UiText_DrawCharacterAtOffset(
							SummonDefinition_Get(id)->name_message_id + (s32)&MsgAbilityName,
							(struct RenderInput *)window, 16, index * 16);
						for (element = 0; element <= 3; element++)
						{
							if (summon->djinn_required[element] != 0)
							{
								UiWindow_SetTilemapEntry(window, element + SUMMON_ELEMENT_TILE,
									col * 2 + SUMMON_COST_X, y, 0);
								UiWindow_PutGlyph(window, summon->djinn_required[element] + '0',
									col * 2 + SUMMON_COST_X + 1, y, 0);
								col++;
							}
						}
						UiWork_SetParamNibble(15);
						visible[index++] = 1;
						if (index <= 3 && (id = ordered[page + index]) != SUMMON_END)
							goto next;
					}
					while (index <= 3)
						visible[index++] = 0;
				}
				drawn_page = page;
			}

			// ページ番号
			if (count > 4)
			{
				for (index = 0; index < (count + 3) / 4; index++)
				{
					tile = index + SUMMON_PAGE_TILE;
					if (index == page / 4)
						tile = index + SUMMON_SELECTED_PAGE_TILE;
					UiWindow_SetTilemapEntry(
						window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
				}
			}
			Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
			render->dirty = 1;
			render->menu_busy = 0;
		}

		//------------------------------------------------------------------
		// 毎フレームの表示
		//------------------------------------------------------------------
		for (index = 0; index <= 3; index++)
		{
			if (visible[index] != 0)
				Runtime_PushSlotEntry((s32 *)&sprites[index], 240);
		}

		// カーソルは 4 フレームごとに揺れる
		cursor_position.x = window->x * 8 - 2;
		cursor_position.y = (row * 2 + window->y) * 8 + 20;
		/* FAKEMATCH: instruction order only: as u32 words, not through the
		   union, these stores do not alias cursor_position, so cur is
		   reloaded before cursor_position.y is stored (differ 2). */
		((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
		((u32 *)cur)[2] = 0;
		cur->oam.f.tile = Resource_GetBuffer((u16)cursor_handle, (s32)Resource_FixedBlockBTiles);
		cur->oam.f.x = cursor_position.x + ((gFrameCount & 4) >> 1) - 4;
		cur->oam.f.y = cursor_position.y - ((gFrameCount & 4) >> 2) + 248;
		sprite = &cursor;
		Runtime_PushSlotEntry((s32 *)sprite, 242);

		// 必要な属性のスタンバイ数を 8 フレームごとに点滅させる
		{
			s32 element, tile, blink;

			blink = gFrameCount & 8;
			for (element = 0; element <= 3; element++)
			{
				tile = 15 - (blink != 0);
				if ((1 << element & mask) == 0)
					tile = 15;
				Ui_SetRectHighlight(counts->x + element * 2 + 1, counts->y + 1, 2, 1, tile);
			}
		}

		// ページ番号（表示中のものは点滅）と左右の矢印
		if (count > 4)
		{
			for (index = 0; index < (count + 3) / 4; index++)
			{
				tile = index + SUMMON_PAGE_TILE;
				if ((gFrameCount & 15) <= 11 && index == page / 4)
					tile = index + SUMMON_SELECTED_PAGE_TILE;
				UiWindow_SetTilemapEntry(
					window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
			}
			UiWindow_SetTilemapEntry(
				window, SUMMON_LEFT_ARROW_TILE, window->width - (count + 3) / 4 - 3, -1, 0);
			UiWindow_SetTilemapEntry(window, SUMMON_RIGHT_ARROW_TILE, window->width - 2, -1, 0);
			/* FAKEMATCH: instruction order only: the one-pass block's
			   barrier orders both arrow calls' arguments and the dirty
			   flag's loads as in the native (differ 10). */
			do
			{
				render->dirty |= 2 << ((u32)(window->y - 1) >> 2);
			} while (0);
		}

		//------------------------------------------------------------------
		// 入力
		//------------------------------------------------------------------
		nav = gLinkCountdownWork;
		/* FAKEMATCH: instruction order only: page, row and preferred are
		   stored in source order before the keys are read (differ 4). */
		do
		{
			nav->page = page;
			nav->row = row;
			nav->preferred_row = preferred;
		} while (0);
		pressed = gKeyState;
		repeated = gKeysRepeat;

		// 自動入力中は 120 フレームごとに決定する
		if (nav->auto_enabled != 0)
		{
			repeated = 0;
			pressed = 0;
			if (nav->auto_delay == 0)
			{
				nav->auto_delay = 120;
				repeated = KEY_A;
				pressed = KEY_A;
			}
			else
			{
				nav->auto_delay--;
			}
		}
		if (pressed & KEY_A)
		{
			result = ordered[page + row];
			break;
		}
		if (gLinkCountdownWork->active == 0 || (pressed & KEY_B))
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			result = -1;
			break;
		}
		if (repeated & KEY_DOWN)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			row++;
			if (row == 4 || page + row == count)
				row = 0;
			preferred = row;
		}
		else if (repeated & KEY_UP)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			row--;
			if (row < 0)
			{
				if (page == ((count - 1) / 4) * 4)
					row = count - page - 1; // 最後のページ
				else
					row = 3;
			}
			preferred = row;
		}
		else if (repeated & KEY_RIGHT)
		{
			// 次のページ（最後からは先頭へ）
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			Runtime_SetMainState19();
			if (page + 4 >= count)
			{
				if (page != 0)
				{
					page = 0;
					row = preferred;
				}
			}
			else
			{
				page += 4;
				row = preferred;
				if (page == ((count - 1) / 4) * 4)
				{
					row = count - page - 1;
					if (row > preferred)
						row = preferred;
				}
			}
		}
		else if (repeated & KEY_LEFT)
		{
			// 前のページ（先頭からは最後へ）
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			Runtime_SetMainState19();
			if (page != 0)
			{
				page -= 4;
				row = preferred;
			}
			else
			{
				page = ((count - 1) / 4) * 4;
				row = preferred;
				if (page != 0)
				{
					row = count - page - 1;
					if (row > preferred)
						row = preferred;
				}
			}
		}
		WaitFrames(1);
	}

	// 後かたづけ
	WaitFrames(1);
	for (index = 0; index < 4; index++)
		Resource_ResetEntry(handles[index]);
	Resource_ResetEntry((u16)cursor_handle);
	UiWork_Finalize(counts, 1);
	UiWork_Finalize(description, 1);
	UiWork_Finalize(window, 1);
	WaitFrames(1);
	return result;
}
#endif

//--------------------------------------------------------------------------
// 戦闘でアイテムを使えるか
// 戻り値 : 0 使える / 1 使えない / 2 戦闘では使えない
// unit   : キャラクター番号
// item   : 持ち物の値
//--------------------------------------------------------------------------
s32 Item_ClassifyUseAbility(s32 unit, s32 item)
{
	s32					   ret;
	struct ItemDefinition *def;

	if (item == 0)
	{
		return 1;
	}
#if GAME_TLA
	def = Item_Get(item);
#else
	def = Item_GetDirectFar(item);
#endif
	ret = 1;
	if (def->use_type == 3) // エナジーを授ける装備
	{
		return ret;
	}
	if (def->action_id == 0)
	{
		return ret;
	}
	if ((def->type != 0) && (Item_CanOwnerEquip(unit, item) == 0))
	{
		return ret;
	}
	if ((SENTOU_OK & BtlActGet(def->action_id)->tgt_flg) == 0)
	{
		return 2;
	}
	return 0;
}
