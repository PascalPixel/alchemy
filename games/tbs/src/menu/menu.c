#include "unit/menu/menu.h"
#include "unit/menu/item.h"
#include "unit/menu/psynergy.h"
#include "unit/menu/status.h"
#include "unit/menu/compare.h"
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
			UiText_DrawCharacterAtOffsetFar((s32)MsgPoisonLabel, window, STATUS_X, cnt * 8 + 8);
			cnt++;
		}
		if (avail[CHARACTER_VENOM])
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgVenomLabel, window, STATUS_X, cnt * 8 + 8);
			cnt++;
		}
		if (avail[CHARACTER_CURSE])
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgCurseLabel, window, STATUS_X, cnt * 8 + 8);
			cnt++;
		}
		if (avail[CHARACTER_HAUNT])
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgHauntLabel, window, STATUS_X, cnt * 8 + 8);
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
		value = unit->class_index + (s32)MsgClassName;
		UiText_DrawCharacterAtOffsetFar(value, window, 0, 32);
		Ui_DrawValuePairRows(unit, window);
		value = unit->experience;
		UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel, window, 0, 64);
		UiText_DrawNumberInWindowFar(value, 8, window, 24, 72);
		break;
	case 6:
		value = unit->class_index + (s32)MsgClassName;
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
			UiText_DrawCharacterAtOffsetFar((s32)MsgCannotEquip, window, 0, 48);
#else
			UiText_DrawCharacterAtOffsetFar((s32)MsgCannotEquip, window, 16, 48);
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
			UiText_DrawCharacterAtOffsetFar(id + (s32)MsgAbilityName, window, 0, 56);
			UiText_DrawCharacterAtOffsetFar((s32)MsgLearned, window, 0, 48);
		}
		else
		{
			UiText_DrawCharacterAtOffsetFar(id + (s32)MsgAbilityName, window, 0, 56);
			UiText_DrawCharacterAtOffsetFar((s32)MsgWillLearn, window, 0, 48);
		}
#else
		if (found)
		{
			UiText_DrawCharacterAtOffsetFar(id + (s32)MsgAbilityName, window, 0, 48);
			UiText_DrawCharacterAtOffsetFar((s32)MsgLearned, window, 0, 56);
		}
		else
		{
			UiText_DrawCharacterAtOffsetFar(id + (s32)MsgAbilityName, window, 0, 48);
			UiText_DrawCharacterAtOffsetFar((s32)MsgWillLearn, window, 0, 56);
		}
#endif
		break;
	}
	case 8:
	{
		u8 *base = MsgPanelStatLabel;

		UiText_DrawCharacterAtOffsetFar((s32)base, window, 0, 40);
		value = unit->atk;
		UiText_DrawNumberInWindowFar(value, 3, window, 64, 40);
		UiText_DrawCharacterAtOffsetFar((s32)base + 1, window, 0, 48);
		value = unit->def;
		UiText_DrawNumberInWindowFar(value, 3, window, 64, 48);
		UiText_DrawCharacterAtOffsetFar((s32)base + 4, window, 0, 56);
		value = unit->agi;
		UiText_DrawNumberInWindowFar(value, 3, window, 64, 56);
		UiText_DrawCharacterAtOffsetFar((s32)base + 3, window, 0, 64);
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
	s32 keta = 1;

	while (keta <= 15)
	{
		v = v / 10;
		if (v <= 9)
		{
			break;
		}
		keta++;
	}

	keta++;
	x -= keta * 8;
	UiText_DrawNumberAtOffsetFar(num, keta, win, x, y);
}

#endif
//--------------------------------------------------------------------------
// ウィンドウの y 40 に HP、y 48 に EP を書く
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
	val = unit->ep;
	UiText_DrawNumberRightAlignedFar(val, win, 48, 48);
	val = unit->max_ep;
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
	UiText_DrawStringInWindowFar((u8 *)Ui_EpRowString, win, 0, 48);
	val = unit->ep;
	UiText_DrawNumberAtOffsetFar(val, 4, win, 16, 48);
	val = unit->max_ep;
	UiText_DrawNumberInWindowFar(val, 4, win, 56, 48);
#endif
}
void UiText_DrawStatComparison(const struct BtlUnit *alt, const struct BtlUnit *base, s32 work)
{
	UiText_DrawCharacterAtOffsetFar((s32)MsgPanelStatLabel, work, 0, 32);
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
	UiText_DrawCharacterAtOffsetFar((s32)MsgPanelDefenseLabel, work, 0, 48);
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
	UiText_DrawCharacterAtOffsetFar((s32)MsgAgilityLabel, work, 0, 64);
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
	UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
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
