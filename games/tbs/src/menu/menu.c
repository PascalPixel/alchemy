#include "unit/menu/menu.h"
#include "unit/runtime/runtime.h"
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
#include "battle_system.h"
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
#include "battle_type.h"
#include "battle_calc.h"
#include "event_work.h"
#include "djinn_menu.h"
#include "a9_motion.h"
#include "battle_effect.h"
#include "four_object_motion.h"

extern u8 IwramCopyWords[];

s32 _call_via_r3(void *, void *, s32, void *);

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
	Scheduler_DisableOverlayCallbacks();
	Map_DisableUpdateCallbackFar();
	GameFlag_ClearBitFar(0x166);
	GameFlag_ClearBitFar(0x152);
}

s32 Scheduler_EnableOverlayCallbacksWithFlags(void)
{
	GameFlag_SetBitFar(0x152);
	GameFlag_SetBitFar(0x166);
	Map_EnableUpdateCallbackFar();
	return Scheduler_EnableUnmaskedOverlayCallbacks();
}

void UiWindow_InitializeWork(s32 unused)
{
	struct InventoryMenuState *work = gMenuWork;

	Dma_Fill32(0, work, 0x29c * 4);
	work->pane_index[0] = -1;
	work->pane_count[0] = 1;
	work->pane_count[1] = 1;
	work->column_count = 1;
	work->row_count = 1;
}

//--------------------------------------------------------------------------
// lifecycle.c
// ウィンドウ：ハンドルで持つウィンドウを開く・閉じる
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// *handle のウィンドウがなければ作って 1 を返す
// すでにあれば 0 を返し、flags に 0x100 がなければ保存した矩形で描きなおす
// flags : 下位 8 ビットがウィンドウの種類
//--------------------------------------------------------------------------
s32 UiWindow_UpdateOrCreate(s32 *handle, s32 x, s32 y, s32 w, s32 h, s32 flags)
{
	s32 cur;
	s32 style;

	cur = *handle;
	if (cur != 0)
	{
		if (0x100 & flags)
		{
			return 0;
		}
		RenderOutput_RedrawSavedRectFar((struct UiWindow *)cur);
		return 0;
	}
	style = flags & 0xff;
	*handle = UiWindow_CreateFar(x, y, w, h, style);
	return 1;
}

//--------------------------------------------------------------------------
// *handle のウィンドウがあれば閉じて、ハンドルを NULL にする
// style はそのまま UiWork_FinalizeFar に渡る
//--------------------------------------------------------------------------
void UiWindow_CloseIfOpen(struct UiWindow **handle, s32 style)
{
	if (*handle != NULL)
	{
		UiWork_FinalizeFar(*handle, style);
		*handle = NULL;
	}
}

#if EDITION_INTERNATIONAL
#define STATUS_X 32
#else
#define STATUS_X 40
#endif

static inline void Owner_Copy(void *dst, void *src)
{
	_call_via_r3(dst, src, 0x14c, IwramCopyWords);
}

void Menu_DrawOwnerStatusPanel(s32 unused, s32 owner, s32 slot, s32 mode)
{
	s32						   created;
	struct ItemDefinition	  *def;
	u32						   item;
	struct InventoryMenuState *state;
	struct BtlUnit			  *unit;
	s32						   window;
	s32						   cnt;
	s32						   value;
	u8						   avail[8];

	created = 0;
	state = gMenuWork;
	unit = UnitGetFar(owner);
	item = unit->inventory[slot];
	def = Item_GetDirectFar(item & ITEM_ID_MASK);
	if (!(mode & 0x100))
		created = UiWindow_UpdateOrCreate((s32 *)&state->status_window, 0, 5, 13, 12, 258);
	window = (s32)state->status_window;
	if (!(mode & 0x100))
	{
		if (!created)
		{
			WaitFrames(1);
			UiWindow_ClearInteriorTilesFar((void *)state->status_window, 0, 0, 88, 32);
		}
		UiText_DrawStringAtOffsetFar(unit->name, window, STATUS_X, 0);
		CharacterMenu_BuildAvailability(avail, 1, owner);
		cnt = 0;
		if (avail[CHARACTER_POISON])
		{
			UiText_DrawCharacterAtOffsetFar(MsgPoisonLabel, window, STATUS_X, cnt * 8 + 8);
			cnt++;
		}
		if (avail[CHARACTER_VENOM])
		{
			UiText_DrawCharacterAtOffsetFar(MsgVenomLabel, window, STATUS_X, cnt * 8 + 8);
			cnt++;
		}
		if (avail[CHARACTER_CURSE])
		{
			UiText_DrawCharacterAtOffsetFar(MsgCurseLabel, window, STATUS_X, cnt * 8 + 8);
			cnt++;
		}
		if (avail[CHARACTER_HAUNT])
		{
			UiText_DrawCharacterAtOffsetFar(MsgHauntLabel, window, STATUS_X, cnt * 8 + 8);
			cnt++;
		}
		if (cnt < 2)
		{
			value = unit->level;
#if EDITION_INTERNATIONAL
			UiText_DrawStringAtOffsetFar(Ui_LvString, window, 40, 16);
#else
			UiText_DrawStringInWindowFar(Ui_LvString, window, 40, 16);
#endif
			UiText_DrawNumberInWindowFar(value, 4, window, 56, 16);
		}
	}
	if (!created)
	{
		WaitFrames(1);
		UiWindow_ClearInteriorTilesFar((void *)state->status_window, 0, 32, 88, 80);
	}
	RenderOutput_ClearListFar((void *)window);
	if (!(mode & 0x100))
		state->cursor = SideObject_CreateFar(owner, 0, 0, window, 0, 0);
	switch (mode & 0xff)
	{
	case 0:
		value = unit->class_index + MsgClassName;
		UiText_DrawCharacterAtOffsetFar(value, window, 0, 32);
		Ui_DrawValuePairRows(unit, window);
		value = unit->experience;
		UiText_DrawCharacterAtOffsetFar(MsgExpLabel, window, 0, 64);
		UiText_DrawNumberInWindowFar(value, 8, window, 24, 72);
		break;
	case 6:
		value = unit->class_index + MsgClassName;
		UiText_DrawCharacterAtOffsetFar(value, window, 0, 32);
		Ui_DrawValuePairRows(unit, window);
		break;
	case 2:
	case 3:
	{
		void *backup;

		if (!Item_CanOwnerEquip(owner, item))
		{
#if EDITION_INTERNATIONAL
			UiText_DrawCharacterAtOffsetFar(MsgCannotEquip, window, 0, 48);
#else
			UiText_DrawCharacterAtOffsetFar(MsgCannotEquip, window, 16, 48);
#endif
			break;
		}
		backup = MemAlloc(0x14c);
		Owner_Copy(backup, unit);
		if (state->equip_preview)
			unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
		else
			Inventory_EquipFar(owner, slot);
		UnitCalcFar(owner);
		UiText_DrawStatComparison(unit, backup, window);
		Owner_Copy(unit, backup);
		MemFree(backup);
		break;
	}
	case 4:
	{
		s32 id = def->action_id;
		s32 found = 0;
		s32 i;

		for (i = 0; i < 32; i++)
		{
			if ((unit->action_slots[i].encoded_action & OWNER_ACTION_ID_MASK) == id)
			{
				found = 1;
				break;
			}
		}
#if defined(TBS_EDITION_ES)
		if (found)
		{
			UiText_DrawCharacterAtOffsetFar(id + MsgAbilityName, window, 0, 56);
			UiText_DrawCharacterAtOffsetFar(MsgLearned, window, 0, 48);
		}
		else
		{
			UiText_DrawCharacterAtOffsetFar(id + MsgAbilityName, window, 0, 56);
			UiText_DrawCharacterAtOffsetFar(MsgWillLearn, window, 0, 48);
		}
#else
		if (found)
		{
			UiText_DrawCharacterAtOffsetFar(id + MsgAbilityName, window, 0, 48);
			UiText_DrawCharacterAtOffsetFar(MsgLearned, window, 0, 56);
		}
		else
		{
			UiText_DrawCharacterAtOffsetFar(id + MsgAbilityName, window, 0, 48);
			UiText_DrawCharacterAtOffsetFar(MsgWillLearn, window, 0, 56);
		}
#endif
		break;
	}
	case 8:
	{
		s32 base = MsgPanelStatLabel;

		UiText_DrawCharacterAtOffsetFar(base, window, 0, 40);
		value = unit->atk;
		UiText_DrawNumberInWindowFar(value, 3, window, 64, 40);
		UiText_DrawCharacterAtOffsetFar(base + 1, window, 0, 48);
		value = unit->def;
		UiText_DrawNumberInWindowFar(value, 3, window, 64, 48);
		UiText_DrawCharacterAtOffsetFar(base + 4, window, 0, 56);
		value = unit->agi;
		UiText_DrawNumberInWindowFar(value, 3, window, 64, 56);
		UiText_DrawCharacterAtOffsetFar(base + 3, window, 0, 64);
		value = unit->luck;
		UiText_DrawNumberInWindowFar(value, 3, window, 64, 64);
		break;
	}
	}
}

#if EDITION_INTERNATIONAL
//--------------------------------------------------------------------------
// num のけた数を数え、右はしが x になるように 1 けた 8 ドットで書く
// （1 けたの数も 2 けた分の幅をとる）
// win  : 書き込み先のウィンドウ
//--------------------------------------------------------------------------
void UiText_DrawNumberRightAlignedFar(s32 num, s32 win, s32 x, s32 y)
{
	s32 v = num;
	s32 digits = 1;

	while (digits <= 15)
	{
		v = v / 10;
		if (v <= 9)
		{
			break;
		}
		digits++;
	}

	digits++;
	x -= digits * 8;
	UiText_DrawNumberAtOffsetFar(num, digits, win, x, y);
}

#endif
//--------------------------------------------------------------------------
// ウィンドウの y 40 に HP、y 48 に PP を書く
// HP は最大の 1/4 未満なら色 4、0 なら色 2 で書く
// unit : キャラクター
// win  : 書き込み先のウィンドウ
//--------------------------------------------------------------------------
void Ui_DrawValuePairRows(struct BtlUnit *unit, s32 win)
{
	s16 val;

#if EDITION_INTERNATIONAL
	UiText_DrawStringAtOffsetFar(Ui_HpString, win, 0, 40);
	UiText_DrawStringInWindowFar((u8 *)Ui_SlashString, win, 48, 40);
	val = unit->max_hp;
	UiText_DrawNumberRightAlignedFar(val, win, 88, 40);
	val = unit->hp;
	if (val < ((u16)unit->max_hp << 16) >> 18)
	{
		UiWork_SetParamNibbleFar(4);
	}
	if (val == 0)
	{
		UiWork_SetParamNibbleFar(2);
	}
	UiText_DrawNumberRightAlignedFar(val, win, 48, 40);
	UiWork_SetParamNibbleFar(15);
	UiText_DrawStringAtOffsetFar(Ui_PpString, win, 0, 48);
	UiText_DrawStringInWindowFar((u8 *)Ui_SlashString, win, 48, 48);
	val = unit->pp;
	UiText_DrawNumberRightAlignedFar(val, win, 48, 48);
	val = unit->max_pp;
	UiText_DrawNumberRightAlignedFar(val, win, 88, 48);
#else
	UiText_DrawStringInWindowFar((u8 *)Ui_HpRowString, win, 0, 40);
	val = unit->max_hp;
	UiText_DrawNumberInWindowFar(val, 4, win, 56, 40);
	val = unit->hp;
	if (val < ((u16)unit->max_hp << 16) >> 18)
	{
		UiWork_SetParamNibbleFar(4);
	}
	if (val == 0)
	{
		UiWork_SetParamNibbleFar(2);
	}
	UiText_DrawNumberAtOffsetFar(val, 4, win, 16, 40);
	UiWork_SetParamNibbleFar(15);
	UiText_DrawStringInWindowFar((u8 *)Ui_PpRowString, win, 0, 48);
	val = unit->pp;
	UiText_DrawNumberAtOffsetFar(val, 4, win, 16, 48);
	val = unit->max_pp;
	UiText_DrawNumberInWindowFar(val, 4, win, 56, 48);
#endif
}
void UiText_DrawStatComparison(const struct BtlUnit *alt, const struct BtlUnit *base, s32 work)
{
	UiText_DrawCharacterAtOffsetFar(MsgPanelStatLabel, work, 0, 32);
	UiText_DrawNumberAtOffsetFar(base->atk, 3, work, 16, 40);
	if (alt->atk != base->atk)
	{
		UiText_DrawNumberAtOffsetFar(alt->atk, 3, work, 64, 40);
		if (alt->atk > base->atk)
		{
			UiIcon_CreateStatChangeArrow((struct RenderInput *)work, 44, 36, 0);
		}
		else
		{
			UiIcon_CreateStatChangeArrow((struct RenderInput *)work, 44, 36, 1);
		}
	}
	UiText_DrawCharacterAtOffsetFar(MsgPanelDefenseLabel, work, 0, 48);
	UiText_DrawNumberAtOffsetFar(base->def, 3, work, 16, 56);
	if (alt->def != base->def)
	{
		UiText_DrawNumberAtOffsetFar(alt->def, 3, work, 64, 56);
		if (alt->def > base->def)
		{
			UiIcon_CreateStatChangeArrow((struct RenderInput *)work, 44, 52, 0);
		}
		else
		{
			UiIcon_CreateStatChangeArrow((struct RenderInput *)work, 44, 52, 1);
		}
	}
	UiText_DrawCharacterAtOffsetFar(MsgAgilityLabel, work, 0, 64);
	UiText_DrawNumberAtOffsetFar(base->agi, 3, work, 16, 72);
	if (alt->agi != base->agi)
	{
		UiText_DrawNumberAtOffsetFar(alt->agi, 3, work, 64, 72);
		if (alt->agi > base->agi)
		{
			UiIcon_CreateStatChangeArrow((struct RenderInput *)work, 44, 68, 0);
		}
		else
		{
			UiIcon_CreateStatChangeArrow((struct RenderInput *)work, 44, 68, 1);
		}
	}
}

s32 UiIcon_CreateWithResource(struct RenderInput *window, s32 unused, s32 x, s32 y)
{
	s32 entry_no;
	s32 result;
	result = 0;
	entry_no = Resource_FindFreeEntry();
	if (entry_no != 0)
	{
		VramBlock_LoadCached(entry_no, 0x80, UiIcon_ResourceTiles);
		result = (s32)RenderOutput_CreateFar(entry_no, 0x40000000, window, x, y);
	}
	return result;
}

s32 UiIcon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y)
{
	s32			  slot;
	unsigned char copy_mode;
	int			  resource_mode;
	s32			  icon;
	icon = 0;
	icon = 0;
	slot = Resource_FindFreeEntry();
	resource_mode = 0x80;
	if (slot != 0)
	{
		VramBlock_LoadCached(slot, copy_mode = resource_mode, UiIcon_ResourceTiles);
		icon = (s32)RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
	}
	return icon;
}

void UiIcon_PrepareObject(struct RenderOutput *object)
{
	if (object != NULL)
	{
		object->active = 1;
		object->oam.x = object->x;
		object->oam.y = object->y;
		object->oam.matrix = 0;
		object->oam.affine_mode = 0;
	}
}

//--------------------------------------------------------------------------
// call_icon_routine.c
// メニュー：えらんだキャラクターのグラフィックを読みなおす
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// キャラクターのグラフィックをスロット 0 に読む
// menu : 呼び出し元のメニュー（使わない）
// unit : キャラクター番号
//--------------------------------------------------------------------------
void PsynergyMenu_CallIconRoutineWithValue(s32 menu, s32 unit)
{
	Ui_LoadCharacterEntryForSlotFar(0, unit, 0);
}

s32 UiMenu_CreateCursor(void *source)
{
	struct PsynergyMenuState *work = source;
	s32						  handle;
	s32						  zero = 0;
	s32						  state;
	struct RenderOutput		 *object;

	work->auxiliary_window = zero;
	UiWindow_UpdateOrCreate(&work->auxiliary_window, zero, zero, 13, 5, 2);
	handle = work->auxiliary_window;
	object = (struct RenderOutput *)UiIcon_CreateWithResourceVariant(
		(struct RenderInput *)handle, -8, 11);
	object->active = 13;
	work->tab_index[0] = 255;
	work->tab_index[1] = zero;
	work->pane_icon[0] = object;
	state = 254;
	object->sentinel = state;
	state -= 255;
	work->pane_icon[1]->sentinel = state;
	return handle;
}

void PsynergyMenu_InitializeEntryObjects(
	void *source, s32 origin_x, s32 origin_y, s32 spacing, s32 unused)
{
	u16						  entry_ids[14];
	struct PsynergyMenuState *entry_state = gMenuWork;
	struct RenderInput		 *window = source;
	s32						  entry_count = (u16)Party_ListActiveOwnersFar(entry_ids);
	s32						  i;

	entry_state->tab_counts[0] = entry_count;
	for (i = 0; i < entry_count; i++)
	{
		struct AnimationObject *entry_object = (struct AnimationObject *)ResourceObject_CreateFar(
			Party_RemapCharacterIdByFlagsFar(entry_ids[i]));
		if (entry_object != 0)
		{
			s32 entry_x;
			s32 source_x;
			s32 position_x;

			entry_state->tab_objects[i] = entry_object;
			source_x = window->x;
			entry_x = spacing + 16;
			entry_x *= i;
			position_x = origin_x + source_x;
			entry_state->tab_x[i] = position_x * 8 + entry_x;
			entry_state->row_positions[i] = (origin_y + window->y) * 8 + 16;
			entry_state->owner_scale[i] = 0x10000;
			((struct FieldSprite *)entry_object)->priority = 0;
			entry_object->flags = 0;
			AnimationObjects_SelectAnimationFar(entry_object, 1);
		}
	}
	for (; i < 8; i++)
	{
		entry_state->tab_objects[i] = 0;
	}
	{
		s32 delay_frames = 200;
		delay_frames <<= 4;
		Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, delay_frames);
	}
}

void Menu_ReleaseEntryObjects(void)
{
	u32						  buf[7];
	struct PsynergyMenuState *base = gMenuWork;
	s32						  count;
	void					**p;
	s32						  i;

	count = (u16)Party_ListActiveOwnersFar(buf);
	if (count != 0)
	{
		p = (void **)base->tab_objects;
		i = count;
		do
		{
			void *entry = *p++;

			if (entry != 0)
			{
				ResourceObject_ReleaseFar(entry);
			}
		} while (--i != 0);
	}
	Scheduler_RemoveCallback((void (*)(void))Menu_UpdateEntryObjectTransforms);
}

void Menu_UpdateEntryObjectTransforms(void)
{
	struct PsynergyMenuState *menu = gMenuWork;
	s32						  pos[2];
	s32						  trans[4];
	s32						  i;
	s32						  cnt;

	cnt = (u16)Party_CountActiveOwnersFar();
	for (i = 0; i < cnt; i++)
	{
		struct AnimationObject *obj = menu->tab_objects[i];
		s32						top = 0x01e20000 - (menu->row_positions[i] << 16);

		if (obj != 0)
		{
			obj->part[0].priority = 0;
			pos[0] = menu->owner_scale[i];
			pos[1] = menu->owner_scale[i];
			trans[0] = menu->tab_x[i] << 16;
			trans[1] = top;
			trans[2] = (menu->row_positions[i] << 16) + top;
			trans[3] = 0;
			Object_ApplyProjectedPlacementFar(obj, trans, pos, 0x4000);
		}
	}
}

void UiMenu_PositionCursor(s32 x_offset, s32 y_offset)
{
	struct PsynergyMenuState *work = gMenuWork;
	struct RenderOutput		 *cursor = work->pane_icon[0];

	cursor->oam.x = cursor->x = UiMenu_CursorBobX[(gFrameCount >> 1) & 7] + x_offset
								+ ((struct RenderInput *)work->auxiliary_window)->x * 8 + 8;
	cursor->oam.y = cursor->y = UiMenu_CursorBobY[(gFrameCount >> 1) & 7] + y_offset
								+ ((struct RenderInput *)work->auxiliary_window)->y * 8 + 8;
}

void UiMenu_SlideCursor(s32 x, s32 y)
{

	struct PsynergyMenuState *work = gMenuWork;
	struct RenderOutput		 *cursor;
	s32						  steps;
	s32						  start_x;
	s32						  start_y;
	s32						  px;
	s32						  py;
	s32						  dx;
	s32						  dy;

	steps = 2;
	if (work->skip_slide != 0)
	{
		work->skip_slide = 0;
		return;
	}
	cursor = work->pane_icon[0];
	{
		s32 sprite_x = cursor->oam.x + 64;
		s32 sprite_y = cursor->oam.y + 64;

		cursor->x = sprite_x;
		cursor->y = sprite_y;
	}
	x += 64;
	y += 64;
	if (cursor->x - 8 > 0)
		cursor->x -= 8;
	py = cursor->y;
	if (py - 8 > 0)
	{
		cursor->y -= 8;
		py = cursor->y;
	}
	start_x = cursor->x << 4;
	dx = ((x << 4) - start_x + 1) / steps;
	start_y = py << 4;
	dy = ((y << 4) - start_y + 1) / steps;
	px = start_x;
	py = start_y;
	do
	{
		px += dx;
		cursor->oam.x = cursor->x =
			(px >> 4) + (((struct RenderInput *)work->auxiliary_window)->x << 3) - 56;
		py += dy;
		cursor->oam.y = cursor->y =
			(py >> 4) + (((struct RenderInput *)work->auxiliary_window)->y << 3) - 56;
		steps--;
		if (steps != 0)
			WaitFrames(1);
	} while (steps != 0);
}

void PsynergyMenu_ReservedNoOp(void)
{
}

void PsynergyMenu_RefreshOwnerEntriesDefault(void)
{
	PsynergyMenu_RefreshOwnerEntries(0x6C, 0x28, 8);
}

void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 columns)
{
	struct RenderOutput **slot;
	struct RenderOutput **scan;
	s32					  index;

	index = 0;
	slot = ((struct PsynergyMenuState *)gMenuWork)->entry_icons;
	scan = slot;
	do
	{
		if (*scan++ != NULL)
		{
			PsynergyMenu_PositionOwnerEntry(slot, index, origin_x, origin_y, columns);
		}
		index++;
		slot++;
	} while (index <= 0x1F);
}

void PsynergyMenu_PositionOwnerEntry(
	struct RenderOutput **slot, s32 index, s32 origin_x, s32 origin_y, s32 columns)
{
	struct RenderOutput *object;

	if (index > 0x1F)
	{
		index = 0;
	}
	object = *slot;
	object->y = (index / columns) * 0x10 + origin_y;
	object->x = (index % columns) * 0x10 + origin_x;
	UiIcon_PrepareObject(object);
}

//--------------------------------------------------------------------------
// position_owner_entry.c
// アイテムメニュー：持ち物のアイコンを格子の位置に置く
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// i 番目（15 より大きければ 0）を、横 cols 個の格子（1 つ 24x16 ドット）の
// 位置に置く
// x0, y0 : 格子の左上
//--------------------------------------------------------------------------
void ItemMenu_PosOwner(struct RenderOutput **slot, s32 i, s32 x0, s32 y0, s32 cols)
{
	struct RenderOutput *obj;

	if (i > 15)
		i = 0;
	obj = *slot;
	obj->y = i / cols * 16 + y0;
	obj->x = i % cols * 24 + x0;
	UiIcon_PrepareObject(obj);
}

void ItemMenu_RefreshEntry(s32 layout)
{
	struct RenderOutput		 **slot;
	struct RenderOutput		 **scan;
	struct RenderOutput		  *object;
	s32						   index;
	s32						   origin_y;
	struct InventoryMenuState *menu;

	menu = gMenuWork;
	origin_y = 0x38;
	if (layout != 1)
	{
		origin_y = 0x28;
	}
	slot = menu->entry_icons;
	index = 0;
	scan = slot;
	do
	{
		object = *scan++;
		if (object != NULL)
		{
			ItemMenu_PosOwner(slot, index, 0x74, origin_y, 5);
		}
		index++;
		slot++;
	} while (index <= 0xE);
}

void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y)
{
	struct InventoryMenuState *menu = gMenuWork;
	s32						   window;
	s32						   height;
	s32						   width;
	s32						   top;
	s32						   left;

	menu->pane_icons[0]->active = 13;
	if (y != -1)
	{
		UiText_GetResourceDimensionsFar(message, &left, &top, &width, &height);
		if (UiWindow_UpdateOrCreate((s32 *)&menu->modal_window, x, y, width, height, 0x102) == 0)
			UiWindow_SetBounds((struct RenderInput *)menu->modal_window, x, y, width, height);
		window = (s32)menu->modal_window;
	}
	else
	{
		window = (s32)menu->info_window;
	}
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	RenderOutput_ClearListFar((void *)window);
#if EDITION_INTERNATIONAL
	if (y == -1)
		UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
	else
		UiText_DrawResourceFar(message, window, 0, 0);
#else
	UiText_DrawResourceFar(message, window, 0, 0);
#endif
	if (x != -1)
	{
		WaitFrames(1);
		do
		{
			WaitFrames(1);
		} while (!(gKeyState & KEY_A) && !(gKeyState & KEY_B) && !(gKeyState & KEY_START));
#if EDITION_INTERNATIONAL
		if (y == -1)
			RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
#endif
		RenderOutput_ClearListFar((void *)window);
	}
	else
	{
		GameFlag_SetBitFar(0x151);
	}
	menu->completion_flag = 1;
	menu->pane_icons[0]->active = 1;
#if EDITION_INTERNATIONAL
	if (y != -1)
		UiWindow_CloseIfOpen(&menu->modal_window, 1);
#endif
}

s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode)
{
	u16					   sorted[16];
	u16					   items[16];
	u8					   order[32];
	struct ItemDefinition *item;
	s32					   pass;
	s32					   i;
	s32					   count;
	s32					   best;
	s32					   best_slot;
	s32					   sorted_count;
	s32					   value;

	best_slot = 0;
	sorted_count = 0;
	Menu_CopyListOrder(mode, order);

	for (i = 0; i < 15; i++)
		items[i] = inventory[i];

	count = 0;
	for (i = 0; i < 15; i++)
	{
		if (items[i] != 0)
			count++;
	}

	for (i = count; i < 15; i++)
		sorted[i] = 0;

	pass = 0;
	while (order[pass] != 0xff)
	{
		for (;;)
		{
			best = 0;
			for (i = 0; i < count; i++)
			{
				if (items[i] == 0)
					continue;
				item = Item_GetDirectFar(items[i]);
				if ((order[pass] & 0x7f) != item->type)
					continue;
				if ((order[pass] & 0x80) && (items[i] & INVENTORY_EQUIPPED) == 0)
					continue;
				value = items[i] & ITEM_ID_MASK;
				if (best < value)
				{
					best_slot = i;
					best = value;
				}
			}
			if (best == 0)
				break;
			sorted[sorted_count] = items[best_slot];
			items[best_slot] = 0;
			sorted_count++;
		}
		pass++;
	}

	for (i = 0; i < count; i++)
		inventory[i] = sorted[i];
	return 1;
}

//--------------------------------------------------------------------------
// copy_list_order.c
// メニュー：並べかえの順番の表を写す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// mode 0〜2 の順番の表（それ以外は標準の表）を order に写す
// 表は 0xff で終わる。写すのは 0xff を含めて 32 バイトまで
//--------------------------------------------------------------------------
void Menu_CopyListOrder(s32 mode, u8 *order)
{
	u8 *src;
	s32 n;

	src = Menu_ListOrderDefault;
	switch (mode)
	{
	case 0:
		src = Menu_ListOrderMode0;
		break;
	case 1:
		src = Menu_ListOrderMode1;
		break;
	case 2:
		src = Menu_ListOrderMode2;
		break;
	}
	*order = *src;
	n = 0;
	if (*order != 0xff)
	{
		do
		{
			if (++n > 31)
				break;
			src++;
			order++;
			*order = *src;
		} while (*order != 0xff);
	}
}

#define PAGE_LABEL_FIRST 49
#define PAGE_CAP_LEFT 0xf128
#define PAGE_CAP_RIGHT 0xf129

s32 Menu_HandlePageInput(s32 horizontal, s32 count, s32 per_page, s32 *cursor, s32 *page)
{
	s32 pages;
	s32 next;
	s32 previous;
	s32 page_forward;
	s32 result;

	result = -1;
	if (count == 0)
		return result;
	Link_DrawShiftedTilePairFar(0x06002500);
	pages = count / per_page;
	if (count % per_page != 0)
		pages++;
	if (horizontal)
	{
		next = gKeysRepeat & KEY_RIGHT;
		previous = gKeysRepeat & KEY_LEFT;
		horizontal = gKeysRepeat & KEY_UP;
		page_forward = gKeysRepeat & KEY_DOWN;
	}
	else
	{
		next = gKeysRepeat & KEY_DOWN;
		previous = gKeysRepeat & KEY_UP;
		horizontal = gKeysRepeat & KEY_LEFT;
		page_forward = gKeysRepeat & KEY_RIGHT;
	}
	if (horizontal)
	{
		AudioCommand_PlayFar(111);
		if (--*page < 0)
			*page = pages - 1;
		if (*cursor + *page * per_page > count - 1)
		{
			*cursor = count - *page * per_page - 1;
			if (*cursor > per_page - 1)
				*cursor = per_page - 1;
		}
		Runtime_SetMainState19();
		result = 1;
		return result;
	}
	if (page_forward)
	{
		AudioCommand_PlayFar(111);
		if (++*page > pages - 1)
			*page = 0;
		if (*cursor + *page * per_page > count - 1)
		{
			*cursor = count - *page * per_page - 1;
			if (*cursor > per_page - 1)
				*cursor = per_page - 1;
		}
		Runtime_SetMainState19();
		result = 1;
		return result;
	}
	if (previous)
	{
		AudioCommand_PlayFar(111);
		if (--*cursor < 0)
		{
			*cursor = per_page - 1;
			*cursor = count - *page * per_page - 1;
			if (*cursor > per_page - 1)
				*cursor = per_page - 1;
		}
		return 0;
	}
	result = -1;
	if (next == 0)
		return result;
	{
		AudioCommand_PlayFar(111);
		result = 0;
		if (++*cursor == count - *page * per_page)
			*cursor = result;
		if (*cursor > per_page - 1)
			*cursor = result;
	}
	return 0;
}

void Palette_LightenBankHighlight(s32 bank)
{
	u16 *palette = (u16 *)(0x05000000 + (bank << 5));
	u32	 red;
	u32	 green;
	u32	 blue;

	Dma_Set((const void *)0x050001e0, palette, 0x80000010, REG_DMA3);
	Dma_Set((const void *)0x050001e0, palette, 0x84000008, REG_DMA3);
	blue = palette[4] >> 10;
	green = (palette[4] >> 5) & 31;
	red = palette[4] & 31;
	blue += 9;
	if (blue > 31)
		blue = 31;
	green += 9;
	if (green > 31)
		green = 31;
	red += 9;
	if (red > 31)
		red = 31;
	palette[4] = (blue << 10) | (green << 5) | red;
}

void Menu_DrawPageIndicator(
	s32 window, s32 item_count, s32 page_size, s32 selected_page, s32 right_edge)
{
	s32 page_count;
	s32 page;
	s32 x;
	s32 tile;

	x = right_edge;
	tile = PAGE_LABEL_FIRST;
	page_count = item_count / page_size;
	if (item_count % page_size != 0)
		page_count++;

	x -= page_count;
	if (page_count > 1)
	{
		UiWindow_SetTilemapEntryFar(window, PAGE_CAP_LEFT, x - 1, -1, 0);

		for (page = 0; page < page_count; page++)
		{
			if (page == selected_page)
				UiWindow_SetTilemapEntryFar(window, tile, x, -1, 2);
			else
				UiWindow_SetTilemapEntryFar(window, tile, x, -1, 3);
			tile++;
			x++;
		}

		UiWindow_SetTilemapEntryFar(window, PAGE_CAP_RIGHT, x, -1, 0);
	}
}

void Render_SetTilemapFlagRect(
	const struct UiWindow *object, s32 x, s32 y, s32 width, s32 height, u32 field)
{
	struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
	u8					*base = (u8 *)work->tilemap;

	x += object->x + 1;
	y += object->y + 1;
	field <<= 12;
	if (x < 0)
	{
		width += x;
		x = 0;
	}
	if (x + width > 29)
	{
		width = 30 - x;
	}
	if (y < 0)
	{
		height += y;
		y = 0;
	}
	if (y + height > 29)
	{
		height = 20 - y;
	}
	if (width > 0 && height > 0)
	{
		y <<= 6;
		x = y + (x << 1);
		do
		{
			u16 *pixel = (u16 *)(x + (u32)base);
			s32	 remaining = width;
			while (remaining != 0)
			{
				u32 value = *pixel;
				value &= 0xFFFFEFFF;
				value |= field;
				remaining--;
				*pixel = value;
				pixel++;
			}
			height--;
			x += 64;
		} while (height != 0);
		work->dirty = 1;
	}
}

void Palette_CopyObjectBankToBackground14(void)
{
	Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, REG_DMA3);
	Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, REG_DMA3);
}

void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y)
{
	struct InventoryMenuState *menu = gMenuWork;
	struct RenderOutput		  *icon;
	s32						   i;

	for (i = 0; i < 32; i++)
	{
		icon = menu->entry_icons[i];
		if (icon != NULL)
			icon->active = 13;
	}
	for (i = first;
		i < page_size + first && (icon = menu->entry_icons[i]) != NULL && i <= menu->item_count - 1;
		i++)
	{
		icon->x = x;
		icon->y = y + (i - first) * 16;
		UiIcon_PrepareObject(icon);
		icon->active = 1;
	}
}

void UiText_DrawWorkValueWithLabel(s32 work)
{
	UiText_DrawNumberAtOffsetFar(gGameState.coins, 7, work, 8, 0);
	UiText_DrawCharacterAtOffsetFar(MsgCoinsLabel, work, 0x40, 0);
#if !EDITION_INTERNATIONAL
	UiWindow_DrawDividerLineFar(work, 12, 0, 12, 2);
#endif
}

void UiWindow_SetBounds(struct RenderInput *window, s32 x, s32 y, s32 width, s32 height)
{
	if (window != NULL)
	{
		window->width = width;
		window->x = x;
		window->height = height;
		window->y = y;
	}
}

void UiWork_SetMenuBusy(void)
{
	((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
}

void UiWork_ClearMenuBusy(void)
{
	((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
}

s32 Audio_PlayCueReturnOne(s32 cue)
{
	AudioCommand_PlayFar(cue);
	return 1;
}

void Menu_CancelSoundTick(void)
{
	if (gKeyState & KEY_START)
	{
		AudioCommand_PlayFar(SOUND_MENU_CANCEL);
		GameFlag_SetBitFar(0x150);
		Scheduler_RemoveCallback(Menu_CancelSoundTick);
	}
}

void Menu_CancelSoundReset(void)
{
	GameFlag_ClearBitFar(0x150);
	Scheduler_AddOrUpdateCallback(Menu_CancelSoundTick, 0xC80);
}

void Menu_EnsureCancelSound(void)
{
	if (GameFlag_TestFar(0x150) == 0)
	{
		Scheduler_RemoveCallback(Menu_CancelSoundTick);
	}
}

void UiPalette_SetColor15(void)
{
	UiWork_SetParamNibbleFar(15);
}

void UiPalette_SetColor2(void)
{
	UiWork_SetParamNibbleFar(2);
}

void UiPalette_SetColor4(void)
{
	UiWork_SetParamNibbleFar(4);
}

// イベントの作業領域（struct EventWork）の別の見え方。0x180 と 0x19a は共有ヘッダーにまだ名前がない
struct AssetSelectionResult
{
	s32 unknown_000[0x60];
	u16 unknown_180;
	u8	unknown_182[0x18];
	u16 unknown_19a;
};

s32 RunAssetSelectionScreen(void)
{
#if EDITION_INTERNATIONAL
	void *backup;
#endif
	struct InventoryMenuState *screen;
	s32						   index;
	s32						   value;
	s32						   category;
	s32						   result;
#if EDITION_INTERNATIONAL
	s32 size;
#endif
	void **process;
	void **cache;

#if EDITION_INTERNATIONAL
	size = 0x2000;
	backup = Runtime_BumpAllocateAlternatePool(size);
#endif
	screen = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
	cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_MENU_CONTROL];
	((struct ObjectSystemWork *)cache[0])->suspended = 1;
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	WaitFrames(1);
	UiWindow_InitializeWork(0);
	screen->party_count = Party_ListActiveOwnersFar(screen->owner_ids);
	ItemMenu_Init(0, 3, 0, 7);
	Resource_LoadPairedBlocks();
	Palette_LightenBankHighlight(14);
	Link_DrawShiftedTilePairFar(0x06002500);
	screen->message_window = UiWindow_CreateFar(13, 0, 17, 3, 2);
#if EDITION_INTERNATIONAL
	Scheduler_EnableOverlayCallbacksWithFlags();
	Iwram_CopyWords(backup, (void *)0x06004000, size);
	Iwram_FillWords((void *)0x06004000, size, 0x33333333);
	UiWork_SetAltFlagAndClearTableFar(1);
#endif
	Menu_CancelSoundReset();
	result = ItemMenu_RunCommands(&category, &value, &index);
	Menu_EnsureCancelSound();
	if (result == 1)
	{
		struct AssetSelectionResult *selection = cache[HEAP_SLOT_EVENT - HEAP_SLOT_MENU_CONTROL];

		selection->unknown_180 = (category << 10) | (index & ITEM_ID_MASK);
		selection->unknown_19a = screen->selected_slots[0];
	}
	RenderOutput_ClearListFar(screen->status_window);
	process = &cache[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU_CONTROL];
	((struct UiRenderWork *)*process)->menu_busy = 1;
	ItemMenu_Close();
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	Menu_ResetTwoResourceEntries();
	Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
	((struct ObjectSystemWork *)cache[0])->suspended = 0;
#if EDITION_INTERNATIONAL
	UiWindow_MarkVisibleTileAttributesFar();
	UiWork_SetAltFlagAndClearTableFar(0);
	Iwram_CopyWords((void *)0x06004000, backup, size);
	((struct UiRenderWork *)*process)->menu_busy = 0;
	MemFree(backup);
	WaitFrames(1);
	Scheduler_DisableOverlayCallbacksWithFlags();
#endif
	WaitFrames(1);
	UiWindow_EraseBorderRectFar(0, 0, 30, 20);
	((struct UiRenderWork *)*process)->menu_busy = 0;
	Event_ClearInvalidPackedValuesFar();
	return result;
}

#if EDITION_INTERNATIONAL
#define MODAL_SHORT_X 14
#define MODAL_REMOVED_X 14
#else
#define MODAL_SHORT_X 15
#define MODAL_REMOVED_X 17
#endif

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

#define IWRAM_COPY_WORDS ((WordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
	/* FAKEMATCH: needed to match: the copy routine as IwramIrqMain plus its offset, passed first; direct calls share the 332-byte size in r5 and move the routine to r6, and through Iwram_CopyWords the routine is loaded before the source and size (2 instructions reordered). */
	return copy(dst, src, size);
}

enum ItemMenuStep
{
	ITEM_STEP_OWNER = 0,
	ITEM_STEP_ITEM = 1,
	ITEM_STEP_USE = 2,
	ITEM_STEP_EQUIP = 3,
	ITEM_STEP_GIVE = 4,
	ITEM_STEP_DROP = 5,
	ITEM_STEP_TARGET = 6,
	ITEM_STEP_TRADE = 7,
	ITEM_STEP_IDLE = 8,
	ITEM_STEP_COMMAND = 9,
	ITEM_STEP_INSPECT = 10,
	ITEM_STEP_REMOVE = 11,
	ITEM_STEP_QUANTITY = 12
};

s32 ItemMenu_RunCommands(s32 *owner_out, s32 *target_out, s32 *item_out)
{
	s32 done;
#if !EDITION_INTERNATIONAL
	s32 owner;
	s32 chosen;
#endif
	s32						   sel;
	s32						   ret;
	struct BtlUnit			  *source;
	struct BtlUnit			  *target;
	struct InventoryMenuState *menu;
	struct ItemDefinition	  *item;
	struct BtlUnit			  *source_copy;
	struct BtlUnit			  *target_copy;
	u32						   state;
	s32						   command;
	s32						   result;
	s32						   qty;
	s32						   amount;
	s32						   aborted;
	s32						   work;
	s32						   n;
	u8						   moved_source;
	u8						   moved_target;
	s8						   stack;

	done = 0;
#if !EDITION_INTERNATIONAL
	owner = 0;
	chosen = 0;
#endif
	sel = 0;
	ret = 0;
	state = ITEM_STEP_OWNER;
	menu = gMenuWork;

	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		switch (state)
		{
		case ITEM_STEP_OWNER:
			menu->selected_slots[0] = 0;
			ItemMenu_SetMsgWin3();
			ItemMenu_SetItemWin3();
			menu->selected_item_icon->active = 13;
			ItemMenu_DrawMsg(0, MsgWhoseItem);
#if EDITION_INTERNATIONAL
			RenderOutput_RedrawSavedRectFar(menu->info_window);
			UiText_DrawWorkValueWithLabel((s32)menu->info_window);
			command = ItemMenu_PrepOwner(0);
			if (command == -1)
			{
				sel = command = 0;
				ret = -1;
				done = 1;
			}
#else
			UiText_DrawWorkValueWithLabel((s32)menu->info_window);
			owner = ItemMenu_PrepOwner(0);
			if (owner == -1)
			{
				chosen = sel = owner = 0;
				ret = -1;
				done = 1;
			}
#endif
			RenderOutput_RedrawSavedRectFar(menu->info_window);
			ItemMenu_HideAllIcons();
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_ITEM:
			if (ItemMenu_Count(menu->pane_owner[0]) == 0)
			{
				state = ITEM_STEP_OWNER;
				break;
			}
			ItemMenu_SetMsgWin3();
			ItemMenu_SetItemWin3();
			menu->selected_item_icon->active = 13;
			menu->pane_icons[0]->active = 1;
			ItemMenu_DrawMsg(0, MsgWhichItem);
			sel = ItemMenu_RunList(0);
			state = ITEM_STEP_OWNER;
			if (sel == -1)
			{
				break;
			}
			menu->list_mode = 0xff;
			state = ITEM_STEP_COMMAND;
			break;

		case ITEM_STEP_COMMAND:
			command = ItemMenu_ChooseCmd();
			if (command == -1)
			{
				state = ITEM_STEP_ITEM;
				menu->completion_flag = 1;
			}
			if (command == ITEM_COMMAND_USE)
			{
				if (BattleFx_HasTriggerFar(menu->selected_items[0] & ITEM_ID_MASK) != 0)
				{
					done = 1;
#if EDITION_INTERNATIONAL
					*owner_out = menu->pane_owner[0];
					*target_out = command;
					*item_out = menu->selected_items[0] & ITEM_ID_MASK;
#else
					*owner_out = owner;
					*target_out = chosen;
					*item_out = sel;
#endif
					ret = 1;
					break;
				}
				result = Item_ClassifyUseMode(menu->pane_owner[0], menu->selected_items[0]);
				if (result == 1)
				{
					state = ITEM_STEP_USE;
				}
				if (result == 2)
				{
					ItemMenu_Use();
					RenderOutput_ClearListFar(menu->info_window);
					InventoryMenu_ShowModalMessage(menu->message_offset + MsgItemUseResult, 0, -1);
#if !EDITION_INTERNATIONAL
					RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
					menu->pane_icons[0]->active = 13;
					menu->item_count =
						ItemMenu_Collect(UnitGetFar(menu->pane_owner[0]), menu->items, 0);
					ItemMenu_DrawIcons(menu->items, 0);
					state = ITEM_STEP_OWNER;
				}
				if (result == -1 || result == 0)
				{
					done = 1;
					*owner_out = menu->pane_owner[0];
					*target_out = menu->pane_owner[1];
					*item_out = menu->selected_items[0] & ITEM_ID_MASK;
					ret = 1;
				}
			}
			if (command == ITEM_COMMAND_EQUIP)
			{
				state = ITEM_STEP_EQUIP;
			}
			if (command == ITEM_COMMAND_GIVE)
			{
				state = ITEM_STEP_TARGET;
			}
			if (command == ITEM_COMMAND_DROP)
			{
				state = ITEM_STEP_DROP;
			}
			if (command == ITEM_COMMAND_REMOVE)
			{
				state = ITEM_STEP_REMOVE;
			}
			if (command == ITEM_COMMAND_INSPECT)
			{
				state = ITEM_STEP_INSPECT;
			}
			break;

		case ITEM_STEP_USE:
			ItemMenu_HideAllIcons();
			ItemMenu_SetMsgWin5();
			ItemMenu_SetItemWin5();
			RenderOutput_RedrawSavedRectFar(menu->message_window);
			ItemMenu_DrawItemHead();
			UiText_DrawCharacterAtOffsetFar(MsgUseOnWhom, (s32)menu->message_window, 16, 16);
#if EDITION_INTERNATIONAL
			if (ItemMenu_SelectTarget(0) != -1)
			{
#else
			chosen = ItemMenu_SelectTarget(0);
			if (chosen != -1)
			{
#endif
				command = 0;
				if (ItemMenu_IsSpecial(menu->selected_items[0] & ITEM_ID_MASK) != 0)
				{
					command = 8;
				}
				result = ItemMenu_Use();
				Menu_DrawOwnerStatusPanel(
					(s32)menu->status_window, menu->pane_owner[1], 0, command);
				if (result != -1)
				{
					RenderOutput_ClearListFar(menu->info_window);
					InventoryMenu_ShowModalMessage(menu->message_offset + MsgItemUseResult, 0, -1);
#if !EDITION_INTERNATIONAL
					RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
					menu->pane_icons[0]->active = 13;
					ItemMenu_TryBreak();
					state = ITEM_STEP_ITEM;
				}
				menu->item_count =
					ItemMenu_Collect(UnitGetFar(menu->pane_owner[0]), menu->items, 0);
				ItemMenu_DrawIcons(menu->items, 0);
				menu->completion_flag = 1;
			}
			else
			{
				state = ITEM_STEP_COMMAND;
			}
			break;

		case ITEM_STEP_TARGET:
			ItemMenu_SetMsgWin5();
			ItemMenu_SetItemWin5();
			RenderOutput_RedrawSavedRectFar(menu->message_window);
			ItemMenu_DrawItemHead();
			UiText_DrawCharacterAtOffsetFar(MsgGiveToWhom, (s32)menu->message_window, 16, 16);
#if EDITION_INTERNATIONAL
			n = ItemMenu_SelectTarget(1);
			state = ITEM_STEP_GIVE;
			if (n == -1)
			{
#else
			chosen = ItemMenu_SelectTarget(1);
			state = ITEM_STEP_GIVE;
			if (chosen == -1)
			{
#endif
				ItemMenu_DrawEquipPreview(
					menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
				state = ITEM_STEP_COMMAND;
			}
			break;

		case ITEM_STEP_DROP:
			ItemMenu_HideAllIcons();
			item = Item_GetDirectFar(menu->selected_items[0] & ITEM_ID_MASK);
			result = 0;
			if ((item->flags & ITEM_STACKABLE) != 0)
			{
				qty = (menu->selected_items[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
				if (qty > 1)
				{
					ItemMenu_DrawItemHead();
					result = ItemMenu_SelectGiveQuantity(0, qty, 1);
				}
			}
			if (result == -1)
			{
				state = ITEM_STEP_COMMAND;
				break;
			}
			menu->pane_owner[1] = 0;
			Resource_LoadByModeIntoSlotFar(2,
				(menu->selected_items[0] & ITEM_ID_MASK) | (result << INVENTORY_QUANTITY_SHIFT),
				(u8)menu->selected_item_icon->index, 0);
			menu->selected_item_icon->active = 1;
			menu->selected_item_icon->x = 120;
			menu->selected_item_icon->y = 28;
			UiIcon_PrepareObject(menu->selected_item_icon);
			UiWindow_ClearInteriorTilesFar((void *)menu->list_window, 0, 72, 120, 96);
			RenderOutput_RedrawSavedRectFar(menu->message_window);
			if (ItemMenu_ConfirmDrop(sel) == 0)
			{
#if EDITION_INTERNATIONAL
				command = menu->pane_owner[0];
				UnitGetFar(command);
				for (work = 0; work < result + 1; work++)
				{
					Inventory_RemoveFar(command, menu->selected_slots[0]);
					Item_AdjustCounterFar(menu->selected_items[0] & ITEM_ID_MASK, 1);
				}
				UnitCalcFar(command);
#else
				owner = menu->pane_owner[0];
				UnitGetFar(owner);
				for (work = 0; work < result + 1; work++)
				{
					Inventory_RemoveFar(owner, menu->selected_slots[0]);
					Item_AdjustCounterFar(menu->selected_items[0] & ITEM_ID_MASK, 1);
				}
				UnitCalcFar(owner);
#endif
				ItemMenu_SetItemWin3();
				ItemMenu_RefreshOwner(menu->pane_owner[0], 0);
				menu->selected_item_icon->active = 13;
				menu->pane_icons[0]->active = 13;
				WaitFrames(1);
				RenderOutput_ClearListFar(menu->info_window);
				InventoryMenu_ShowModalMessage(MsgDroppedIt, MODAL_SHORT_X, 13);
				menu->completion_flag = 1;
				state = ITEM_STEP_ITEM;
			}
			else
			{
				state = ITEM_STEP_COMMAND;
			}
			UnitCalcFar(menu->pane_owner[0]);
			menu->selected_item_icon->active = 13;
			Event_ClearInvalidPackedValuesFar();
			break;

		case ITEM_STEP_GIVE:
			aborted = 0;
			item = Item_GetDirectFar(menu->selected_items[0] & ITEM_ID_MASK);
			if ((item->flags & ITEM_STACKABLE) != 0)
			{
				qty = InventoryMenu_GetItemQuantity(
					menu->pane_owner[1], menu->selected_items[0] & ITEM_ID_MASK);
				if (qty == 30)
				{
					aborted = 1;
				}
				if (ItemMenu_Count(menu->pane_owner[1]) == 15 && qty == 0)
				{
					state = ITEM_STEP_TRADE;
					break;
				}
				stack = (menu->selected_items[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
				if (aborted == 0)
				{
					if (qty + stack > 30)
					{
						amount = 30 - qty;
					}
					else
					{
						amount = stack;
					}
					if (stack > 1)
					{
						result = ItemMenu_SelectGiveQuantity(0, amount, 0);
					}
					else
					{
						result = 0;
					}
					if (result == -1)
					{
						state = ITEM_STEP_TARGET;
						break;
					}
					for (command = 0; command < result + 1; command++)
					{
						qty = Inventory_AddItemFar(
							menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
						if (qty != -1)
						{
							Inventory_RemoveFar(menu->pane_owner[0], menu->selected_slots[0]);
							menu->selected_slots[1] = qty;
						}
						else
						{
							aborted = 1;
						}
					}
				}
			}
			else
			{
				result = Inventory_AddItemFar(menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
				if (result == -1)
				{
					state = ITEM_STEP_TRADE;
					break;
				}
				menu->selected_slots[1] = result;
				result = Inventory_RemoveFar(menu->pane_owner[0], menu->selected_slots[0]);
				if (result == -1)
				{
					aborted = 1;
				}
			}
			UnitCalcFar(menu->pane_owner[0]);
			UnitCalcFar(menu->pane_owner[1]);
			Owner_RefreshClassActionsFar(menu->pane_owner[0]);
			Owner_RefreshClassActionsFar(menu->pane_owner[1]);
			result = 1;
			if (aborted == 0)
			{
				menu->pane_owner[0] = menu->pane_owner[1];
				menu->selected_items[0] &= ITEM_ID_MASK;
#if EDITION_INTERNATIONAL
				ItemMenu_SetMsgWin6();
#endif
				RenderOutput_RedrawSavedRectFar(menu->message_window);
				ItemMenu_DrawItemHead();
				result = ItemMenu_ConfirmEquip(0);
			}
			if (GameFlag_TestFar(0x150) != 0)
			{
				break;
			}
			ItemMenu_RefreshOwner(menu->pane_owner[1], 1);
			menu->pane_icons[0]->active = 13;
			WaitFrames(1);
			if (aborted == 1)
			{
				RenderOutput_ClearListFar(menu->info_window);
				InventoryMenu_ShowModalMessage(MsgCannotHoldMore, 15, 14);
			}
			else
			{
				RenderOutput_ClearListFar(menu->info_window);
				if (result == 1)
				{
					InventoryMenu_ShowModalMessage(MsgGiven, 15, 14);
				}
				else
				{
					ItemMenu_DrawEquipPreview(
						menu->pane_owner[1], menu->selected_slots[1], 0, menu->pane_owner[1]);
					InventoryMenu_ShowModalMessage(MsgEquippedIt, 15, 14);
					item = Item_GetDirectFar(menu->selected_items[0]);
					if ((item->flags & ITEM_CURSED) != 0)
					{
						AudioCommand_PlayFar(0x67);
						RenderOutput_ClearListFar(menu->info_window);
						InventoryMenu_ShowModalMessage(MsgEquippedIt + 7, MODAL_SHORT_X, 14);
					}
				}
			}
			Event_ClearInvalidPackedValuesFar();
			state = ITEM_STEP_OWNER;
			break;

		case ITEM_STEP_TRADE:
			aborted = 0;
			ItemMenu_SetMsgWin3();
			ItemMenu_SetItemWin3();
			ItemMenu_DrawMsg(0, MsgSwapForWhat);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
			if (ItemMenu_RunList(1) == -1)
			{
#else
			sel = ItemMenu_RunList(1);
			if (sel == -1)
			{
#endif
				state = ITEM_STEP_TARGET;
				break;
			}
			source = UnitGetFar(menu->pane_owner[0]);
			target = UnitGetFar(menu->pane_owner[1]);
			source_copy = MemAlloc(BATTLE_UNIT_SIZE);
			target_copy = MemAlloc(BATTLE_UNIT_SIZE);
			CopyWords(IWRAM_COPY_WORDS, source_copy, source, BATTLE_UNIT_SIZE);
			CopyWords(IWRAM_COPY_WORDS, target_copy, target, BATTLE_UNIT_SIZE);
			moved_source = 0;
			while (moved_source <= 29)
			{
				result = Inventory_RemoveFar(menu->pane_owner[0], menu->selected_slots[0]);
				if (result == 2)
				{
					break;
				}
				if (result == -1)
				{
					aborted = 1;
					break;
				}
				moved_source++;
			}
			moved_source++;
			moved_target = 0;
			while (moved_target <= 29)
			{
				if ((menu->selected_items[1] & INVENTORY_EQUIPPED) != 0)
				{
					item = Item_GetDirectFar(menu->selected_items[1] & ITEM_ID_MASK);
					if ((item->flags & ITEM_CANNOT_UNEQUIP) != 0)
					{
						aborted = 1;
					}
				}
				result = Inventory_RemoveFar(menu->pane_owner[1], menu->selected_slots[1]);
				if (result == 2)
				{
					break;
				}
				if (result == -1)
				{
					aborted = 1;
					break;
				}
				moved_target++;
			}
			moved_target++;
			while (moved_source != 0)
			{
				result = Inventory_AddItemFar(menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
				if (result == -1)
				{
					aborted = 1;
					break;
				}
				menu->selected_slots[1] = result;
				moved_source--;
			}
			while (moved_target != 0)
			{
				result = Inventory_AddItemFar(menu->pane_owner[0], menu->selected_items[1] & 0x5ff);
				if (result == -1)
				{
					aborted = 1;
					break;
				}
				menu->selected_slots[0] = result;
				moved_target--;
			}
			WaitFrames(1);
			if (aborted == 1)
			{
				CopyWords(IWRAM_COPY_WORDS, source, source_copy, BATTLE_UNIT_SIZE);
				CopyWords(IWRAM_COPY_WORDS, target, target_copy, BATTLE_UNIT_SIZE);
				RenderOutput_ClearListFar(menu->info_window);
#if defined(TBS_EDITION_FR)
				InventoryMenu_ShowModalMessage(MsgCannotTrade, 11, 14);
				RenderOutput_RedrawSavedRectFar(menu->status_window);
#else
				InventoryMenu_ShowModalMessage(MsgCannotTrade, 15, 14);
#endif
			}
			else
			{
				UnitCalcFar(menu->pane_owner[0]);
				UnitCalcFar(menu->pane_owner[1]);
				Owner_RefreshClassActionsFar(menu->pane_owner[0]);
				Owner_RefreshClassActionsFar(menu->pane_owner[1]);
				ItemMenu_SetMsgWin5();
#if EDITION_INTERNATIONAL
				ItemMenu_SetMsgWin6();
				ItemMenu_HidePageIcons();
#endif
				RenderOutput_RedrawSavedRectFar(menu->message_window);
				menu->pane_owner[0] = menu->pane_owner[1];
				menu->selected_items[0] &= ITEM_ID_MASK;
				ItemMenu_DrawItemHead();
				result = ItemMenu_ConfirmEquip(0);
				if (GameFlag_TestFar(0x150) == 0)
				{
					RenderOutput_ClearListFar(menu->info_window);
					ItemMenu_SetItemWin5();
					ItemMenu_RefreshOwner(menu->pane_owner[1], 1);
					if (result == 0)
					{
						ItemMenu_DrawEquipPreview(
							menu->pane_owner[1], menu->selected_slots[1], 0, menu->pane_owner[1]);
						InventoryMenu_ShowModalMessage(MsgEquippedIt, 15, 14);
						item = Item_GetDirectFar(menu->selected_items[0]);
						if ((item->flags & ITEM_CURSED) != 0)
						{
							AudioCommand_PlayFar(0x67);
							RenderOutput_ClearListFar(menu->info_window);
							InventoryMenu_ShowModalMessage(MsgEquippedIt + 7, MODAL_SHORT_X, 14);
						}
					}
					else
					{
#if defined(TBS_EDITION_IT)
						InventoryMenu_ShowModalMessage(MsgTraded, 13, 14);
#else
						InventoryMenu_ShowModalMessage(MsgTraded, 15, 14);
#endif
					}
				}
			}
			MemFree(target_copy);
			MemFree(source_copy);
			Event_ClearInvalidPackedValuesFar();
			state = ITEM_STEP_OWNER;
			break;

		case ITEM_STEP_EQUIP:
			result = Inventory_EquipFar(menu->pane_owner[0], menu->selected_slots[0]);
			if (result == -1)
			{
				state = ITEM_STEP_ITEM;
				break;
			}
			if (result == -2)
			{
				RenderOutput_ClearListFar(menu->info_window);
				InventoryMenu_ShowModalMessage(MsgCannotRemoveItem, 0, -1);
#if !EDITION_INTERNATIONAL
				RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
				state = ITEM_STEP_ITEM;
				break;
			}
			UnitCalcFar(menu->pane_owner[0]);
			Owner_RefreshClassActionsFar(menu->pane_owner[0]);
			menu->pane_icons[0]->active = 13;
			menu->item_count = ItemMenu_Collect(UnitGetFar(menu->pane_owner[0]), menu->items, 0);
			ItemMenu_DrawIcons(menu->items, 0);
			WaitFrames(1);
			ItemMenu_DrawEquipPreview(
				menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
			RenderOutput_ClearListFar(menu->info_window);
			InventoryMenu_ShowModalMessage(MsgEquippedIt, 15, 8);
			item = Item_GetDirectFar(menu->selected_items[0]);
			if ((item->flags & ITEM_CURSED) != 0)
			{
				AudioCommand_PlayFar(0x67);
				RenderOutput_ClearListFar(menu->info_window);
				InventoryMenu_ShowModalMessage(MsgEquippedIt + 7, MODAL_SHORT_X, 8);
			}
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_REMOVE:
			UnitGetFar(menu->pane_owner[0])->inventory[menu->selected_slots[0]] &=
				~INVENTORY_EQUIPPED;
			UnitCalcFar(menu->pane_owner[0]);
			Owner_RefreshClassActionsFar(menu->pane_owner[0]);
			menu->pane_icons[0]->active = 13;
			menu->item_count = ItemMenu_Collect(UnitGetFar(menu->pane_owner[0]), menu->items, 0);
			ItemMenu_DrawIcons(menu->items, 0);
			WaitFrames(1);
			menu->equip_preview = 1;
			ItemMenu_DrawEquipPreview(
				menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
			menu->equip_preview = 0;
			RenderOutput_ClearListFar(menu->info_window);
			InventoryMenu_ShowModalMessage(MsgRemoved, MODAL_REMOVED_X, 8);
			Event_ClearInvalidPackedValuesFar();
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_INSPECT:
			menu->pane_icons[0]->active = 13;
			Menu_SelectQuantity(menu->selected_items[0]);
			RenderOutput_RedrawSavedRectFar(menu->status_window);
			ItemMenu_DrawEquipPreview(
				menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
			menu->pane_icons[0]->active = 1;
			state = ITEM_STEP_COMMAND;
			break;

		case ITEM_STEP_QUANTITY:
			result = ItemMenu_SelectGiveQuantity(0, 30, 0);
			if (result != -1)
			{
				qty = result;
			}
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_IDLE:
			break;

		default:
			done = 1;
			break;
		}
	}

	if (GameFlag_TestFar(0x150) != 0)
	{
		ret = -1;
	}
	return ret;
}

s32 ItemMenu_Use(void)
{
	struct InventoryMenuState *menu;
	s32						   result;

	menu = gMenuWork;
	result = Item_Use(menu->selected_slots[0], menu->pane_owner[0], menu->pane_owner[1]);

	if (result == -1)
	{
		AudioCommand_PlayFar(SOUND_MENU_ERROR);
		RenderOutput_ClearListFar(menu->info_window);
		InventoryMenu_ShowModalMessage(menu->message_offset + MsgItemUseResult, result, result);
#if !EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
		menu->completion_flag = 1;
		return result;
	}

	Item_PlayUseAnimation(menu->selected_items[0] & ITEM_ID_MASK);
	UnitCalcFar(menu->pane_owner[0]);
	UnitCalcFar(menu->pane_owner[1]);
	return 1;
}

void ItemMenu_Init(s32 x, s32 y, s32 mode, s32 columns)
{
	struct InventoryMenuState *menu = gMenuWork;
	s32						   index;

	PsynergyMenu_InitializeEntryObjects((void *)UiMenu_CreateCursor(menu), 2, 2, 8, 0);
	for (index = 3; index >= 0; index--)
		menu->owner_y[index] = 30;

	{
		u8	zero = 0;
		s32 style;
		menu->help_window = NULL;
		menu->status_window = NULL;
		style = 2;
		menu->info_window = UiWindow_CreateFar(0, 17, 30, 3, style);
		menu->item_window = NULL;
		menu->selected_column = zero;
		menu->selected_row = zero;
		menu->column_count = 8;
		menu->row_count = style;
	}
}

void Menu_SpawnIconEntries(struct InventoryMenuState *state, s32 arg1)
{
	struct RenderOutput **output0;
	s32					  index0;
	s32					  fifth0;
	struct RenderOutput **output1;
	s32					  index1;
	s32					  fifth1;
	struct RenderOutput **output2;
	s32					  index2;
	s32					  fifth2;

	index0 = 0;
	fifth0 = 0xA8;
	output0 = &state->entry_icons[0];
	do
	{
		*output0++ =
			RenderOutput_CreateFromResourceFar(2, index0, (struct RenderInput *)arg1, 0xF8, fifth0);
		index0++;
	} while (index0 <= 7);

	index1 = 8;
	fifth1 = 0xA8;
	output1 = &state->entry_icons[8];
	do
	{
		*output1++ = RenderOutput_CreateFromResourceFar(
			2, index1, (struct RenderInput *)arg1, 0x100, fifth1);
		index1++;
	} while (index1 <= 15);

	index2 = 16;
	fifth2 = 0xA8;
	output2 = &state->entry_icons[16];
	do
	{
		*output2++ = RenderOutput_CreateFromResourceFar(
			2, index2, (struct RenderInput *)arg1, 0x100, fifth2);
		index2++;
	} while (index2 <= 31);
}

void ItemMenu_HideAllIcons(void)
{
	s32					  hidden_state = 13;
	struct RenderOutput **icons = ((struct InventoryMenuState *)gMenuWork)->entry_icons;
	s32					  slot;

	for (slot = 31; slot >= 0; slot--)
	{
		struct RenderOutput *icon = *icons++;
		if (icon != 0)
		{
			icon->active = hidden_state;
		}
	}
}

#if EDITION_INTERNATIONAL
void ItemMenu_HidePageIcons(void)
{
	struct InventoryMenuState *menu = gMenuWork;
	s32						   slot = 0;
	s32						   hidden_state = 13;
	struct RenderOutput		 **icon_slot = menu->entry_icons;

	do
	{
		struct RenderOutput *icon = *icon_slot++;

		if (icon != 0 && slot % 5 == 0)
		{
			icon->active = hidden_state;
		}
		slot++;
	} while (slot <= 31);
}
#endif

void ItemMenu_Close(void)
{
	struct InventoryMenuState *menu;
	struct RenderOutput		  *cursor;

	menu = gMenuWork;
	Menu_ReleaseEntryObjects();
	ItemMenu_HideAllIcons();
	WaitFrames(1);
	cursor = menu->cursor;
	cursor->active = 0xD;
	UiWindow_CloseIfOpen(&menu->main_window, 1);
	UiWindow_CloseIfOpen(&menu->item_window, 1);
	UiWindow_CloseIfOpen(&menu->message_window, 1);
	UiWindow_CloseIfOpen(&menu->status_window, 1);
	UiWindow_CloseIfOpen(&menu->help_window, 1);
	UiWindow_CloseIfOpen(&menu->info_window, 1);
	UiWindow_CloseIfOpen(&menu->equip_window, 1);
	UiWindow_CloseIfOpen(&menu->list_window, 1);
	UiWindow_CloseIfOpen(&menu->unknown_window_38, 1);
	UiWindow_CloseIfOpen(&menu->modal_window, 1);
	UiWindow_CloseIfOpen(&menu->unknown_window_40, 1);
}

s32 ItemMenu_PrepOwner(s32 pane)
{
	struct InventoryMenuState *menu = gMenuWork;
	s32						   result = 0;
	s32						   index;
	struct BtlUnit			  *owner;

	index = menu->pane_index[pane];
	RenderOutput_ClearListFar(menu->info_window);
	menu->pane_count[pane] = menu->party_count;
	if (index == -1)
	{
		menu->pane_index[pane] = result;
		index = 0;
	}
	else
	{
		UiMenu_SlideCursor(index * 24 - 10, 16);
	}
	owner = UnitGetFar(menu->owner_ids[index]);
	menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
	result = ItemMenu_RunOwnerSelection(menu->owner_ids, menu->items);
	UiIcon_PrepareObject(menu->pane_icons[pane]);
	WaitFrames(1);
	return result;
}

s32 ItemMenu_RunOwnerSelection(u16 *owner_ids, u16 *items)
{
	struct InventoryMenuState *menu;
	s32						   selection;
	s32						   count;
	s32						   result;
	struct BtlUnit			  *owner;
	u8						   sort_mode;
	s32						   by_category;
	s32						   pending;
	s32						   window;
	s32						   i;

	menu = gMenuWork;
	selection = menu->pane_index[0];
	count = menu->pane_count[0];
	pending = 1;
	result = 0;
	sort_mode = 0;
	by_category = 0;
	owner = UnitGetFar(owner_ids[selection]);
	if (UiWindow_UpdateOrCreate((s32 *)&menu->item_window, 13, 3, 17, 10, 2))
		Menu_SpawnIconEntries(menu, (s32)menu->item_window);
	if (UiWindow_UpdateOrCreate((s32 *)&menu->help_window, 13, 13, 17, 4, 2))
	{
		menu->selected_item_icon = RenderOutput_CreateFromResourceFar(
			2, 0, (struct RenderInput *)menu->help_window, 0, result);
		menu->selected_item_icon->active = 13;
	}
#if EDITION_INTERNATIONAL
	UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp, (s32)menu->help_window, 0, 0);
	UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp + 1, (s32)menu->help_window, 0, 8);
#else
	UiText_DrawStringInWindowFar(ItemMenu_ArrangeKeysString, (s32)menu->help_window, 0, 0);
	UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp, (s32)menu->help_window, 32, 0);
	UiText_DrawStringInWindowFar(ItemMenu_EquipmentKeyString, (s32)menu->help_window, 16, 8);
	UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp + 1, (s32)menu->help_window, 32, 8);
#endif
	menu->pane_icons[0]->active = pending;
	while (!GameFlag_TestFar(0x150))
	{
		selection = (selection + count) % count;
		UiMenu_PositionCursor(selection * 24 - 10, 16);
		if (pending)
		{
			sort_mode = 0;
			pending = 0;
			window = (s32)menu->status_window;
			owner = UnitGetFar(owner_ids[selection]);
			if (by_category)
			{
				menu->item_count =
					ItemMenu_Collect(UnitGetFar(owner_ids[selection]), menu->items, 0);
				ItemMenu_OpenCategory(owner_ids[selection]);
				Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 8);
			}
			else
			{
				ItemMenu_RefreshOwner(owner_ids[selection], 0);
				Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 0);
			}
			for (i = 3; i >= 0; i--)
				menu->owner_y[i] = 30;
			menu->owner_y[selection] = 26;
		}
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			if (gKeysHeld & KEY_L)
			{
				sort_mode = (sort_mode + 4) % 4;
				InventoryMenu_SortByListOrder(owner->inventory, sort_mode);
				sort_mode++;
				ItemMenu_RefreshOwner(owner_ids[selection], 0);
				AudioCommand_PlayFar(112);
			}
			else if (ItemMenu_Count(owner_ids[selection]))
			{
				AudioCommand_PlayFar(112);
				result = owner_ids[selection];
				break;
			}
			else
			{
				AudioCommand_PlayFar(114);
			}
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		if (gKeyState & KEY_R)
		{
			by_category = 1;
			pending = 1;
		}
		if (!(gKeysHeld & KEY_R) && by_category == 1)
		{
			by_category = 0;
			pending = 1;
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			AudioCommand_PlayFar(111);
			selection--;
			pending = 1;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			AudioCommand_PlayFar(111);
			selection++;
			pending = 1;
		}
	}
	menu->pane_index[0] = selection;
	menu->selected_owner = owner_ids[selection];
	menu->pane_owner[0] = owner_ids[selection];
	return result;
}

void ItemMenu_OpenCategory(s32 owner_id)
{
	struct InventoryMenuState *menu;

	menu = gMenuWork;
	ItemMenu_PosCategory();
	RenderOutput_RedrawSavedRectFar(menu->item_window);
	ItemMenu_DrawCategory((s32)menu->item_window, owner_id, 0);
}

s32 ItemMenu_SelectTarget(s32 mode)
{

	struct InventoryMenuState *menu;
	s32						   window;
	s32						   count;
	s32						   selection;
	s32						   pending;
	u8						   result;
	s32						   shown;
	s32						   quantity;
	struct RenderOutput		  *marker;

	menu = gMenuWork;
	window = (s32)menu->item_window;
	selection = menu->pane_index[1];
	count = menu->party_count;
	pending = 1;
	result = 0;
	shown = 0;
	UiWindow_SetBounds((struct RenderInput *)window, 13, 5, 17, 12);
	RenderOutput_RedrawSavedRectFar((void *)menu->item_window);
	UnitGetFar(menu->owner_ids[menu->pane_index[0]]);
	Scheduler_AddOrUpdateCallback(EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
	while (!GameFlag_TestFar(0x150))
	{
		if (pending)
		{
			pending = 0;
			selection = (selection + count) % count;
			window = (s32)menu->item_window;
			UnitGetFar(menu->owner_ids[selection]);
			marker = menu->pane_icons[1];
			marker->oam.x = marker->x = ((menu->main_window->x + selection * 3) << 3) - 2;
			if (mode == 1)
			{
				ItemMenu_RefreshOwner(menu->owner_ids[selection], 1);
				UiWindow_DrawDividerLineFar(window, 0, 9, 16, 9);
				UiWindow_ClearInteriorTilesFar((void *)window, 0, 72, 120, 80);
				if (selection != menu->pane_index[0])
				{
					quantity = InventoryMenu_GetItemQuantity(
						menu->owner_ids[selection], menu->selected_items[0] & ITEM_ID_MASK);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
					if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
						UiText_DrawCharacterAtOffsetFar(MsgTradeForWhat, window, 0, 72);
					else if (quantity != 0)
					{
						UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
						UiText_DrawCharacterAtOffsetFar(MsgInStock, window, STOCK_LABEL_X, 72);
					}
					else
					{
						UiText_DrawCharacterAtOffsetFar(MsgNoneInStock, window, 16, 72);
					}
#else
					if (quantity != 0)
					{
						UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
						UiText_DrawCharacterAtOffsetFar(MsgInStock, window, STOCK_LABEL_X, 72);
					}
					else
					{
						UiText_DrawCharacterAtOffsetFar(MsgNoneInStock, window, 16, 72);
					}
					if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
#if EDITION_INTERNATIONAL
						UiText_DrawCharacterAtOffsetFar(MsgTradeForWhat, window, 0, 72);
#else
						UiText_DrawCharacterAtOffsetFar(MsgTradeForWhat, window, 16, 72);
#endif
#endif
				}
				ItemMenu_DrawEquipPreview(
					menu->pane_owner[0], menu->selected_slots[0], 0, menu->owner_ids[selection]);
			}
			if (mode == 0)
			{
				if (ItemMenu_IsSpecial(menu->selected_items[0] & ITEM_ID_MASK))
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->owner_ids[selection],
						menu->selected_slots[0], 8);
				else
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->owner_ids[selection],
						menu->selected_slots[0], 0);
				if (!GameFlag_TestFar(0x151) && !shown)
				{
#if EDITION_INTERNATIONAL
					RenderOutput_RedrawSavedRectFar((void *)menu->info_window);
					UiText_DrawCharacterAtOffsetFar(
						(menu->selected_items[0] & ITEM_ID_MASK) + MsgItemPlainName,
						(s32)menu->info_window, 0, 0);
#else
					RenderOutput_ClearListFar((void *)menu->info_window);
					UiText_DrawResourceFar(
						(menu->selected_items[0] & ITEM_ID_MASK) + MsgItemPlainName,
						(s32)menu->info_window, 0, 0);
#endif
					shown = 1;
				}
				else
				{
					GameFlag_ClearBitFar(0x151);
				}
			}
		}
		UiMenu_PositionCursor(selection * 24 - 10, 16);
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			if (mode == 1 && selection == menu->pane_index[0])
			{
				AudioCommand_PlayFar(114);
				continue;
			}
			AudioCommand_PlayFar(112);
			result = menu->owner_ids[selection];
			break;
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			result = 255;
			break;
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			AudioCommand_PlayFar(111);
			selection--;
			pending = 1;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			AudioCommand_PlayFar(111);
			pending = 1;
			selection++;
		}
	}
	marker = menu->pane_icons[1];
	menu->pane_index[1] = selection;
	UiIcon_PrepareObject(marker);
	marker->active = 13;
	EquipmentMenu_StartCompatibilityIndicators();
	WaitFrames(1);
	menu->pane_index[1] = selection;
	menu->selected_owner = menu->owner_ids[selection];
	menu->pane_owner[1] = menu->owner_ids[selection];
	return (s8)result;
}

void EquipmentMenu_UpdateCompatibilityIndicators(void)
{
	struct InventoryMenuState *menu;
	s8						   member_index;

	menu = gMenuWork;
	if ((gFrameCount & 31) == 0 && menu->party_count != 0)
	{
		member_index = 0;
		do
		{
			if (Item_CanOwnerEquip(
					menu->owner_ids[member_index], menu->selected_items[0] & ITEM_ID_MASK)
				!= 0)
			{
				struct AnimationObject *indicator = menu->owner_objects[member_index];
				AnimationObjects_SelectAnimationFar(indicator, 3);
			}
			else
			{
				struct AnimationObject *indicator = menu->owner_objects[member_index];
				AnimationObjects_SelectAnimationFar(indicator, 1);
			}
			member_index++;
		} while (member_index < menu->party_count);
	}
}

void EquipmentMenu_StartCompatibilityIndicators(void)
{
	struct InventoryMenuState *menu;
	s8						   member_index;

	menu = gMenuWork;
	if (menu->party_count != 0)
	{
		member_index = 0;
		do
		{
			struct AnimationObject *indicator = menu->owner_objects[member_index];
			AnimationObjects_SelectAnimationFar(indicator, 1);
			member_index++;
		} while (member_index < menu->party_count);
	}
	Scheduler_RemoveCallback(&EquipmentMenu_UpdateCompatibilityIndicators);
}

s32 ItemMenu_IsSpecial(s32 item_id)
{
	switch (item_id)
	{
	case 0xC1:
	case 0xC2:
	case 0xC3:
	case 0xC4:
		return 1;
	}
	return 0;
}

void ItemMenu_DrawMsg(s32 unused, s32 message)
{
	struct InventoryMenuState *menu;

	menu = gMenuWork;
	RenderOutput_RedrawSavedRectFar(menu->message_window);
	UiText_DrawCharacterAtOffsetFar(message, (s32)menu->message_window, 0, 0);
}

void Menu_HideEmptyEntryIcons(const u16 *items)
{
	struct InventoryMenuState *menu = gMenuWork;
	s32						   slot;

	for (slot = 0; slot < 32; slot++)
	{
		if (items[slot] == 0)
		{
			UiIcon_PrepareObject(menu->entry_icons[slot]);
			menu->entry_icons[slot]->active = 13;
		}
	}
}

s32 ItemMenu_Count(s32 owner_id)
{
	s32	 item_id;
	s32	 remaining;
	s32	 count;
	u16 *slots;

	count = 0;
	slots = UnitGetFar(owner_id)->inventory;
	remaining = 0xE;
	do
	{
		item_id = ITEM_ID_MASK & *slots;
		slots += 1;
		if (item_id != 0)
		{
			count += 1;
		}
		remaining -= 1;
	} while (remaining >= 0);
	return count;
}

s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item)
{
	struct BtlUnit *state;
	s32				i;
	s32				quantity;

	quantity = 0;
	state = UnitGetFar(owner);
	i = 0;
	do
	{
		if (state->inventory[i] != 0 && (state->inventory[i] & ITEM_ID_MASK) == item)
		{
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
			quantity += (state->inventory[i] & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
			quantity++;
#else
			quantity = (state->inventory[i] & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
			quantity++;
			break;
#endif
		}
		i++;
	} while (i <= 14);
	return quantity;
}

s32 ItemMenu_Collect(struct BtlUnit *owner, u16 *items, s32 mode)
{
	s32 count;
	s32 i;

	for (i = 0; i < 32; i++)
	{
		items[i] = 0;
	}
	count = 0;
	for (i = 0; i < 15; i++)
	{
		items[i] = 0;
		if (owner->inventory[i] != 0)
		{
			items[count] = owner->inventory[i];
			count++;
		}
	}
	return count;
}

void ItemMenu_DrawIcons(u16 *items, s32 style)
{
	s32					  remaining;
	u16					 *entries;
	struct RenderOutput **icons;
	s32					  item_id;

	icons = ((struct InventoryMenuState *)gMenuWork)->entry_icons;
	entries = items;
	remaining = 14;
	do
	{
		item_id = *entries++;
		if (item_id != 0)
		{
			if (style == 0)
			{
				Resource_LoadByModeIntoSlotFar(2, item_id, (u8)(*icons)->index, 0);
			}
			else
			{
				Resource_LoadByModeIntoSlotFar(7, item_id, (u8)(*icons)->index, 0);
			}
		}
		icons++;
		remaining--;
	} while (remaining >= 0);
	Menu_HideEmptyEntryIcons(items);
}

void ItemMenu_RefreshOwner(s32 owner_id, s32 mode)
{
	struct InventoryMenuState *menu;
	struct BtlUnit			  *owner;
	u16						  *items;

	menu = gMenuWork;
	owner = UnitGetFar(owner_id);
	items = menu->items;
	menu->item_count = ItemMenu_Collect(owner, items, 0);
	RenderOutput_RedrawSavedRectFar(menu->item_window);
	ItemMenu_RefreshEntry(mode);
	ItemMenu_DrawIcons(items, 0);
	if (ItemMenu_Count(owner_id) == 0)
		UiText_DrawCharacterAtOffsetFar(MsgItemMenuEmpty, (s32)menu->item_window, 8, 24);
}

void InventoryMenu_NoOp(void)
{
}

void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target)
{
	/* FAKEMATCH: Reusing the expired owner value for the saved unit keeps
       owner in r8 and style in r10; a separate saved pointer swaps those
       registers (44 aligned instructions differ). The type-6 buffer alias
       preserves the native shared draw tail; without it the body adds
       three instructions and seven aligned instructions differ. */
	struct InventoryMenuState *menu = gMenuWork;
	s32						   style = 0;
	struct BtlUnit			  *state = UnitGetFar(owner);
	s32						   item = state->inventory[slot];
	s32						   equipped;

	if (mode == 1)
		style = 0x100;
	switch (Item_GetDirectFar(item & ITEM_ID_MASK)->type)
	{
	case 0:
		Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 7:
	case 8:
	case 9:
		if (owner == target)
		{
			style |= 2;
			Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
		}
		else
		{
			s32 size;

			state = UnitGetFar(target);
			size = BATTLE_UNIT_SIZE;
			owner = (s32)MemAlloc(size);
			Iwram_CopyWords((void *)owner, state, size);
			equipped = Inventory_RemoveFirstUnflagged(target);
			if (equipped != 0)
			{
				item &= ~INVENTORY_EQUIPPED;
				equipped = Inventory_AddItemFar(target, item);
				if (equipped != -1)
				{
					style |= 2;
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, equipped, style);
				}
				else
				{
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
				}
			}
			else
			{
				Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
			}
			Iwram_CopyWords(state, (void *)owner, size);
			MemFree((void *)owner);
		}
		break;
	case 6:
		if (target == owner)
		{
			style |= 4;
			Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
		}
		else
		{
			s32	  size;
			void *buffer;

			state = UnitGetFar(target);
			size = BATTLE_UNIT_SIZE;
			owner = (s32)MemAlloc(size);
			buffer = (void *)owner;
			Iwram_CopyWords(buffer, state, size);
			equipped = Inventory_RemoveFirstUnflagged(target);
			if (equipped != 0)
			{
				equipped = Inventory_AddItemFar(target, item);
				if (equipped != -1)
				{
					style |= 4;
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, equipped, style);
				}
				else
				{
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
				}
			}
			else
			{
				Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
			}
			Iwram_CopyWords(state, (void *)owner, size);
			buffer = (void *)owner;
			MemFree(buffer);
		}
		break;
	}
}

s32 Inventory_RemoveFirstUnflagged(s32 owner)
{
	struct BtlUnit *state = UnitGetFar(owner);
	s32				result = 0;
	s32				i;

	for (i = 0; i < 15; i++)
	{
		if (state->inventory[i] == 0)
		{
			result = 1;
			break;
		}
		if ((state->inventory[i] & INVENTORY_EQUIPPED) == 0)
		{
			s32 quantity = state->inventory[i] >> INVENTORY_QUANTITY_SHIFT;
			s32 count = quantity + 1;
			s32 cnt;

			if (quantity == 0)
				count = 1;
			for (cnt = 0; cnt < count; cnt++)
				result = Inventory_RemoveFar(owner, i);
			if (result != 2)
				return 0;
			result = 1;
			break;
		}
	}
	return result;
}

#if EDITION_INTERNATIONAL

s32 ItemMenu_CmdCursorX(s32 column, s32 row)
{
	if (column > 2 || row > 2 || column < 0 || row < 0)
		return 0;
	return ItemMenu_CommandColumnXTable[row * 3 + column];
}

s32 ItemMenu_CmdCursorY(s32 column, s32 row)
{
	s32 y;

	(void)column;
	y = 0x1E;
	if (row != 0)
	{
		y = 0x26;
	}
	return y;
}
#endif

s32 ItemMenu_ChooseCmd(void)
{
	struct InventoryMenuState *menu;
	s8						   command_storage[6];
	u16						  *redraw_flag;
	s32						   col;
	s32						   saved;
	s32						   row;
	s32						   index;
	s32						   need_redraw;
	s8						  *command_states;
	s32						   x;
	s32						   y;

	index = 0;
	col = 0;
	row = 0;
	menu = gMenuWork;
	need_redraw = 1;
	command_states = command_storage;
	ItemMenu_BuildCmd(command_states);
	redraw_flag = &menu->flags;
	if (*redraw_flag != 1)
	{
		s32 message_window;

		ItemMenu_HideAllIcons();
		RenderOutput_RedrawSavedRectFar((void *)menu->list_window);
		message_window = (s32)menu->message_window;
		ItemMenu_SetMsgWin7();
		RenderOutput_RedrawSavedRectFar((void *)message_window);
		UiWindow_DrawDividerLineFar(message_window, 0, 3, 0x10, 3);
		ItemMenu_DrawItemHead();
		ItemMenu_DrawCmd(command_states, message_window);
#if EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar((void *)menu->info_window);
		UiText_DrawCharacterAtOffsetFar((menu->selected_items[0] & ITEM_ID_MASK) + MsgItemPlainName,
			(s32)menu->info_window, 0, 0);
#else
		RenderOutput_ClearListFar((void *)menu->info_window);
		UiText_DrawResourceFar((menu->selected_items[0] & ITEM_ID_MASK) + MsgItemPlainName,
			(s32)menu->info_window, 0, 0);
#endif
	}
	*redraw_flag = 0;

	saved = (s8)menu->list_mode;
	if (saved == -1)
	{
		if (command_states[2] == 1)
		{
			col = 2;
			row = 0;
		}
		if (command_states[3] == 1)
		{
			col = 0;
			row = 1;
		}
		if (command_states[1] == 1)
		{
			col = 1;
			row = 0;
		}
		if (command_states[4] == 1)
		{
			col = 1;
			row = 1;
		}
		if (command_states[0] == 1)
		{
			col = 0;
			row = 0;
		}
	}
	else
	{
		col = (s8)(saved % 3);
		row = (s8)(saved / 3);
		index = row * 3 + col;
	}
#if EDITION_INTERNATIONAL
	x = ItemMenu_CmdCursorX(col, row);
	y = ItemMenu_CmdCursorY(col, row);
	UiMenu_SlideCursor(x, y);
#else
	UiMenu_SlideCursor(col * 40 + 86, row * 8 + 30);
#endif

	for (;;)
	{
		if (GameFlag_TestFar(0x150) != 0)
			break;
		if (need_redraw != 0)
		{
			need_redraw = 0;
			col = (col + 3) % 3;
			row = (row + 2) % 2;
			index = row * 3 + col;
			EquipmentMenu_StartCompatibilityIndicators();
			if (index > 2)
			{
				menu->equip_preview = 1;
				ItemMenu_DrawEquipPreview(
					menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
				if (index == 3)
				{
					Scheduler_AddOrUpdateCallback(
						&EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
				}
			}
			else if (index != 0)
			{
				menu->equip_preview = 0;
				ItemMenu_DrawEquipPreview(
					menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
			}
			else
			{
				Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->pane_owner[0], 0, 0);
			}
		}
#if EDITION_INTERNATIONAL
		x = ItemMenu_CmdCursorX(col, row);
		y = ItemMenu_CmdCursorY(col, row);
		UiMenu_PositionCursor(x, y);
#else
		UiMenu_PositionCursor(col * 40 + 86, row * 8 + 30);
#endif
		WaitFrames(1);

		if ((gKeyState & KEY_A) != 0)
		{
			if (command_states[index] == -1)
			{
				AudioCommand_PlayFar(114);
			}
			else
			{
				switch (index)
				{
				case 0:
					AudioCommand_PlayFar(174);
					break;
				case 1:
					AudioCommand_PlayFar(175);
					break;
				case 2:
				case 3:
				case 5:
					AudioCommand_PlayFar(112);
					break;
				case 4:
					AudioCommand_PlayFar(117);
					break;
				default:
					AudioCommand_PlayFar(112);
					break;
				}
				menu->list_mode = index;
				break;
			}
		}
		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(113);
			index = -1;
			menu->list_mode = index;
			break;
		}

		if ((gKeysRepeat & KEY_UP) != 0)
		{
			row -= 1;
			need_redraw = 1;
			AudioCommand_PlayFar(111);
		}
		else if ((gKeysRepeat & KEY_DOWN) != 0)
		{
			row += 1;
			need_redraw = 1;
			AudioCommand_PlayFar(111);
		}
		else if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			col += 1;
			need_redraw = 1;
			AudioCommand_PlayFar(111);
		}
		else if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			col -= 1;
			need_redraw = 1;
			AudioCommand_PlayFar(111);
		}
	}

	menu->equip_preview = 0;
	EquipmentMenu_StartCompatibilityIndicators();
	return index;
}

#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1

void ItemMenu_BuildCmd(s8 *command_states)
{
	struct InventoryMenuState *menu;
	struct ItemDefinition	  *item;

	menu = gMenuWork;
	item = Item_GetDirectFar(ITEM_ID_MASK & menu->selected_items[0]);

	if (item->type == 0)
	{
		command_states[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;
		command_states[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
	}
	else
	{
		command_states[ITEM_COMMAND_USE] = COMMAND_DISABLED;
		command_states[ITEM_COMMAND_EQUIP] = COMMAND_AVAILABLE;
	}

	if (Item_ClassifyUseMode(menu->pane_owner[0], menu->selected_items[0]) != -1)
		command_states[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;
	else
		command_states[ITEM_COMMAND_USE] = COMMAND_DISABLED;

	if (menu->selected_items[0] & INVENTORY_BROKEN)
		command_states[ITEM_COMMAND_USE] = COMMAND_DISABLED;

	if (Item_CanOwnerEquip(menu->pane_owner[0], menu->selected_items[0] & ITEM_ID_MASK) == 0)
	{
		command_states[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
	}

	command_states[ITEM_COMMAND_GIVE] = COMMAND_AVAILABLE;
	command_states[ITEM_COMMAND_DROP] = COMMAND_AVAILABLE;
	command_states[ITEM_COMMAND_INSPECT] = COMMAND_AVAILABLE;

	if (menu->selected_items[0] & INVENTORY_EQUIPPED)
	{
		command_states[ITEM_COMMAND_REMOVE] = COMMAND_AVAILABLE;
		command_states[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
	}
	else
	{
		command_states[ITEM_COMMAND_REMOVE] = COMMAND_DISABLED;
	}

	if (item->flags & ITEM_CANNOT_UNEQUIP)
	{
		command_states[ITEM_COMMAND_REMOVE] = COMMAND_DISABLED;
		if (menu->selected_items[0] & INVENTORY_EQUIPPED)
		{
			command_states[ITEM_COMMAND_GIVE] = COMMAND_DISABLED;
			command_states[ITEM_COMMAND_DROP] = COMMAND_DISABLED;
		}
	}

	if (BattleFx_HasTriggerFar(menu->selected_items[0] & ITEM_ID_MASK) != 0)
		command_states[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;

	if (menu->party_count <= 1)
		command_states[ITEM_COMMAND_GIVE] = COMMAND_DISABLED;

	if (item->flags & ITEM_INDISPENSABLE)
		command_states[ITEM_COMMAND_DROP] = COMMAND_DISABLED;
}

#if EDITION_INTERNATIONAL
#define ITEM_TEXT_X 0x20
#else
#define ITEM_TEXT_X 0x28
#endif

void ItemMenu_DrawCmd(const s8 *command_states, s32 window)
{
	s32 disabled;
	s32 value;
	u32 message;

	UiWork_SetParamNibbleFar(0xf);
	value = command_states[ITEM_COMMAND_USE];
	disabled = -1;
	if (value == disabled)
		UiWork_SetParamNibbleFar(0xe);

	message = MsgItemCommandUse;
	UiText_DrawCharacterAtOffsetFar(message, window, 0, 0x18);
	UiWork_SetParamNibbleFar(0xf);
	if (command_states[ITEM_COMMAND_EQUIP] == disabled)
		UiWork_SetParamNibbleFar(0xe);

	UiText_DrawCharacterAtOffsetFar(message + 1, window, ITEM_TEXT_X, 0x18);
	UiWork_SetParamNibbleFar(0xf);
	if (command_states[ITEM_COMMAND_GIVE] == disabled)
		UiWork_SetParamNibbleFar(0xe);

	UiText_DrawCharacterAtOffsetFar(message + 2, window, 0, 0x20);
	UiWork_SetParamNibbleFar(0xf);
	if (command_states[ITEM_COMMAND_DROP] == disabled)
		UiWork_SetParamNibbleFar(0xe);

	UiText_DrawCharacterAtOffsetFar(message + 3, window, 0x50, 0x20);
	UiWork_SetParamNibbleFar(0xf);
	if (command_states[ITEM_COMMAND_INSPECT] == disabled)
		UiWork_SetParamNibbleFar(0xe);

	UiText_DrawCharacterAtOffsetFar(message + 4, window, 0x50, 0x18);
	UiWork_SetParamNibbleFar(0xf);
	if (command_states[ITEM_COMMAND_REMOVE] == disabled)
		UiWork_SetParamNibbleFar(0xe);

	UiText_DrawCharacterAtOffsetFar(message + 5, window, ITEM_TEXT_X, 0x20);
	UiWork_SetParamNibbleFar(0xf);
}

s32 Item_ClassifyUseMode(s32 owner, s32 itemId)
{
	s32					   masked = itemId;
	struct ItemDefinition *def;
	s32					   result = -1;

	masked &= ITEM_ID_MASK;
	def = Item_GetDirectFar(masked);

	if (BattleFx_HasTriggerFar(masked) != 0)
	{
		return 0;
	}

	{
		struct BtlAct *action = BtlActGet(def->action_id & OWNER_ACTION_ID_MASK);

		if (def->action_id != 0)
		{
			if (def->type != 0)
			{
				if (def->use_type != 3)
				{
					if (Item_CanOwnerEquip(owner, masked) != 0)
					{
						result = 1;
					}
				}
			}
			else
			{
				result = 1;
			}

			if (result == 1)
			{
				s32 mask = 0x80;
				u8	flags = action->tgt_flg;

				if ((flags & 0x40) != 0)
				{
					u8 range = action->rng;
					result = (range == 0xff) ? 2 : 1;
				}
				else
				{
					result = (flags & mask) ? -1 : 0;
				}
			}
		}
	}

	return result;
}

void ItemMenu_TryBreak(void)
{
	struct InventoryMenuState *menu;

	menu = gMenuWork;
	if ((Item_GetDirectFar(ITEM_ID_MASK & menu->selected_items[0])->use_type == 2)
		&& (Random16() < 0x2000U))
	{
		Inventory_BreakFar(menu->pane_owner[0], menu->selected_slots[0]);
		AudioCommand_PlayFar(SOUND_ITEM_BREAK);
		InventoryMenu_ShowModalMessage(MsgItemBroke, 0, -1);
#if !EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
	}
}

s32 ItemMenu_OpenDetail(s32 item_index)
{
	struct InventoryMenuState *menu = gMenuWork;

	UiWindow_UpdateOrCreate((s32 *)&menu->equip_window, 0, 0, 13, 10, 2);
	Palette_CopyObjectBankToBackground14();

	if (menu->selected_items[item_index] != 0)
		ItemMenu_DrawItemDetails((s32)menu->equip_window, menu->selected_items[item_index]);

	return 1;
}

s32 Menu_SelectQuantity(s32 value)
{
#if EDITION_INTERNATIONAL
	s32						   changed = 1;
	struct InventoryMenuState *menu = gMenuWork;
	struct RenderOutput		  *confirmState = menu->selected_item_icon;
#else
	struct InventoryMenuState *menu = gMenuWork;
	s32						   changed = 1;
#endif
	s32 window;
	s32 quantity = 0;

#if EDITION_INTERNATIONAL
	confirmState->active = 13;
	window = UiWindow_CreateFar(0, 0, 30, 10, 2);
#else
	window = UiWindow_CreateFar(0, 0, 13, 10, 2);
#endif
	Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);

	{
		struct RenderOutput *iconState = menu->cursor;
		iconState->active = 13;
	}
	Palette_CopyObjectBankToBackground14();
	WaitFrames(1);

	for (;;)
	{
		if (GameFlag_TestFar(336) != 0)
			break;

		if (changed != 0)
		{
			changed = 0;
			quantity = (quantity + 5) % 5;
			ItemMenu_DrawItemDetails(window, value);
		}

		if (gKeyState & 1)
			break;
		if (gKeyState & 2)
		{
			quantity = -1;
			break;
		}

		if (gKeysRepeat & 0x40)
		{
			quantity -= 1;
			changed = 1;
		}
		if (gKeysRepeat & 0x80)
		{
			quantity += 1;
			changed = 1;
		}
		WaitFrames(1);
	}

	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	WaitFrames(1);
	UiWork_FinalizeFar((void *)window, 1);
	RenderOutput_RedrawSavedRectFar(menu->main_window);
	Palette_LightenBankHighlight(14);
	{
		s32 delay = 0xc80;

		Scheduler_AddOrUpdateCallback((const void *)Menu_UpdateEntryObjectTransforms, delay);
	}

	{
		struct RenderOutput *iconState = menu->cursor;
#if EDITION_INTERNATIONAL
		iconState->active = 1;
#else
		u8 one = 1;

		iconState->active = one;
		menu->flags = one;
#endif
	}
#if EDITION_INTERNATIONAL
	UiWindow_DrawFrameFar(13, 0, 17, 10);
#endif

	return quantity;
}

void ItemMenu_DrawItemDetails(s32 window, s32 item)
{
	struct ItemDefinition *def;
	s32					   i;
	s32					   found;
	s32					   amount;
	s32					   consumable;
	s8					   row;

	row = 0;
	consumable = 0;
	def = Item_GetDirectFar(item & ITEM_ID_MASK);
	if (def->type != 0)
	{
		found = 0;
		if (def->primary_bonus == 0 && def->secondary_bonus == 0)
		{
			for (i = 0; i < 4; i++)
			{
				if (def->effects[i].kind != 0 || def->use_type == 3)
				{
					found = 1;
					break;
				}
			}
		}
		else
		{
			found = 1;
		}
		if (found == 1)
		{
			UiText_DrawCharacterAtOffsetFar(MsgEquipEffectHeading, window, 16, 0);
			row = 1;
		}
		if (def->primary_bonus != 0)
		{
			UiText_DrawCharacterAtOffsetFar(MsgStatLabel, window, 0, row * 8);
			amount = def->primary_bonus;
			ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
			row++;
		}
		if (def->secondary_bonus != 0)
		{
			UiText_DrawCharacterAtOffsetFar(MsgDefenseLabel, window, 0, row * 8);
			amount = def->secondary_bonus;
			ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
			row++;
		}
	}
	for (i = 0; i < 4; i++)
	{
		if (def->effects[i].kind == 0)
			continue;
		amount = def->effects[i].amount;
		switch (def->effects[i].kind)
		{
		case 0:
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 26:
			UiText_DrawCharacterAtOffsetFar(
				def->effects[i].kind + MsgEquipEffectName, window, 0, row * 8);
			ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
			break;
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
			UiWindow_SetTilemapEntryFar(
				window, (u8)((def->effects[i].kind - 15) % 4) + 1, 0, row, 2);
			UiText_DrawCharacterAtOffsetFar(
				def->effects[i].kind + MsgEquipEffectName, window, 8, row * 8);
			ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
			break;
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
			UiText_DrawCharacterAtOffsetFar(
				def->effects[i].kind + MsgEquipEffectName, window, 0, row * 8);
#if EDITION_INTERNATIONAL
			UiText_DrawStringInWindowFar(Menu_TimesString, window, 64, row * 8);
			if (amount > 9)
			{
				UiText_DrawNumberInWindowFar(1, 1, window, 72, row * 8);
				UiText_DrawStringInWindowFar(Menu_PointString, window, 80, row * 8);
				UiText_DrawNumberInWindowFar(amount - 10, 1, window, 88, row * 8);
			}
			else
			{
				UiText_DrawNumberInWindowFar(0, 1, window, 72, row * 8);
				UiText_DrawStringInWindowFar(Menu_PointString, window, 80, row * 8);
				UiText_DrawNumberInWindowFar(amount, 1, window, 88, row * 8);
			}
#else
			if (amount > 9)
			{
				UiText_DrawNumberInWindowFar(1, 1, window, 48, row * 8);
				UiText_DrawStringInWindowFar(Menu_PointString, window, 56, row * 8);
				UiText_DrawNumberInWindowFar(amount - 10, 1, window, 64, row * 8);
			}
			else
			{
				UiText_DrawNumberInWindowFar(0, 1, window, 48, row * 8);
				UiText_DrawStringInWindowFar(Menu_PointString, window, 56, row * 8);
				UiText_DrawNumberInWindowFar(amount, 1, window, 64, row * 8);
			}
			UiText_DrawCharacterAtOffsetFar(MsgEffectRateSuffix, window, 72, row * 8);
#endif
			break;
		case 23:
		case 25:
		case 27:
			UiText_DrawCharacterAtOffsetFar(
				def->effects[i].kind + MsgEquipEffectName, window, 0, row * 8);
			break;
		}
		row++;
	}
	if (def->flags & ITEM_CURSED)
	{
		UiText_DrawCharacterAtOffsetFar(MsgItemCursed, window, 0, row * 8);
		row++;
	}
	if (def->use_type == 3)
	{
		UiText_DrawCharacterAtOffsetFar(MsgBestowsPsynergy, window, 0, row * 8);
		consumable = 1;
		row++;
	}
	if (def->use_type != 4 && def->use_type != 0)
	{
		if (!consumable)
		{
			UiText_DrawCharacterAtOffsetFar(MsgUsesHeading, window, 16, row * 8);
			row++;
		}
		switch (def->use_type)
		{
		case 0:
			break;
		case 1:
			UiText_DrawCharacterAtOffsetFar(MsgSingleUse, window, 0, row * 8);
			row++;
			break;
		case 2:
		{
			s32 m;

			if (item & INVENTORY_BROKEN)
			{
				m = MsgBrokenNotice;
				UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
				row++;
				m++;
				UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
				row++;
			}
			else
			{
				m = MsgMightBreak;
				UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
				row++;
				m++;
				UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
				row++;
			}
			break;
		}
		}
	}
	if (def->flags & ITEM_STACKABLE)
	{
		if (row != 0)
			row++;
		i = MsgNumberHeading;
		UiText_DrawCharacterAtOffsetFar(i, window, 16, row * 8);
		row++;
		amount = (item & INVENTORY_QUANTITY_MASK) / INVENTORY_QUANTITY_STEP;
#if EDITION_INTERNATIONAL
		UiWork_PushValueSlotFar(amount + 1, 5);
		UiText_DrawCharacterAtOffsetFar(i + 1, window, 0, row * 8);
#else
		UiText_DrawNumberInWindowFar(amount + 1, 2, window, 0, row * 8);
		UiText_DrawCharacterAtOffsetFar(i + 1, window, 16, row * 8);
#endif
		row++;
	}
	found = 0;
	if (row == 0)
	{
		if (def->flags & 4)
		{
			UiText_DrawCharacterAtOffsetFar(MsgRareItem, window, 0, 0);
			found = 1;
		}
		if (!found)
		{
			if (def->flags & ITEM_INDISPENSABLE)
			{
				UiText_DrawCharacterAtOffsetFar(MsgImportantItem, window, 0, 0);
				found = 1;
			}
			if (!found)
				UiText_DrawCharacterAtOffsetFar(MsgDetailsUnknown, window, 0, 0);
		}
	}
}

void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y)
{
	s32 digits;
	s32 magnitude;

	UiText_DrawNumberInWindowFar(delta, 3, window, x, y);
	digits = 1;
	magnitude = delta;
	if (delta < 0)
	{
		magnitude = -delta;
	}
	if (magnitude > 9)
	{
		digits = 2;
	}
	magnitude = delta;
	if (delta < 0)
	{
		magnitude = -delta;
	}
	if (magnitude > 99)
	{
		digits = 3;
	}
	if (delta > 0)
	{
		UiText_DrawStringInWindowFar(&Menu_PlusSignString, window, x - digits * 8 + 16, y);
	}
	else
	{
		UiText_DrawStringInWindowFar(&Menu_MinusSignString, window, x - digits * 8 + 16, y);
	}
}

void ItemMenu_SetItemWin5(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->item_window, 13, 5, 17, 10);
}

void ItemMenu_SetItemWin3(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->item_window, 13, 3, 17, 10);
}

void ItemMenu_SetMsgWin5(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window, 13, 0, 17,
		5);
}

void ItemMenu_SetMsgWin6(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window, 13, 0, 17,
		6);
}

void ItemMenu_SetMsgWin7(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window, 13, 0, 17,
		7);
}

void ItemMenu_SetMsgWin3(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window, 13, 0, 17,
		3);
}

#if EDITION_INTERNATIONAL
#define GIVE_CURSOR_X 128
#define GIVE_ASK_X 32
#define GIVE_ASK_Y 0
#else
#define GIVE_CURSOR_X 88
#define GIVE_ASK_X 0
#define GIVE_ASK_Y 32
#endif

s32 ItemMenu_SelectGiveQuantity(s32 base, s32 range, s32 single)
{
	s32						   quantity = base;
	s32						   slot;
	s32						   changed;
	s32						   other_count;
	s32						   count;
	struct InventoryMenuState *menu = gMenuWork;
	void					  *tiles = Runtime_AllocateBlock(14, 0x400);
	s32						   window;
	struct RenderOutput		  *object;

	changed = 1;
	other_count = 0;
	window = (s32)menu->message_window;
	ItemMenu_SetMsgWin7();
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	if (single == 0)
		other_count = InventoryMenu_GetItemQuantity(
			menu->pane_owner[1], menu->selected_items[0] & ITEM_ID_MASK);
	count =
		InventoryMenu_GetItemQuantity(menu->pane_owner[0], menu->selected_items[0] & ITEM_ID_MASK);
	slot = Resource_FindFreeEntry();
	if (slot != 96)
	{
		VramBlock_LoadCached(slot, 256, 0);
		RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 48, 32);
		object = RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 80, 32);
		object->oam.index += 4;
		UiMenu_SlideCursor(GIVE_CURSOR_X, 40);
		while (!GameFlag_TestFar(0x150))
		{
			if (changed)
			{
				changed = 0;
				quantity = (range + quantity) % range;
				RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
				UiText_DrawCharacterAtOffsetFar(MsgGiveHowMany, window, GIVE_ASK_X, GIVE_ASK_Y);
				Dma_Set(Menu_QuantityTiles, tiles, 0x84000040, REG_DMA3);
				Shop_FillSelectorFar(30, 14, tiles);
				Shop_FillSelectorFar(range + base, 0, tiles);
				Shop_FillSelectorFar(base + quantity + 1, 10, tiles);
				Shop_FillSelectorFar(base, 2, tiles);
				VramBlock_LoadCached(slot, 256, tiles);
				UiText_DrawNumberInWindowFar(quantity + 1, 2, window, 32, 32);
				UiText_DrawCharacterAtOffsetFar(
					(menu->selected_items[0] & ITEM_ID_MASK) + MsgItemName, window, 16, 8);
				UiText_DrawNumberInWindowFar(count - quantity - 1, 2, window, 16, 24);
#if EDITION_INTERNATIONAL
				if (single == 0)
					UiText_DrawNumberInWindowFar(other_count + quantity + 1, 2, window, 80, 24);
#else
				UiText_DrawCharacterAtOffsetFar(MsgItemCounter, window, 32, 24);
				if (single == 0)
				{
					UiText_DrawNumberInWindowFar(other_count + quantity + 1, 2, window, 80, 24);
					UiText_DrawCharacterAtOffsetFar(MsgItemCounter, window, 96, 24);
				}
#endif
				UiText_DrawStringAtOffsetFar(UnitGetFar(menu->pane_owner[0])->name, window, 16, 16);
				if (single == 0)
					UiText_DrawStringAtOffsetFar(
						UnitGetFar(menu->pane_owner[1])->name, window, 80, 16);
			}
			if (gKeyState & KEY_A)
			{
				AudioCommand_PlayFar(112);
				break;
			}
			if (gKeyState & KEY_B)
			{
				quantity = -1;
				AudioCommand_PlayFar(113);
				break;
			}
			UiMenu_PositionCursor(GIVE_CURSOR_X, 40);
			if (gKeysRepeat & KEY_LEFT)
			{
				quantity--;
				changed = 1;
				AudioCommand_PlayFar(111);
			}
			if (gKeysRepeat & KEY_RIGHT)
			{
				quantity++;
				changed = 1;
				AudioCommand_PlayFar(111);
			}
			WaitFrames(1);
		}
	}
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	RenderOutput_ClearListFar((void *)window);
	Runtime_ReleaseHeapBlock(14);
	menu->selected_item_icon->active = 13;
	if (GameFlag_TestFar(0x150))
		quantity = -1;
	return quantity;
}

void ItemMenu_DrawItemHead(void)
{
	struct InventoryMenuState *menu = gMenuWork;

	Resource_LoadByModeIntoSlotFar(
		2, menu->selected_items[0], (u8)menu->selected_item_icon->index, 0);
	menu->selected_item_icon->active = 1;
	menu->selected_item_icon->x = 112;
	menu->selected_item_icon->y = 8;
	UiIcon_PrepareObject(menu->selected_item_icon);
	UiText_DrawStringAtOffsetFar(
		UnitGetFar(menu->pane_owner[0])->name, (s32)menu->message_window, 16, 0);
	UiText_DrawCharacterAtOffsetFar(
		(menu->selected_items[0] & ITEM_ID_MASK) + MsgItemName, (s32)menu->message_window, 16, 8);
}

s32 ItemMenu_ConfirmDrop(s32 a0)
{
	s32 win;
	s32 slot;
	s32 text;
	s32 label;
	s32 rows = 2;
	s32 sel = 1;
	s32 changed = 1;

	win = UiWindow_CreateFar(13, 3, 17, 10, 2);
	slot = a0 & ITEM_ID_MASK;
	Item_GetDirectFar(slot);
	UiText_DrawCharacterAtOffsetFar(slot + MsgItemName, win, 24, 0);
	text = MsgConfirmDrop;
#if EDITION_INTERNATIONAL
	UiText_DrawCharacterAtOffsetFar(text, win, 0, 16);
#else
	UiText_DrawCharacterAtOffsetFar(text, win, 8, 16);
#endif
	text++;
#if EDITION_INTERNATIONAL
	UiText_DrawCharacterAtOffsetFar(text, win, 0, 24);
#else
	UiText_DrawCharacterAtOffsetFar(text, win, 8, 24);
#endif
	label = MsgYes;
	UiText_DrawCharacterAtOffsetFar(label, win, 24, 40);
	label++;
	UiText_DrawCharacterAtOffsetFar(label, win, 24, 56);
	UiMenu_SlideCursor(104, 86);
	for (;;)
	{
		if (GameFlag_TestFar(0x150))
			break;
		if (changed)
		{
			changed = 0;
			sel = (sel + rows) % rows;
		}
		if (gKeyState & KEY_A)
		{
			AudioCommand_PlayFar(112);
			break;
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			sel = 1;
			break;
		}
		UiMenu_PositionCursor(104, (sel << 4) + 70);
		if (gKeysRepeat & KEY_UP)
		{
			sel--;
			changed = 1;
			AudioCommand_PlayFar(111);
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			sel++;
			changed = 1;
			AudioCommand_PlayFar(111);
		}
		WaitFrames(1);
	}
	if (GameFlag_TestFar(0x150))
	{
		sel = 1;
	}
	UiWork_FinalizeFar((void *)win, 1);
	return sel;
}

s32 ItemMenu_ConfirmEquip(s32 unused)
{
	s32						   rows = 2;
	s32						   sel = 0;
	s32						   changed = 1;
	struct InventoryMenuState *menu = gMenuWork;
	struct BtlUnit			  *state = UnitGetFar(menu->pane_owner[1]);
	struct BtlUnit			  *saved;
	s32						   win;
	s32						   item;
	s32						   owner;

	item = menu->selected_slots[1];
	owner = menu->pane_owner[1];
	ItemMenu_DrawEquipPreview(owner, item, 0, owner);
	saved = MemAlloc(BATTLE_UNIT_SIZE);
	Iwram_CopyWords(saved, state, BATTLE_UNIT_SIZE);
	win = (s32)menu->message_window;
	if ((u32)(Inventory_EquipFar(menu->pane_owner[1], menu->selected_slots[1]) + 2) <= 1)
	{
		sel = 1;
	}
	else
	{
#if EDITION_INTERNATIONAL
		UiText_DrawCharacterAtOffsetFar(MsgYes, win, 24, 24);
		UiText_DrawCharacterAtOffsetFar(MsgYes + 1, win, 72, 24);
		UiWindow_ClearInteriorTilesFar((void *)win, 16, 16, 96, 24);
		UiText_DrawCharacterAtOffsetFar(MsgEquipThisItem, win, 0, 16);
		UiMenu_SlideCursor(110, 32);
#else
		UiText_DrawCharacterAtOffsetFar(MsgYes, win, 96, 0);
		UiText_DrawCharacterAtOffsetFar(MsgYes + 1, win, 96, 16);
		UiWindow_ClearInteriorTilesFar((void *)win, 16, 16, 96, 24);
		UiText_DrawCharacterAtOffsetFar(MsgEquipThisItem, win, 8, 16);
		UiMenu_SlideCursor(184, 5);
#endif
		for (;;)
		{
			if (GameFlag_TestFar(0x150))
				break;
			if (changed)
			{
				changed = 0;
				sel = (sel + rows) % rows;
			}
			if (gKeyState & KEY_A)
			{
				AudioCommand_PlayFar(175);
				break;
			}
			if (gKeyState & KEY_B)
			{
				AudioCommand_PlayFar(113);
				sel = 1;
				break;
			}
#if EDITION_INTERNATIONAL
			UiMenu_PositionCursor(sel * 48 + 110, 32);
			if (gKeysRepeat & KEY_LEFT)
			{
#else
			UiMenu_PositionCursor(184, (sel << 4) + 5);
			if (gKeysRepeat & KEY_UP)
			{
#endif
				sel--;
				changed = 1;
				AudioCommand_PlayFar(111);
			}
#if EDITION_INTERNATIONAL
			if (gKeysRepeat & KEY_RIGHT)
			{
#else
			if (gKeysRepeat & KEY_DOWN)
			{
#endif
				sel++;
				changed = 1;
				AudioCommand_PlayFar(111);
			}
			WaitFrames(1);
		}
	}
	if (GameFlag_TestFar(0x150))
		sel = 1;
	if (sel == 1)
		Iwram_CopyWords(state, saved, BATTLE_UNIT_SIZE);
	MemFree(saved);
	UnitCalcFar(menu->pane_owner[1]);
	Owner_RefreshClassActionsFar(menu->pane_owner[1]);
	return sel;
}

void Resource_LoadPairedBlocks(void)
{
	struct InventoryMenuState *state = gMenuWork;
	s32						   value = Resource_FindFreeEntry();

	state->resource_slots[0] = value;
	VramBlock_LoadCached(value, 128, Menu_ArrowTiles);
	value = Resource_FindFreeEntry();
	state->resource_slots[1] = value;
	VramBlock_LoadCached(value, 128, Menu_SmallDownArrowTiles);
}

s32 ItemMenu_PageResult(struct MenuResult *result, s32 index)
{
	s32						   encoded;
	struct InventoryMenuState *menu = gMenuWork;
	s32						   limit;
	s32						   remainder;
	s32						   quotient;
	s32						   groups;
	s32						   value;

	limit = ItemMenu_Count(menu->pane_owner[index]);
	encoded = (s32)UnitGetFar(menu->pane_owner[index]);
	value = menu->selected_index_by_owner[menu->pane_owner[index]];
	if ((value + 1) > limit)
	{
		value = limit - 1;
	}
	quotient = value / 5;
	remainder = value % 5;
	groups = limit / 5;
	if (limit % 5 != 0)
	{
		groups++;
	}
	result->owner_state = encoded;
	result->page = quotient;
	result->page_count = groups;
	result->row = remainder;
	result->entry_count = limit;
	result->selected_index = value;
	return 1;
}

s32 ItemMenu_DrawItemDetailPage(s32 arg0, void *arg1, struct MenuResult *state)
{
	struct InventoryMenuState *menu;
	s32						   page;
	s32						   combined;
	s32						   row;

	page = state->page;
	menu = gMenuWork;
	combined = page * 5;
	combined += state->row;
	state->selected_index = combined;

#if EDITION_INTERNATIONAL
	RenderOutput_RedrawSavedRectFar(menu->info_window);
#else
	RenderOutput_ClearListFar(menu->info_window);
#endif
	WaitFrames(1);

	combined = state->selected_index;
	if (menu->items[combined] != 0)
	{
		s32 masked = (menu->items[combined] & ITEM_ID_MASK) + MsgItemPlainName;
#if EDITION_INTERNATIONAL
		UiText_DrawCharacterAtOffsetFar(masked, (s32)menu->info_window, 0, 0);
#else
		UiText_DrawResourceFar(masked, (s32)menu->info_window, 0, 0);
#endif
	}

	row = 0;
	do
	{
		if (row == state->row)
		{
			Render_SetTilemapFlagRect((void *)menu->item_window, 1, row * 2 + 1, 14, 1, 14);
		}
		else
		{
			Render_SetTilemapFlagRect((void *)menu->item_window, 1, row * 2 + 1, 14, 1, 15);
		}
		row++;
	} while (row <= 4);

	WaitFrames(1);
	return 1;
}

#if EDITION_INTERNATIONAL
#define PAGE_X 116
#define ENTRY_X 24
#else
#define PAGE_X 120
#define ENTRY_X 32
#endif

s32 ItemMenu_DrawNamePage(s32 window, s32 unused, const struct MenuResult *state)
{
	struct InventoryMenuState *menu = gMenuWork;
	u32						   page;
	u32						   first_entry;
	u32						   visible_count;
	u8						   row;
	const u16				  *item_id;

	(void)unused;

	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

	page = state->page;
	first_entry = page * 5;
	visible_count = (u8)(state->entry_count - first_entry);
	if (visible_count > 5)
	{
		visible_count = 5;
	}

	Menu_SetPageIcons(5, first_entry, window, PAGE_X, 34);
	Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);

	row = 0;
	if (visible_count > row)
	{
		item_id = &menu->items[first_entry];
		do
		{
			UiText_DrawCharacterAtOffsetFar((item_id[0] & ITEM_ID_MASK) + MsgItemName,
				(s32)menu->item_window, ENTRY_X, row * 16 + 8);
			row++;
			item_id++;
		} while (visible_count > row);
	}

	return 1;
}

s32 InventoryMenu_ItemNamePageReturnTrue(void)
{
	return 1;
}

void InventoryMenu_ItemNamePageNoOp(void)
{
}

s32 ItemMenu_RunList(s32 pane)
{
	struct InventoryMenuState *menu;
	s32						   window;
	s32						   changed;
	s32						   nav;
	s32						   tab;
	u8						   i;
	s32						   result;
	s32						   redraw;
	s32						   done;
	struct BtlUnit			  *owner;
	s32						   prev;
	struct RenderOutput		  *icon;
	s32						   work[5];
	struct MenuResult		   state;

	menu = gMenuWork;
	result = 0;
	prev = 0;
	menu->equip_preview = result;
	UiWindow_UpdateOrCreate((s32 *)&menu->list_window, 13, 3, 17, 14, 2);
	window = (s32)menu->list_window;
	done = 0;

	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		owner = UnitGetFar(menu->pane_owner[pane]);
		menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
		ItemMenu_DrawIcons(menu->items, 0);
		menu->selected_item_icon->active = 13;
		ItemMenu_PageResult(&state, pane);
		UiMenu_PositionCursor(98, state.selected_index * 16 + 36);
		changed = 1;
		redraw = 1;

		while (GameFlag_TestFar(0x150) == 0)
		{
			UiMenu_PositionCursor(98, state.row * 16 + 36);

			if (changed != 0)
			{
				changed = 0;
				if (owner->inventory[prev] != 0)
				{
					icon = menu->entry_icons[prev];
					UiIcon_PrepareObject(icon);
				}
				if (redraw != 0)
				{
					WaitFrames(1);
					ItemMenu_DrawNamePage(window, 0, &state);
					if (pane == 0)
					{
						UiText_DrawCharacterAtOffsetFar(
							MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
					}
					redraw = 0;
				}
				ItemMenu_DrawItemDetailPage(window, work, &state);
				menu->selected_items[pane] = menu->items[state.selected_index];
				ItemMenu_DrawEquipPreview(
					menu->pane_owner[pane], state.selected_index, 0, menu->pane_owner[pane]);
				if (menu->items[state.selected_index] != 0)
				{
					icon = menu->entry_icons[state.selected_index];
					icon->active = 9;
					icon->anim_step = 0;
					icon->sentinel = 250;
				}
				for (i = 0; i < menu->party_count; i++)
				{
					AnimationObjects_SelectAnimationFar(menu->owner_objects[i], 1);
				}
			}

			if ((gFrameCount & 31) == 0)
			{
				for (i = 0; i < menu->party_count; i++)
				{
					if (Item_CanOwnerEquip(
							menu->owner_ids[i], ITEM_ID_MASK & menu->items[state.selected_index])
						!= 0)
					{
						AnimationObjects_SelectAnimationFar(menu->owner_objects[i], 3);
					}
				}
			}

			WaitFrames(1);
			prev = state.selected_index;
			nav =
				Menu_HandlePageInput(0, state.entry_count, LIST_PAGE_SIZE, &state.row, &state.page);
			if (nav == 1)
			{
				redraw = 1;
				changed = 1;
			}
			if (nav == 0)
			{
				changed = 1;
			}
			if (nav == -1)
			{
				changed = 0;
			}

			if ((gKeyState & KEY_A) != 0 && menu->items[state.selected_index] != 0)
			{
				AudioCommand_PlayFar(173);
				result = menu->items[state.selected_index];
				done = 1;
				break;
			}
			if ((gKeyState & KEY_B) != 0)
			{
				AudioCommand_PlayFar(113);
				result = -1;
				done = 1;
				break;
			}
			if ((gKeysRepeat & KEY_R) != 0 || (gKeysRepeat & KEY_L) != 0)
			{
				if (pane == 1)
				{
					AudioCommand_PlayFar(114);
					WaitFrames(1);
				}
				else
				{
					AudioCommand_PlayFar(111);
					menu->selected_index_by_owner[menu->pane_owner[pane]] = state.selected_index;
					tab = menu->pane_index[pane];
					do
					{
						if ((gKeysRepeat & KEY_R) != 0)
						{
							tab++;
						}
						else
						{
							tab--;
						}
						tab = (tab + menu->party_count) % menu->party_count;
						menu->selected_owner = menu->owner_ids[tab];
						menu->pane_owner[pane] = menu->owner_ids[tab];
						menu->pane_index[pane] = tab;
						menu->item_count =
							ItemMenu_Collect(UnitGetFar(menu->pane_owner[pane]), menu->items, 0);
					} while (menu->item_count == 0);
					for (i = 0; i <= 3; i++)
					{
						menu->owner_y[i] = 30;
					}
					menu->owner_y[tab] = 26;
					break;
				}
			}
		}
	}

	UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
	UiIcon_PrepareObject(menu->grid_cursor);
	menu->selected_slots[pane] = state.selected_index;
	menu->selected_index_by_owner[menu->pane_owner[pane]] = state.selected_index;
	menu->selected_items[pane] = result;
	if (GameFlag_TestFar(0x150) != 0)
	{
		result = -1;
	}
	WaitFrames(1);
	return result;
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
#define PROMPT_SAVES_TILES 1
#define PROMPT_TILES ((void *)0x06004000)
#define PROMPT_TILES_SIZE 0x2000
#endif

s32 Menu_OpenConfirmPrompt(void)
{
#if defined(PROMPT_SAVES_TILES)
	void *saved = Runtime_BumpAllocateAlternatePool(PROMPT_TILES_SIZE);
#endif
	struct PsynergyMenuState *state = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
	s32						  high;
	s32						  unused;
	s32						  low;
	s32						  result;

	((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 1;
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	WaitFrames(1);
	UiWindow_InitializeWork(0);
	state->owner_count = Party_ListActiveOwnersFar(state->owner_table);
	ItemMenu_Init(0, 3, 0, 7);
	state->message_window = UiWindow_CreateFar(13, 0, 17, 3, 2);
	Palette_LightenBankHighlight(14);
	Link_DrawShiftedTilePairFar(0x06002500);
#if defined(PROMPT_SAVES_TILES)
	Scheduler_EnableOverlayCallbacksWithFlags();
	Iwram_CopyWords(saved, PROMPT_TILES, PROMPT_TILES_SIZE);
	Iwram_FillWords(PROMPT_TILES, PROMPT_TILES_SIZE, 0x33333333);
	UiWork_SetAltFlagAndClearTableFar(1);
#endif
	Menu_CancelSoundReset();
	result = Menu_ResolveSelectedAction(&high, &unused, &low);
	Menu_EnsureCancelSound();
	if (result == 1)
	{
		struct EventWork *target = WORK_SLOT(HEAP_SLOT_EVENT);
		u16				  flags;
		BtlActGet(OWNER_ACTION_ID_MASK & state->pane_action[0]);
		flags = low | (high << 10);
		target->psynergy_request = flags;
	}
	RenderOutput_ClearListFar((void *)state->status_window);
	((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 1;
	ItemMenu_Close();
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	Runtime_ReleaseHeapBlock(0x37);
	((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 0;
#if defined(PROMPT_SAVES_TILES)
	UiWindow_MarkVisibleTileAttributesFar();
	UiWork_SetAltFlagAndClearTableFar(0);
	Iwram_CopyWords(PROMPT_TILES, saved, PROMPT_TILES_SIZE);
	((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 0;
	MemFree(saved);
	WaitFrames(1);
	Scheduler_DisableOverlayCallbacksWithFlags();
#endif
	WaitFrames(1);
	UiWindow_EraseBorderRectFar(0, 0, 30, 20);
	((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 0;
	Event_ClearInvalidPackedValuesFar();
	return result;
}

s32 Menu_ResolveSelectedAction(s32 *out_owner, s32 *unused, s32 *out_action)
{
	struct PsynergyMenuState *work;
	s32						  result;
	s32						  state;
	s32						  done;
	s32						  selection;
	s32						  classification;
	s32						  raw;
	s32						  self_flag;
	struct BtlAct			 *action;

	state = 0;
	done = 0;
	work = gMenuWork;
	result = 0;

	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		switch (state)
		{
		case 0:
		{
			s32 cancelled;
			s32 chosen;

			work->pane_row[0] = 0;
			ItemMenu_DrawMsg(0, MsgPsynergyChooseOwner);
			chosen = PsynergyMenu_SelectPartySlot(0);
			cancelled = -1;
			/* FAKEMATCH: the redundant cancellation copy ends the CSE block, so the cancellation and done constants take the native registers; XOR equality keeps the copy into the result. Plain forms measured 2026-10-13, each differ 12 (r2 and r3 exchanged): chosen == -1 setting result to -1 or to chosen, with either store first, the call tested directly, and chosen == cancelled. */
			if (chosen == -1)
				cancelled = chosen;
			if (((u32)chosen ^ (u32)cancelled) == 0)
			{
				done = 1;
				result = cancelled;
			}
			RenderOutput_RedrawSavedRectFar(work->info_window);
			state = 1;
			break;
		}

		case 1:
			WaitFrames(1);
			UnitGetFar(work->owner_ids[0]);
			state = 0;
			if (work->psynergy_count != 0)
			{
				{
					struct PsynergyMenuState *saved_work = work;

					/* FAKEMATCH: the discarded conditional restoration puts labels between the psynergy_count and mode loads, so reload_cse_move2add builds the mode offset from its own constant (movs #154, lsls #2) as the bytes do; both arms disappear. Plain forms measured 2026-10-13: without the block move2add makes it adds r2, #80 from the count offset (differ 51). */
					if (done)
						work = NULL;
					if (done)
						work = saved_work;
				}
				switch (work->mode)
				{
				case 0:
					ItemMenu_DrawMsg(0, MsgPsynergyChooseAbility);
					break;
				case 1:
					ItemMenu_DrawMsg(0, MsgPsynergyChooseForL);
					break;
				case 2:
					ItemMenu_DrawMsg(0, MsgPsynergyChooseForR);
					break;
				}
				ItemMenu_PosCategory();
				Menu_DrawOwnerStatusPanel(work->status_window, work->owner_ids[0], 0, 0);
				selection = PsynergyMenu_RunList(0);
				state = 0;
				if (selection != -1)
				{
					state = 2;
					if (work->mode != 0)
					{
						if (work->mode == 1)
						{
							PsynergyMenu_SetShortcut(work->owner_ids[0], selection, 0);
							RenderOutput_ClearListFar((void *)work->info_window);
							InventoryMenu_ShowModalMessage(MsgShortcutSetL, -1, -1);
#if !EDITION_INTERNATIONAL
							RenderOutput_RedrawSavedRectFar(work->info_window);
#endif
						}
						else
						{
							PsynergyMenu_SetShortcut(work->owner_ids[0], selection, 1);
							RenderOutput_ClearListFar((void *)work->info_window);
							InventoryMenu_ShowModalMessage(MsgShortcutSetR, -1, -1);
#if !EDITION_INTERNATIONAL
							RenderOutput_RedrawSavedRectFar(work->info_window);
#endif
						}
						state = 0;
					}
				}
			}
			break;

		case 3:
			ItemMenu_DrawMsg(0, MsgPsynergyChooseTarget);
			self_flag = PsynergyMenu_SelectTarget(0);
			state = 4;
			if (self_flag == -1)
			{
				work->flags |= 1;
				state = 1;
			}
			break;

		case 2:
			classification = PsynergyMenu_ClassifySelectedPsynergy();
			if (classification == 1)
			{
				state = 3;
				break;
			}
			if (classification == 2)
			{
				work->owner_ids[1] = 9;
				state = 4;
				break;
			}
			done = 1;
			result = 1;
			*out_owner = work->owner_ids[0];
			*out_action = work->pane_action[0] & OWNER_ACTION_ID_MASK;
			break;

		case 4:
			self_flag = 0;
			raw = work->pane_action[0];
			result = BattleEffect_ApplyToTargets(raw, work->owner_ids[0], work->owner_ids[1], 0);
			if (work->owner_ids[1] == 9)
			{
				work->owner_ids[1] = work->owner_ids[0];
				self_flag = 9;
			}
			if (result != -1)
			{
				action = BtlActGet(work->pane_action[0] & OWNER_ACTION_ID_MASK);
				UnitPpAddFar(work->owner_ids[0], -action->pp_cost);
			}
			UnitCalcFar(work->owner_ids[0]);
			if (result != -1)
			{
				Menu_DrawOwnerStatusPanel(work->status_window, work->owner_ids[1], 0, 0);
				Ability_PlayUseAnimation(work->pane_action[0] & OWNER_ACTION_ID_MASK);
				RenderOutput_ClearListFar((void *)work->info_window);
				InventoryMenu_ShowModalMessage(work->use_result + MsgItemUseResult, 0, -1);
			}
			else
			{
				AudioCommand_PlayFar(114);
				RenderOutput_ClearListFar((void *)work->info_window);
				InventoryMenu_ShowModalMessage(work->use_result + MsgItemUseResult, result, result);
			}
			if (result != -1)
			{
				result = 1;
				work->flags |= 1;
				state = 1;
			}
			else
			{
				work->skip_slide = 1;
				if (self_flag == 9)
				{
					work->flags |= 1;
					state = 1;
				}
				else
				{
					state = 3;
				}
			}
			break;

		default:
			done = 1;
			break;
		}
	}

	if (GameFlag_TestFar(0x150) != 0)
	{
		result = -1;
	}
	return result;
}

s32 PsynergyMenu_ClassifySelectedPsynergy(void)
{
	struct BtlAct *psynergy;
	s32			   diff;
	s32			   ret;

	psynergy =
		BtlActGet(OWNER_ACTION_ID_MASK & ((struct PsynergyMenuState *)gMenuWork)->pane_action[0]);
	if (BattleFx_HasMatchingEvent5Far(psynergy->type_0c) != 0)
	{
		return 0;
	}
	ret = 2;
	if (psynergy->rng != 0xff)
	{
		u8 kind = psynergy->target_mode;
		diff = kind ^ 2;
		ret = (0 - diff) | diff;
		ret = (u32)ret >> 0x1f;
		ret = 1 - ret;
	}
	return ret;
}

s32 PsynergyMenu_SelectPartySlot(s32 party_slot)
{
	/* FAKEMATCH: retain the original scalar tab/icon address lanes. Direct indexed owner fields change the 244-byte native object to 246 bytes and reorder loads around the cursor call. */
	struct PsynergyMenuState *menu = gMenuWork;
	s32						  tab_offset = party_slot + ((u8 *)menu->tab_index - (u8 *)menu);
	s32						  icon_offset =
		party_slot * sizeof(menu->pane_icon[0]) + ((u8 *)menu->pane_icon - (u8 *)menu);
	struct RenderOutput *icon;
	u8					 byte_val;
	s32					 tab;
	s32					 owner_offset;
	s32					 table_offset;
	s32					 owner_id;
	struct BtlUnit		*owner;
	u16					*actions;
	u8					*counts;
	s32					 badge;
	s32					 result;
	s32					 icon_offset2;

	result = 0;
	icon = *(struct RenderOutput **)((u8 *)menu + icon_offset);
	icon->active = 1;
	icon->anim_step = result;
	menu->cursor_icon->active = 13;
	counts = (u8 *)menu + ((u8 *)menu->tab_counts - (u8 *)menu->tab_index);
	tab = *(s8 *)((u8 *)menu + tab_offset);
	byte_val = menu->owner_count;
	counts[tab_offset] = byte_val;

	if (tab == -1)
	{
		*((u8 *)menu + tab_offset) = 0;
		owner_offset = 0;
	}
	else
	{
		owner_offset = tab * sizeof(menu->owner_table[0]);
		UiMenu_SlideCursor(tab * 24 - 10, 16);
	}

	table_offset = owner_offset + ((u8 *)menu->owner_table - (u8 *)menu);
	owner_id = *(u16 *)((u8 *)menu + table_offset);
	owner = UnitGetFar(owner_id);
	actions = menu->psynergies;
	badge = PsynergyMenu_CollectActions(owner, actions, 2);
	menu->psynergy_count = badge;
	result = PsynergyMenu_SetupActionIcons(menu->owner_table, actions);

	icon_offset2 = party_slot * sizeof(menu->pane_icon[0]) + ((u8 *)menu->pane_icon - (u8 *)menu);
	icon = *(struct RenderOutput **)((u8 *)menu + icon_offset2);
	UiIcon_PrepareObject(icon);
	WaitFrames(1);
	return result;
}

s32 PsynergyMenu_SetupActionIcons(u16 *owner_ids, u16 *unused)
{
	struct PsynergyMenuState *menu;
	s32						  selection;
	s32						  count;
	s32						  pending;
	s32						  result;
	struct BtlUnit			 *owner;
	s32						  window;
	s32						  shown;
	s32						  i;
	u16						 *actions;
	s32						  action_count;
	s8						  found;

	menu = gMenuWork;
	selection = menu->tab_index[0];
	count = menu->tab_counts[0];
	pending = 1;
	result = 0;
	shown = 0;
	menu->mode = result;
	owner = UnitGetFar(owner_ids[selection]);
	if (UiWindow_UpdateOrCreate(&menu->psynergy_window, 13, 3, 17, 10, 2))
		Menu_SpawnIconEntries((struct InventoryMenuState *)menu, menu->psynergy_window);
	if (UiWindow_UpdateOrCreate(&menu->shortcut_window, 13, 13, 17, 4, 2))
	{
		menu->cursor_icon = RenderOutput_CreateFromResourceFar(
			2, 0, (struct RenderInput *)menu->shortcut_window, 0, result);
		menu->cursor_icon->active = 13;
	}
	while (!GameFlag_TestFar(0x150))
	{
		if (pending)
		{
			pending = 0;
			selection = (count + selection) % count;
			window = menu->status_window;
			owner = UnitGetFar(owner_ids[selection]);
			PsynergyMenu_RefreshOwnerPsynergy(owner_ids[selection]);
			Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 0);
			PsynergyMenu_DrawShortcuts(menu->shortcut_window, owner_ids[selection]);
			PsynergyMenu_CallIconRoutineWithValue((s32)menu, owner_ids[selection]);
			for (i = 3; i >= 0; i--)
				menu->row_positions[i] = 0x1e;
			menu->row_positions[selection] = 0x1a;
			if (!GameFlag_TestFar(0x151) && !shown)
			{
				RenderOutput_ClearListFar((void *)menu->info_window);
#if EDITION_INTERNATIONAL
				RenderOutput_RedrawSavedRectFar((void *)menu->info_window);
#endif
				UiText_DrawWorkValueWithLabel((s32)menu->info_window);
				shown = 1;
			}
			else
			{
				GameFlag_ClearBitFar(0x151);
			}
		}
		UiMenu_PositionCursor(selection * 24 - 10, 16);
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			if (menu->psynergy_count)
			{
				AudioCommand_PlayFar(112);
				result = owner_ids[selection];
				break;
			}
			AudioCommand_PlayFar(114);
		}
		if ((gKeyState & KEY_L) || (gKeyState & KEY_R))
		{
			result = owner_ids[selection];
			if (gKeyState & KEY_L)
				menu->mode = 1;
			else
				menu->mode = 2;
			actions = MemAlloc(64);
			found = PsynergyMenu_CollectActions((struct BtlUnit *)owner, actions, 1);
			MemFree(actions);
			action_count = found;
			if (action_count == 0)
			{
				menu->mode = action_count;
				AudioCommand_PlayFar(114);
			}
			else
			{
				AudioCommand_PlayFar(112);
				break;
			}
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			AudioCommand_PlayFar(111);
			selection--;
			pending = 1;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			AudioCommand_PlayFar(111);
			selection++;
			pending = 1;
		}
	}
	menu->tab_index[0] = selection;
	menu->selected_owner = owner_ids[selection];
	menu->owner_ids[0] = owner_ids[selection];
	return result;
}

void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id)
{
	u16						 *psynergies;
	struct PsynergyMenuState *menu;
	struct BtlUnit			 *owner;

	menu = gMenuWork;
	owner = UnitGetFar(owner_id);
	psynergies = menu->psynergies;
	menu->psynergy_count = PsynergyMenu_CollectActions(owner, psynergies, 2);
	RenderOutput_RedrawSavedRectFar((void *)menu->psynergy_window);
	PsynergyMenu_RefreshOwnerEntries(0x6c, 0x20, 8);
	PsynergyMenu_DrawPsynergyIcons(psynergies);
	if (menu->psynergy_count == 0)
	{
		UiText_DrawCharacterAtOffsetFar(MsgPsynergyMenuEmpty, (s32)menu->psynergy_window, 0, 0x18);
	}
}

s32 PsynergyMenu_ReturnTrue(void)
{
	return 1;
}

void PsynergyMenu_NoOp(void)
{
}

s32 PsynergyMenu_SelectTarget(s32 mode)
{

	struct PsynergyMenuState *menu;
	s32						  selection;
	s32						  count;
	s32						  pending;
	s32						  result;
	s32						  shown;
	struct RenderOutput		 *marker;

	menu = gMenuWork;
	selection = menu->tab_index[1];
	count = menu->owner_count;
	pending = 1;
	result = 0;
	shown = 0;
	UnitGetFar(menu->owner_table[menu->tab_index[0]]);
	UiMenu_SlideCursor(selection * 24 - 10, 16);
	while (!GameFlag_TestFar(0x150))
	{
		if (pending)
		{
			pending = 0;
			selection = (selection + count) % count;
			UnitGetFar(menu->owner_table[selection]);
			marker = menu->pane_icon[1];
			marker->oam.x = marker->x =
				((((struct RenderInput *)menu->auxiliary_window)->x + selection * 3) << 3) - 2;
			if (mode == 0)
			{
				Menu_DrawOwnerStatusPanel(menu->status_window, menu->owner_table[selection], 0, 0);
				PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_table[selection]);
				if (!GameFlag_TestFar(0x151) && !shown)
				{
#if EDITION_INTERNATIONAL
					RenderOutput_RedrawSavedRectFar((void *)menu->info_window);
					UiText_DrawCharacterAtOffsetFar(
						(menu->pane_action[0] & OWNER_ACTION_ID_MASK) + MsgAbilityDescription,
						(s32)menu->info_window, 0, 0);
#else
					RenderOutput_ClearListFar((void *)menu->info_window);
					UiText_DrawResourceFar(
						(menu->pane_action[0] & OWNER_ACTION_ID_MASK) + MsgAbilityDescription,
						(s32)menu->info_window, 0, 0);
#endif
					shown = 1;
				}
				else
				{
					GameFlag_ClearBitFar(0x151);
				}
			}
		}
		UiMenu_PositionCursor(selection * 24 - 10, 16);
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			AudioCommand_PlayFar(112);
			result = menu->owner_table[selection];
			break;
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			AudioCommand_PlayFar(111);
			selection--;
			pending = 1;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			AudioCommand_PlayFar(111);
			selection++;
			pending = 1;
		}
	}
	marker = menu->pane_icon[1];
	menu->tab_index[1] = selection;
	UiIcon_PrepareObject(marker);
	marker->active = 13;
	WaitFrames(1);
	menu->tab_index[1] = selection;
	menu->selected_owner = menu->owner_table[selection];
	menu->owner_ids[1] = menu->owner_table[selection];
	return result;
}

s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut)
{
	s32 id = psynergy & OWNER_ACTION_ID_MASK;
	s32 code = (owner << 10) | id;

	if (shortcut == 0)
	{
		gGameState.first_shortcut = code;
	}
	else
	{
		gGameState.second_shortcut = code;
	}
	return 1;
}

#if EDITION_INTERNATIONAL

s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner)
{
	s32 left;
	s32 top;
	s32 width;
	s32 height;
	s32 wide;

	if (gGameState.first_shortcut != 0 && gGameState.second_shortcut != 0)
		UiText_DrawCharacterAtOffsetFar(MsgShortcutChangeHelp, window, 0, -8);
	else
		UiText_DrawCharacterAtOffsetFar(MsgShortcutLabel, window, 0, -8);

	UiText_GetResourceDimensionsFar(
		(gGameState.first_shortcut & 0x3ff) + MsgAbilityName, &left, &top, &width, &height);
	if (width < 0 || width > 10)
		wide = 1;
	else
		wide = 0;
	if (gGameState.first_shortcut != 0)
	{
		UiWork_PushValueSlotFar(gGameState.first_shortcut & 0x3ff, 4);
		UiText_DrawCharacterAtOffsetFar(MsgShortcutNameL, window, 0, 0);
		if (wide == 0)
			UiText_DrawStringAtOffsetFar(
				UnitGetFar(gGameState.first_shortcut >> 10), window, 80, 0);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyL, window, 0, 0);
	}

	UiText_GetResourceDimensionsFar(
		(gGameState.second_shortcut & 0x3ff) + MsgAbilityName, &left, &top, &width, &height);
	if (width < 0 || width > 10)
		wide = 1;
	else
		wide = 0;
	if (gGameState.second_shortcut != 0)
	{
		UiWork_PushValueSlotFar(gGameState.second_shortcut & 0x3ff, 4);
		UiText_DrawCharacterAtOffsetFar(MsgShortcutNameR, window, 0, 8);
		if (wide == 0)
			UiText_DrawStringAtOffsetFar(
				UnitGetFar(gGameState.second_shortcut >> 10), window, 80, 8);
		UiWork_SetParamNibbleFar(15);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyR, window, 0, 8);
	}
	return 1;
}
#else

s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner)
{
	s32 left;
	s32 top;
	s32 width;
	s32 height;
	u16 wide;

	if (gGameState.first_shortcut != 0 && gGameState.second_shortcut != 0)
		UiText_DrawCharacterAtOffsetFar(MsgShortcutChangeHelp, window, 0, -8);
	else
		UiText_DrawCharacterAtOffsetFar(MsgShortcutLabel, window, 0, -8);

	UiText_GetResourceDimensionsFar(
		(gGameState.first_shortcut & 0x3ff) + MsgAbilityName, &left, &top, &width, &height);
	if (width < 0 || width > 11)
		wide = 1;
	else
		wide = 0;
	if (wide)
		UiText_DrawStringInWindowFar((u8 *)Menu_ShortcutLString, window, 0, 0);
	else
		UiText_DrawStringInWindowFar((u8 *)Menu_ShortcutLColonString, window, 0, 0);
	if (gGameState.first_shortcut != 0)
	{
		if (wide)
			UiText_DrawCharacterAtOffsetFar(
				(gGameState.first_shortcut & 0x3ff) + MsgAbilityName, window, 8, 0);
		else
			UiText_DrawCharacterAtOffsetFar(
				(gGameState.first_shortcut & 0x3ff) + MsgAbilityName, window, 16, 0);
		UiText_DrawStringAtOffsetFar(UnitGetFar(gGameState.first_shortcut >> 10), window, 80, 0);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyL, window, 24, 0);
	}

	UiText_GetResourceDimensionsFar(
		(gGameState.second_shortcut & 0x3ff) + MsgAbilityName, &left, &top, &width, &height);
	if (width < 0 || width > 11)
		wide = 1;
	else
		wide = 0;
	if (wide)
		UiText_DrawStringInWindowFar((u8 *)Menu_ShortcutRString, window, 0, 8);
	else
		UiText_DrawStringInWindowFar((u8 *)Menu_ShortcutRColonString, window, 0, 8);
	if (gGameState.second_shortcut != 0)
	{
		if (wide)
			UiText_DrawCharacterAtOffsetFar(
				(gGameState.second_shortcut & 0x3ff) + MsgAbilityName, window, 8, 8);
		else
			UiText_DrawCharacterAtOffsetFar(
				(gGameState.second_shortcut & 0x3ff) + MsgAbilityName, window, 16, 8);
		UiText_DrawStringAtOffsetFar(UnitGetFar(gGameState.second_shortcut >> 10), window, 80, 8);
		UiWork_SetParamNibbleFar(15);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyR, window, 24, 8);
	}
	return 1;
}
#endif

void PsynergyMenu_CreateEntryGrid(void)
{
	s32						  window;
	struct PsynergyMenuState *menu;
	struct RenderOutput		 *cursor;
	s32						  index;
	s32						  x;
	s32						  y;
	struct RenderOutput		**output;

	menu = gMenuWork;
	window = UiMenu_CreateCursor(menu);
	PsynergyMenu_InitializeEntryObjects((void *)window, 2, 2, 8, 0);

	window = UiWindow_CreateFar(0, 5, 30, 15, 2);
	menu->psynergy_window = window;
	menu->selected_column = 0;
	menu->selected_row = 0;
	menu->column_count = 8;
	menu->row_count = 2;

	cursor =
		(struct RenderOutput *)UiIcon_CreateWithResourceVariant((struct RenderInput *)window, 0, 4);
	cursor->active = 13;
	menu->entry_grid_cursor = cursor;
	SideObject_CreateFar(0, 0, 0, window, 0, 0);

	y = 8;
	index = 0;
	output = &menu->entry_icons[0];
	x = 96;
	do
	{
		*output++ =
			RenderOutput_CreateFromResourceFar(4, index, (struct RenderInput *)window, x, y);
		index++;
		x += 16;
	} while (index <= 7);

	index = 8;
	y = 24;
	output = &menu->entry_icons[8];
	x = 96;
	do
	{
		*output++ =
			RenderOutput_CreateFromResourceFar(4, index, (struct RenderInput *)window, x, y);
		index++;
		x += 16;
	} while (index <= 15);
}

void PsynergyMenu_CloseWindows(void)
{
	struct PsynergyMenuState *menu;

	menu = gMenuWork;
	Menu_ReleaseEntryObjects();
	UiWork_FinalizeFar((void *)menu->auxiliary_window, 1);
	UiWork_FinalizeFar((void *)menu->psynergy_window, 1);
	UiWork_FinalizeFar((void *)menu->message_window, 1);
}

void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies)
{
	s32					  remaining;
	struct RenderOutput **icons;
	u16					 *p;
	s32					  psynergy_id;

	icons = ((struct PsynergyMenuState *)gMenuWork)->entry_icons;
	p = psynergies;
	remaining = 31;
	do
	{
		psynergy_id = *p++;
		if (psynergy_id != 0)
		{
			Resource_LoadByModeIntoSlotFar(4, psynergy_id, (u8)(*icons)->index, 0);
		}
		icons++;
		remaining--;
	} while (remaining >= 0);
	Menu_HideEmptyEntryIcons(psynergies);
}

s32 PsynergyMenu_CollectActions(struct BtlUnit *owner, u16 *actions, s32 mode)
{
	/* FAKEMATCH: the doubled zero store through the byte-offset cursor makes loop.c replace the counter with the pointer (274 bytes); a single indexed clear keeps the counter (266). Measured 2026-10-13: a single store through a pointer walking down from actions + 31, through the byte offset, or as a pre-decrement do-while differs 31 to 33; actions[n] = 0 from 31 down differs 43. */
	s32 n;
	s32 passes;
	s32 count;
	s32 i;
	s32 j;

	passes = (mode != 2) ? 4 : 3;
	for (n = 62; n >= 0; n -= 2)
	{
		u16 *q = (u16 *)((u8 *)actions + n);
		*q = 0;
		*q = 0;
	}
	count = 0;

	if (mode == 1)
	{
		for (i = 0; i < 32; i++)
		{
			if (owner->action_slots[i].encoded_action != 0)
			{
				if (BtlActGet(owner->action_slots[i].encoded_action & OWNER_ACTION_ID_MASK)->type_0c
					!= 0)
				{
					actions[count] = owner->action_slots[i].encoded_action;
					count++;
				}
			}
		}
	}
	else
	{
		for (j = 0; j < passes; j++)
		{
			for (i = 0; i < 32; i++)
			{
				if (owner->action_slots[i].encoded_action != 0)
				{
					struct BtlAct *ability =
						BtlActGet(owner->action_slots[i].encoded_action & OWNER_ACTION_ID_MASK);

					if (j == 0 && (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0))
					{
						actions[count] = owner->action_slots[i].encoded_action;
						count++;
					}
					else if (j == 1)
					{
					}
					else if (j == 2)
					{
					}
					else if (j == 3 && ability->type_0c == 0 && (ability->tgt_flg & 0x40) == 0)
					{
						actions[count] = owner->action_slots[i].encoded_action;
						count++;
					}
				}
			}
		}
	}

	return count;
}

s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index)
{
	s32						  owner_state;
	struct PsynergyMenuState *menu = gMenuWork;
	s32						  entry_count;
	s32						  row;
	s32						  page;
	s32						  page_count;
	s32						  selected_index;
	s32						  owner;

	owner_state = (s32)UnitGetFar(menu->owner_ids[index]);
	entry_count = menu->psynergy_count;
	owner = menu->owner_ids[index];
	selected_index = menu->selected_index_by_owner[owner];
	if ((selected_index + 1) > entry_count)
	{
		selected_index = entry_count - 1;
	}
	page = selected_index / 5;
	row = selected_index % 5;
	page_count = entry_count / 5;
	if (entry_count % 5 != 0)
	{
		page_count++;
	}
	result->owner_state = owner_state;
	result->page = page;
	result->page_count = page_count;
	result->row = row;
	result->entry_count = entry_count;
	result->selected_index = selected_index;
	return 1;
}

#if EDITION_INTERNATIONAL
#define DETAIL_ROW_X 0
#define DETAIL_ROW_WIDTH 15
#else
#define DETAIL_ROW_X 1
#define DETAIL_ROW_WIDTH 14
#endif

s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work, struct MenuResult *state)
{
	struct PsynergyMenuState *menu;
	s32						  combined;
	s32						  row;

	menu = gMenuWork;
	combined = state->page * 5;
	combined += state->row;
	state->selected_index = combined;

	if (GameFlag_TestFar(0x151) == 0)
	{
#if EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar(menu->info_window);
#else
		RenderOutput_ClearListFar(menu->info_window);
#endif
		WaitFrames(1);

		combined = state->selected_index;
		if (menu->psynergies[combined] != 0)
		{
			s32 masked = (menu->psynergies[combined] & 0x1ff) + MsgAbilityDescription;
#if EDITION_INTERNATIONAL
			UiText_DrawCharacterAtOffsetFar(masked, (s32)menu->info_window, 0, 0);
#else
			UiText_DrawResourceFar(masked, (s32)menu->info_window, 0, 0);
#endif
		}
	}
	else
	{
		GameFlag_ClearBitFar(0x2ff);
	}

	row = 0;
	do
	{
		if (row == state->row)
		{
			Render_SetTilemapFlagRect(
				(void *)menu->psynergy_window, DETAIL_ROW_X, row * 2 + 1, DETAIL_ROW_WIDTH, 1, 14);
		}
		else
		{
			Render_SetTilemapFlagRect(
				(void *)menu->psynergy_window, DETAIL_ROW_X, row * 2 + 1, DETAIL_ROW_WIDTH, 1, 15);
		}
		row++;
	} while (row <= 4);

	WaitFrames(1);
	return 1;
}

s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state)
{
	u32						  first_entry;
	u32						  visible_count;
	u8						  row;
	s32						  cursor;
	struct BtlUnit			 *owner;
	struct BtlAct			 *ability;
	struct PsynergyMenuState *menu = gMenuWork;

	(void)unused;

	RenderOutput_RedrawSavedRectFar((void *)window);
	UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

	if (2 & menu->flags)
	{
		UiText_DrawCharacterAtOffsetFar(MsgShortcutHelp, window, 0, 88);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar(MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
	}

	first_entry = state->page * 5;
	visible_count = (u8)(state->entry_count - first_entry);
	if (visible_count > 5)
	{
		visible_count = 5;
	}

#if EDITION_INTERNATIONAL
	Menu_SetPageIcons(5, first_entry, window, 0x70, 0x22);
#else
	Menu_SetPageIcons(5, first_entry, window, 0x78, 0x22);
#endif
	Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);
#if EDITION_INTERNATIONAL
	UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window, 0x60, 0);
#else
	UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window, 0x48, 0);
#endif

	row = 0;
	if (visible_count > row)
	{
		cursor = first_entry;
		do
		{
			owner = UnitGetFar(menu->owner_ids[0]);
			ability = BtlActGet(OWNER_ACTION_ID_MASK & menu->psynergies[cursor]);

			if (ability->pp_cost > owner->pp)
			{
				UiWork_SetParamNibbleFar(2);
			}
			else if (PsynergyMenu_IsActionRestricted(
						 OWNER_ACTION_ID_MASK & menu->psynergies[cursor])
					 != 0)
			{
				UiWork_SetParamNibbleFar(4);
			}
			else
			{
				UiWork_SetParamNibbleFar(15);
			}

			UiText_DrawCharacterAtOffsetFar(
				(OWNER_ACTION_ID_MASK & menu->psynergies[cursor]) + MsgAbilityName,
#if EDITION_INTERNATIONAL
				window, 16, row * 16 + 8);
#else
				window, 32, row * 16 + 8);
#endif
			UiText_DrawNumberAtOffsetFar(ability->pp_cost, 2, window, 104, row * 16 + 8);
			UiWork_SetParamNibbleFar(15);

			row++;
			cursor++;
		} while (visible_count > row);
	}

	return 1;
}

s32 PsynergyMenu_RunList(s32 pane)
{
	struct PsynergyMenuState *menu;
	struct BtlAct			 *ability;
	struct RenderOutput		 *icon;
	s32						  window;
	s32						  changed;
	s32						  nav;
	s8						  mode;
	s32						  tab;
	u8						  i;
	s32						  result;
	s32						  redraw;
	s32						  done;
	struct BtlUnit			 *owner;
	s32						  prev;
	s32						  prompt;
	s32						  work[5];
	struct MenuResult		  state;

	menu = gMenuWork;
	result = 0;
	prev = 0;
	prompt = 0;
	menu->pane_icon[pane]->active = 13;
	UiWindow_UpdateOrCreate(&menu->list_window, 13, 3, 17, 14, 2);
	window = menu->list_window;
	done = 0;

	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		owner = UnitGetFar(menu->owner_ids[pane]);
		if (menu->mode != 0)
		{
			menu->psynergy_count = PsynergyMenu_CollectActions(owner, menu->psynergies, 1);
		}
		else
		{
			menu->psynergy_count = PsynergyMenu_CollectActions(owner, menu->psynergies, 2);
		}
		PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
		PsynergyMenu_BuildPageResult(&state, pane);
		redraw = 1;
		changed = 1;
		menu->pane_icon[pane]->active = 1;

		while (GameFlag_TestFar(0x150) == 0)
		{
#if EDITION_INTERNATIONAL
			UiMenu_PositionCursor(88, state.row * 16 + 36);
#else
			UiMenu_PositionCursor(98, state.row * 16 + 36);
#endif

			if (changed != 0)
			{
				changed = 0;
				if (menu->psynergies[prev] != 0)
				{
					icon = menu->entry_icons[prev];
					UiIcon_PrepareObject(icon);
				}
				if (redraw != 0)
				{
					redraw = 0;
					WaitFrames(1);
					PsynergyMenu_DrawActionPage(window, 0, &state);
				}
				PsynergyMenu_DrawDetailPage(window, work, &state);
				menu->pane_action[pane] = menu->psynergies[state.selected_index];
				menu->cursor_icon->active = 13;
				if (menu->psynergies[state.selected_index] != 0)
				{
					icon = menu->entry_icons[state.selected_index];
					icon->active = 9;
					icon->anim_step = 0;
					icon->sentinel = 250;
				}
				for (i = 0; i < menu->owner_count; i++)
				{
					AnimationObjects_SelectAnimationFar(menu->tab_objects[i], 1);
				}
			}

			WaitFrames(1);
			prev = state.selected_index;

			if ((gKeysHeld & KEY_SELECT) == 0)
			{
				nav = Menu_HandlePageInput(
					0, state.entry_count, LIST_PAGE_SIZE, &state.row, &state.page);
			}
			else
			{
				nav = -1;
			}
			if (nav == 1)
			{
				redraw = 1;
				changed = 1;
			}
			if (nav == 0)
			{
				changed = 1;
			}
			if (nav == -1)
			{
				changed = 0;
			}

			if (menu->mode == 0)
			{
				if ((gKeyState & KEY_SELECT) != 0 && prompt == 0)
				{
					ability =
						BtlActGet(menu->psynergies[state.selected_index] & OWNER_ACTION_ID_MASK);
					if (ability->type_0c == 0)
					{
						AudioCommand_PlayFar(114);
					}
					else
					{
						AudioCommand_PlayFar(174);
						prompt = 1;
						menu->flags |= 2;
						UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
						UiText_DrawCharacterAtOffsetFar(MsgShortcutHelp, window, 0, 88);
					}
				}
				if ((gKeysHeld & KEY_SELECT) == 0 && prompt == 1)
				{
					prompt = 0;
					menu->flags &= 0xfffd;
					UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
					UiText_DrawCharacterAtOffsetFar(
						MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
				}
			}

			if ((gKeyState & KEY_A) != 0)
			{
				if (menu->mode != 0)
				{
					AudioCommand_PlayFar(130);
					result = menu->psynergies[state.selected_index];
					done = 1;
					break;
				}
				if (menu->psynergies[state.selected_index] != 0)
				{
					if (PsynergyMenu_IsActionRestricted(menu->psynergies[state.selected_index])
						!= 0)
					{
						AudioCommand_PlayFar(114);
						continue;
					}
					ability =
						BtlActGet(menu->psynergies[state.selected_index] & OWNER_ACTION_ID_MASK);
					if (ability->pp_cost > owner->pp)
					{
						AudioCommand_PlayFar(114);
					}
					else
					{
						AudioCommand_PlayFar(173);
						result = menu->psynergies[state.selected_index];
						done = 1;
						break;
					}
				}
			}

			if ((gKeyState & KEY_B) != 0)
			{
				AudioCommand_PlayFar(113);
				result = -1;
				done = 1;
				break;
			}

			if (((gKeysRepeat & KEY_R) != 0 || (gKeysRepeat & KEY_L) != 0)
				&& (gKeysHeld & KEY_SELECT) == 0)
			{
				mode = menu->mode != 0 ? 1 : 2;
				AudioCommand_PlayFar(111);
				menu->selected_index_by_owner[menu->owner_ids[pane]] = state.selected_index;
				tab = menu->tab_index[pane];
				do
				{
					if ((gKeysRepeat & KEY_R) != 0)
					{
						tab++;
					}
					else
					{
						tab--;
					}
					tab = (tab + menu->owner_count) % menu->owner_count;
					menu->selected_owner = menu->owner_table[tab];
					menu->owner_ids[0] = menu->owner_table[tab];
					menu->psynergy_count = PsynergyMenu_CollectActions(
						UnitGetFar(menu->owner_ids[0]), menu->psynergies, mode);
				} while (menu->psynergy_count == 0);
				menu->tab_index[pane] = tab;
				for (i = 0; i < 4; i++)
				{
					menu->row_positions[i] = 30;
				}
				menu->row_positions[tab] = 26;
				Menu_DrawOwnerStatusPanel(menu->status_window, menu->owner_table[tab], 0, 0);
				PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_table[tab]);
				break;
			}

			if ((gKeyState & KEY_L) != 0 && (gKeysHeld & KEY_SELECT) != 0)
			{
				ability = BtlActGet(menu->psynergies[state.selected_index] & OWNER_ACTION_ID_MASK);
				if (ability->type_0c == 0)
				{
					AudioCommand_PlayFar(114);
				}
				else
				{
					AudioCommand_PlayFar(130);
					if (PsynergyMenu_SetShortcut(
							menu->owner_ids[pane], menu->psynergies[state.selected_index], 0)
						!= 0)
					{
						result = menu->psynergies[state.selected_index];
						menu->mode = 1;
						done = 1;
						break;
					}
				}
			}

			if ((gKeyState & KEY_R) != 0 && (gKeysHeld & KEY_SELECT) != 0)
			{
				ability = BtlActGet(menu->psynergies[state.selected_index] & OWNER_ACTION_ID_MASK);
				if (ability->type_0c == 0)
				{
					AudioCommand_PlayFar(114);
				}
				else
				{
					if (PsynergyMenu_SetShortcut(
							menu->owner_ids[pane], menu->psynergies[state.selected_index], 1)
						!= 0)
					{
						AudioCommand_PlayFar(130);
						result = menu->psynergies[state.selected_index];
						menu->mode = 2;
						done = 1;
						break;
					}
				}
			}
		}
	}

	menu->flags &= 0xfffd;
	UiIcon_PrepareObject(menu->entry_grid_cursor);
	menu->pane_row[pane] = state.selected_index;
	menu->selected_index_by_owner[menu->owner_ids[pane]] = state.selected_index;
	menu->pane_action[pane] = result;
	if (GameFlag_TestFar(0x150) != 0)
	{
		result = -1;
	}
	WaitFrames(1);
	return result;
}

s32 PsynergyMenu_IsActionRestricted(s32 no)
{
	struct BtlAct *action = BtlActGet((u32)(no << 18) >> 18);

	if (action->type_0c != 0 || (action->tgt_flg & 0xc0) == 0xc0)
		return 0;
	return 1;
}

#if EDITION_INTERNATIONAL
#define ROW_CNT 8
#else
#define ROW_CNT 4
#endif

s32 Menu_OpenCharacterSelector(void)
{
	struct CharacterMenuState *state = Runtime_AllocateHeapBlock(55, 0x0a70);
	s32						   result;
	s32						   index;

	gMenuCtrlWork->suspended = 1;
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	WaitFrames(1);
	UiWindow_InitializeWork(0);

	state->party_count = Party_ListActiveOwnersFar(state->owner_ids);
	Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
	state->selector_window = UiWindow_CreateFar(13, 0, 17, 5, 2);
	for (index = 0; index < ROW_CNT; index++)
		state->owner_y[index] = 30;
	state->page = 3;

	result = CharacterSelector_Run();

	RenderOutput_ClearListFar(state->status_window);
	ItemMenu_Close();
	gMenuCtrlWork->suspended = 0;
	WaitFrames(1);
	Runtime_ReleaseHeapBlock(55);
	return result;
}

s32 CharacterSelector_Run(void)
{
	struct CharacterMenuState *work = gMenuWork;
	s32						   result = 0;

	/* FAKEMATCH: the one-pass block ends the schedule after the work load, so the zero is stored before the slot offset, as native; the store without it, or with result declared first, differs 4 (the offset moves into the load's delay), a constant 0 store 14, two returns 26, the call result kept in result and tested, or a ?: on it, 19. */
	do
	{
		work->selected_slots[0] = result;
	} while (0);
	if (CharacterMenu_SelectOwner(0) == -1)
		result = -1;
	else
		result = work->selected_ids[0];
	return result;
}

#if EDITION_INTERNATIONAL
#define SAVED_TILE_BYTES 0x2000
#else
#define SAVED_TILE_BYTES 0x800
#endif

s32 ActionMenu_Open(void)
{
	struct CharacterMenuState *state = Runtime_AllocateHeapBlock(55, 0x0a70);
	void					  *palette = Runtime_BumpAllocateAlternatePool(64);
	void					  *tiles = Runtime_BumpAllocateAlternatePool(SAVED_TILE_BYTES);
	s32						   result;
	s32						   index;

	((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 1;
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	WaitFrames(1);
	Scheduler_EnableOverlayCallbacksWithFlags();
	UiWindow_InitializeWork(0);
	state->page = 0;
	state->party_count = Party_ListActiveOwnersFar(state->owner_ids);
	Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
	Iwram_CopyWords(palette, (void *)0x05000000, 64);
#if !EDITION_INTERNATIONAL
	Iwram_CopyWords(tiles, (void *)0x06004000, SAVED_TILE_BYTES);
#endif
	Palette_LightenBankHighlight(14);
	Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, REG_DMA3);
	Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, REG_DMA3);
	Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, REG_DMA3);
	Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, REG_DMA3);
#if EDITION_INTERNATIONAL
	Iwram_CopyWords(tiles, (void *)0x06004000, SAVED_TILE_BYTES);
	Iwram_FillWords((void *)0x06004000, SAVED_TILE_BYTES, 0x33333333);
	UiWork_SetAltFlagAndClearTableFar(1);
#endif
	state->selector_window = UiWindow_CreateFar(13, 0, 17, 5, 2);
	for (index = 0; index < MENU_ROW_COUNT; index++)
		state->owner_y[index] = 30;
	if (Party_SumDjinnCountsFar(-1) != 0)
		FourObjectMotion_InitializeTopRow(state->selector_window, 0);
	for (index = 0; index < 4; index++)
	{
		state->row_x[index] = 130 + index * 32;
		state->row_y[index] = 0x80;
	}
	Link_DrawShiftedTilePairFar(0x06002500);
	Menu_CancelSoundReset();
	state->page = 0;

	result = Menu_RunActionFlow();

	Menu_EnsureCancelSound();
	RenderOutput_ClearListFar(state->status_window);
	FourObjectMotion_ClearSlotsAndSchedule();
	Scheduler_DisableOverlayCallbacksWithFlags();
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	WaitFrames(1);
#if EDITION_INTERNATIONAL
	UiWindow_MarkVisibleTileAttributesFar();
	UiWork_SetAltFlagAndClearTableFar(0);
	WaitFrames(1);
#endif
	Iwram_CopyWords((void *)0x05000000, palette, 64);
	Iwram_CopyWords((void *)0x06004000, tiles, SAVED_TILE_BYTES);
	MemFree(tiles);
	MemFree(palette);
	((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 1;
	ItemMenu_Close();
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	Runtime_ReleaseHeapBlock(55);
	((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 0;
	WaitFrames(1);
	UiWindow_EraseBorderRectFar(0, 0, 30, 20);
	((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 0;
	return result;
}

s32 Menu_RunActionFlow(void)
{
	struct CharacterMenuState *state = gMenuWork;
	s32						   step = 0;
	s32						   finished = step;
	s32						   result = 0;
	u32						   changed;

	while (!finished && !GameFlag_TestFar(0x150))
	{
		switch (step)
		{
		case 0:
			state->selected_slots[0] = finished;
			if (CharacterMenu_SelectOwner(0) == -1)
			{
				result = -1;
				finished = 1;
			}
			step = 1;
			break;
		case 1:
			state->owner_cursors[0]->active = 13;
			result = CharacterMenu_SelectCommand();
			step = result == -1 ? 0 : 2;
			break;
		case 2:
			state->owner_cursors[0]->active = 13;
			result = PsynergyMenu_SelectAction();
			step = 0;
			if (result != -1)
				step = 3;
			break;
		case 3:
			state->owner_cursors[0]->active = 13;
			result = ItemMenu_SelectItem();
			changed = ~result;
			step = (-changed | changed) >> 31;
			break;
		default:
			finished = 1;
			break;
		}
	}

	if (GameFlag_TestFar(0x150))
		result = -1;

	return result;
}

s32 CharacterMenu_SelectOwner(s32 slot)
{
	struct CharacterMenuState *menu;
	struct RenderOutput		  *cursor;
	s32						   result;
	s32						   index;

	menu = gMenuWork;
	index = menu->owner_index[slot];
	result = 0;
	cursor = menu->owner_cursors[slot];
	cursor->active = 1;
	cursor->anim_step = 0;
	RenderOutput_RedrawSavedRectFar(menu->owner_window);
	if (GameFlag_TestFar(0x172))
		UiWindow_DrawDividerLineFar((s32)menu->owner_window, 9, 1, 9, 3);
	if (index == -1)
		menu->owner_index[slot] = 0;
	else
		UiMenu_SlideCursor(index * 24 - 10, 16);
	if (menu->page == 3)
		result = PsynergyMenu_SelectOwner();
	else
		result = CharacterSelector_RunRearrange();
	cursor = menu->owner_cursors[slot];
	UiIcon_PrepareObject(cursor);
	WaitFrames(1);
	return result;
}

s32 DjinnMenu_ShowCurrentList(void)
{
	struct DjinnMenuWork *menu;
	s32					  window;
	s32					  row;
	s32					  col;

	menu = gMenuWork;
	ItemMenu_ResetCategory();
	AudioCommand_PlayFar(112);
	RenderOutput_RedrawSavedRectFar(menu->help_window);
	UiText_DrawCharacterAtOffsetFar(MsgReturnHelp, menu->help_window, 0, 16);
	menu->cursor->active = 13;
	menu->second_cursor->active = 13;
	WaitFrames(1);
	window = menu->window;
	{
		s32 i;

		for (i = 0; i < 4; i++)
		{
			menu->slot_x[i] = 32 + i * 56;
			menu->slot_y[i] = 70;
			menu->owner_y[i] = 30;
		}
	}
	RenderOutput_RedrawSavedRectFar(window);
	UiWindow_DrawDividerLineFar(window, 0, 11, 28, 11);
	UiText_DrawResourceFar(MsgCurrentDjinn, menu->help_window, -96, 132);
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 7; col++)
		{
			s32 id = row * 20 + col;

			if (GameFlag_TestFar(48 + id))
			{
				UiWindow_SetTilemapEntryFar(window, 0x1001 + row, row * 7 + 1, col + 3, 0);
				UiText_DrawCharacterAtOffsetFar(
					MsgDjinnName + id, window, row * 56 + 16, col * 8 + 24);
			}
		}
	}
	(*(struct UiWork **)&gWindowWork)->dirty = 1;
	for (;;)
	{
		if (GameFlag_TestFar(0x150))
			break;
		WaitFrames(1);
		if (gKeyState & (KEY_A | KEY_B | KEY_SELECT))
			break;
	}
	RenderOutput_RedrawSavedRectFar(menu->window);
	RenderOutput_ClearListFar(menu->help_window);
	{
		s32 i;

		for (i = 0; i < 4; i++)
		{
			menu->slot_x[i] = 130 + i * 32;
			menu->slot_y[i] = 128;
		}
	}
	menu->cursor->active = 1;
	menu->second_cursor->active = 1;
	AudioCommand_PlayFar(113);
	/* Returns no value: the ROM has the s32 epilogue (pop {r1}; bx r1) after this void call. */
}

struct InventoryMenuState;

s32 CharacterSelector_RunRearrange(void)
{
	struct CharacterMenuState *work;
	s32						   cursor;
	s32						   count;
	s32						   page;
	s32						   redraw;
	s32						   result;
	s32						   i;

	work = gMenuWork;
	cursor = work->owner_index[0];
	count = work->owner_count[0];
	redraw = 1;
	result = 0;
	page = work->page;
	UnitGetFar(work->owner_ids[cursor]);
	for (i = 0; i < 4; i++)
	{
		work->row_x[i] = 130 + i * 32;
		work->row_y[i] = 0x80;
	}
	Palette_LightenBankHighlight(14);
	Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, REG_DMA3);
	Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, REG_DMA3);
	Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, REG_DMA3);
	Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, REG_DMA3);
	while (!GameFlag_TestFar(0x150))
	{
		if (redraw)
		{
			redraw = 0;
			RenderOutput_RedrawSavedRectFar((s32)work->selector_window);
			UiText_DrawCharacterAtOffsetFar(MsgRearrangeHelp, (s32)work->selector_window, 0, 0);
			if (GameFlag_TestFar(48))
				UiText_DrawCharacterAtOffsetFar(
					MsgDjinnListHelp, (s32)work->selector_window, 0, 16);
			UiText_DrawCharacterAtOffsetFar(MsgRearrangeHelp - 3, (s32)work->selector_window, 0, 8);
			cursor = (cursor + count) % count;
			UnitGetFar(work->owner_ids[cursor]);
			page = (page + 3) % 3;
			Menu_CreateWindowAndEntryObjects(work->owner_ids[cursor], page);
			PsynergyMenu_CallIconRoutineWithValue(work, work->owner_ids[cursor]);
			for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
				work->owner_y[i] = 30;
			work->owner_y[cursor] = 26;
		}
		UiMenu_PositionCursor(cursor * 24 - 10, 16);
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			AudioCommand_PlayFar(0x70);
			result = 1;
			break;
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(0x71);
			result = -1;
			break;
		}
		if (gKeysRepeat & KEY_R)
		{
			if (CharacterSelector_MoveEntry(cursor, 1))
			{
				AudioCommand_PlayFar(0x70);
				cursor++;
				Menu_ReleaseEntryObjects();
				PsynergyMenu_InitializeEntryObjects(work->owner_window, 2, 2, 8, 0);
				for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
					work->owner_y[i] = 30;
				work->owner_y[cursor] = 26;
			}
			else
			{
				AudioCommand_PlayFar(0x72);
			}
			WaitFrames(1);
		}
		else if (gKeysRepeat & KEY_L)
		{
			if (CharacterSelector_MoveEntry(cursor, 0))
			{
				AudioCommand_PlayFar(0x70);
				cursor--;
				Menu_ReleaseEntryObjects();
				PsynergyMenu_InitializeEntryObjects(work->owner_window, 2, 2, 8, 0);
				for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
					work->owner_y[i] = 30;
				work->owner_y[cursor] = 26;
			}
			else
			{
				AudioCommand_PlayFar(0x72);
			}
			WaitFrames(1);
		}
		else if ((gKeyState & KEY_SELECT) && GameFlag_TestFar(48))
		{
			DjinnMenu_ShowCurrentList();
			redraw = 1;
		}
		else
		{
			if (gKeysRepeat & KEY_LEFT)
			{
				AudioCommand_PlayFar(0x6f);
				if (count > 1)
				{
					cursor--;
					redraw = 1;
				}
			}
			if (gKeysRepeat & KEY_RIGHT)
			{
				AudioCommand_PlayFar(0x6f);
				if (count > 1)
				{
					redraw = 1;
					cursor++;
				}
			}
		}
	}
	work->owner_index[0] = cursor;
	work->selected_owner = work->owner_ids[cursor];
	work->selected_ids[0] = work->owner_ids[cursor];
	return result;
}

s32 PsynergyMenu_SelectOwner(void)
{
	struct PsynergyMenuState *menu = gMenuWork;
	s32						  selection = menu->tab_index[0];
	s32						  count = menu->tab_counts[0];
	s32						  pending = 1;
	s32						  frame = menu->flags;
	s32						  result;
	s32						  i;

	UnitGetFar(menu->owner_table[selection]);
	RenderOutput_RedrawSavedRectFar(menu->message_window);
	UiText_DrawCharacterAtOffsetFar(MsgChooseCharacter, menu->message_window, 0, 0);
	UiText_DrawCharacterAtOffsetFar(MsgChooseCharacter + 1, menu->message_window, 0, 16);
	for (;;)
	{
		if (pending)
		{
			pending = 0;
			selection = (selection + count) % count;
			UnitGetFar(menu->owner_table[selection]);
			frame = (frame + 3) % 3;
			Menu_CreateWindowAndEntryObjects(menu->owner_table[selection], frame);
			PsynergyMenu_CallIconRoutineWithValue(menu, menu->owner_table[selection]);
			for (i = 0; i < MENU_ROW_COUNT; i++)
				menu->row_positions[i] = 30;
			menu->row_positions[selection] = 26;
			menu->psynergy_count = PsynergyMenu_CollectActions(
				UnitGetFar(menu->owner_table[selection]), (u16 *)menu->psynergies, 0);
			PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
			Menu_PlaceEntryObjectsInGrid(96, 96, 8);
			Menu_HideEmptyEntryIcons(menu->psynergies);
		}
		UiMenu_PositionCursor(selection * 24 - 10, 16);
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			AudioCommand_PlayFar(112);
			result = 1;
			break;
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		{
			if (gKeysRepeat & 32)
			{
				AudioCommand_PlayFar(111);
				if (count > 1)
				{
					selection--;
					pending = 1;
				}
			}
			if (gKeysRepeat & 16)
			{
				AudioCommand_PlayFar(111);
				if (count > 1)
				{
					selection++;
					pending = 1;
				}
			}
		}
	}
	menu->tab_index[0] = selection;
	menu->selected_owner = menu->owner_table[selection];
	menu->owner_ids[0] = menu->owner_table[selection];
	return result;
}

s32 CharacterSelector_MoveEntry(s32 selected_index, s32 direction)
{
	struct CharacterMenuState *state = gMenuWork;
	u32						   reordered[14];
	s32						   index;

	if (state->party_count <= 1)
	{
		return 0;
	}
	if (direction == 1)
	{
		if (selected_index == state->party_count - 1)
		{
			return 0;
		}
	}
	else
	{
		if (selected_index == 0)
		{
			return 0;
		}
	}

	{
		u32	 zero = 0;
		u32 *clear = &reordered[13];

		do
		{
			*clear = zero;
			clear--;
		} while ((s32)clear >= (s32)reordered);
	}
	for (index = 0; index < state->party_count; index++)
	{
		reordered[index] = state->owner_ids[index];
	}

	if (direction == 1)
	{
		index = reordered[selected_index];
		reordered[selected_index] = reordered[selected_index + 1];
		reordered[selected_index + 1] = index;
	}
	else
	{
		index = reordered[selected_index];
		reordered[selected_index] = reordered[selected_index - 1];
		reordered[selected_index - 1] = index;
	}

	for (index = 0; index < state->party_count; index++)
	{
		Party_RemoveActiveOwnerFar(state->owner_ids[index]);
	}
	for (index = 0; index < state->party_count; index++)
	{
		Party_AddActiveOwnerFar(reordered[index]);
	}
	state->party_count = Party_ListActiveOwnersFar(state->owner_ids);
	return 1;
}

void Menu_InitSelectorCursorAndEntries(s32 unused0, s32 unused1, s32 unused2, s32 unused3)
{
	struct CharacterMenuState *state;

	state = gMenuWork;
	PsynergyMenu_InitializeEntryObjects(UiMenu_CreateCursor(state), 2, 2, 8, 0);
	state->unknown_window_028 = 0;
	state->status_window = 0;
	state->help_window = 0;
	state->action_window = 0;
	state->selected_column = 0;
	state->selected_row = 0;
	state->column_count = 8;
	state->row_count = 2;
}

void Menu_CreateWindowAndEntryObjects(s32 resource, s32 frame)
{
	s32						   created;
	s32						   handle;
	struct RenderOutput		  *object;
	struct CharacterMenuState *state;

	state = gMenuWork;
	created = 0;
	handle = (s32)state->status_window;
	if (handle == 0)
	{
		created = UiWindow_UpdateOrCreate((s32 *)&state->status_window, 0, 5, 0x1E, 0xF, 2);
		handle = (s32)state->status_window;
	}
	if (created != 0)
	{
		object = SideObject_CreateFar(resource, 0, 0, handle, 0, 0);
		state->status_cursor = object;
		if ((object->sentinel = 0xF0, state->page) == 3)
		{
			Menu_SpawnIconEntries((struct InventoryMenuState *)state, handle);
		}
		Menu_CreateEightEntryObjects(handle);
		CharacterMenu_DrawStatusAilments((struct UiWindow *)handle, resource, 0x100);
		return;
	}
	CharacterMenu_DrawStatusAilments((struct UiWindow *)handle, resource, 0);
}

s32 CharacterMenu_SelectCommand(void)
{
	struct CharacterMenuState *menu;
	s32						   pane;
	s32						   result;
	s32						   selected;
	s8						   count;
	s32						   has_ailments;
	s32						   done;
	s32						   has_djinn;
	s32						   redraw;
	s32						   tab;
	s32						   total;
	s32						   i;
	u8						   entries[8];

	menu = gMenuWork;
	pane = 0;
	result = 0;
	selected = 0;
	total = Party_SumDjinnCountsFar(-1);
	has_djinn = 0;
	if (total != 0)
		has_djinn = 1;
	UiWindow_UpdateOrCreate((s32 *)&menu->help_window, 0, 0, 30, 5, 2);
	Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
	for (i = 3; i >= 0; i--)
		menu->row_y[i] = 104;
	done = 0;
	UiMenu_SlideCursor(-10, 88);

	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		UnitGetFar(menu->selected_ids[0]);
		CharacterMenu_DrawStatusAilments(menu->status_window, menu->selected_ids[0], 1);
		count = CharacterMenu_BuildAvailability(entries, 1, menu->selected_ids[0]);
		has_ailments = 0;
		if (count == 0)
			count = 1;
		else
			has_ailments = 1;
		redraw = 1;

		while (GameFlag_TestFar(0x150) == 0)
		{
			if (redraw != 0)
			{
				s32 limit;

				redraw = 0;
				limit = count;
				pane = (pane + 2) % 2;
				if (pane == 0)
				{
					selected = (selected + limit) % limit;
					RenderOutput_RedrawSavedRectFar((s32)menu->help_window);
					if (has_ailments == 0)
					{
						UiText_DrawCharacterAtOffsetFar(
							MsgSwitchCharacterHelp, (s32)menu->status_window, SWITCH_HELP_X, -24);
						UiText_DrawCharacterAtOffsetFar(
							MsgSwitchCharacterHelp + 1, (s32)menu->status_window, 0, -24);
					}
				}
				else
				{
					RenderOutput_RedrawSavedRectFar((s32)menu->help_window);
					if (has_djinn != 0)
						selected = (selected + 8) % 8;
					else
						selected = (selected + 7) % 7;
				}
				CharacterMenu_DrawSelectionCursor(pane, selected, entries, 0);
				RenderOutput_ClearListFar((s32)menu->help_window);
				WaitFrames(1);
				if (pane == 0)
					CharacterMenu_DrawSelectionLabels((s32)menu->help_window, selected, entries);
				else
					StatusMenu_ShowOwnerProgressMessage(menu->help_window, selected, has_djinn);
			}
			menu->owner_cursors[0]->active = 1;
			if (pane == 0)
				UiMenu_PositionCursor(-10, selected * 16 + 88);
			else if (selected <= 3)
				UiMenu_PositionCursor(24, selected * 8 + 48);
			else
				UiMenu_PositionCursor(48, selected * 8 + 80);
			WaitFrames(1);
			if (gKeysRepeat & KEYS_DPAD)
				CharacterMenu_DrawSelectionCursor(pane, selected, entries, 1);
			if (gKeyState & KEY_A)
			{
				AudioCommand_PlayFar(112);
				done = 1;
				result = 1;
				break;
			}
			if (gKeyState & KEY_B)
			{
				AudioCommand_PlayFar(113);
				done = 1;
				result = -1;
				break;
			}
			if (gKeysRepeat & KEY_UP)
			{
				AudioCommand_PlayFar(111);
				redraw = 1;
				selected--;
			}
			if (gKeysRepeat & KEY_DOWN)
			{
				AudioCommand_PlayFar(111);
				redraw = 1;
				selected++;
			}
			if (gKeysRepeat & KEY_RIGHT)
			{
				AudioCommand_PlayFar(111);
				redraw = 1;
				pane++;
			}
			if (gKeysRepeat & KEY_LEFT)
			{
				AudioCommand_PlayFar(111);
				redraw = 1;
				pane--;
			}
			if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L))
			{
				AudioCommand_PlayFar(111);
				tab = menu->owner_index[0];
				if (gKeysRepeat & KEY_R)
					tab++;
				else
					tab--;
				tab = (tab + menu->party_count) % menu->party_count;
				menu->selected_owner = menu->owner_ids[tab];
				menu->selected_ids[0] = menu->owner_ids[tab];
				menu->owner_index[0] = tab;
				PsynergyMenu_CallIconRoutineWithValue(menu, menu->owner_ids[tab]);
				break;
			}
		}
	}
	RenderOutput_ClearListFar((s32)menu->help_window);
	RenderOutput_RedrawSavedRectFar((s32)menu->help_window);
	UiWindow_ClearInteriorTilesFar((s32)menu->status_window, 64, 56, 224, 96);
	Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
	for (i = 3; i >= 0; i--)
		menu->row_y[i] = 128;
	ItemMenu_ResetCategory();
	return result;
}

void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries, s32 invert)
{
	struct CharacterMenuState *state = gMenuWork;
	u32						   different;
	s32						   count;
	s32						   index;
	s32						   x;
	s32						   y;
	s32						   last;
	u8						   width;

	if (mode == 0)
	{
		y = selected * 2 + 5;
		x = 0;
		width = 5;
		count = 0;
		index = 0;
		while (index <= 4)
		{
			if (entries[index] != 0)
			{
				if (selected == count)
				{
					width = CharacterMenu_CursorWidths[index];
					break;
				}
				count++;
			}
			index++;
		}
	}
	else if (selected <= 3)
	{
		y = selected;
		x = 5;
		width = 13;
	}
	else
	{
		y = selected + 4;
		x = 8;
		width = 20;
	}

	different = 1 ^ invert;
	last = 15 - (((0u - different) | different) >> 31);
	Render_SetTilemapFlagRect(state->status_window, x, y, width, 1, last);
}

void CharacterMenu_DrawSelectionLabels(s32 target, s32 selected, const u8 *entries)
{
	s32 count = 0;
	s32 index = 0;

	do
	{
		if (entries[index] != 0)
		{
			if (selected == count)
			{
				s32 message = MsgStatusAdvice + index * 2;
				UiText_DrawResourceFar(message, target, 0, -1);
				UiText_DrawResourceFar(message + 1, target, 0, 15);
			}
			count++;
		}
		index++;
	} while (index <= 4);

	if (count == 0)
		UiText_DrawResourceFar(MsgStatusNormal, target, 0, 0);
}

#if EDITION_INTERNATIONAL
#define PROGRESS_TEXT_MAX 0x80
#else
#define PROGRESS_TEXT_MAX 0x40
#endif

void StatusMenu_ShowOwnerProgressMessage(
	struct UiWindow *destination, s32 message_variant, s32 preserve_variant)
{
	struct CharacterMenuState *menu = gMenuWork;
	void					  *buffer;

	if (preserve_variant == 0 && message_variant > 3)
	{
		message_variant++;
	}
	if (message_variant == 1)
	{
		struct BtlUnit *owner = UnitGetFar(menu->selected_ids[0]);

		if (owner->level == 99)
		{
			message_variant = 8;
		}
		else
		{
#if EDITION_INTERNATIONAL
			u64 remaining = Owner_GetLevelThresholdFar(menu->selected_ids[0], owner->level + 1)
							- owner->experience;
#else
			u32 remaining = Owner_GetLevelThresholdFar(menu->selected_ids[0], owner->level + 1)
							- owner->experience;
#endif
			UiWork_PushValueSlotFar(remaining, 5);
		}
	}

	buffer = MemAlloc(PROGRESS_TEXT_MAX * 2);
	UiText_CopyMessageStringFar(message_variant + MsgProgressHelp, buffer, PROGRESS_TEXT_MAX);
	UiText_RenderWideStringAtOffsetFar(buffer, destination, 0, -1);
	MemFree(buffer);
}

void CharacterMenu_DrawStatusAilments(struct UiWindow *window, s32 unit, s32 mode)
{
	struct CharacterMenuState *state = gMenuWork;
	struct BtlUnit			  *status;
	s32						   row;
	s32						   has_djinn;
	s32						   keep;
	s32						   i;
	u8						   ailments[8];

	if (Party_SumDjinnCountsFar(-1) != 0)
		has_djinn = 1;
	else
		has_djinn = 0;
	status = UnitGetFar(unit);
	if ((mode & 0xff) == 1)
		row = 7;
	else
		row = 10;
	state->status_cursor->active = 1;
	ItemMenu_DrawOwnerStatus((s32)window, unit, mode);
	CharacterMenu_BuildAvailability(ailments, 1, unit);
	CharacterMenu_UpdateSelectionIcons(ailments);
	keep = mode & 0x100;
	if (keep == 0)
		UiWindow_ClearInteriorTilesFar(window, 0, 40, 96, 96);
	i = 0;
	if (ailments[CHARACTER_DOWN] != 0)
	{
		UiText_DrawCharacterAtOffsetFar(MsgDownLabel, window, 16, 40);
		i = 1;
	}
	if (ailments[CHARACTER_POISON] != 0)
	{
		UiText_DrawCharacterAtOffsetFar(MsgPoisonLabel, window, 16, i * 16 + 40);
		i++;
	}
	if (ailments[CHARACTER_VENOM] != 0)
	{
		UiText_DrawCharacterAtOffsetFar(MsgVenomLabel, window, 16, i * 16 + 40);
		i++;
	}
	if (ailments[CHARACTER_CURSE] != 0)
	{
		UiText_DrawCharacterAtOffsetFar(MsgCurseLabel, window, 16, i * 16 + 40);
		i++;
	}
	if (ailments[CHARACTER_HAUNT] != 0)
	{
		UiText_DrawCharacterAtOffsetFar(MsgHauntLabel, window, 16, i * 16 + 40);
		i++;
	}
	if (i == 0)
		UiText_DrawCharacterAtOffsetFar(MsgNormalLabel, window, 0, 40);
	CharacterMenu_UpdateSelectionIcons(ailments);
	ItemMenu_ApplyFlags(ailments);
	if (state->page == 3)
		return;
	if (keep == 0)
	{
		WaitFrames(1);
		UiWindow_ClearInteriorTilesFar(window, 64, 56, 224, 96);
	}
	UiWork_SetParamNibbleFar(15);
	if (mode == 1 || has_djinn == 1)
	{
		UiWindow_SetTilemapEntryFar(window, 1, 15, row, 4);
		UiWindow_SetTilemapEntryFar(window, 2, 19, row, 4);
		UiWindow_SetTilemapEntryFar(window, 3, 23, row, 4);
		UiWindow_SetTilemapEntryFar(window, 4, 27, row, 4);
	}
	if (has_djinn)
		UiText_DrawCharacterAtOffsetFar(MsgDjinnLabel, window, 64, row * 8 + 8);
	if (mode == 1)
	{
		if (!has_djinn)
			row--;
#if EDITION_INTERNATIONAL
		UiText_DrawStringAtOffsetFar(Menu_LvString, window, 64, row * 8 + 16);
#else
		UiText_DrawStringInWindowFar(Menu_LvString, window, 64, row * 8 + 16);
#endif
		UiText_DrawCharacterAtOffsetFar(MsgPowerLabel, window, 64, row * 8 + 24);
		UiText_DrawCharacterAtOffsetFar(MsgPowerLabel + 1, window, 64, row * 8 + 32);
	}
	for (i = 0; i <= 3; i++)
	{
		if (has_djinn)
			UiText_DrawNumberInWindowFar(
				status->djinn_owned_counts[i], 1, window, i * 32 + 120, row * 8 + 8);
		if ((mode & 0xff) == 1)
		{
			if (has_djinn)
			{
				UiText_DrawNumberInWindowFar(
					status->djinn_active_counts[i], 1, window, i * 32 + 104, row * 8 + 8);
				UiText_DrawStringInWindowFar(Menu_SlashString, window, i * 32 + 112, row * 8 + 8);
			}
			UiText_DrawNumberInWindowFar(
				Owner_GetResistanceValueFar(unit, i), 2, window, i * 32 + 112, row * 8 + 16);
			UiText_DrawNumberInWindowFar(status->elm[i].pow, 3, window, i * 32 + 104, row * 8 + 24);
			UiText_DrawNumberInWindowFar(status->elm[i].res, 3, window, i * 32 + 104, row * 8 + 32);
		}
	}
}

#if EDITION_INTERNATIONAL
void Runtime_RunBusyWaitDelay(void)
{
	s32 count = 0xff;

spin:
	count--;
	if (count >= 0)
		goto spin;
}

#endif

#if EDITION_INTERNATIONAL
#define STAT_DIGITS 3
#define STAT_X 200
#else
#define STAT_DIGITS 4
#define STAT_X 192
#endif

void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags)
{
	struct InventoryMenuState *menu;
	struct BtlUnit			  *unit;

	menu = gMenuWork;
	unit = UnitGetFar(owner);
	menu->cursor->active = 1;
	flags &= 0x100;
	if (flags == 0)
	{
		UiWindow_ClearInteriorTilesFar(window, 0, 0, 128, 40);
	}
	UiText_DrawStringAtOffsetFar(unit->name, window, 40, 0);
	UiText_DrawCharacterAtOffsetFar(unit->class_index + MsgClassName, window, 0, 32);
#if EDITION_INTERNATIONAL
	UiText_DrawStringAtOffsetFar(Menu_LvString, window, 104, 0);
#else
	UiText_DrawStringInWindowFar(Menu_LvString, window, 104, 0);
#endif
	UiWork_SetParamNibbleFar(15);
	UiText_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
#if EDITION_INTERNATIONAL
	UiText_DrawStringAtOffsetFar(Menu_HpString, window, 40, 16);
#else
	UiText_DrawStringInWindowFar(Menu_HpString, window, 40, 16);
#endif
	UiText_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
	UiText_DrawNumberInWindowFar(unit->max_hp, 4, window, 112, 16);
	UiText_DrawStringInWindowFar(Menu_SlashString, window, 104, 16);
#if EDITION_INTERNATIONAL
	UiText_DrawStringAtOffsetFar(Menu_PpString, window, 40, 24);
#else
	UiText_DrawStringInWindowFar(Menu_PpString, window, 40, 24);
#endif
	UiText_DrawNumberInWindowFar(unit->pp, 4, window, 72, 24);
	UiText_DrawNumberInWindowFar(unit->max_pp, 4, window, 112, 24);
	UiText_DrawStringInWindowFar(Menu_SlashString, window, 104, 24);
	UiText_DrawCharacterAtOffsetFar(MsgExpLabel, window, 40, 8);
	UiText_DrawNumberInWindowFar(unit->experience, 7, window, 88, 8);
	if (flags == 0)
	{
		WaitFrames(1);
		UiWindow_ClearInteriorTilesFar(window, 144, 0, 224, 40);
	}
	UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 23, window, 152, 0);
	UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 22, window, 152, 8);
	UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 21, window, 152, 16);
	UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 20, window, 152, 24);
	UiText_DrawNumberInWindowFar(unit->atk, STAT_DIGITS, window, STAT_X, 0);
	UiText_DrawNumberInWindowFar(unit->def, STAT_DIGITS, window, STAT_X, 8);
	UiText_DrawNumberInWindowFar(unit->agi, STAT_DIGITS, window, STAT_X, 16);
	UiText_DrawNumberInWindowFar(unit->luck, STAT_DIGITS, window, STAT_X, 24);
}

s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id)
{
	struct BtlUnit *state;
	s32				i;
	s32				count;
	s32				mode;

	state = UnitGetFar(id);
	for (i = 4; i >= 0; i--)
		output[i] = 0;

	count = 0;
	if (state->hp == 0 && requested == 1)
	{
		output[CHARACTER_DOWN] = requested;
		count = 1;
	}

	mode = state->poison;
	if (mode != 0)
	{
		if (mode == 1)
			output[CHARACTER_POISON] = mode;
		else
			output[CHARACTER_VENOM] = 1;
		count++;
	}
	if (state->restraint != 0)
	{
		output[CHARACTER_CURSE] = 1;
		count++;
	}
	if (state->haunt != 0)
	{
		output[CHARACTER_HAUNT] = 1;
		count++;
	}
	return count;
}

#if EDITION_INTERNATIONAL
#define GROUP_LEN 5
#else
#define GROUP_LEN 6
#endif

s32 Menu_BuildPageResult(struct MenuResult *result, s32 index)
{
	s32						   encoded;
	struct InventoryMenuState *menu = gMenuWork;
	s32						   limit;
	s32						   remainder;
	s32						   quotient;
	s32						   groups;
	s32						   value;

	encoded = (s32)UnitGetFar(menu->pane_owner[index]);
	limit = menu->item_count;
	value = menu->selected_index_by_owner[menu->pane_owner[index]];
	if ((value + 1) > limit)
	{
		value = limit - 1;
	}
	if (limit == 0)
	{
		value = 0;
	}
	quotient = value / GROUP_LEN;
	remainder = value % GROUP_LEN;
	groups = limit / GROUP_LEN;
	if (limit % GROUP_LEN != 0)
	{
		groups++;
	}
	result->owner_state = encoded;
	result->page = quotient;
	result->page_count = groups;
	result->row = remainder;
	result->entry_count = limit;
	result->selected_index = value;
	return 1;
}

static __inline__ u32 XorWord(u32 word, u32 mask)
{
	return word ^ mask;
}

extern const struct TileMask Menu_PatternRowMasks[];

void Menu_BuildPatternTiles(void)
{
#if EDITION_INTERNATIONAL
	u32 *vram = (u32 *)0x06005000;
#else
	u32 *vram = (u32 *)0x06004000;
#endif
	s32 set;
	s32 n;

	for (set = 0; set < 2; set++)
	{
		for (n = 0; n < 6; n++)
		{
			u32 *tile = vram + set * 0x60 + n * 0x10;
			s32	 x;

			Iwram_FillWords(tile, 64, 0x44444444);
			for (x = 1; x <= 7; x++)
			{
				s32 mi = n;

				if (set == 1 && x <= 1)
				{
					continue;
				}
				if (set == 0 && n > x - 2)
				{
					mi = x - 2;
					if (mi < 0)
					{
						mi = 0;
					}
				}
				tile[x] = XorWord(tile[x], Menu_PatternRowMasks[mi].word0);
				tile[x + 8] = XorWord(tile[x + 8], Menu_PatternRowMasks[mi].word1);
			}
		}
	}
}

void PsynergyMenu_DrawRange(s32 window, s32 x, s32 y, s32 range, s32 unused)
{
	s32 n;
	s32 tile;

	(void)unused;

	n = range * 2;
	tile = n + RANGE_ICON_TILE + 1;
	UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0);
	UiWindow_SetTilemapEntryFar(window, n + RANGE_ICON_TILE, x + 1, y, 0);
	UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused, struct MenuResult *state)
{
	struct PsynergyMenuState *menu;
	struct BtlAct			 *ability;
	s32						  row;
	s32						  base;

	menu = gMenuWork;
	state->selected_index = state->page * PAGE_ROWS + state->row;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
	RenderOutput_RedrawSavedRectFar(menu->info_window);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
	UiWindow_ClearInteriorTilesFar((s32)menu->info_window, 0, 0, 224, 8);
#else
	RenderOutput_ClearListFar(menu->info_window);
#endif
	WaitFrames(1);
	if (menu->psynergies[state->selected_index] != 0)
	{
#if EDITION_INTERNATIONAL
		UiText_DrawCharacterAtOffsetFar(
#else
		UiText_DrawResourceFar(
#endif
			(menu->psynergies[state->selected_index] & OWNER_ACTION_ID_MASK)
				+ MsgAbilityDescription,
			(s32)menu->info_window, 0, 0);
		ability = BtlActGet(menu->psynergies[state->selected_index] & OWNER_ACTION_ID_MASK);
#if EDITION_INTERNATIONAL
		UiWindow_ClearInteriorTilesFar(window, 0, 96, 224, 104);
		row = 0;
		if (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0)
		{
			row = 2;
		}
		if ((ability->tgt_flg & 0x80) != 0)
		{
			row |= 1;
		}
		if (row == 3)
		{
			UiText_DrawCharacterAtOffsetFar(MsgUsableAnywhere, window, 0, 96);
		}
		else if (row == 2)
		{
			UiText_DrawCharacterAtOffsetFar(MsgUsableInField, window, 0, 96);
		}
		else if (row == 1)
		{
			UiText_DrawCharacterAtOffsetFar(MsgUsableInBattle, window, 0, 96);
		}
#else
		UiWindow_ClearInteriorTilesFar(window, 0, 72, 64, 96);
		row = 0;
		if (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0)
		{
			UiText_DrawCharacterAtOffsetFar(MsgUsableInField, window, 0, 72);
			row = 1;
		}
		if ((ability->tgt_flg & 0x80) != 0)
		{
			UiText_DrawCharacterAtOffsetFar(MsgUsableInBattle, window, 0, row * 8 + 72);
			row++;
		}
		UiText_DrawCharacterAtOffsetFar(MsgCanBeUsed, window, 0, row * 8 + 72);
#endif
	}

	base = state->page * PAGE_ROWS;
	for (row = 0; row <= PAGE_ROWS - 1; row++)
	{
		if (row == state->row)
		{
			ability = BtlActGet(menu->psynergies[base + row] & OWNER_ACTION_ID_MASK);
			if (ability->damage_class != 4)
			{
				UiWindow_SetTilemapEntryFar(
					window, ability->damage_class + 1, 24, row * 2 + LIST_FIRST_ROW, 0);
				Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1, 14);
				Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1, 14);
			}
			else
			{
				Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1, 14);
			}
		}
		else
		{
			ability = BtlActGet(menu->psynergies[base + row] & OWNER_ACTION_ID_MASK);
			if (ability->damage_class != 4)
			{
				UiWindow_SetTilemapEntryFar(
					window, ability->damage_class + 1, 24, row * 2 + LIST_FIRST_ROW, 4);
				Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1, 15);
				Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1, 15);
			}
			else
			{
				Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1, 15);
			}
		}
	}
	WaitFrames(1);
	return 1;
}

s32 PsynergyMenu_DrawListPage(s32 window, s32 unused, const struct MenuResult *res)
{
	struct PsynergyMenuState *menu = gMenuWork;
	struct BtlUnit			 *owner;
	u32						  first;
	u32						  rows;
	u8						  row;
	s32						  ofs;

	(void)unused;

	owner = UnitGetFar(menu->owner_ids[0]);

	RenderOutput_RedrawSavedRectFar(window);

	first = res->page * PAGE_ROWS;
	rows = (u8)(res->entry_count - first);
	if (rows > PAGE_ROWS)
	{
		rows = PAGE_ROWS;
	}

	Menu_SetPageIcons(PAGE_ROWS, first, window, LIST_ICONS_X, LIST_ICONS_Y);
	Menu_DrawPageIndicator(window, res->entry_count, PAGE_ROWS, res->page, 28);

	UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window, LIST_PP_X, 0);

	row = 0;
	if (rows > row)
	{
		ofs = first;
		do
		{
			struct BtlAct *act;
			s32			   msg;
			s32			   y;
			s32			   range;
			act = BtlActGet(OWNER_ACTION_ID_MASK & menu->psynergies[ofs]);
			msg = (menu->psynergies[ofs] & OWNER_ACTION_ID_MASK) + MsgAbilityName;
			y = row * 16 + LIST_FIRST_ROW * 8;

			UiText_DrawCharacterAtOffsetFar(msg, window, LIST_NAME_X, y);
			UiText_DrawNumberAtOffsetFar(act->pp_cost, 2, window, LIST_PP_NUMBER_X, y);

			range = act->rng;
			if (range == 0xff)
			{
				range = 11;
			}
			else
			{
				range--;
			}
			PsynergyMenu_DrawRange(window, 25, row * 2 + LIST_FIRST_ROW, range, 0);

			row++;
			ofs++;
		} while (rows > row);
	}

	if (menu->psynergy_count == 0)
	{
		UiText_DrawCharacterAtOffsetFar(MsgNoPsynergy, window, 96, LIST_EMPTY_Y);
	}

	UiText_DrawStringAtOffsetFar(owner->name, (void *)window, 40, 0);
	UiText_DrawCharacterAtOffsetFar(owner->class_index + MsgClassName, window, 0, 32);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
	UiText_DrawStringAtOffsetFar(&Menu_LvString, (void *)window, 0, 48);
#else
	UiText_DrawStringInWindowFar(&Menu_LvString, window, 0, 48);
#endif
	UiText_DrawNumberInWindowFar(owner->level, 2, window, 24, 48);

	return 1;
}

struct BtlUnit;

s32 PsynergyMenu_SelectAction(void)
{

	struct PsynergyMenuState *menu;
	s32						  result;
	s32						  done;
	s32						  redraw;
	s32						  first;
	s32						  nav;
	s32						  tab;
	s32						  i;
	struct MenuResult		  state;

	menu = gMenuWork;
	result = 0;
	done = 0;
	Menu_BuildPatternTiles();
	RenderOutput_RedrawSavedRectFar(menu->message_window);
	UiWindow_UpdateOrCreate((s32 *)&menu->info_window, 0, 0, 30, 5, 2);
	for (i = 3; i >= 0; i--)
		menu->slot_y[i] = -16;
	{
		s32					  priority = 245;
		struct RenderOutput **icons = menu->entry_icons;

		for (i = 31; i >= 0; i--)
		{
			struct RenderOutput *icon = *icons++;

			if (icon != 0)
				icon->sentinel = priority;
		}
	}
	Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
	Menu_SpawnIconEntries(menu, menu->message_window);
	UiText_DrawCharacterAtOffsetFar(
		MsgSwitchCharacterHelp, menu->status_window, SWITCH_HELP_X, -24);
	UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 2, menu->status_window, 0, -24);

	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		ItemMenu_PosCategory();
		RenderOutput_RedrawSavedRectFar(menu->status_window);
		menu->psynergy_count =
			PsynergyMenu_CollectActions(UnitGetFar(menu->owner_ids[0]), menu->psynergies, 0);
		WaitFrames(1);
		Menu_BuildPageResult(&state, 0);
		PsynergyMenu_DrawPreparedPsynergyIcons(menu->status_window, menu->owner_ids[0]);
		redraw = 1;
		first = 1;

		while (GameFlag_TestFar(0x150) == 0)
		{
			if (redraw != 0)
			{
				redraw = 0;
				if (first != 0)
				{
					first = 0;
					PsynergyMenu_DrawListPage(menu->status_window, 0, &state);
				}
				PsynergyMenu_DrawRangePage(menu->status_window, 0, &state);
				WaitFrames(1);
			}
			WaitFrames(1);
			nav = Menu_HandlePageInput(0, state.entry_count, PAGE_ROWS, &state.row, &state.page);
			menu->pane_icon[0]->active = 1;
#if EDITION_INTERNATIONAL
			UiMenu_PositionCursor(55, state.row * 16 + 60);
#else
			UiMenu_PositionCursor(58, state.row * 16 + 52);
#endif
			if (nav == 1)
			{
				first = 1;
				redraw = 1;
			}
			if (nav == 0)
				redraw = 1;
			if (nav == -1)
				redraw = 0;
			if (gKeyState & KEY_A)
			{
				AudioCommand_PlayFar(112);
				result = 1;
				done = 1;
				break;
			}
			if (gKeyState & KEY_B)
			{
				AudioCommand_PlayFar(113);
				result = -1;
				done = 1;
				Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
				break;
			}
			if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L))
			{
				AudioCommand_PlayFar(111);
				tab = menu->tab_index[0];
				menu->selected_index_by_owner[menu->owner_table[tab]] = state.selected_index;
				if (gKeysRepeat & KEY_R)
					tab++;
				else
					tab--;
				tab = (tab + menu->owner_count) % menu->owner_count;
				menu->selected_owner = menu->owner_table[tab];
				menu->owner_ids[0] = menu->owner_table[tab];
				menu->tab_index[0] = tab;
				PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_table[tab]);
				break;
			}
		}
	}
	RenderOutput_ClearListFar((s32)menu->info_window);
	RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
	ItemMenu_HideAllIcons();
	RenderOutput_ClearListFar(menu->message_window);
	RenderOutput_RedrawSavedRectFar(menu->status_window);
	return result;
}

s32 PsynergyMenu_ActionReturnTrue(void)
{
	return 1;
}

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
	struct PsynergyMenuState *menu = gMenuWork;

	UnitGetFar(owner_id);
	ItemMenu_HideAllIcons();
	PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void)
{
}

void PsynergyMenu_PreparedIconsSecondNoOp(void)
{
}

#if EDITION_INTERNATIONAL
#define EQUIP_SLOT_X 1
#define EQUIP_SLOT_WIDTH 12
#else
#define EQUIP_SLOT_X 2
#define EQUIP_SLOT_WIDTH 9
#endif

s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state)
{
	struct InventoryMenuState *menu;
	struct ItemDefinition	  *item;
	s32						   base;
	s32						   row;

	menu = gMenuWork;
	base = state->page * PAGE_ROWS;
	state->selected_index = base + state->row;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
	RenderOutput_RedrawSavedRectFar(menu->info_window);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
	UiWindow_ClearInteriorTilesFar((s32)menu->info_window, 0, 0, 224, 8);
#else
	RenderOutput_ClearListFar(menu->info_window);
#endif
	WaitFrames(1);
	if (menu->items[state->selected_index] != 0)
	{
#if EDITION_INTERNATIONAL
		UiText_DrawCharacterAtOffsetFar(
			(menu->items[state->selected_index] & ITEM_ID_MASK) + MsgItemPlainName,
			(s32)menu->info_window, 0, 0);
#else
		UiText_DrawResourceFar(
			(menu->items[state->selected_index] & ITEM_ID_MASK) + MsgItemPlainName,
			(s32)menu->info_window, 0, 0);
#endif
	}

	for (row = 0; row < PAGE_ROWS; row++)
	{
		if (row == state->row)
		{
			item = Item_GetDirectFar(menu->items[state->selected_index] & ITEM_ID_MASK);
			if (item->element != 4)
			{
				UiWindow_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1, 0);
				Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 14);
			}
			else
			{
				Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 14);
			}
		}
		else
		{
			item = Item_GetDirectFar(menu->items[base + row] & ITEM_ID_MASK);
			if (item->element != 4)
			{
				UiWindow_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1, 4);
				Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 15);
			}
			else
			{
				Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 15);
			}
		}
	}

	for (row = 0; row <= 3; row++)
	{
		Render_SetTilemapFlagRect(
			(s32)menu->equip_window, EQUIP_SLOT_X, row * 2 + 1, EQUIP_SLOT_WIDTH, 1, 15);
	}

	if (menu->items[state->selected_index] & INVENTORY_EQUIPPED)
	{
		item = Item_GetDirectFar(menu->items[state->selected_index] & ITEM_ID_MASK);
		switch (item->type)
		{
		case 1:
			Render_SetTilemapFlagRect(
				(s32)menu->equip_window, EQUIP_SLOT_X, 1, EQUIP_SLOT_WIDTH, 1, 14);
			break;
		case 4:
			Render_SetTilemapFlagRect(
				(s32)menu->equip_window, EQUIP_SLOT_X, 3, EQUIP_SLOT_WIDTH, 1, 14);
			break;
		case 3:
			Render_SetTilemapFlagRect(
				(s32)menu->equip_window, EQUIP_SLOT_X, 5, EQUIP_SLOT_WIDTH, 1, 14);
			break;
		case 2:
			Render_SetTilemapFlagRect(
				(s32)menu->equip_window, EQUIP_SLOT_X, 7, EQUIP_SLOT_WIDTH, 1, 14);
			break;
		}
	}
	WaitFrames(1);
	return 1;
}

#if EDITION_INTERNATIONAL
#define SHOP_PAGE_ICONS_X 119
#define SHOP_ITEM_NAME_X 128
#define SHOP_STAT_X 80
#else
#define SHOP_PAGE_ICONS_X 123
#define SHOP_ITEM_NAME_X 136
#define SHOP_STAT_X 72
#endif

s32 Shop_DrawItemPage(s32 window, s32 unused, struct MenuResult *state)
{
	struct InventoryMenuState *menu = gMenuWork;
	struct BtlUnit			  *unit;
	u16						  *item;
	s32						   first;
	u32						   count;
	u32						   row;
	s32						   message;

	unit = UnitGetFar(menu->pane_owner[0]);
	UiWindow_ClearInteriorTilesFar(window, 128, 8, 224, 96);
	first = state->page * PAGE_ROWS;
	count = (u8)(state->entry_count - first);
	if (count > PAGE_ROWS)
		count = PAGE_ROWS;
	Menu_SetPageIcons(PAGE_ROWS, first, window, SHOP_PAGE_ICONS_X, 52);
	Menu_DrawPageIndicator(window, state->entry_count, PAGE_ROWS, state->page, 28);
	if (menu->item_count == 0)
	{
#if defined(TBS_EDITION_DE)
		UiText_DrawCharacterAtOffsetFar(MsgItemMenuEmpty, window, 112, 8);
#else
		UiText_DrawCharacterAtOffsetFar(MsgItemMenuEmpty, window, 120, 8);
#endif
	}
	else
	{
		row = 0;
		if (count > row)
		{
			item = &menu->items[first];
			do
			{
				UiText_DrawCharacterAtOffsetFar(
					(*item & ITEM_ID_MASK) + MsgItemName, window, SHOP_ITEM_NAME_X, (row << 4) + 8);
				row = (u8)(row + 1);
				item++;
			} while (count > row);
		}
	}
	UiText_DrawStringAtOffsetFar((s32)unit->name, window, 40, 0);
	message = MsgStatLabel;
	UiText_DrawCharacterAtOffsetFar(message, window, 32, 16);
	UiText_DrawCharacterAtOffsetFar(message + 1, window, 32, 24);
	UiText_DrawNumberInWindowFar(unit->atk, 3, window, SHOP_STAT_X, 16);
	UiText_DrawNumberInWindowFar(unit->def, 3, window, SHOP_STAT_X, 24);
	return 1;
}

s32 ItemMenu_SelectItem(void)
{

	struct InventoryMenuState *menu;
	struct BtlUnit			  *owner;
	s32						   result;
	s32						   done;
	s32						   redraw;
	s32						   first;
	s32						   nav;
	s32						   tab;
	s32						   i;
	u16						   saved[15];
	struct MenuResult		   state;

	menu = gMenuWork;
	{
		s32 id = menu->pane_owner[0];

		result = 0;
		UnitGetFar(menu->owner_ids[id]);
	}
#if EDITION_INTERNATIONAL
	UiWindow_UpdateOrCreate((s32 *)&menu->equip_window, 0, 10, 15, 10, 2);
#else
	UiWindow_UpdateOrCreate((s32 *)&menu->equip_window, 0, 10, 13, 10, 2);
#endif
	ItemMenu_PosCategory();

	for (i = 0; i < 32; i++)
	{
		struct RenderOutput *icon = menu->entry_icons[i];

		if (icon != 0)
			icon->sentinel = 240;
	}

	Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
	UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, (s32)menu->status_window, 64, -24);
	UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 3, (s32)menu->status_window, 0, -24);
	ItemMenu_PosCategory();
	WaitFrames(1);
	Menu_SpawnIconEntries(menu, (s32)menu->message_window);

	done = 0;
	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		ItemMenu_PosCategory();
		RenderOutput_RedrawSavedRectFar(menu->status_window);
		owner = UnitGetFar(menu->pane_owner[0]);
		for (i = 0; i <= 14; i++)
			saved[i] = owner->inventory[i];
		menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
		InventoryMenu_SortByListOrder(menu->items, 0);
		Menu_BuildPageResult(&state, 0);
		RenderOutput_RedrawSavedRectFar(menu->equip_window);
		ItemMenu_DrawCategory((s32)menu->equip_window, menu->pane_owner[0], 1);
		WaitFrames(1);
		ItemMenu_DrawIcons(menu->items, 0);
		redraw = 1;
		first = 1;

		while (GameFlag_TestFar(0x150) == 0)
		{
			if (redraw != 0)
			{
				RenderOutput_ClearListFar(menu->info_window);
				redraw = 0;
				if (first != 0)
				{
					first = 0;
					Shop_DrawItemPage((s32)menu->status_window, 0, &state);
				}
				ItemMenu_DrawEquipPage((s32)menu->status_window, 0, &state);
			}
			menu->pane_icons[0]->active = 1;
			UiMenu_PositionCursor(96, state.row * 16 + 52);
			WaitFrames(1);

			nav = Menu_HandlePageInput(0, state.entry_count, PAGE_ROWS, &state.row, &state.page);
			if (nav == 1)
			{
				first = 1;
				redraw = 1;
			}
			if (nav == 0)
				redraw = 1;
			if (nav == -1)
				redraw = 0;

			if (gKeyState & KEY_A)
			{
				AudioCommand_PlayFar(0x70);
				result = 1;
				done = 1;
				break;
			}
			if (gKeyState & KEY_B)
			{
				AudioCommand_PlayFar(0x71);
				result = -1;
				done = 1;
				break;
			}
			if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L))
			{
				AudioCommand_PlayFar(0x6f);
				tab = menu->pane_index[0];
				menu->selected_index_by_owner[menu->owner_ids[tab]] = state.selected_index;
				if (gKeysRepeat & KEY_R)
					tab++;
				else
					tab--;
				for (i = 0; i <= 14; i++)
					owner->inventory[i] = saved[i];
				tab = (tab + menu->party_count) % menu->party_count;
				menu->selected_owner = menu->owner_ids[tab];
				menu->pane_owner[0] = menu->owner_ids[tab];
				menu->pane_index[0] = tab;
				PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_ids[tab]);
				break;
			}
		}
	}

	RenderOutput_ClearListFar(menu->info_window);
	RenderOutput_RedrawSavedRectFar(menu->info_window);
	RenderOutput_ClearListFar(menu->message_window);
	ItemMenu_HideAllIcons();
	RenderOutput_RedrawSavedRectFar(menu->status_window);
	Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
	return result;
}

s32 Menu_MoveGridCursor(s32 *arg0, s32 *arg1, s32 arg2)
{
	s32 row;
	s32 col;

	col = *arg0;
	row = *arg1;
	switch (arg2)
	{
	case 0x40:
		row -= 1;
		if (row < 0)
		{
			row = 5;
		}
		if (row <= 3)
		{
			if (row == 3)
			{
				if (col <= 4)
				{
					col = 0;
				}
				else
				{
					col = 1;
				}
			}
			else if (col > 1)
			{
				col = 1;
			}
			if ((row == 3) && (col == 1))
			{
				row = 2;
			}
		}
		break;
	case 0x80:
		row += 1;
		if (row > 5)
		{
			row = 0;
		}
		if ((row == 3) && (col == 1))
		{
			row = 4;
		}
		if (row == 4)
		{
			col = 0;
		}
		break;
	case 0x20:
		col -= 1;
		if (row == 3)
		{
			col += 1;
		}
		else if (row > 3)
		{
			if (col < 0)
			{
				col = 7;
			}
		}
		else if (col < 0)
		{
			col = 1;
		}
		break;
	case 0x10:
		col += 1;
		if (row == 3)
		{
			col -= 1;
		}
		else if (row > 3)
		{
			if (col > 7)
			{
				col = 0;
			}
		}
		else if (col > 1)
		{
			col = 0;
		}
		break;
	}
	*arg0 = col;
	*arg1 = row;
	return (row * 9) + col;
}

s32 Menu_ReservedStatusOne(void)
{
	return 1;
}

void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode)
{
	struct InventoryMenuState *menu = gMenuWork;
	u16						  *items;

	UnitGetFar(owner_id);
	ItemMenu_PosCategory();
	ItemMenu_HideAllIcons();
	UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels, window, 0, 0);
	UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 1, window, 0, 32);
	UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 2, window, 0, 16);
	UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 3, window, 0, 48);
	items = menu->items;
	ItemMenu_DrawEquippedItemNames(window, items);
	if (mode == 0)
	{
		WaitFrames(1);
		ItemMenu_DrawIcons(items, 1);
		ItemMenu_ArrangeCategoryItemIcons(items);
	}
}

#if EDITION_INTERNATIONAL
#define NAME_X 8
#else
#define NAME_X 16
#endif

void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items)
{
	s32 i;
	s32 item;

	for (i = 0; i < 15; i++)
	{
		if (items[i] & INVENTORY_EQUIPPED)
		{
			item = items[i] & ITEM_ID_MASK;
			switch (Item_GetDirectFar(item)->type)
			{
			case 1:
				UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, NAME_X, 8);
				break;
			case 2:
				UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, NAME_X, 56);
				break;
			case 3:
				UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, NAME_X, 40);
				break;
			case 4:
				UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, NAME_X, 24);
				break;
			}
		}
	}
}

void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase)
{
	s32					  i;
	struct RenderOutput	 *obj;
	struct RenderOutput **tbl;

	i = 0;
	tbl = ((struct InventoryMenuState *)gMenuWork)->entry_icons;
	do
	{
		obj = *tbl++;
		if (obj != NULL)
		{
			Menu_PlaceEntryObjectInGrid(obj, i, origin_x, origin_y, phase);
		}
		i += 1;
	} while (i <= 0x1F);
}

void Menu_PlaceEntryObjectInGrid(
	struct RenderOutput *obj, s32 index, s32 origin_x, s32 origin_y, s32 phase)
{
	s32 no;

	no = index;
	if (no > 0x1F)
	{
		no = 0;
	}
	obj->y = (no / phase * 0x10) + origin_y;
	obj->x = (no % phase * 0x10) + origin_x;
	UiIcon_PrepareObject(obj);
}

void ItemMenu_ArrangeCategoryItemIcons(u16 *items)
{
	struct InventoryMenuState *state;
	struct RenderOutput		  *icon;
	s32						   i;

	state = gMenuWork;
	ItemMenu_PosCategory();
	for (i = 0; i < 15; i++)
	{
		if (items[i] != 0 && (items[i] & INVENTORY_EQUIPPED) != 0)
		{
			icon = state->entry_icons[i];
			if (icon != 0)
			{
				switch (Item_GetDirectFar(items[i] & ITEM_ID_MASK)->type)
				{
				case 1:
					icon->x = 216;
					icon->y = 32;
					break;
				case 2:
					icon->x = 216;
					icon->y = 80;
					break;
				case 3:
					icon->x = 216;
					icon->y = 64;
					break;
				case 4:
					icon->x = 216;
					icon->y = 48;
					break;
				}
				UiIcon_PrepareObject(icon);
			}
		}
	}
}

void ItemMenu_PosCategory(void)
{
	struct InventoryMenuState *state = gMenuWork;
	s32						   x = 248;
	struct RenderOutput		 **entry = state->entry_icons;
	s32						   y = 168;
	s32						   remaining = 31;

	do
	{
		struct RenderOutput *object = *entry++;

		if (object != 0)
		{
			object->x = x;
			object->y = y;
			UiIcon_PrepareObject(object);
		}
		remaining--;
	} while (remaining >= 0);
}

s32 Menu_CreateEightEntryObjects(s32 resource)
{
	struct RenderOutput		 **slot;
	struct RenderOutput		  *obj;
	s32						   i;
	struct InventoryMenuState *state;
	s32						   param;

	state = gMenuWork;
	i = 0;
	param = 0xA8;
	slot = state->category_icons;
	do
	{
		obj = RenderOutput_CreateFromResourceFar(2, i, (struct RenderInput *)resource, 0xF8, param);
		i += 1;
		*slot = obj;
		slot += 1;
	} while (i <= 7);
	return 1;
}

void ItemMenu_ApplyFlags(const u8 *flags)
{
	// The y-field pointer is the loop's 18th real insn before combine; loop.c reduces the flags[index] giv only while its 17 reaches the insn count.
	/* FAKEMATCH: the y-field pointer keeps the category flag index a counter, not a walking pointer, as native; entry->y = value in this loop, a for loop over category_icons[index], ++index in the test or the two tests nested differ 22, value as s16, u16 or s8 23 to 28, value += 16 before the sentinel store 2 (the add moves ahead of the call). */
	struct InventoryMenuState *base;
	struct RenderOutput		 **slot;
	struct RenderOutput		  *entry;
	s32						   index;
	s32						   value;

	base = gMenuWork;
	ItemMenu_ResetCategory();
	index = 0;
	slot = base->category_icons;
	value = 88;
	do
	{
		entry = *slot++;
		if (entry != 0 && flags[index] != 0)
		{
			entry->x = 8;
			{
				s16 *y = &entry->y;

				*y = value;
			}
			entry->sentinel = 240;
			UiIcon_PrepareObject(entry);
			value += 16;
		}
		index++;
	} while (index <= 4);
}

void ItemMenu_ResetCategory(void)
{
	struct InventoryMenuState *state = gMenuWork;
	s32						   index;

	for (index = 0; index < 5; index++)
	{
		struct RenderOutput *object = state->category_icons[index];

		if (object != 0)
		{
			object->x = 248;
			object->y = 168;
			object->sentinel = 240;
			UiIcon_PrepareObject(object);
		}
	}
}

s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled)
{
	struct InventoryMenuState *state = gMenuWork;
	s32						   index = 0;

	do
	{
		if (enabled[index] != 0)
		{
			s32 kind;
			switch (index)
			{
			case CHARACTER_DOWN:
				kind = 16;
				break;
			case CHARACTER_POISON:
				kind = 1;
				break;
			case CHARACTER_VENOM:
				kind = 2;
				break;
			case CHARACTER_CURSE:
				kind = 15;
				break;
			case CHARACTER_HAUNT:
				kind = 7;
				break;
			default:
				kind = 0;
				break;
			}
			Resource_LoadByModeIntoSlotFar(8, kind, (u8)state->category_icons[index]->index, 0);
		}
		index++;
	} while (index <= 4);
	return 1;
}

void Item_PrepareUsePalette(void)
{
	Palette_CopyObjectBankToBackground14();
	Palette_LightenBankHighlight(13);
}

void Item_UseNoOpCallback(void)
{
}

s32 Item_Use(s32 slot, s32 owner_id, s32 target_id)
{
	struct InventoryMenuState *work;
	s32						   result;
	s32						   item_id;
	struct BtlUnit			  *owner;
	struct ItemDefinition	  *item;

	owner = UnitGetFar(owner_id);
	work = gMenuWork;
	item_id = ITEM_ID_MASK & owner->inventory[slot];
	item = Item_GetDirectFar(item_id);
	result =
		BattleEffect_ApplyToTargets(OWNER_ACTION_ID_MASK & item->action_id, owner_id, target_id, 1);
	if (result != -1)
	{
		item = Item_GetDirectFar(owner->inventory[slot]);
		if (item->use_type == 1)
		{
			Inventory_RemoveFar(owner_id, slot);
			work->item_count = ItemMenu_Collect(owner, work->items, 0);
		}
		if (item->use_type == 4)
		{
			if (item_id == 0xb8)
				item_id = 0xb9;
			owner->inventory[slot] = item_id;
		}
		result = 0;
	}
	return result;
}

s32 Item_ReturnOne(void)
{
	return 1;
}

s32 BattleEffect_ApplyToTargets(s32 effect_id, s32 source_id, s32 target_id, s32 fixed_scale)
{
	struct BtlAct			  *effect;
	struct InventoryMenuState *runtime;
	struct BtlUnit			  *target;
	struct BtlUnit			  *source;
	s32						   later_target;
	s32						   changed;
	u8						   index;
	s16						   scale;
	s32						   random_adjust;
	s32						   random_nonone;
	s32						   amount;
	s32						   result_code;

	effect = BtlActGet(effect_id);
	runtime = gMenuWork;
	changed = 0;
	result_code = 0;
	later_target = 0;

	if (target_id != 9)
		target = UnitGetFar(target_id);
	else
		target = UnitGetFar(0);

	/* FAKEMATCH: the dead test loads effect->rng ahead of the count
	 * guard, where the native keeps it for the first pass through the
	 * loop; it preserves the target-index and power-load registers.
	 * Measured: without it the load moves past the guard and the
	 * allocation shifts (485 instructions for 484, differ 49). */
	if (effect->rng == 0xff && later_target != 0)
		result_code = 3;

	for (index = 0; index < runtime->party_count; index++)
	{
		if (effect->rng == 0xff)
		{
			target_id = runtime->owner_ids[index];
			target = UnitGetFar(target_id);
		}

		amount = effect->pow;
		switch (effect->tgt_flg & 0xf)
		{
		case BATTLE_DAMAGE_HP_HEAL:
			if (fixed_scale == 0)
			{
				if (effect->damage_class != 4)
				{
					s32 stat_offset;

					source = UnitGetFar(source_id);
					stat_offset = effect->damage_class;
					scale = source->elm[stat_offset].pow;
				}
				else
					scale = 100;
				amount = BtlRcvCalc(amount, scale, 0x100);
			}

			if (target->hp <= 0)
			{
				if (later_target == 0)
					result_code = 2;
			}
			else
			{
				if (target->hp == target->max_hp)
				{
					if (later_target == 0)
						result_code = 4;
				}
				else
				{
					target->hp += amount;
					if (target->hp > target->max_hp)
					{
						amount -= target->hp - target->max_hp;
						target->hp = target->max_hp;
						if (later_target == 0)
							result_code = 0;
					}
					else if (later_target == 0)
					{
						result_code = 1;
					}
					UnitRatioFar(target_id);
					changed = 1;
					if (effect->rng == 0xff)
					{
						later_target = 1;
						result_code = 3;
					}
				}
			}
			break;

		case 9:
			random_adjust = (Random16() * 4) >> 16;
			if (random_adjust == 0)
			{
				random_adjust = -1;
			}
			else
			{
				random_nonone = 1 ^ random_adjust;
				random_adjust = (u32)((-random_nonone) | random_nonone) >> 31;
				random_adjust = 1 - random_adjust;
			}

			switch (effect_id & OWNER_ACTION_ID_MASK)
			{
			case 0x104:
				target->base_hp += amount + random_adjust;
				result_code = 0x10;
				changed = 1;
				break;
			case 0x105:
				target->base_pp += amount + random_adjust;
				result_code = 0x11;
				changed = 1;
				break;
			case 0x108:
				target->base_agility += amount + random_adjust;
				result_code = 0x12;
				changed = 1;
				break;
			case 0x109:
				target->base_luck += amount;
				result_code = 0x13;
				changed = 1;
				break;
			case 0x106:
				target->base_attack += amount + random_adjust;
				UiWork_PushValueSlotFar(3, 5);
				result_code = 0x14;
				changed = 1;
				break;
			case 0x107:
				target->base_defense += amount + random_adjust;
				UiWork_PushValueSlotFar(4, 5);
				result_code = 0x15;
				changed = 1;
				break;
			}
			break;

		case BATTLE_DAMAGE_PP_HEAL:
			if (target->pp == target->max_pp)
			{
				if (later_target == 0)
					result_code = 7;
			}
			else
			{
				target->pp += amount;
				if (target->pp > target->max_pp)
				{
					amount -= target->pp - target->max_pp;
					target->pp = target->max_pp;
					if (later_target == 0)
						result_code = 5;
				}
				else if (later_target == 0)
				{
					result_code = 6;
				}
				UnitRatioFar(target_id);
				changed = 1;
				if (effect->rng == 0xff)
				{
					later_target = 1;
					result_code = 8;
				}
			}
			break;

		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 10:
			break;
		}

		switch (effect->efx)
		{
		case 1:
			if (target->hp <= 0 || target->hp == target->max_hp)
			{
				if (later_target == 0)
					result_code = 2;
			}
			else
			{
				target->hp += amount;
				if (target->hp > target->max_hp)
				{
					target->hp = target->max_hp;
					if (later_target == 0)
						result_code = 0;
				}
				else if (later_target == 0)
				{
					result_code = 1;
				}
				UnitRatioFar(target_id);
				changed = 1;
			}
			break;

		case 2:
			if (target->pp == target->max_pp)
			{
				if (later_target == 0)
					result_code = 7;
			}
			else
			{
				target->pp += amount;
				if (target->pp > target->max_pp)
				{
					target->pp = target->max_pp;
					if (later_target == 0)
						result_code = 5;
				}
				else if (later_target == 0)
				{
					result_code = 6;
				}
				UnitRatioFar(target_id);
				changed = 1;
			}
			break;

		case EFX_REVIVE_FULL:
			if (target->hp == 0)
			{
				target->hp = target->max_hp;
				UnitRatioFar(target_id);
				changed = 1;
				if (later_target == 0)
					result_code = 0xc;
			}
			else if (later_target == 0)
			{
				result_code = 0xd;
			}
			break;

		case EFX_REVIVE_HALF:
			if (target->hp == 0)
			{
				target->hp = target->max_hp / 2;
				UnitRatioFar(target_id);
				if (later_target == 0)
					result_code = 0xc;
			}
			else if (later_target == 0)
			{
				result_code = 0xd;
			}
			break;

		case EFX_REVIVE_80:
			if (target->hp == 0)
			{
				target->hp = target->max_hp * 7 / 10;
				UnitRatioFar(target_id);
				if (later_target == 0)
					result_code = 0xc;
			}
			else if (later_target == 0)
			{
				result_code = 0xd;
			}
			break;

		case EFX_CURE_POISON:
			if (target->poison != 0)
			{
				target->poison = 0;
				changed = 1;
				if (later_target == 0)
					result_code = 0xa;
			}
			else if (later_target == 0)
			{
				result_code = 0xb;
			}
			break;
		}

		if (effect->rng != 0xff)
			break;
	}

	if (changed == 0)
	{
		runtime->message_offset = result_code;
		return -1;
	}

	for (index = 0; index < runtime->party_count; index++)
		UnitCalcFar(runtime->owner_ids[index]);
	runtime->message_offset = result_code;
	return 0;
}

void Item_PlayUseAnimation(u32 item)
{
	Ability_PlayUseAnimation(Item_GetDirectFar(item)->action_id & OWNER_ACTION_ID_MASK);
}

void Ability_PlayUseAnimation(s32 action)
{
	s32			   animation_type;
	u32			   target_type;
	struct BtlAct *ability;

	ability = BtlActGet(action);
	animation_type = 0xf & ability->tgt_flg;
	switch (animation_type)
	{
	case 1:
		Audio_PlayCueReturnOne(0x7e);

	case 11:
		Audio_PlayCueReturnOne(0x7e);
		return;
	default:
		target_type = ability->efx - 1;
		switch (target_type)
		{
		case 4:
			Audio_PlayCueReturnOne(0x52);
			return;
		case 2:
			Audio_PlayCueReturnOne(0x54);
			return;
		default:
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
			Audio_PlayCueReturnOne(0x5b);

		case 0:
		case 1:
		case 30:
		case 31:
			return;
		}
		break;
	}
}

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

// 移した Djinn を一覧の中で探し、その位置を選び直す
/* FAKEMATCH: a statement macro local to DjinnMenu_RunScreen, used at its two places, not a habit of the game (S2): the do { } while (0) wrapper ends the selection store's scheduling region. As plain statements the two shifts of found * DJINN_LIST_ROWS swap (differ 2); as an inline function with its own counter, one more register is saved. */
#define DjinnMenu_ReselectPairEntry(menu, i)                              \
	do                                                                    \
	{                                                                     \
		s32 column;                                                       \
		s32 found;                                                        \
		u8	entry;                                                        \
                                                                          \
		column = (u16)((u32)(menu)->selection[1] % DJINN_LIST_ROWS);      \
		found = 0;                                                        \
		entry = (menu)->pair_entries[0];                                  \
		for ((i) = 0; (i) < (menu)->backdrop->list.counts[column]; (i)++) \
		{                                                                 \
			if (entry == (u8)(menu)->backdrop->list.ids[column][(i)])     \
			{                                                             \
				found = (i);                                              \
				break;                                                    \
			}                                                             \
		}                                                                 \
		(menu)->selection[0] = column + found * DJINN_LIST_ROWS;          \
	} while (0)

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
			DjinnMenu_ReselectPairEntry(menu, i);
			menu->cursor->active = 1;
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
			DjinnMenu_ReselectPairEntry(menu, i);
			menu->cursor->active = 1;
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
	UiText_DrawCharacterAtOffsetFar(MsgUnleashEffect, menu->list_window, 0, 80);
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
					MsgDjinnName + ((ENTRY & DJINN_ENTRY_ELEMENT) >> 5) * DJINN_PER_ELEMENT
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
	message = MsgDjinnInfoPrompt;
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
	list_message = MsgDjinnInfoTopic;
	do
	{
		UiText_DrawCharacterAtOffsetFar(cnt + list_message, list, 0, cnt * 8);
		cnt++;
	} while (cnt <= 6);
	do
	{
		RenderOutput_PrepareForRedrawFar(win_a);
		UiText_DrawResourceFar(selection + MsgDjinnInfoTopic, win_a, 0, 0);
		channel =
			(struct UiChannelSlot *)UiText_OpenEntryMessageFar(win_b, selection + MsgDjinnInfoText);
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
					message = MsgDjinnStandbyAction;
				else
					message = MsgDjinnSetAction;
			}
			else if (buf[n] & 2)
			{
				message = MsgDjinnGiveAction;
			}
			else
			{
				message = MsgDjinnTradeAction;
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
			UiText_DrawCharacterAtOffsetFar(owner->class_index + MsgClassName, window, 0, 8);
			UiText_DrawStringAtOffsetFar(DjinnMenu_TextLevel, window, 48, 0);
			UiText_DrawNumberInWindowFar(owner->level, 2, window, 72, 0);
			if (mode == 0)
				UiText_DrawCharacterAtOffsetFar(MsgDjinnCharacterStatus, window, 0, 16);
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
				UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel, state->help_window, 8, 0);
#else
				UiWork_PushValueSlotFar(state->shown_owner[0], 1);
				UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel, state->help_window, 0, 0);
				UiWork_PushValueSlotFar(DJINN_ELEMENT(state->pair_entries[0]) * 20
											+ DJINN_NUMBER(state->pair_entries[0]) + 300,
					4);
				UiWindow_SetTilemapEntryFar(
					state->help_window, DJINN_ELEMENT(state->pair_entries[0]) + 0x5001, 6, 0, 0);
				UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel + 1, state->help_window, 56, 0);
#endif
				UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel + 2, state->help_window, 0, 8);
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
						UiText_DrawCharacterAtOffsetFar(MsgDjinnSetAll, state->help_window, 0, 0);
					}
					else
					{
						UiText_DrawCharacterAtOffsetFar(
							MsgDjinnAllStandby, state->help_window, 0, 0);
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
							MsgDjinnRecovering, state->help_window, 0, 16);
						FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
						FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
					}
					if (refresh & 2)
						FourObjectMotion_SetSlotPhase(mode, 0);
				}
				else
				{
					UiText_DrawCharacterAtOffsetFar(MsgDjinnChoose, state->help_window, 0, 0);
					if (Trade_CanOfferDjinnFar(
							DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))
						|| Djinn_IsActiveFar(
							DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
					{
						if (Trade_CanOfferDjinnFar(
								DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
						{
							UiText_DrawCharacterAtOffsetFar(
								MsgDjinnChoose + 3, state->help_window, 0, 16);
							FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
						}
						else
						{
							UiText_DrawCharacterAtOffsetFar(
								MsgDjinnChoose + 2, state->help_window, 0, 16);
							FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
						}
						FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
					}
					else
					{
						UiText_DrawCharacterAtOffsetFar(
							MsgDjinnChoose + 4, state->help_window, 0, 16);
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
				UiText_DrawCharacterAtOffsetFar(MsgUnleashEffect, state->list_window, 0, 80);
#if EDITION_INTERNATIONAL
				UiWindow_ClearInteriorTilesFar(state->list_window, 0, 96, 224, 104);
				UiText_DrawCharacterAtOffsetFar(
					DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + MsgDjinnEffect,
					state->list_window, 0, 96);
#else
				UiText_DrawResourceFar(
					DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + MsgDjinnEffect,
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
					work = UiText_OpenMessageWindowFar(MsgDjinnTutorialRecovery, 9, 9, 1);
					gGameState.message_speed = 1;
					while (UiWork_IsCompleteFar() == 0)
						WaitFrames(1);
					UiWork_FinalizeFar(work, 1);
					DjinnMenu_DrawElementList(&lists->list);
					WaitFrames(1);
					work = UiText_OpenMessageWindowFar(MsgDjinnTutorialSetAgain, 9, 9, 1);
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
					work = UiText_OpenMessageWindowFar(MsgDjinnTutorialGive, 9, 9, 1);
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
					work = UiText_OpenMessageWindowFar(MsgDjinnTutorialSetAfterGive, 9, 9, 1);
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
					work = UiText_OpenMessageWindowFar(MsgDjinnTutorialPowers, 9, 9, 1);
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
					work = UiText_OpenMessageWindowFar(MsgDjinnTutorialUnleash, 9, 9, 1);
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
					work = UiText_OpenMessageWindowFar(MsgDjinnTutorialDemonstrate, 9, 9, 1);
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
				UiText_DrawResourceFar(MsgDjinnNowRecovering, state->list_window, 0, 96);
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
				UiText_DrawResourceFar(MsgDjinnNowRecovering, state->list_window, 0, 96);
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
		// Plain, entry dies at the owner's AND, so reload ties the result to entry instead of the mask.
		/* FAKEMATCH: the barrier keeps entry live through owner extraction and passes result through it, so the owner mask keeps its native register (ands r3, r2); plain differs 8 (ands r2, r3), an input-only barrier 2 (a sched2 tie swaps the final index shift and table-base copy), the fields stored in other orders 13-1106, entry as s32 or u16 1186 and 1165. */
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
				state->max_pp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_MAX_X, y * 8 + 64);
			UiText_DrawNumberInWindowFar(
				state->hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
			UiText_DrawNumberInWindowFar(
				state->pp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
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
				state->pp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
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
		label = MsgPreviewStatLabel;
		UiText_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
		UiText_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
		UiText_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
		UiText_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
		UiText_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
		UiText_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
		UiText_DrawCharacterAtOffsetFar(
			MsgClassName + saved->class_index, window, x * 8, y * 8 + 32);
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
					MsgClassName + state->class_index, window, px, y * 8 + 48);
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
			if (state->pp != saved->pp)
			{
				UiText_DrawNumberInWindowFar(state->pp, 4, window, x * 8 + 72, y * 8 + 64);
				if (state->pp > saved->pp)
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
				MsgAbilityName + (actions[first] & OWNER_ACTION_ID_MASK), window, x * 8 + 16,
				(y + line * 2) * 8 + 8);
			UiText_DrawNumberAtOffsetFar(
				BtlActGet(actions[first])->pp_cost, 2, window, x * 8 + 88, (y + line * 2) * 8 + 8);
			line++;
		}
		UiWork_SetParamNibbleFar(15);
		UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window, x * 8 + PREVIEW_PP_X, y * 8);
		if (mode != 3)
		{
			s32 lines;

			lines = 0;
			if (gained)
			{
				UiWork_SetParamNibbleFar(4);
				UiText_DrawCharacterAtOffsetFar(MsgPsynergyGained, window, x * 8, y * 8 + 88);
				lines = 1;
			}
			if (lost)
			{
				UiWork_SetParamNibbleFar(2);
				UiText_DrawCharacterAtOffsetFar(
					MsgPsynergyLost, window, x * 8, (y + lines) * 8 + 88);
				lines++;
			}
			if (lines == 0)
				UiText_DrawCharacterAtOffsetFar(MsgPsynergyUnchanged, window, x * 8, y * 8 + 88);
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

void FourObjectMotion_InitializeTopRow(void *unused0, s32 unused1)
{
	struct FourObjectMotionState *state = gMenuWork;
	s32							  index;

	for (index = 0; index < 4; index++)
	{
		struct AnimationObject *object = state->objects[index];

		if (object != NULL)
		{
			ResourceObject_ReleaseFar(object);
			state->objects[index] = NULL;
		}
	}

	for (index = 0; index < 4; index++)
	{
		struct AnimationObject *object =
			ResourceObject_CreateFar(FourObjectMotion_ResourceIds[index]);

		if (object != NULL)
		{
			AnimationObjects_SelectAnimationFar(object, 2);
		}
		state->objects[index] = object;
		state->positions_x[index] = 0x10;
		state->positions_y[index] = 0x20;
	}
	Scheduler_AddOrUpdateCallback((s32)FourObjectMotion_UpdateAllPositions, 0xc80);
}

// 4 つのオブジェクトを解放し、スケジューラから update を外す
#define DEFINE_FOUR_OBJECT_RELEASE(name, update)                    \
	void name(void)                                                 \
	{                                                               \
		struct FourObjectMotionState *state = gMenuWork;            \
		s32							  index = 0;                    \
                                                                    \
		do                                                          \
		{                                                           \
			struct AnimationObject *object = state->objects[index]; \
                                                                    \
			if (object != NULL)                                     \
			{                                                       \
				ResourceObject_ReleaseFar(object);                  \
				state->objects[index] = NULL;                       \
			}                                                       \
			index++;                                                \
		} while (index < 4);                                        \
		Scheduler_RemoveCallback((u32)update);                      \
	}

DEFINE_FOUR_OBJECT_RELEASE(FourObjectMotion_ClearSlotsAndSchedule,
	FourObjectMotion_UpdateAllPositions)

#define FIXED_ONE 0x10000u
#define VERTICAL_ORIGIN 0x01e20000u

void FourObjectMotion_UpdateAllPositions(void)
{
	struct FourObjectMotionState *state = gMenuWork;
	s32							  index;
	u32							  motion[2];
	u32							  request[4];

	index = 0;
	while (1)
	{
		u32 object = (u32)state->objects[index];

		if (object != 0)
		{
			u32 vertical = VERTICAL_ORIGIN - ((s32)state->vertical_origins[index] << 16);
			((struct FieldSprite *)object)->priority = 0;
			motion[0] = FIXED_ONE;
			motion[1] = FIXED_ONE;
			request[0] = (s32)state->positions_x[index] << 16;
			request[1] = vertical;
			request[2] = ((s32)state->positions_y[index] << 16) + vertical;
			request[3] = 0;
			Object_ApplyProjectedPlacementFar(object, request, motion, 0x4000);
		}
		index += 1;
		if (index > 3)
			break;
	}
}

void FourObjectMotion_UpdateBottomRow(void)
{
	struct FourObjectMotionState *work = gMenuWork;
	s32							  i;
	u32							  motion[2];
	u32							  request[4];

	Link_DrawShiftedTilePairFar((void *)0x06002500);
	i = 0;
	do
	{
		u32 obj = (u32)work->objects[i];

		if (obj != 0)
		{
			u32 y = (241u << 17) - ((u32)(s32)work->vertical_origins[i] << 16);
			s32 phase;
			s32 limit;
			/* Ordinary s32/s16 locals, a direct field test, an earlier limit
			 * initializer and an inline sign/limit helper all differ 81 in
			 * EN (119 native, 118 built), changing the outer high-register
			 * allocation as well as this read. Plain locals take r3,
			 * local-alloc's first free register (REG_ALLOC_ORDER 3, 2, 1, 0),
			 * which costs limit its r3 preference. */
			/* FAKEMATCH: pins the row's second read to r2, as native; plain locals take r3 and reshuffle the high registers (differ 81), reading py before request[3] = 0 puts it in r2 but emits the ldrsh before the zero store (4 in all six editions), and with py plain the loop index as u8, s8 or u16 differs 130 to 140, as s16 82. */
			register s32 py asm("r2");

			((struct FieldSprite *)obj)->priority = 0;
			phase = work->phases[i];
			if (phase < 0)
			{
				motion[0] = -phase;
				motion[1] = -phase;
			}
			else
			{
				motion[0] = phase + (s32)(0x10000 - phase) / 3;
				motion[1] = motion[0];
				work->phases[i] = motion[0];
			}
			request[0] = (u32)(s32)work->positions_x[i] << 16;
			request[1] = y;
			request[2] = ((u32)(s32)work->positions_y[i] << 16) + y;
			request[3] = 0;
			py = work->positions_y[i];
			limit = 0x8000;
			if (py >= 0)
				limit = 0x4000;
			Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
		}
		i++;
	} while (i <= 3);
}

void FourObjectMotion_InitializeBottomRow(s32 unused0, s32 unused1)
{
	struct FourObjectMotionState *state = gMenuWork;
	s32							  index;

	for (index = 0; index < 4; index++)
	{
		struct AnimationObject *object = state->objects[index];

		if (object != NULL)
		{
			ResourceObject_ReleaseFar(object);
			state->objects[index] = NULL;
		}
	}
	for (index = 0; index < 4; index++)
	{
		struct AnimationObject *object =
			ResourceObject_CreateFar(FourObjectMotion_ResourceIds[index]);

		if (object != NULL)
			AnimationObjects_SelectAnimationFar(object, 2);
		state->objects[index] = object;
		state->phases[index] = 0x10000;
		state->positions_x[index] = 0x10;
		state->positions_y[index] = 0xc8;
	}
	Scheduler_AddOrUpdateCallback((s32)FourObjectMotion_UpdateBottomRow, 0xc80);
}

s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative)
{
	struct FourObjectMotionState *state = gMenuWork;

	if (state->objects[index] != NULL)
	{
		state->positions_x[index] = x;
		state->positions_y[index] = negative != 0 ? y | (s32)0xffff8000 : y;
		return 0x23c + index * 2;
	}
	return index;
}

s32 FourObjectMotion_SetSlotPhase(s32 index, s32 phase)
{
	((struct FourObjectMotionState *)gMenuWork)->phases[index] = phase;
}

s32 FourObjectMotion_ReplaceSlot(s32 index, s32 kind, s32 value)
{
	struct FourObjectMotionState *state = gMenuWork;

	if (state->objects[index] != NULL)
	{
		ResourceObject_ReleaseFar(state->objects[index]);
		state->objects[index] = NULL;
	}
	{
		struct AnimationObject *object =
			ResourceObject_CreateFar(FourObjectMotion_ResourceIds[kind]);

		if (object != NULL)
			AnimationObjects_SelectAnimationFar(object, value);
		state->objects[index] = object;
	}
	return 1;
}

DEFINE_FOUR_OBJECT_RELEASE(FourObjectMotion_ClearSlotsAndScheduleAlt,
	FourObjectMotion_UpdateBottomRow)

void Menu_EnableAllItemObjects(void)
{
	struct InventoryMenuState *state = gMenuWork;
	s32						   index;

	for (index = 0; index < state->party_count; ++index)
	{
		AnimationObjects_SelectAnimationFar(state->owner_objects[index], 1);
	}
}

s32 OwnerAction_RunCompareLoop(u32 mode)
{
	struct DjinnMenuWork	 *state;
	struct DjinnMenuBackdrop *scene;
	struct UiRenderWork		 *render;
	struct UiWindow			 *list;
	s32						  count[2];
	s32						  cursor[2];
	s32						  anim;
	u32						  dirty;
	s32						  frame;
	s32						  page;
	s32						  entry;
	s32						  swap;
	s32						  step;
	s32						  row;
	s32						  result;
	s32						  zero;
	s32						  i;
	s32						  j;

	state = gMenuWork;
	anim = 0;
	result = 0;
	dirty = 1;
	frame = 0;
	cursor[0] = 0;
	page = 2;
	state->cursor->active = 13;
	cursor[1] = 0;

	for (j = 3; j >= 0; j--)
	{
		state->owner_y[j] = 0xc8;
	}
	state->owner_x[state->selected_column[1]] = 0x78;
	state->owner_y[state->selected_column[1]] = 0x20;
	state->owner_x[state->selected_column[0]] = 0x10;
	state->owner_y[state->selected_column[0]] = 0x20;

	RenderOutput_ClearListFar((struct UiWindow *)state->list_window);
	WaitFrames(1);
	UiWindow_CloseIfOpen((struct UiWindow **)&state->help_window, 1);
	UiWindow_UpdateOrCreate(&state->help_window, 0, 0, 30, 5, 2);

	count[0] = 1;
	count[1] = 1;

	if (mode <= 1)
	{
		u16			   *diff_work;
		struct BtlUnit *unit_copy;
		struct BtlUnit *unit;
		s32				id;
		s32				first_element;
		s32				first_index;
		s32				second_element;
		s32				second_index;
		s32				first_flag;
		s32				second_flag;
		s32				out_a;
		s32				out_b;
		s32				size;

		diff_work = Runtime_BumpAllocateAlternatePool(96);
		size = sizeof(*unit_copy);
		unit_copy = Runtime_BumpAllocateAlternatePool(size);
		second_element = 0;
		second_index = 0;
		id = state->shown_owner[0];
		first_element = state->pair_element[0];
		first_index = state->pair_index[0] & 31;
		first_flag = (u16)(state->pair_entries[0] & 0x8000);

		unit = UnitGetFar(id);
		Iwram_CopyWords(unit_copy, unit, size);
		Djinn_DeactivateFar(id, first_element, first_index);
		if (mode == 0)
		{
			second_element = state->pair_element[1];
			second_index = state->pair_index[1] & 31;
			second_flag = (u16)(state->pair_entries[1] & 0x8000);
			Djinn_AddToOwnerFar(id, second_element, second_index);
			if (second_flag != 0)
			{
				Djinn_ActivateFar(id, second_element, second_index);
			}
		}
		UnitCalcFar(id);
		count[0] = OwnerAction_DiffSlots(
			unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
		Iwram_CopyWords(unit, unit_copy, size);

		id = state->shown_owner[1];
		unit = UnitGetFar(id);
		Iwram_CopyWords(unit_copy, unit, size);
		if (mode == 0)
		{
			Djinn_DeactivateFar(id, second_element, second_index);
		}
		Djinn_AddToOwnerFar(id, first_element, first_index);
		if (first_flag != 0)
		{
			Djinn_ActivateFar(id, first_element, first_index);
		}
		UnitCalcFar(id);
		count[1] = OwnerAction_DiffSlots(
			unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
		Iwram_CopyWords(unit, unit_copy, size);
		MemFree(unit_copy);
		MemFree(diff_work);
	}
	else if (mode - 2 <= 1)
	{
		u16			   *diff_work;
		struct BtlUnit *unit_copy;
		struct BtlUnit *unit;
		s32				first_id;
		s32				first_element;
		s32				first_index;
		s32				apply_second;
		s32				out_a;
		s32				out_b;

		diff_work = Runtime_BumpAllocateAlternatePool(96);
		unit_copy = Runtime_BumpAllocateAlternatePool(sizeof(*unit_copy));
		first_id = state->shown_owner[0];
		first_element = state->pair_element[0];
		first_index = state->pair_index[0] & 31;
		apply_second = (mode == 3);

		unit = UnitGetFar(first_id);
		Iwram_CopyWords(unit_copy, unit, sizeof(*unit_copy));
		Djinn_DeactivateFar(first_id, first_element, first_index);
		if (apply_second != 0)
		{
			Djinn_ActivateFar(first_id, first_element, first_index);
		}
		UnitCalcFar(first_id);
		count[0] = OwnerAction_DiffSlots(
			unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
		Iwram_CopyWords(unit, unit_copy, sizeof(*unit_copy));
		count[1] = count[0];
		page = 0;
		MemFree(unit_copy);
		MemFree(diff_work);
	}

	count[0] = ((count[0] - 1) / 5) + 1;
	if (count[0] == 0)
	{
		count[0] = 1;
	}
	count[1] = ((count[1] - 1) / 5) + 1;
	if (count[1] == 0)
	{
		count[1] = 1;
	}

	UiWindow_UpdateOrCreate(&state->window, 0, 5, 15, 15, 2);
	UiWindow_UpdateOrCreate(&state->second_window, 15, 5, 15, 15, 2);
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)state->help_window);

	if (mode == 2)
	{
		UiText_DrawCharacterAtOffsetFar(
			MsgDjinnCompareStandby, (struct UiWindow *)state->help_window, 96, 0);
		UiText_DrawCharacterAtOffsetFar(
			MsgDjinnCompareConfirm, (struct UiWindow *)state->help_window, 96, 16);
	}
	else if (mode == 3)
	{
		UiText_DrawCharacterAtOffsetFar(
			MsgDjinnCompareSet, (struct UiWindow *)state->help_window, 96, 0);
		UiText_DrawCharacterAtOffsetFar(
			MsgDjinnCompareConfirm, (struct UiWindow *)state->help_window, 96, 16);
	}
	else if (mode == 0)
	{
		UiText_DrawCharacterAtOffsetFar(
			MsgDjinnCompareTrade, (struct UiWindow *)state->help_window, 128, 0);
		UiText_DrawCharacterAtOffsetFar(
			MsgDjinnCompareConfirm, (struct UiWindow *)state->help_window, 128, 8);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar(
			MsgDjinnCompareGive, (struct UiWindow *)state->help_window, 128, 0);
		UiText_DrawCharacterAtOffsetFar(
			MsgDjinnCompareConfirm, (struct UiWindow *)state->help_window, 128, 8);
	}

	row = (mode == 1);
	if ((state->pair_entries[0] & 0x8000) == 0)
	{
		UiWork_SetParamNibbleFar(2);
	}
	UiText_DrawCharacterAtOffsetFar(
		((state->pair_entries[0] & 0xe0) >> 5) * 20 + (state->pair_entries[0] & 31) + MsgDjinnName,
		(struct UiWindow *)state->help_window, 48, row * 8);
	UiWindow_SetTilemapEntryFar((struct UiWindow *)state->help_window,
		((state->pair_entries[0] & 0xe0) >> 5) + 0x5001, 5, row, zero = 0);
	UiWork_SetParamNibbleFar(15);

	if (mode == 0)
	{
		if ((state->pair_entries[1] & 0x8000) == 0)
		{
			UiWork_SetParamNibbleFar(2);
		}
		UiText_DrawCharacterAtOffsetFar(((state->pair_entries[1] & 0xe0) >> 5) * 20
											+ (state->pair_entries[1] & 31) + MsgDjinnName,
			(struct UiWindow *)state->help_window, 48, 16);
		UiWindow_SetTilemapEntryFar((struct UiWindow *)state->help_window,
			((state->pair_entries[1] & 0xe0) >> 5) + 0x5001, 5, 2, mode);
		UiWork_SetParamNibbleFar(15);
	}
	else if (mode - 2 <= 1)
	{
		if ((state->pair_entries[0] & 0x8000) != 0)
		{
			UiWork_SetParamNibbleFar(2);
		}
		UiText_DrawCharacterAtOffsetFar(((state->pair_entries[0] & 0xe0) >> 5) * 20
											+ (state->pair_entries[0] & 31) + MsgDjinnName,
			(struct UiWindow *)state->help_window, 48, 16);
		UiWindow_SetTilemapEntryFar((struct UiWindow *)state->help_window,
			((state->pair_entries[0] & 0xe0) >> 5) + 0x5001, 5, 2, zero);
		UiWork_SetParamNibbleFar(15);
		UiWindow_SetTilemapEntryFar((struct UiWindow *)state->help_window, 0xf296, 7, 1, zero);
	}

	for (;;)
	{
		s32 input_edge;
		s32 input_held;

		render = (struct UiRenderWork *)gWindowWork[0];
		scene = state->backdrop;
		input_edge = gKeyState;
		input_held = gKeysRepeat;

		if (scene->tutorial_step != 0)
		{
			s16 *saved;
			s32	 size;

			size = 0x200;
			saved = Runtime_BumpAllocateAlternatePool(size);
			input_edge = 0;
			input_held = 0;
			scene->tutorial_timer += 1;

			switch (scene->tutorial_step)
			{
			case 5:
			case 12:
			case 23:
				if (scene->tutorial_timer == 100)
				{
					input_held = 1;
					input_edge = 1;
					scene->tutorial_timer = 0;
					scene->tutorial_step += 1;
				}
				break;
			case 10:
				if (scene->tutorial_timer == 60)
				{
					s32 work;

					Iwram_CopyWords(saved, render, size);
					work = UiText_OpenMessageWindowFar(MsgDjinnTutorialSetChanges, 8, 0, 1);
					/* FAKEMATCH: the one-pass block keeps the window copy before the cursor store, as native; plain, it is scheduled after the store (differ 8; all four plain 26). With all four plain: an inline save-and-open helper at both cases 84, the save and open as a statement macro 8, a cursor-store statement macro 12 at each of the two plain stores, so no layer. */
					do
					{
						state->cursor->active = 1;
					} while (0);
					UiMenu_SlideCursor(2, 96);
					gGameState.message_speed = 1;
					while (UiWork_IsCompleteFar() == 0)
					{
						WaitFrames(1);
					}
					UiWork_FinalizeFar(work, 1);
					Iwram_CopyWords(render, saved, size);
					UiWindow_MarkVisibleTileAttributesFar();
					render->dirty = 1;
					dirty = 1;
					scene->tutorial_timer = 0;
					scene->tutorial_step = 11;
					/* FAKEMATCH: one-pass block keeps the step store before the cursor store, as native; plain it follows the cursor load (differ 4; all four plain 26). Save/open as a macro: UiWork_FinalizeFar to step matches, but case 11 has an extra WaitFrames there, so none serves both; around dirty, timer and step 10, step alone or from render->dirty 4, restore (UiWork_FinalizeFar to render->dirty) 22. */
					do
					{
						state->cursor->active = 13;
					} while (0);
				}
				break;
			case 11:
				if (scene->tutorial_timer == 60)
				{
					s32 work;

					Iwram_CopyWords(saved, render, size);
					work = UiText_OpenMessageWindowFar(MsgDjinnTutorialClassPsynergy, 8, 0, 1);
					/* FAKEMATCH: the one-pass block keeps the window copy before the cursor store, as native; plain, it is scheduled after the store (differ 10; all four plain 26). With all four plain: an inline save-and-open helper at both cases 84, the save and open as a statement macro 8, a cursor-store statement macro 12 at each of the two plain stores, so no layer. */
					do
					{
						state->cursor->active = 1;
					} while (0);
					UiMenu_SlideCursor(106, 56);
					gGameState.message_speed = 1;
					while (UiWork_IsCompleteFar() == 0)
					{
						WaitFrames(1);
					}
					UiWork_FinalizeFar(work, 1);
					Iwram_CopyWords(render, saved, size);
					UiWindow_MarkVisibleTileAttributesFar();
					render->dirty = 1;
					WaitFrames(1);
					dirty = 1;
					scene->tutorial_timer = 0;
					scene->tutorial_step = 12;
					/* FAKEMATCH: one-pass block keeps the step store before the cursor store, as native; plain it follows the cursor load (differ 4; all four plain 26). Save/open as a macro: UiWork_FinalizeFar to step matches, but case 11 has an extra WaitFrames there, so none serves both; around dirty, timer and step 10, step alone or from render->dirty 4, restore (UiWork_FinalizeFar to render->dirty) 22. */
					do
					{
						state->cursor->active = 13;
					} while (0);
				}
				break;
			default:
				break;
			}
			MemFree(saved);
		}

		if (dirty != 0)
		{
			swap = 1;
			if (mode <= 1)
			{
				UiWindow_ClearInteriorTilesFar(
					(struct RenderInput *)state->help_window, 128, 16, 224, 24);
				if (page == 1)
				{
					UiText_DrawCharacterAtOffsetFar(
						MsgDjinnStatusHelp, (struct UiWindow *)state->help_window, 128, 16);
				}
				else
				{
					UiWork_PushValueSlotFar(state->shown_owner[page != 2], 1);
					UiText_DrawCharacterAtOffsetFar(
						MsgDjinnPsyHelp, (struct UiWindow *)state->help_window, 128, 16);
				}
			}
			render->menu_busy = 1;
			RenderOutput_PrepareForRedrawFar(state->window);
			RenderOutput_PrepareForRedrawFar(state->second_window);
			if ((dirty >> 1) != 0)
			{
				WaitFrames(1);
			}

			if (mode == 3)
			{
				entry = 0;
				swap = 0;
			}
			else if (mode == 2)
			{
				entry = 1;
				swap = 0;
			}
			else
			{
				entry = 2;
			}

			switch (page)
			{
			default:
				Menu_RunPairedEntryAction(mode, 0);
				break;
			case 1:
				if (mode == 1)
				{
					entry = 4;
				}
				DjinnMenu_DrawStatPreview(
					state->window, 0, 0, state->shown_owner[1], page, 0, entry, 0, 0);
				DjinnMenu_DrawStatPreview(state->second_window, 0, 0, state->shown_owner[1], page,
					0, entry, cursor[1] + 1, 0);
				break;
			case 0:
				if (mode == 1)
				{
					entry = 1;
					swap = 0;
				}
				DjinnMenu_DrawStatPreview(
					state->window, 0, 0, state->pair_owner[0], page, swap, entry, page, 1);
				DjinnMenu_DrawStatPreview(state->second_window, 0, 0, state->pair_owner[0], page,
					swap, entry, cursor[0] + 1, 1);
				break;
			}
			render->menu_busy = 0;
		}

		if (page <= 1)
		{
			list = (struct UiWindow *)state->second_window;
			if (count[page] > 1)
			{
				s32 top;

				for (i = 0; i < count[page]; i++)
				{
					s32 tile;

					tile = 0xf031 + i;
					if (i > 9)
					{
						tile = 0xf030;
					}
					if (i == cursor[page])
					{
						tile -= 0x1000;
					}
					UiWindow_SetTilemapEntryFar(
						list, tile, list->width - count[page] + i - 2, -1, 0);
				}
				top = -1;
				UiWindow_SetTilemapEntryFar(list, 0xf128, list->width - count[page] - 3, top, 0);
				UiWindow_SetTilemapEntryFar(list, 0xf129, list->width - 2, top, 0);
				UiRender_MarkRow(render, list->y);
			}
		}

		frame++;
		step = (frame % 60) - 5;
		if (step < 0)
		{
			step = 0;
		}
		if (step > 29)
		{
			step = 29;
		}
		FourObjectMotion_SetSlotPhase(0, 0xffff4000);
		FourObjectMotion_SetSlotPhase(1, 0xffff4000);

		if (mode <= 1)
		{
			s32 rise;
			s32 slide;

			rise = -Iwram_MulQ16(16, Trig_Sin((step % 30) * 0x444));
			if (rise < -6)
			{
				rise = -6;
			}
			if (rise > 12)
			{
				rise = 12;
			}
			slide = (step % 35) * 2;
			FourObjectMotion_SetSlotPosition(0, slide + 34, rise + 20, 0);
			if (mode == 0)
			{
				FourObjectMotion_SetSlotPosition(1, 99 - slide, 36 - rise, 0);
			}
		}
		else
		{
			FourObjectMotion_SetSlotPosition(0, 32, 30, 0);
		}

		if (dirty != 0)
		{
			dirty = 0;
			anim = Menu_GetModuloOfSum(anim, 2);
		}
		UiMenu_PositionCursor(anim * 56 + 80, 16);

		if ((frame & 3) == 0)
		{
			void *tile;
			u32	  fill;

			tile = (void *)0x060052c0;
			fill = 0x44444444;
			if ((frame & 4) != 0)
			{
				Iwram_CopyWords(tile, Menu_BackdropFrameTile, 32);
			}
			else
			{
				Iwram_FillWords(tile, 32, fill);
			}
		}

		if ((input_edge & 1) != 0)
		{
			if (anim == 0)
			{
				result = 1;
				break;
			}
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		if ((input_edge & 8) != 0)
		{
			AudioCommand_PlayFar(113);
			result = -2;
			break;
		}
		else if ((input_edge & 2) != 0)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		else if ((input_edge & 0x100) != 0)
		{
			if (mode <= 1)
			{
				page = ((page + 1) % 3);
				dirty = 2;
				AudioCommand_PlayFar(111);
			}
		}
		else if ((input_held & 32) != 0)
		{
			if (page <= 1)
			{
				cursor[page]--;
				cursor[page] = Menu_GetModuloOfSum(cursor[page], count[page]);
				AudioCommand_PlayFar(111);
				Runtime_SetMainState19();
				dirty = 1;
			}
		}
		else if ((input_held & 16) != 0)
		{
			if (page <= 1)
			{
				AudioCommand_PlayFar(111);
				Runtime_SetMainState19();
				cursor[page]++;
				dirty = 1;
				cursor[page] = Menu_GetModuloOfSum(cursor[page], count[page]);
			}
		}
		WaitFrames(1);
	}

	FourObjectMotion_SetSlotPhase(0, 0);
	FourObjectMotion_SetSlotPhase(1, 0);
	Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);

	((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
	UiWindow_CloseIfOpen((struct UiWindow **)&state->help_window, 1);
	WaitFrames(1);
	UiWindow_UpdateOrCreate(&state->help_window, 13, 0, 17, 5, 2);
	UiWindow_CloseIfOpen((struct UiWindow **)&state->window, 1);
	UiWindow_CloseIfOpen((struct UiWindow **)&state->second_window, 1);
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)state->list_window);
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)state->unknown_window_028);
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)state->status_window);
	((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
	WaitFrames(1);
	return result;
}

s32 DjinnMenu_ShowOwnerStatus(void)
{
	s32					  cursor[2];
	s32					  pages[2];
	s32					  gained;
	s32					  lost;
	s32					  phase;
	s32					  pending;
	s32					  frame;
	struct UiWindow		**left;
	struct UiWindow		**right;
	struct UiRenderWork	 *render;
	u32					  keys;
	u32					  repeat;
	struct DjinnMenuWork *menu;
	struct BtlUnit		 *owner;
	struct UiWindow		 *window;
	u16					 *actions;
	void				 *saved;
	s32					  result;
	s32					  tile;
	s32					  i;
	s32					  side;
	s32					 *page;
	s32					  blink;

	menu = gMenuWork;
	phase = 0;
	pending = 1;
	result = 0;
	frame = 0;
	menu->cursor->active = 13;
	cursor[1] = cursor[0] = result;
	for (side = 3; side >= 0; side--)
		menu->owner_y[side] = 200;
	RenderOutput_ClearListFar((struct UiWindow *)menu->list_window);
	WaitFrames(1);
	page = pages;
	page[1] = page[0] = 1;
	actions = Runtime_BumpAllocateAlternatePool(96);
	saved = Runtime_BumpAllocateAlternatePool(332);
	owner = UnitGetFar(menu->shown_owner[0]);
	page[1] = page[0] =
		OwnerAction_DiffSlots(owner->action_slots, owner->action_slots, actions, &gained, &lost);
	MemFree(saved);
	MemFree(actions);
	page[0] = (page[0] - 1) / 6 + 1;
	if (page[0] == 0)
		page[0] = 1;
	page[1] = (page[1] - 1) / 6 + 1;
	if (page[1] == 0)
		page[1] = 1;
	left = (struct UiWindow **)&menu->window;
	UiWindow_UpdateOrCreate(left, 0, 5, 15, 15, 2);
	right = (struct UiWindow **)&menu->second_window;
	UiWindow_UpdateOrCreate(right, 15, 5, 15, 15, 2);
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->help_window);
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->status_window);
	UiText_DrawCharacterAtOffsetFar(MsgCurrentStatus, (struct UiWindow *)menu->status_window, 0, 0);
	UiText_DrawCharacterAtOffsetFar(
		MsgCurrentStatus + 2, (struct UiWindow *)menu->status_window, 0, 16);
	side = 0;
	while (1)
	{
		render = (struct UiRenderWork *)gWindowWork[0];
		keys = gKeyState;
		repeat = gKeysRepeat;
		if (pending)
		{
			render->menu_busy = 1;
			RenderOutput_PrepareForRedrawFar((struct UiWindow *)menu->window);
			RenderOutput_PrepareForRedrawFar((struct UiWindow *)menu->second_window);
			DjinnMenu_DrawStatPreview(
				(struct UiWindow *)menu->window, 0, 0, menu->pair_owner[0], 0, 0, 3, 0, 1);
			DjinnMenu_DrawStatPreview((struct UiWindow *)menu->second_window, 0, 0,
				menu->pair_owner[0], 0, 0, 3, cursor[0] + 1, 1);
			render->menu_busy = 0;
		}
		if (page[side] > 1)
		{
			window = (struct UiWindow *)menu->second_window;
			for (i = 0; i < page[side]; i++)
			{
				tile = 0xf031 + i;
				if (i > 9)
					tile = 0xf030;
				if (i == cursor[side])
					tile -= 0x1000;
				UiWindow_SetTilemapEntryFar(
					window, tile, window->width - page[side] + i - 2, -1, 0);
			}
			UiWindow_SetTilemapEntryFar(window, 0xf128, window->width - page[side] - 3, -1, 0);
			UiWindow_SetTilemapEntryFar(window, 0xf129, window->width - 2, -1, 0);
			UiRender_MarkRow(render, window->y);
		}
		frame++;
		blink = frame % 60 - 5;
		if (blink < 0)
			blink = 0;
		if (blink > 29)
			blink = 29;
		FourObjectMotion_SetSlotPosition(0, 32, 200, 0);
		if (pending)
		{
			pending = 0;
			phase = Menu_GetModuloOfSum(phase, 2);
		}
		if ((frame & 3) == 0)
		{
			if (frame & 4)
				Iwram_CopyWords((void *)0x060052c0, Menu_BackdropFrameTile, 32);
			else
				Iwram_FillWords((void *)0x060052c0, 32, 0x44444444);
		}
		if (keys & KEY_START)
		{
			AudioCommand_PlayFar(113);
			result = -2;
			break;
		}
		if (keys & (KEY_A | KEY_B | KEY_R | KEY_L))
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		if (repeat & KEY_LEFT)
		{
			cursor[side]--;
			cursor[side] = Menu_GetModuloOfSum(cursor[side], page[side]);
			AudioCommand_PlayFar(111);
			Runtime_SetMainState19();
			pending = 1;
		}
		else if (repeat & KEY_RIGHT)
		{
			cursor[side]++;
			AudioCommand_PlayFar(111);
			Runtime_SetMainState19();
			pending = 1;
			cursor[side] = Menu_GetModuloOfSum(cursor[side], page[side]);
		}
		WaitFrames(1);
	}
	Scheduler_AddOrUpdateCallback((s32)Menu_UpdateEntryObjectTransforms, 0xc80);
	((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
	UiWindow_CloseIfOpen((struct UiWindow **)&menu->help_window, 1);
	WaitFrames(1);
	UiWindow_UpdateOrCreate((struct UiWindow **)&menu->help_window, 13, 0, 17, 5, 2);
	UiWindow_CloseIfOpen(left, 1);
	UiWindow_CloseIfOpen(right, 1);
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->list_window);
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->unknown_window_028);
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->status_window);
	((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
	WaitFrames(1);
	return result;
}

s32 Djinn_MarkBalancedEntries(s8 *tbl, s32 self)
{
	s32					  sp0;
	s32					  cnt;
	s32					  i;
	s8					 *p;
	struct DjinnMenuWork *state;

	state = gMenuWork;
	cnt = 0;
	i = 0;
	if (cnt < state->owner_count)
	{
		p = tbl;
		do
		{
			*p = 0;
			if (i != self)
			{
				sp0 = cnt;
				if (Djinn_CheckTurnBalance(self, i) == 0)
				{
					*p = 1;
					cnt += 1;
				}
			}
			i += 1;
			p += 1;
		} while (i < state->owner_count);
	}
	return cnt;
}

s32 Djinn_CheckTurnBalance(s32 from, s32 to)
{
	struct DjinnMenuWork *work;
	s8					  counts[16];
	u8					  i;
	u8					  j;
	s32					  difference;
	s32					  balanced;

	work = gMenuWork;
	Djinn_CountTurns((u8 *)counts);
	counts[from] -= 1;
	counts[to] += 1;
	balanced = 1;
	for (i = 0; i < work->owner_count; i++)
	{
		j = i;
		if (i < work->owner_count)
		{
			while (1)
			{
				j++;
				if (j >= work->owner_count)
					break;
				difference = counts[i] - counts[j];
				if (difference < -1 || difference > 1)
				{
					balanced = 0;
					break;
				}
			}
		}
	}
	return balanced;
}

void Djinn_CountTurns(u8 *counts)
{
	struct DjinnMenuWork *work;
	struct BtlUnit		 *unit;
	s32					  owner_index;
	s32					  element;
	s32					  bit;
	s32					  count;

	work = gMenuWork;
	for (owner_index = 0; owner_index < work->owner_count; owner_index++)
	{
		unit = UnitGetFar(work->owners[owner_index]);
		count = 0;
		for (element = 0; element <= 3; element++)
		{
			for (bit = 0; bit <= 19; bit++)
			{
				if (unit->djinn_active[element] & (1 << bit))
					count++;
				else if (unit->djinn_available[element] & (1 << bit))
					count++;
			}
		}
		counts[owner_index] = count;
	}
}

void Resource_LoadPairedBlocksIfAvailable(void)
{
	s32						   first_slot;
	s32						   second_slot;
	struct InventoryMenuState *active_state;

	active_state = gMenuWork;
	first_slot = Resource_FindFreeEntry();
	active_state->resource_slots[0] = first_slot;
	if (first_slot != -1)
	{
		VramBlock_LoadCached(first_slot, 0x80, Menu_PairSpriteATiles);
	}
	second_slot = Resource_FindFreeEntry();
	active_state->resource_slots[1] = second_slot;
	if (second_slot != -1)
	{
		VramBlock_LoadCached(second_slot, 0x80, Menu_PairSpriteBTiles);
	}
}

void Menu_ResetTwoResourceEntries(void)
{
	struct InventoryMenuState *state;

	state = gMenuWork;
	Resource_ResetEntry((u16)state->resource_slots[0]);
	Resource_ResetEntry((u16)state->resource_slots[1]);
}

s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot)
{
	s32 resource_info;
	s32 selected_slot;
	s32 buffer;
	s32 allocation;

	allocation = (s32)Runtime_AllocateBlock(0x11, 0x608);
	selected_slot = slot;
	Ability_LoadGlyphFar(resource_id, 0, &selected_slot, &resource_info, 1);
	buffer = Resource_GetBuffer(slot, allocation + 0x400);
	Runtime_ReleaseHeapBlock(0x11);
	return buffer;
}

s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resource_id)
{
	s32 slot;

	slot = Resource_FindFreeEntry();
	if (slot != 0x60)
	{
		UiIcon_LoadResourceIntoSlot(resource_id, slot);
		RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
	}
}

s32 UiIcon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
	struct RenderOutput		  *object;
	u32						   resource;
	s32						   v = variant;
	struct RenderInput		  *w = window;
	s32						   xx = x;
	struct InventoryMenuState *data = gMenuWork;

	if (v == 0)
		resource = (u16)data->resource_slots[0];
	else
		resource = (u16)data->resource_slots[1];
	object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
	if (object == 0)
	{
		/* FAKEMATCH: the redundant failure branch retains the argument
         * lifetimes through CSE; plain return -1 changes 16 aligned rows. */
		if (variant || xx)
			return -1;
		else
			return -1;
	}
	object->kind = 0;
	object->anim_step = 0;
	object->active = 1;
	return 1;
}

s32 UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
	struct RenderOutput		  *object;
	u32						   resource;
	s32						   v = variant;
	struct RenderInput		  *w = window;
	s32						   xx = x;
	struct InventoryMenuState *data = gMenuWork;

	if (v == 0)
	{
		resource = (u16)data->resource_slots[0];
		y -= 3;
	}
	else
	{
		resource = (u16)data->resource_slots[1];
		y -= 4;
	}
	object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
	if (object == 0)
	{
		/* FAKEMATCH: the redundant failure branch retains the argument
         * lifetimes through CSE; plain return -1 changes 22 aligned rows. */
		if (v || x)
			return -1;
		else
			return -1;
	}
	object->kind = 0;
	object->anim_step = 0;
	object->active = 1;
	return 1;
}
