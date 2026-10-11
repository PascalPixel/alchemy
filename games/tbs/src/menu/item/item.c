#include "uiwindow.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "m7_interfaces.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "inventory_menu.h"
#include "io_reg.h"
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
#include "psynergy_menu.h"
#include "animspr.h"
#include "field_sprite.h"
#include "scene.h"
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

//--------------------------------------------------------------------------
// lifecycle.c
// ウィンドウ：ハンドルで持つウィンドウを開く・閉じる
//--------------------------------------------------------------------------

void			 UiWork_FinalizeFar();
void			 RenderOutput_RedrawSavedRectFar(struct UiWindow *window);

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
	*handle = (s32)UiWindow_CreateFar(x, y, w, h, style);
	return 1;
}

//--------------------------------------------------------------------------
// *handle のウィンドウがあれば閉じて、ハンドルを NULL にする
// 引数 1 つの宣言だが、呼び出し元の 2 つめの引数が r1 のまま
// UiWork_FinalizeFar に渡る
//--------------------------------------------------------------------------
void UiWindow_CloseIfOpen(void **handle)
{
	if (*handle != NULL)
	{
		UiWork_FinalizeFar(*handle);
		*handle = NULL;
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

void PsynergyMenu_InitializeEntryObjects(void *source, s32 origin_x, s32 origin_y, s32 spacing);
void UiWindow_CloseIfOpen(void **handle);

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Scheduler_DisableOverlayCallbacks();
    Map_DisableUpdateCallbackFar();
    GameFlag_ClearBitFar(0x166);
    return GameFlag_ClearBitFar(0x152);
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
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x8500029c, REG_DMA3);
    work->pane_index[0] = -1;
    work->pane_count[0] = 1;
    work->pane_count[1] = 1;
    work->column_count = 1;
    work->row_count = 1;
}

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
	*handle = (s32)UiWindow_CreateFar(x, y, w, h, style);
	return 1;
}

//--------------------------------------------------------------------------
// *handle のウィンドウがあれば閉じて、ハンドルを NULL にする
// 引数 1 つの宣言だが、呼び出し元の 2 つめの引数が r1 のまま
// UiWork_FinalizeFar に渡る
//--------------------------------------------------------------------------
void UiWindow_CloseIfOpen(void **handle)
{
	if (*handle != NULL)
	{
		UiWork_FinalizeFar(*handle);
		*handle = NULL;
	}
}

s32 Item_CanOwnerEquip(s32 owner, s32 item);
struct ItemDefinition *Item_GetDirectFar(s32 item);

#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)

#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)
#define UiText_DrawStringInWindowFar ((void (*)(const void*,s32,s32,s32))UiText_DrawStringInWindowFar)
#define Inventory_EquipFar ((void (*)(s32,s32))Inventory_EquipFar)

#if EDITION_INTERNATIONAL
#define STATUS_X 32
#else
#define STATUS_X 40
#endif

extern u8 IwramCopyWords[];

void WaitFrames(s32 frames);
void UiText_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
void UiIcon_CreateStatChangeArrow();
s32 _call_via_r3(void *, void *, s32, void *);

static inline void Owner_Copy(void *dst, void *src)
{
    _call_via_r3(dst, src, 0x14c, IwramCopyWords);
}

void Menu_DrawOwnerStatusPanel(s32 unused, s32 owner, s32 slot, s32 mode)
{
    s32 created;
    struct ItemDefinition *def;
    u32 item;
    struct InventoryMenuState *state;
    struct BtlUnit *unit;
    s32 window;
    s32 cnt;
    s32 value;
    u8 avail[8];

    created = 0;
    state = gMenuWork;
    unit = UnitGetFar(owner);
    item = unit->inventory[slot];
    def = Item_GetDirectFar(item & 0x1ff);
    if (!(mode & 0x100))
        created = UiWindow_UpdateOrCreate((s32 *)&state->status_window, 0, 5, 13, 12, 258);
    window = (s32)state->status_window;
    if (!(mode & 0x100)) {
        if (!created) {
            WaitFrames(1);
            UiWindow_ClearInteriorTilesFar((s32)state->status_window, 0, 0, 88, 32);
        }
        UiText_DrawStringAtOffsetFar(unit->name, window, STATUS_X, 0);
        CharacterMenu_BuildAvailability(avail, 1, owner);
        cnt = 0;
        if (avail[CHARACTER_POISON]) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgPoisonLabel, window, STATUS_X, cnt * 8 + 8);
            cnt++;
        }
        if (avail[CHARACTER_VENOM]) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgVenomLabel, window, STATUS_X, cnt * 8 + 8);
            cnt++;
        }
        if (avail[CHARACTER_CURSE]) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgCurseLabel, window, STATUS_X, cnt * 8 + 8);
            cnt++;
        }
        if (avail[CHARACTER_HAUNT]) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgHauntLabel, window, STATUS_X, cnt * 8 + 8);
            cnt++;
        }
        if (cnt < 2) {
            value = unit->level;
#if EDITION_INTERNATIONAL
            UiText_DrawStringAtOffsetFar(Ui_LvString, window, 40, 16);
#else
            UiText_DrawStringInWindowFar(Ui_LvString, window, 40, 16);
#endif
            UiText_DrawNumberInWindowFar(value, 4, window, 56, 16);
        }
    }
    if (!created) {
        WaitFrames(1);
        UiWindow_ClearInteriorTilesFar((s32)state->status_window, 0, 32, 88, 80);
    }
    RenderOutput_ClearListFar(window);
    if (!(mode & 0x100))
        state->cursor = SideObject_CreateFar(owner, 0, 0, window, 0, 0);
    switch (mode & 0xff) {
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
    case 3: {
        void *backup;

        if (!Item_CanOwnerEquip(owner, item)) {
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
            unit->inventory[slot] &= 0xfdff;
        else
            Inventory_EquipFar(owner, slot);
        UnitCalcFar(owner);
        UiText_DrawStatComparison(unit, backup, window);
        Owner_Copy(unit, backup);
        MemFree(backup);
        break;
    }
    case 4: {
        s32 id = def->action_id;
        s32 found = 0;
        s32 i;

        for (i = 0; i < 32; i++) {
            if ((unit->action_slots[i].encoded_action & 0x3fff) == id) {
                found = 1;
                break;
            }
        }
#if defined(TBS_EDITION_ES)
        if (found) {
            UiText_DrawCharacterAtOffsetFar(id + (s32)MsgAbilityName, window, 0, 56);
            UiText_DrawCharacterAtOffsetFar((s32)MsgLearned, window, 0, 48);
        } else {
            UiText_DrawCharacterAtOffsetFar(id + (s32)MsgAbilityName, window, 0, 56);
            UiText_DrawCharacterAtOffsetFar((s32)MsgWillLearn, window, 0, 48);
        }
#else
        if (found) {
            UiText_DrawCharacterAtOffsetFar(id + (s32)MsgAbilityName, window, 0, 48);
            UiText_DrawCharacterAtOffsetFar((s32)MsgLearned, window, 0, 56);
        } else {
            UiText_DrawCharacterAtOffsetFar(id + (s32)MsgAbilityName, window, 0, 48);
            UiText_DrawCharacterAtOffsetFar((s32)MsgWillLearn, window, 0, 56);
        }
#endif
        break;
    }
    case 8: {
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
	UiText_DrawStringInWindowFar(Ui_SlashString, win, 48, 40);
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
	UiText_DrawStringInWindowFar(Ui_SlashString, win, 48, 48);
	val = unit->ep;
	UiText_DrawNumberRightAlignedFar(val, win, 48, 48);
	val = unit->max_ep;
	UiText_DrawNumberRightAlignedFar(val, win, 88, 48);
#else
	UiText_DrawStringInWindowFar(Ui_HpRowString, win, 0, 40);
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
	UiText_DrawStringInWindowFar(Ui_EpRowString, win, 0, 48);
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
    if (alt->atk != base->atk) {
        UiText_DrawNumberAtOffsetFar(alt->atk, 3, work, 64, 40);
        if (alt->atk > base->atk) {
            UiIcon_CreateStatChangeArrow(work, 44, 36, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 36, 1);
        }
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgPanelDefenseLabel, work, 0, 48);
    UiText_DrawNumberAtOffsetFar(base->def, 3, work, 16, 56);
    if (alt->def != base->def) {
        UiText_DrawNumberAtOffsetFar(alt->def, 3, work, 64, 56);
        if (alt->def > base->def) {
            UiIcon_CreateStatChangeArrow(work, 44, 52, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 52, 1);
        }
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgAgilityLabel, work, 0, 64);
    UiText_DrawNumberAtOffsetFar(base->agi, 3, work, 16, 72);
    if (alt->agi != base->agi) {
        UiText_DrawNumberAtOffsetFar(alt->agi, 3, work, 64, 72);
        if (alt->agi > base->agi) {
            UiIcon_CreateStatChangeArrow(work, 44, 68, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 68, 1);
        }
    }
}

s32 UiIcon_CreateWithResource(struct RenderInput *window, s32 unused, s32 x, s32 y)
{
    s32 entry_no;
    s32 result;
    result = 0;
    entry_no = Resource_FindFreeEntry();
    if (entry_no != 0) {
        VramBlock_LoadCached(entry_no, 0x80, UiIcon_ResourceTiles);
        result = (s32)RenderOutput_CreateFar(entry_no, 0x40000000, window, x, y);
    }
    return result;
}

s32 UiIcon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y)
{
  s32 slot;
  unsigned char copy_mode;
  int resource_mode;
  s32 icon;
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
    if (object != NULL) {
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

void Object_ApplyProjectedPlacementFar(void *, s32 *, s32 *, s32);

extern volatile u32 gFrameCount;

s32 UiMenu_CreateCursor(void *source)
{
    struct PsynergyMenuState *work = source;
    s32 handle;
    s32 zero = 0;
    s32 state;
    struct RenderOutput *object;

    work->auxiliary_window = zero;
    UiWindow_UpdateOrCreate(&work->auxiliary_window, zero, zero, 13, 5, 2);
    handle = work->auxiliary_window;
    object = (struct RenderOutput *)UiIcon_CreateWithResourceVariant((struct RenderInput *)handle, -8, 11);
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

void PsynergyMenu_InitializeEntryObjects(void *source, s32 origin_x, s32 origin_y, s32 spacing)
{
    u16 entry_ids[14];
    struct PsynergyMenuState *entry_state = gMenuWork;
    struct RenderInput *window = source;
    s32 entry_count = (u16)Party_ListActiveOwnersFar(entry_ids);
    s32 i;

    entry_state->tab_counts[0] = entry_count;
    for (i = 0; i < entry_count; i++) {
        struct AnimationObject *entry_object = (struct AnimationObject *)ResourceObject_CreateFar(Party_RemapCharacterIdByFlagsFar(entry_ids[i]));
        if (entry_object != 0) {
            s32 entry_x;
            s32 source_x;
            s32 position_x;

            entry_state->tab_objects[i] = entry_object;
            source_x = window->x;
            entry_x = spacing + 16;
            entry_x *= i;
            position_x = origin_x + source_x;
            entry_state->tab_x[i] = position_x * 8 + entry_x;
            entry_state->row_positions[i] =
                (origin_y + window->y) * 8 + 16;
            entry_state->owner_scale[i] = 0x10000;
            ((struct FieldSprite *)entry_object)->priority = 0;
            entry_object->flags = 0;
            AnimationObjects_SelectAnimationFar(entry_object, 1);
        }
    }
    for (; i < 8; i++) {
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
    u32 buf[7];
    struct PsynergyMenuState *base = gMenuWork;
    s32 count;
    void **p;
    s32 i;

    count = (u16)Party_ListActiveOwnersFar((s16 *)buf);
    if (count != 0) {
        p = (void **)base->tab_objects;
        i = count;
        do {
            void *entry = *p++;

            if (entry != 0) {
                ResourceObject_ReleaseFar(entry);
            }
        } while (--i != 0);
    }
    Scheduler_RemoveCallback((void (*)(void))Menu_UpdateEntryObjectTransforms);
}

void Menu_UpdateEntryObjectTransforms(void)
{
    /* FAKEMATCH: signed sprite-byte writes and the byte cursor preserve loop registers and write order. */
    u8 *p;
    s32 pos[2];
    s32 trans[4];
    s32 *pp;
    s32 *tp;
    s16 *hp;
    s32 i;
    s32 cnt;

    p = (u8 *)gMenuWork;
    cnt = (u16)Party_CountActiveOwnersFar();
    i = 0;
    if (i < cnt) {
        pp = pos;
        tp = trans;
        hp = (s16 *)(p + 308);
        p += 276;
        do {
            void *obj;
            s32 top;

            top = 0x01e20000 - (hp[8] << 16);
            obj = *(void **)p;
            if (obj != 0) {
                *((s8 *)obj + 9) &= -13;
                pos[0] = *(s32 *)(p + 64);
                pp[1] = *(s32 *)(p + 64);
                tp[1] = top;
                tp[0] = hp[0] << 16;
                /* FAKEMATCH: needed to match: a one-pass block keeps the transform stores in order */
                do {
                    tp[2] = (hp[8] << 16) + top;
                } while (0);
                tp[3] = 0;
                Object_ApplyProjectedPlacementFar(obj, tp, pp, 0x4000);
            }
            i++;
            hp++;
            p += 4;
        } while (i < cnt);
    }
}

void UiMenu_PositionCursor(s32 x_offset, s32 y_offset)
{
    struct PsynergyMenuState *work = gMenuWork;
    struct RenderOutput *cursor = work->pane_icon[0];

    cursor->oam.x =
        cursor->x =
        UiMenu_CursorBobX[(gFrameCount >> 1) & 7] + x_offset
        + ((struct RenderInput *)work->auxiliary_window)->x * 8 + 8;
    cursor->oam.y =
        cursor->y =
        UiMenu_CursorBobY[(gFrameCount >> 1) & 7] + y_offset
        + ((struct RenderInput *)work->auxiliary_window)->y * 8 + 8;
}

void UiMenu_SlideCursor(s32 x, s32 y)
{

    struct PsynergyMenuState *work = gMenuWork;
    struct RenderOutput *cursor;
    s32 steps;
    s32 start_x;
    s32 start_y;
    s32 px;
    s32 py;
    s32 dx;
    s32 dy;

    steps = 2;
    if (work->skip_slide != 0) {
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
    if (py - 8 > 0) {
        cursor->y -= 8;
        py = cursor->y;
    }
    start_x = cursor->x << 4;
    dx = ((x << 4) - start_x + 1) / steps;
    start_y = py << 4;
    dy = ((y << 4) - start_y + 1) / steps;
    px = start_x;
    py = start_y;
    do {
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
    s32 index;

    index = 0;
    slot = ((struct PsynergyMenuState *)gMenuWork)->entry_icons;
    scan = slot;
    do {
        if (*scan++ != NULL) {
            PsynergyMenu_PositionOwnerEntry(slot, index, origin_x, origin_y, columns);
        }
        index++;
        slot++;
    } while (index <= 0x1F);
}

void PsynergyMenu_PositionOwnerEntry(struct RenderOutput **slot, s32 index,
    s32 origin_x, s32 origin_y,
    s32 columns) {
    struct RenderOutput *object;

    if (index > 0x1F) {
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

#undef RenderOutput_ClearListFar

void ItemMenu_RefreshEntry(s32 layout)
{
    struct RenderOutput **slot;
    struct RenderOutput **scan;
    struct RenderOutput *object;
    s32 index;
    s32 origin_y;
    struct InventoryMenuState *menu;

    menu = gMenuWork;
    origin_y = 0x38;
    if (layout != 1) {
        origin_y = 0x28;
    }
    slot = menu->entry_icons;
    index = 0;
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            ItemMenu_PosOwner(slot, index, 0x74, origin_y, 5);
        }
        index++;
        slot++;
    } while (index <= 0xE);
}

#undef RenderOutput_ClearListFar
#define RenderOutput_ClearListFar ((void (*)(void *))RenderOutput_ClearListFar)

#define UiText_GetResourceDimensionsFar ((void (*)(s32,s32*,s32*,s32*,s32*))UiText_GetResourceDimensionsFar)

void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 window;
    s32 height;
    s32 width;
    s32 top;
    s32 left;

    menu->pane_icons[0]->active = 13;
    if (y != -1) {
        UiText_GetResourceDimensionsFar(message, &left, &top, &width, &height);
        if (UiWindow_UpdateOrCreate((s32 *)&menu->modal_window, x, y, width, height, 0x102) == 0)
            UiWindow_SetBounds((struct RenderInput *)menu->modal_window, x, y, width, height);
        window = (s32)menu->modal_window;
    } else {
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
    if (x != -1) {
        WaitFrames(1);
        do {
            WaitFrames(1);
        } while (!(gKeyState & KEY_A) && !(gKeyState & KEY_B) && !(gKeyState & KEY_START));
#if EDITION_INTERNATIONAL
        if (y == -1)
            RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
#endif
        RenderOutput_ClearListFar((void *)window);
    } else {
        GameFlag_SetBitFar(0x151);
    }
    menu->completion_flag = 1;
    menu->pane_icons[0]->active = 1;
#if EDITION_INTERNATIONAL
    if (y != -1)
        UiWindow_CloseIfOpen((s32 *)&menu->modal_window, 1);
#endif
}

s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode)
{
    u16 sorted[16];
    u16 items[16];
    u8 order[32];
    struct ItemDefinition *item;
    s32 pass;
    s32 i;
    s32 count;
    s32 best;
    s32 best_slot;
    s32 sorted_count;
    s32 value;

    best_slot = 0;
    sorted_count = 0;
    Menu_CopyListOrder(mode, order);

    for (i = 0; i < 15; i++)
        items[i] = inventory[i];

    count = 0;
    for (i = 0; i < 15; i++) {
        if (items[i] != 0)
            count++;
    }

    for (i = count; i < 15; i++)
        sorted[i] = 0;

    pass = 0;
    while (order[pass] != 0xff) {
        for (;;) {
            best = 0;
            for (i = 0; i < count; i++) {
                if (items[i] == 0)
                    continue;
                item = Item_GetDirectFar(items[i]);
                if ((order[pass] & 0x7f) != item->type)
                    continue;
                if ((order[pass] & 0x80) && (items[i] & 0x200) == 0)
                    continue;
                value = items[i] & 0x1ff;
                if (best < value) {
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

void AudioCommand_PlayFar(s32 cue);
void Runtime_SetMainState19(void);

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
        goto done;
    Link_DrawShiftedTilePairFar(0x06002500);
    pages = count / per_page;
    if (count % per_page != 0)
        pages++;
    if (horizontal) {
        next = gKeysRepeat & KEY_RIGHT;
        previous = gKeysRepeat & KEY_LEFT;
        horizontal = gKeysRepeat & KEY_UP;
        page_forward = gKeysRepeat & KEY_DOWN;
    } else {
        next = gKeysRepeat & KEY_DOWN;
        previous = gKeysRepeat & KEY_UP;
        horizontal = gKeysRepeat & KEY_LEFT;
        page_forward = gKeysRepeat & KEY_RIGHT;
    }
    if (horizontal) {
        AudioCommand_PlayFar(111);
        if (--*page < 0)
            *page = pages - 1;
        if (*cursor + *page * per_page > count - 1) {
            *cursor = count - *page * per_page - 1;
            if (*cursor > per_page - 1)
                *cursor = per_page - 1;
        }
        Runtime_SetMainState19();
        result = 1;
        goto done;
    }
    if (page_forward) {
        AudioCommand_PlayFar(111);
        if (++*page > pages - 1)
            *page = 0;
        if (*cursor + *page * per_page > count - 1) {
            *cursor = count - *page * per_page - 1;
            if (*cursor > per_page - 1)
                *cursor = per_page - 1;
        }
        Runtime_SetMainState19();
        result = 1;
        goto done;
    }
    if (previous) {
        AudioCommand_PlayFar(111);
        if (--*cursor < 0) {
            *cursor = per_page - 1;
            *cursor = count - *page * per_page - 1;
            if (*cursor > per_page - 1)
                *cursor = per_page - 1;
        }
        goto row_changed;
    }
    result = -1;
    if (next == 0)
        goto done;
    {
        AudioCommand_PlayFar(111);
        result = 0;
        if (++*cursor == count - *page * per_page)
            *cursor = result;
        if (*cursor > per_page - 1)
            *cursor = result;
    }
row_changed:
    result = 0;
done:
    return result;
}

void Palette_LightenBankHighlight(s32 bank)
{
    u16 *palette = (u16 *)(0x05000000 + (bank << 5));
    u32 red;
    u32 green;
    u32 blue;

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
    s32 window,
    s32 item_count,
    s32 page_size,
    s32 selected_page,
    s32 right_edge
)
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
    if (page_count > 1) {
        UiWindow_SetTilemapEntryFar(window, PAGE_CAP_LEFT, x - 1, -1, 0);

        for (page = 0; page < page_count; page++) {
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
    u8 *base = (u8 *)work->tilemap;

    x += object->x + 1;
    y += object->y + 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        y <<= 6;
        x = y + (x << 1);
        do {
            u16 *pixel = (u16 *)(x + (u32)base);
            s32 remaining = width;
            while (remaining != 0) {
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
    struct RenderOutput *icon;
    s32 i;

    for (i = 0; i < 32; i++) {
        icon = menu->entry_icons[i];
        if (icon != NULL)
            icon->active = 13;
    }
    for (i = first; i < page_size + first && (icon = menu->entry_icons[i]) != NULL
         && i <= menu->item_count - 1; i++) {
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

void UiWindow_SetBounds(struct RenderInput *window, s32 x, s32 y,
    s32 width, s32 height)
{
    if (window != NULL) {
        window->width = width;
        window->x = x;
        window->height = height;
        window->y = y;
    }
}

#undef Inventory_EquipFar
#undef RenderOutput_ClearListFar
#undef UiText_DrawStringInWindowFar
#undef UiText_GetResourceDimensionsFar

void UiWork_SetMenuBusy(void)
{
    u8 *base = gWindowWork[0];
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 1;
    *p = flag;
}

void UiWork_ClearMenuBusy(void)
{
    u8 *base = gWindowWork[0];
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 0;
    *p = flag;
}

s32 Audio_PlayCueReturnOne(s32 cue)
{
    AudioCommand_PlayFar(cue);
    return 1;
}

void Menu_CancelSoundTick(void)
{
    if (gKeyState & KEY_START) {
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
    if (GameFlag_TestFar(0x150) == 0) {
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

#define Link_DrawShiftedTilePairFar ((void (*)(void*))Link_DrawShiftedTilePairFar)

struct AssetSelectionResult {
    s32 unknown_000[0x60];
    u16 unknown_180;
    u8 unknown_182[0x18];
    u16 unknown_19a;
};

#if EDITION_INTERNATIONAL
typedef s32 (*CopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*FillFn)(void *dst, s32 size, u32 value);

#endif
void Event_ClearInvalidPackedValuesFar(void);

s32 RunAssetSelectionScreen(void)
{
	s32 Party_ListActiveOwnersFar(void *session);
#if EDITION_INTERNATIONAL
    void *backup;
#endif
    struct InventoryMenuState *screen;
    s32 index;
    s32 value;
    s32 category;
    s32 result;
#if EDITION_INTERNATIONAL
    s32 size;
#endif
    void **process;
    void **cache;

#if EDITION_INTERNATIONAL
    size = 0x2000;
    backup = Runtime_BumpAllocateAlternatePool(size);
#endif
    screen = (struct InventoryMenuState *)Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
    cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_MENU_CONTROL];
    ((struct ObjectSystemWork *)cache[0])->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);
    screen->party_count = Party_ListActiveOwnersFar(screen->owner_ids);
    ItemMenu_Init(0, 3, 0, 7);
    Resource_LoadPairedBlocks();
    Palette_LightenBankHighlight(14);
    Link_DrawShiftedTilePairFar((void *)0x06002500);
    screen->message_window = (struct UiWindow *)UiWindow_CreateFar(13, 0, 17, 3, 2);
#if EDITION_INTERNATIONAL
    Scheduler_EnableOverlayCallbacksWithFlags();
    Iwram_CopyWords(backup, (void *)0x06004000, size);
    Iwram_FillWords((void *)0x06004000, size, 0x33333333);
    UiWork_SetAltFlagAndClearTableFar(1);
#endif
    Menu_CancelSoundReset();
    result = ItemMenu_RunCommands(&category, &value, &index);
    Menu_EnsureCancelSound();
    if (result == 1) {
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

#define Event_ClearInvalidPackedValuesFar ((int (*)())Event_ClearInvalidPackedValuesFar)
#define Item_AdjustCounterFar ((void (*)(s32,s32))Item_AdjustCounterFar)
#if EDITION_INTERNATIONAL
#define MODAL_SHORT_X   14
#define MODAL_REMOVED_X 14
#else
#define MODAL_SHORT_X   15
#define MODAL_REMOVED_X 17
#endif

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

#define IWRAM_COPY_WORDS ((WordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: needed to match: the copy routine as IwramIrqMain plus its offset, passed first; direct calls share the 332-byte size in r5 and move the routine to r6, and through Iwram_CopyWords the routine is loaded before the source and size (2 instructions reordered). */
    return copy(dst, src, size);
}

void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused, s32 style);

enum ItemMenuStep {
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
    s32 sel;
    s32 ret;
    struct BtlUnit *source;
    struct BtlUnit *target;
    struct InventoryMenuState *menu;
    struct ItemDefinition *item;
    struct BtlUnit *source_copy;
    struct BtlUnit *target_copy;
    u32 state;
    s32 command;
    s32 result;
    s32 qty;
    s32 amount;
    s32 aborted;
    s32 work;
    s32 n;
    u8 moved_source;
    u8 moved_target;
    s8 stack;

    done = 0;
#if !EDITION_INTERNATIONAL
    owner = 0;
    chosen = 0;
#endif
    sel = 0;
    ret = 0;
    state = ITEM_STEP_OWNER;
    menu = gMenuWork;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        switch (state) {
        case ITEM_STEP_OWNER:
            menu->selected_slots[0] = 0;
            ItemMenu_SetMsgWin3();
            ItemMenu_SetItemWin3();
            menu->selected_item_icon->active = 13;
            ItemMenu_DrawMsg(0, (s32)MsgWhoseItem);
#if EDITION_INTERNATIONAL
            RenderOutput_RedrawSavedRectFar(menu->info_window);
            UiText_DrawWorkValueWithLabel((s32)menu->info_window);
            command = ItemMenu_PrepOwner(0);
            if (command == -1) {
                sel = command = 0;
                ret = -1;
                done = 1;
            }
#else
            UiText_DrawWorkValueWithLabel((s32)menu->info_window);
            owner = ItemMenu_PrepOwner(0);
            if (owner == -1) {
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
            if (ItemMenu_Count(menu->pane_owner[0]) == 0) {
                state = ITEM_STEP_OWNER;
                break;
            }
            ItemMenu_SetMsgWin3();
            ItemMenu_SetItemWin3();
            menu->selected_item_icon->active = 13;
            menu->pane_icons[0]->active = 1;
            ItemMenu_DrawMsg(0, (s32)MsgWhichItem);
            sel = ItemMenu_RunList(0);
            state = ITEM_STEP_OWNER;
            if (sel == -1) {
                break;
            }
            menu->list_mode = 0xff;
            state = ITEM_STEP_COMMAND;
            break;

        case ITEM_STEP_COMMAND:
            command = ItemMenu_ChooseCmd();
            if (command == -1) {
                state = ITEM_STEP_ITEM;
                menu->completion_flag = 1;
            }
            if (command == ITEM_COMMAND_USE) {
                if (BattleFx_HasTriggerFar(menu->selected_items[0] & ITEM_ID_MASK) != 0) {
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
                result = Item_ClassifyUseMode(
                    menu->pane_owner[0], menu->selected_items[0]);
                if (result == 1) {
                    state = ITEM_STEP_USE;
                }
                if (result == 2) {
                    ItemMenu_Use();
                    RenderOutput_ClearListFar(menu->info_window);
                    InventoryMenu_ShowModalMessage(
                        menu->message_offset + (s32)MsgItemUseResult, 0, -1);
#if !EDITION_INTERNATIONAL
                    RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
                    menu->pane_icons[0]->active = 13;
                    menu->item_count = ItemMenu_Collect(
                        UnitGetFar(menu->pane_owner[0]), menu->items, 0);
                    ItemMenu_DrawIcons(menu->items, 0);
                    state = ITEM_STEP_OWNER;
                }
                if (result == -1 || result == 0) {
                    done = 1;
                    *owner_out = menu->pane_owner[0];
                    *target_out = menu->pane_owner[1];
                    *item_out = menu->selected_items[0] & ITEM_ID_MASK;
                    ret = 1;
                }
            }
            if (command == ITEM_COMMAND_EQUIP) {
                state = ITEM_STEP_EQUIP;
            }
            if (command == ITEM_COMMAND_GIVE) {
                state = ITEM_STEP_TARGET;
            }
            if (command == ITEM_COMMAND_DROP) {
                state = ITEM_STEP_DROP;
            }
            if (command == ITEM_COMMAND_REMOVE) {
                state = ITEM_STEP_REMOVE;
            }
            if (command == ITEM_COMMAND_INSPECT) {
                state = ITEM_STEP_INSPECT;
            }
            break;

        case ITEM_STEP_USE:
            ItemMenu_HideAllIcons();
            ItemMenu_SetMsgWin5();
            ItemMenu_SetItemWin5();
            RenderOutput_RedrawSavedRectFar(menu->message_window);
            ItemMenu_DrawItemHead();
            UiText_DrawCharacterAtOffsetFar(
                (s32)MsgUseOnWhom, (s32)menu->message_window, 16, 16);
#if EDITION_INTERNATIONAL
            if (ItemMenu_SelectTarget(0) != -1) {
#else
            chosen = ItemMenu_SelectTarget(0);
            if (chosen != -1) {
#endif
                command = 0;
                if (ItemMenu_IsSpecial(
                        menu->selected_items[0] & ITEM_ID_MASK) != 0) {
                    command = 8;
                }
                result = ItemMenu_Use();
                Menu_DrawOwnerStatusPanel(
                    (s32)menu->status_window, menu->pane_owner[1], 0, command);
                if (result != -1) {
                    RenderOutput_ClearListFar(menu->info_window);
                    InventoryMenu_ShowModalMessage(
                        menu->message_offset + (s32)MsgItemUseResult, 0, -1);
#if !EDITION_INTERNATIONAL
                    RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
                    menu->pane_icons[0]->active = 13;
                    ItemMenu_TryBreak();
                    state = ITEM_STEP_ITEM;
                }
                menu->item_count = ItemMenu_Collect(
                    UnitGetFar(menu->pane_owner[0]), menu->items, 0);
                ItemMenu_DrawIcons(menu->items, 0);
                menu->completion_flag = 1;
            } else {
                state = ITEM_STEP_COMMAND;
            }
            break;

        case ITEM_STEP_TARGET:
            ItemMenu_SetMsgWin5();
            ItemMenu_SetItemWin5();
            RenderOutput_RedrawSavedRectFar(menu->message_window);
            ItemMenu_DrawItemHead();
            UiText_DrawCharacterAtOffsetFar(
                (s32)MsgGiveToWhom, (s32)menu->message_window, 16, 16);
#if EDITION_INTERNATIONAL
            n = ItemMenu_SelectTarget(1);
            state = ITEM_STEP_GIVE;
            if (n == -1) {
#else
            chosen = ItemMenu_SelectTarget(1);
            state = ITEM_STEP_GIVE;
            if (chosen == -1) {
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
            if ((item->flags & ITEM_STACKABLE) != 0) {
                qty = (menu->selected_items[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
                if (qty > 1) {
                    ItemMenu_DrawItemHead();
                    result = ItemMenu_SelectGiveQuantity(0, qty, 1);
                }
            }
            if (result == -1) {
                state = ITEM_STEP_COMMAND;
                break;
            }
            menu->pane_owner[1] = 0;
            Resource_LoadByModeIntoSlotFar(
                2,
                (menu->selected_items[0] & ITEM_ID_MASK) | (result << INVENTORY_QUANTITY_SHIFT),
                (u8)menu->selected_item_icon->index,
                0);
            menu->selected_item_icon->active = 1;
            menu->selected_item_icon->x = 120;
            menu->selected_item_icon->y = 28;
            UiIcon_PrepareObject(menu->selected_item_icon);
            UiWindow_ClearInteriorTilesFar((s32)menu->list_window, 0, 72, 120, 96);
            RenderOutput_RedrawSavedRectFar(menu->message_window);
            if (ItemMenu_ConfirmDrop(sel) == 0) {
#if EDITION_INTERNATIONAL
                command = menu->pane_owner[0];
                UnitGetFar(command);
                for (work = 0; work < result + 1; work++) {
                    Inventory_RemoveFar(command, menu->selected_slots[0]);
                    Item_AdjustCounterFar(menu->selected_items[0] & ITEM_ID_MASK, 1);
                }
                UnitCalcFar(command);
#else
                owner = menu->pane_owner[0];
                UnitGetFar(owner);
                for (work = 0; work < result + 1; work++) {
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
                InventoryMenu_ShowModalMessage((s32)MsgDroppedIt, MODAL_SHORT_X, 13);
                menu->completion_flag = 1;
                state = ITEM_STEP_ITEM;
            } else {
                state = ITEM_STEP_COMMAND;
            }
            UnitCalcFar(menu->pane_owner[0]);
            menu->selected_item_icon->active = 13;
            Event_ClearInvalidPackedValuesFar();
            break;

        case ITEM_STEP_GIVE:
            aborted = 0;
            item = Item_GetDirectFar(menu->selected_items[0] & ITEM_ID_MASK);
            if ((item->flags & ITEM_STACKABLE) != 0) {
                qty = InventoryMenu_GetItemQuantity(
                    menu->pane_owner[1], menu->selected_items[0] & ITEM_ID_MASK);
                if (qty == 30) {
                    aborted = 1;
                }
                if (ItemMenu_Count(menu->pane_owner[1]) == 15 &&
                    qty == 0) {
                    state = ITEM_STEP_TRADE;
                    break;
                }
                stack = (menu->selected_items[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
                if (aborted == 0) {
                    /* FAKEMATCH: the loop only places a label between the count and its sign-extended use. */
                    do {
                        amount = stack;
                    } while (0);
                    if (qty + amount > 30) {
                        amount = 30 - qty;
                    }
                    if (stack > 1) {
                        result = ItemMenu_SelectGiveQuantity(0, amount, 0);
                    } else {
                        result = 0;
                    }
                    if (result == -1) {
                        state = ITEM_STEP_TARGET;
                        break;
                    }
                    for (command = 0; command < result + 1; command++) {
                        qty = Inventory_AddItemFar(
                            menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
                        if (qty != -1) {
                            Inventory_RemoveFar(
                                menu->pane_owner[0], menu->selected_slots[0]);
                            menu->selected_slots[1] = qty;
                        } else {
                            aborted = 1;
                        }
                    }
                }
            } else {
                result = Inventory_AddItemFar(
                    menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
                if (result == -1) {
                    state = ITEM_STEP_TRADE;
                    break;
                }
                menu->selected_slots[1] = result;
                result = Inventory_RemoveFar(menu->pane_owner[0], menu->selected_slots[0]);
                if (result == -1) {
                    aborted = 1;
                }
            }
            UnitCalcFar(menu->pane_owner[0]);
            UnitCalcFar(menu->pane_owner[1]);
            Owner_RefreshClassActionsFar(menu->pane_owner[0]);
            Owner_RefreshClassActionsFar(menu->pane_owner[1]);
            result = 1;
            if (aborted == 0) {
                menu->pane_owner[0] = menu->pane_owner[1];
                menu->selected_items[0] &= ITEM_ID_MASK;
#if EDITION_INTERNATIONAL
                ItemMenu_SetMsgWin6();
#endif
                RenderOutput_RedrawSavedRectFar(menu->message_window);
                ItemMenu_DrawItemHead();
                result = ItemMenu_ConfirmEquip(0);
            }
            if (GameFlag_TestFar(0x150) != 0) {
                break;
            }
            ItemMenu_RefreshOwner(menu->pane_owner[1], 1);
            menu->pane_icons[0]->active = 13;
            WaitFrames(1);
            if (aborted == 1) {
                RenderOutput_ClearListFar(menu->info_window);
                InventoryMenu_ShowModalMessage((s32)MsgCannotHoldMore, 15, 14);
            } else {
                RenderOutput_ClearListFar(menu->info_window);
                if (result == 1) {
                    InventoryMenu_ShowModalMessage(
                        (s32)MsgGiven, 15, 14);
                } else {
                    ItemMenu_DrawEquipPreview(
                        menu->pane_owner[1], menu->selected_slots[1], 0, menu->pane_owner[1]);
                    InventoryMenu_ShowModalMessage(
                        (s32)MsgEquippedIt, 15, 14);
                    item = Item_GetDirectFar(menu->selected_items[0]);
                    if ((item->flags & ITEM_CURSED) != 0) {
                        AudioCommand_PlayFar(0x67);
                        RenderOutput_ClearListFar(menu->info_window);
                        InventoryMenu_ShowModalMessage(
                            (s32)MsgEquippedIt + 7, MODAL_SHORT_X, 14);
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
            ItemMenu_DrawMsg(0, (s32)MsgSwapForWhat);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            if (ItemMenu_RunList(1) == -1) {
#else
            sel = ItemMenu_RunList(1);
            if (sel == -1) {
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
            while (moved_source <= 29) {
                result = Inventory_RemoveFar(menu->pane_owner[0], menu->selected_slots[0]);
                if (result == 2) {
                    break;
                }
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                moved_source++;
            }
            moved_source++;
            moved_target = 0;
            while (moved_target <= 29) {
                if ((menu->selected_items[1] & INVENTORY_EQUIPPED) != 0) {
                    item = Item_GetDirectFar(menu->selected_items[1] & ITEM_ID_MASK);
                    if ((item->flags & ITEM_CANNOT_UNEQUIP) != 0) {
                        aborted = 1;
                    }
                }
                result = Inventory_RemoveFar(
                    menu->pane_owner[1], menu->selected_slots[1]);
                if (result == 2) {
                    break;
                }
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                moved_target++;
            }
            moved_target++;
            while (moved_source != 0) {
                result = Inventory_AddItemFar(
                    menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                menu->selected_slots[1] = result;
                moved_source--;
            }
            while (moved_target != 0) {
                result = Inventory_AddItemFar(
                    menu->pane_owner[0], menu->selected_items[1] & 0x5ff);
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                menu->selected_slots[0] = result;
                moved_target--;
            }
            WaitFrames(1);
            if (aborted == 1) {
                CopyWords(IWRAM_COPY_WORDS, source, source_copy, BATTLE_UNIT_SIZE);
                CopyWords(IWRAM_COPY_WORDS, target, target_copy, BATTLE_UNIT_SIZE);
                RenderOutput_ClearListFar(menu->info_window);
#if defined(TBS_EDITION_FR)
                InventoryMenu_ShowModalMessage((s32)MsgCannotTrade, 11, 14);
                RenderOutput_RedrawSavedRectFar(menu->status_window);
#else
                InventoryMenu_ShowModalMessage((s32)MsgCannotTrade, 15, 14);
#endif
            } else {
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
                if (GameFlag_TestFar(0x150) == 0) {
                    RenderOutput_ClearListFar(menu->info_window);
                    ItemMenu_SetItemWin5();
                    ItemMenu_RefreshOwner(menu->pane_owner[1], 1);
                    if (result == 0) {
                        ItemMenu_DrawEquipPreview(
                            menu->pane_owner[1], menu->selected_slots[1], 0, menu->pane_owner[1]);
                        InventoryMenu_ShowModalMessage(
                            (s32)MsgEquippedIt, 15, 14);
                        item = Item_GetDirectFar(menu->selected_items[0]);
                        if ((item->flags & ITEM_CURSED) != 0) {
                            AudioCommand_PlayFar(0x67);
                            RenderOutput_ClearListFar(menu->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)MsgEquippedIt + 7, MODAL_SHORT_X, 14);
                        }
                    } else {
#if defined(TBS_EDITION_IT)
                        InventoryMenu_ShowModalMessage(
                            (s32)MsgTraded, 13, 14);
#else
                        InventoryMenu_ShowModalMessage(
                            (s32)MsgTraded, 15, 14);
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
            if (result == -1) {
                state = ITEM_STEP_ITEM;
                break;
            }
            if (result == -2) {
                RenderOutput_ClearListFar(menu->info_window);
                InventoryMenu_ShowModalMessage((s32)MsgCannotRemoveItem, 0, -1);
#if !EDITION_INTERNATIONAL
                RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
                state = ITEM_STEP_ITEM;
                break;
            }
            UnitCalcFar(menu->pane_owner[0]);
            Owner_RefreshClassActionsFar(menu->pane_owner[0]);
            menu->pane_icons[0]->active = 13;
            menu->item_count = ItemMenu_Collect(
                UnitGetFar(menu->pane_owner[0]), menu->items, 0);
            ItemMenu_DrawIcons(menu->items, 0);
            WaitFrames(1);
            ItemMenu_DrawEquipPreview(
                menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
            RenderOutput_ClearListFar(menu->info_window);
            InventoryMenu_ShowModalMessage((s32)MsgEquippedIt, 15, 8);
            item = Item_GetDirectFar(menu->selected_items[0]);
            if ((item->flags & ITEM_CURSED) != 0) {
                AudioCommand_PlayFar(0x67);
                RenderOutput_ClearListFar(menu->info_window);
                InventoryMenu_ShowModalMessage(
                    (s32)MsgEquippedIt + 7, MODAL_SHORT_X, 8);
            }
            state = ITEM_STEP_ITEM;
            break;

        case ITEM_STEP_REMOVE:
            UnitGetFar(menu->pane_owner[0])->inventory[menu->selected_slots[0]]
                &= 0xfdff;
            UnitCalcFar(menu->pane_owner[0]);
            Owner_RefreshClassActionsFar(menu->pane_owner[0]);
            menu->pane_icons[0]->active = 13;
            menu->item_count = ItemMenu_Collect(
                UnitGetFar(menu->pane_owner[0]), menu->items, 0);
            ItemMenu_DrawIcons(menu->items, 0);
            WaitFrames(1);
            menu->equip_preview = 1;
            ItemMenu_DrawEquipPreview(
                menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
            menu->equip_preview = 0;
            RenderOutput_ClearListFar(menu->info_window);
            InventoryMenu_ShowModalMessage((s32)MsgRemoved, MODAL_REMOVED_X, 8);
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
            if (result != -1) {
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

    if (GameFlag_TestFar(0x150) != 0) {
        ret = -1;
    }
    return ret;
}

#undef Event_ClearInvalidPackedValuesFar

#define AudioCommand_PlayFar ((s32 (*)(s32))AudioCommand_PlayFar)

s32 ItemMenu_Use(void)
{
    struct InventoryMenuState *menu;
    s32 result;

    menu = gMenuWork;
    result = Item_Use(
        menu->selected_slots[0], menu->pane_owner[0], menu->pane_owner[1]);

    if (result == -1) {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
        RenderOutput_ClearListFar(menu->info_window);
        InventoryMenu_ShowModalMessage(
            menu->message_offset + (s32)MsgItemUseResult, result, result);
#if !EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
        menu->completion_flag = 1;
        return result;
    }

    Item_PlayUseAnimation(menu->selected_items[0] & 0x1ff);
    UnitCalcFar(menu->pane_owner[0]);
    UnitCalcFar(menu->pane_owner[1]);
    return 1;
}

#undef AudioCommand_PlayFar

void ItemMenu_Init(s32 x, s32 y, s32 mode, s32 columns)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 index;

    PsynergyMenu_InitializeEntryObjects(UiMenu_CreateCursor(menu), 2, 2, 8, 0);
    for (index = 3; index >= 0; index--)
        menu->owner_y[index] = 30;

    {
        u8 zero = 0;
        s32 style;
        menu->help_window = NULL;
        menu->status_window = NULL;
        style = 2;
        menu->info_window =
            (struct UiWindow *)UiWindow_CreateFar(0, 17, 30, 3, style);
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
    s32 index0;
    s32 fifth0;
    struct RenderOutput **output1;
    s32 index1;
    s32 fifth1;
    struct RenderOutput **output2;
    s32 index2;
    s32 fifth2;

    index0 = 0;
    fifth0 = 0xA8;
    output0 = &state->entry_icons[0];
    do {
        *output0++ = RenderOutput_CreateFromResourceFar(2, index0, (struct RenderInput *)arg1, 0xF8, fifth0);
        index0++;
    } while (index0 <= 7);

    index1 = 8;
    fifth1 = 0xA8;
    output1 = &state->entry_icons[8];
    do {
        *output1++ = RenderOutput_CreateFromResourceFar(2, index1, (struct RenderInput *)arg1, 0x100, fifth1);
        index1++;
    } while (index1 <= 15);

    index2 = 16;
    fifth2 = 0xA8;
    output2 = &state->entry_icons[16];
    do {
        *output2++ = RenderOutput_CreateFromResourceFar(2, index2, (struct RenderInput *)arg1, 0x100, fifth2);
        index2++;
    } while (index2 <= 31);
}

void ItemMenu_HideAllIcons(void)
{
    s32 hidden_state = 13;
    struct RenderOutput **icons = ((struct InventoryMenuState *)gMenuWork)->entry_icons;
    s32 slot;

    for (slot = 31; slot >= 0; slot--) {
        struct RenderOutput *icon = *icons++;
        if (icon != 0) {
            icon->active = hidden_state;
        }
    }
}

#if EDITION_INTERNATIONAL
void ItemMenu_HidePageIcons(void)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 slot = 0;
    s32 hidden_state = 13;
    struct RenderOutput **icon_slot = menu->entry_icons;

    do {
        struct RenderOutput *icon = *icon_slot++;

        if (icon != 0 && slot % 5 == 0) {
            icon->active = hidden_state;
        }
        slot++;
    } while (slot <= 31);
}
#endif

void ItemMenu_Close(void)
{
    struct InventoryMenuState *menu;
    struct RenderOutput *cursor;

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
    s32 result = 0;
    s32 index;
    struct BtlUnit *owner;

    index = menu->pane_index[pane];
    RenderOutput_ClearListFar(menu->info_window);
    menu->pane_count[pane] = menu->party_count;
    if (index == -1) {
        menu->pane_index[pane] = result;
        index = 0;
    } else {
        UiMenu_SlideCursor(index * 24 - 10, 16);
    }
    owner = UnitGetFar(menu->owner_ids[index]);
    menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
    result = ItemMenu_RunOwnerSelection(menu->owner_ids, menu->items);
    UiIcon_PrepareObject(menu->pane_icons[pane]);
    WaitFrames(1);
    return result;
}

#undef UiText_DrawStringInWindowFar
#define UiText_DrawStringInWindowFar ((void (*)(const u8*,s32,s32,s32))UiText_DrawStringInWindowFar)
#if !EDITION_INTERNATIONAL
#endif
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 style);

s32 ItemMenu_RunOwnerSelection(u16 *owner_ids, u16 *items)
{
    struct InventoryMenuState *menu;
    s32 selection;
    s32 count;
    s32 result;
    struct BtlUnit *owner;
    u8 sort_mode;
    s32 by_category;
    s32 pending;
    s32 window;
    s32 i;

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
    if (UiWindow_UpdateOrCreate((s32 *)&menu->help_window, 13, 13, 17, 4, 2)) {
        menu->selected_item_icon = RenderOutput_CreateFromResourceFar(2, 0, (struct RenderInput *)menu->help_window, 0, result);
        menu->selected_item_icon->active = 13;
    }
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp, (s32)menu->help_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp + 1, (s32)menu->help_window, 0, 8);
#else
    UiText_DrawStringInWindowFar(ItemMenu_ArrangeKeysString, (s32)menu->help_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp, (s32)menu->help_window, 32, 0);
    UiText_DrawStringInWindowFar(ItemMenu_EquipmentKeyString, (s32)menu->help_window, 16, 8);
    UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp + 1, (s32)menu->help_window, 32, 8);
#endif
    menu->pane_icons[0]->active = pending;
    while (!GameFlag_TestFar(0x150)) {
        selection = (selection + count) % count;
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        if (pending) {
            sort_mode = 0;
            pending = 0;
            window = (s32)menu->status_window;
            owner = UnitGetFar(owner_ids[selection]);
            if (by_category) {
                menu->item_count = ItemMenu_Collect(UnitGetFar(owner_ids[selection]), menu->items, 0);
                ItemMenu_OpenCategory(owner_ids[selection]);
                Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 8);
            } else {
                ItemMenu_RefreshOwner(owner_ids[selection], 0);
                Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 0);
            }
            for (i = 3; i >= 0; i--)
                menu->owner_y[i] = 30;
            menu->owner_y[selection] = 26;
        }
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            if (gKeysHeld & KEY_L) {
                sort_mode = (sort_mode + 4) % 4;
                InventoryMenu_SortByListOrder(owner->inventory, sort_mode);
                sort_mode++;
                ItemMenu_RefreshOwner(owner_ids[selection], 0);
                AudioCommand_PlayFar(112);
            } else if (ItemMenu_Count(owner_ids[selection])) {
                AudioCommand_PlayFar(112);
                result = owner_ids[selection];
                break;
            } else {
                AudioCommand_PlayFar(114);
            }
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        if (gKeyState & KEY_R) {
            by_category = 1;
            pending = 1;
        }
        if (!(gKeysHeld & KEY_R) && by_category == 1) {
            by_category = 0;
            pending = 1;
        }
        if (gKeysRepeat & KEY_LEFT) {
            AudioCommand_PlayFar(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & KEY_RIGHT) {
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

#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)
#undef RenderOutput_ClearListFar
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)

void UiWindow_DrawDividerLineFar(s32 window, s32 unused, s32 x, s32 y, s32 width);
void GameFlag_ClearBitFar(s32 flag);

s32 ItemMenu_SelectTarget(s32 mode)
{

    struct InventoryMenuState *menu;
    s32 window;
    s32 count;
    s32 selection;
    s32 pending;
    u8 result;
    s32 shown;
    s32 quantity;
    struct RenderOutput *marker;

    menu = gMenuWork;
    window = (s32)menu->item_window;
    selection = menu->pane_index[1];
    count = menu->party_count;
    pending = 1;
    result = 0;
    shown = 0;
    UiWindow_SetBounds((struct RenderInput *)window, 13, 5, 17, 12);
    RenderOutput_RedrawSavedRectFar((s32)menu->item_window);
    UnitGetFar(menu->owner_ids[menu->pane_index[0]]);
    Scheduler_AddOrUpdateCallback(EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
    while (!GameFlag_TestFar(0x150)) {
        if (pending) {
            pending = 0;
            selection = (selection + count) % count;
            window = (s32)menu->item_window;
            UnitGetFar(menu->owner_ids[selection]);
            marker = menu->pane_icons[1];
            marker->oam.x = marker->x = ((menu->main_window->x + selection * 3) << 3) - 2;
            if (mode == 1) {
                ItemMenu_RefreshOwner(menu->owner_ids[selection], 1);
                UiWindow_DrawDividerLineFar(window, 0, 9, 16, 9);
                UiWindow_ClearInteriorTilesFar(window, 0, 72, 120, 80);
                if (selection != menu->pane_index[0]) {
                    quantity = InventoryMenu_GetItemQuantity(menu->owner_ids[selection], menu->selected_items[0] & ITEM_ID_MASK);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
                    if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
                        UiText_DrawCharacterAtOffsetFar((s32)MsgTradeForWhat, window, 0, 72);
                    else if (quantity != 0) {
                        UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
                        UiText_DrawCharacterAtOffsetFar((s32)MsgInStock, window, STOCK_LABEL_X, 72);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)MsgNoneInStock, window, 16, 72);
                    }
#else
                    if (quantity != 0) {
                        UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
                        UiText_DrawCharacterAtOffsetFar((s32)MsgInStock, window, STOCK_LABEL_X, 72);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)MsgNoneInStock, window, 16, 72);
                    }
                    if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
#if EDITION_INTERNATIONAL
                        UiText_DrawCharacterAtOffsetFar((s32)MsgTradeForWhat, window, 0, 72);
#else
                        UiText_DrawCharacterAtOffsetFar((s32)MsgTradeForWhat, window, 16, 72);
#endif
#endif
                }
                ItemMenu_DrawEquipPreview(menu->pane_owner[0], menu->selected_slots[0], 0, menu->owner_ids[selection]);
            }
            if (mode == 0) {
                if (ItemMenu_IsSpecial(menu->selected_items[0] & ITEM_ID_MASK))
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->owner_ids[selection], menu->selected_slots[0], 8);
                else
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->owner_ids[selection], menu->selected_slots[0], 0);
                if (!GameFlag_TestFar(0x151) && !shown) {
#if EDITION_INTERNATIONAL
                    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
#else
                    RenderOutput_ClearListFar((s32)menu->info_window);
#endif
#if EDITION_INTERNATIONAL
                    UiText_DrawCharacterAtOffsetFar((menu->selected_items[0] & ITEM_ID_MASK) + (s32)MsgItemPlainName, (s32)menu->info_window, 0, 0);
#else
                    UiText_DrawResourceFar((menu->selected_items[0] & ITEM_ID_MASK) + (s32)MsgItemPlainName, (s32)menu->info_window, 0, 0);
#endif
                    shown = 1;
                } else {
                    GameFlag_ClearBitFar(0x151);
                }
            }
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            if (mode == 1 && selection == menu->pane_index[0]) {
                AudioCommand_PlayFar(114);
                continue;
            }
            AudioCommand_PlayFar(112);
            result = menu->owner_ids[selection];
            break;
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            result = 255;
            break;
        }
        if (gKeysRepeat & KEY_LEFT) {
            AudioCommand_PlayFar(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & KEY_RIGHT) {
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

#undef RenderOutput_RedrawSavedRectFar

#define gFrameCount (*(s32 *)&gFrameCount)

void EquipmentMenu_UpdateCompatibilityIndicators(void)
{
    struct InventoryMenuState *menu;
    s8 member_index;

    menu = gMenuWork;
    if ((gFrameCount & 31) == 0 && menu->party_count != 0) {
        member_index = 0;
        do {
            if (Item_CanOwnerEquip(menu->owner_ids[member_index],
                                   menu->selected_items[0] & ITEM_ID_MASK) != 0) {
                struct AnimationObject *indicator = menu->owner_objects[member_index];
                AnimationObjects_SelectAnimationFar(indicator, 3);
            } else {
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
    s8 member_index;

    menu = gMenuWork;
    if (menu->party_count != 0) {
        member_index = 0;
        do {
            struct AnimationObject *indicator = menu->owner_objects[member_index];
            AnimationObjects_SelectAnimationFar(indicator, 1);
            member_index++;
        } while (member_index < menu->party_count);
    }
    Scheduler_RemoveCallback(&EquipmentMenu_UpdateCompatibilityIndicators);
}

s32 ItemMenu_IsSpecial(s32 item_id)
{
    s32 result;
    s32 first;

    if (item_id > 0xC4) {
        goto L0;
    }
    first = 0xC1;
    if (item_id < first) {
        goto L0;
    }
    result = 1;
    return result;
L0:
    result = 0;
    return result;
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
    s32 slot;

    for (slot = 0; slot < 32; slot++) {
        if (items[slot] == 0) {
            UiIcon_PrepareObject(menu->entry_icons[slot]);
            menu->entry_icons[slot]->active = 13;
        }
    }
}

s32 ItemMenu_Count(s32 owner_id)
{
    s32 item_id;
    s32 remaining;
    s32 count;
    u16 *slots;

    count = 0;
    slots = UnitGetFar(owner_id)->inventory;
    remaining = 0xE;
    do {
        item_id = ITEM_ID_MASK & *slots;
        slots += 1;
        if (item_id != 0) {
            count += 1;
        }
        remaining -= 1;
    } while (remaining >= 0);
    return count;
}

s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item)
{
    struct BtlUnit *state;
    s32 i;
    s32 quantity;

    quantity = 0;
    state = UnitGetFar(owner);
    i = 0;
    do {
        if (state->inventory[i] != 0 && (state->inventory[i] & 0x1ff) == item) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            quantity += (state->inventory[i] & 0xf800) >> 11;
            quantity++;
#else
            quantity = (state->inventory[i] & 0xf800) >> 11;
            quantity++;
            break;
#endif
        }
        i++;
    } while (i <= 14);
    return quantity;
}

s32 Inventory_RemoveFirstUnflagged(s32 owner);

s32 ItemMenu_Collect(struct BtlUnit *owner, u16 *items, s32 mode)
{
    s32 count;
    s32 i;

    for (i = 0; i < 32; i++) {
        items[i] = 0;
    }
    count = 0;
    for (i = 0; i < 15; i++) {
        items[i] = 0;
        if (owner->inventory[i] != 0) {
            items[count] = owner->inventory[i];
            count++;
        }
    }
    return count;
}

void ItemMenu_DrawIcons(u16 *items, s32 style)
{
    s32 remaining;
    u16 *entries;
    struct RenderOutput **icons;
    s32 item_id;

    icons = ((struct InventoryMenuState *)gMenuWork)->entry_icons;
    entries = items;
    remaining = 14;
    do {
        item_id = *entries++;
        if (item_id != 0) {
            if (style == 0) {
                Resource_LoadByModeIntoSlotFar(
                    2, item_id, (u8)(*icons)->index, 0);
            } else {
                Resource_LoadByModeIntoSlotFar(
                    7, item_id, (u8)(*icons)->index, 0);
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
    struct BtlUnit *owner;
    u16 *items;

    menu = gMenuWork;
    owner = UnitGetFar(owner_id);
    items = menu->items;
    menu->item_count = ItemMenu_Collect(owner, items, 0);
    RenderOutput_RedrawSavedRectFar(menu->item_window);
    ItemMenu_RefreshEntry(mode);
    ItemMenu_DrawIcons(items, 0);
    if (ItemMenu_Count(owner_id) == 0)
        UiText_DrawCharacterAtOffsetFar(
            (s32)MsgItemMenuEmpty, (s32)menu->item_window, 8, 24);
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
    s32 style = 0;
    struct BtlUnit *state = UnitGetFar(owner);
    s32 item = state->inventory[slot];
    s32 equipped;

    if (mode == 1)
        style = 0x100;
    switch (Item_GetDirectFar(item & ITEM_ID_MASK)->type) {
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
        if (owner == target) {
            style |= 2;
            Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
        } else {
            s32 size;

            state = UnitGetFar(target);
            size = BATTLE_UNIT_SIZE;
            owner = (s32)MemAlloc(size);
            Iwram_CopyWords((void *)owner, state, size);
            equipped = Inventory_RemoveFirstUnflagged(target);
            if (equipped != 0) {
                item &= ~0x200;
                equipped = Inventory_AddItemFar(target, item);
                if (equipped != -1) {
                    style |= 2;
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, equipped, style);
                } else {
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
                }
            } else {
                Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
            }
            Iwram_CopyWords(state, (void *)owner, size);
            MemFree((void *)owner);
        }
        break;
    case 6:
        if (target == owner) {
            style |= 4;
            Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
        } else {
            s32 size;
            void *buffer;

            state = UnitGetFar(target);
            size = BATTLE_UNIT_SIZE;
            owner = (s32)MemAlloc(size);
            buffer = (void *)owner;
            Iwram_CopyWords(buffer, state, size);
            equipped = Inventory_RemoveFirstUnflagged(target);
            if (equipped != 0) {
                equipped = Inventory_AddItemFar(target, item);
                if (equipped != -1) {
                    style |= 4;
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, equipped, style);
                } else {
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
                }
            } else {
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
    s32 result = 0;
    s32 i;

    for (i = 0; i < 15; i++) {
        if (state->inventory[i] == 0) {
            result = 1;
            break;
        }
        if ((state->inventory[i] & INVENTORY_EQUIPPED) == 0) {
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

void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 width, s32 height, s32 style);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused1, s32 unused2);

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
    if (row != 0) {
        y = 0x26;
    }
    return y;
}
#endif

s32 ItemMenu_ChooseCmd(void)
{
    struct InventoryMenuState *menu;
    s8 command_storage[6];
    u16 *redraw_flag;
    s32 col;
    s32 saved;
    s32 row;
    s32 index;
    s32 need_redraw;
    s8 *command_states;
    s32 x;
    s32 y;

    index = 0;
    col = 0;
    row = 0;
    menu = gMenuWork;
    need_redraw = 1;
    command_states = command_storage;
    ItemMenu_BuildCmd(command_states);
    redraw_flag = &menu->flags;
    if (*redraw_flag != 1) {
        s32 message_window;

        ItemMenu_HideAllIcons();
        RenderOutput_RedrawSavedRectFar((s32)menu->list_window);
        message_window = (s32)menu->message_window;
        ItemMenu_SetMsgWin7();
        RenderOutput_RedrawSavedRectFar(message_window);
        UiWindow_DrawDividerLineFar(message_window, 0, 3, 0x10, 3);
        ItemMenu_DrawItemHead();
        ItemMenu_DrawCmd(command_states, message_window);
#if EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
        UiText_DrawCharacterAtOffsetFar(
            (menu->selected_items[0] & 0x1ff) + (s32)MsgItemPlainName,
            (s32)menu->info_window, 0, 0);
#else
        RenderOutput_ClearListFar((s32)menu->info_window);
        UiText_DrawResourceFar(
            (menu->selected_items[0] & 0x1ff) + (s32)MsgItemPlainName,
            (s32)menu->info_window, 0, 0);
#endif
    }
    *redraw_flag = 0;

    saved = (s8)menu->list_mode;
    if (saved == -1) {
        if (command_states[2] == 1) {
            col = 2;
            row = 0;
        }
        if (command_states[3] == 1) {
            col = 0;
            row = 1;
        }
        if (command_states[1] == 1) {
            col = 1;
            row = 0;
        }
        if (command_states[4] == 1) {
            col = 1;
            row = 1;
        }
        if (command_states[0] == 1) {
            col = 0;
            row = 0;
        }
    } else {
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

    for (;;) {
        if (GameFlag_TestFar(0x150) != 0)
            break;
        if (need_redraw != 0) {
            need_redraw = 0;
            col = (col + 3) % 3;
            row = (row + 2) % 2;
            index = row * 3 + col;
            EquipmentMenu_StartCompatibilityIndicators();
            if (index > 2) {
                menu->equip_preview = 1;
                ItemMenu_DrawEquipPreview(menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
                if (index == 3) {
                    Scheduler_AddOrUpdateCallback(&EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
                }
            } else if (index != 0) {
                menu->equip_preview = 0;
                ItemMenu_DrawEquipPreview(menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
            } else {
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

        if ((gKeyState & KEY_A) != 0) {
            if (command_states[index] == -1) {
                AudioCommand_PlayFar(114);
            } else {
                switch (index) {
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
        if ((gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(113);
            index = -1;
            menu->list_mode = index;
            break;
        }

        if ((gKeysRepeat & KEY_UP) != 0) {
            row -= 1;
            need_redraw = 1;
            AudioCommand_PlayFar(111);
        } else if ((gKeysRepeat & KEY_DOWN) != 0) {
            row += 1;
            need_redraw = 1;
            AudioCommand_PlayFar(111);
        } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
            col += 1;
            need_redraw = 1;
            AudioCommand_PlayFar(111);
        } else if ((gKeysRepeat & KEY_LEFT) != 0) {
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
    struct ItemDefinition *item;

    menu = gMenuWork;
    item = Item_GetDirectFar(ITEM_ID_MASK & menu->selected_items[0]);

    if (item->type == 0) {
        command_states[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;
        command_states[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
    } else {
        command_states[ITEM_COMMAND_USE] = COMMAND_DISABLED;
        command_states[ITEM_COMMAND_EQUIP] = COMMAND_AVAILABLE;
    }

    if (Item_ClassifyUseMode(menu->pane_owner[0], menu->selected_items[0]) != -1)
        command_states[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;
    else
        command_states[ITEM_COMMAND_USE] = COMMAND_DISABLED;

    if (menu->selected_items[0] & INVENTORY_BROKEN)
        command_states[ITEM_COMMAND_USE] = COMMAND_DISABLED;

    if (Item_CanOwnerEquip(
            menu->pane_owner[0],
            menu->selected_items[0] & ITEM_ID_MASK) == 0) {
        command_states[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
    }

    command_states[ITEM_COMMAND_GIVE] = COMMAND_AVAILABLE;
    command_states[ITEM_COMMAND_DROP] = COMMAND_AVAILABLE;
    command_states[ITEM_COMMAND_INSPECT] = COMMAND_AVAILABLE;

    if (menu->selected_items[0] & INVENTORY_EQUIPPED) {
        command_states[ITEM_COMMAND_REMOVE] = COMMAND_AVAILABLE;
        command_states[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
    } else {
        command_states[ITEM_COMMAND_REMOVE] = COMMAND_DISABLED;
    }

    if (item->flags & ITEM_CANNOT_UNEQUIP) {
        command_states[ITEM_COMMAND_REMOVE] = COMMAND_DISABLED;
        if (menu->selected_items[0] & INVENTORY_EQUIPPED) {
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

    message = (u32)MsgItemCommandUse;
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
    s32 masked = itemId;
    struct ItemDefinition *def;
    s32 result = -1;

    masked &= ITEM_ID_MASK;
    def = Item_GetDirectFar(masked);

    if (BattleFx_HasTriggerFar(masked)!= 0) {
        return 0;
    }

    {
        struct BtlAct *action = BtlActGet(def->action_id & 0x3fff);

        if (def->action_id != 0) {
            if (def->type != 0) {
                if (def->use_type != 3) {
                    if (Item_CanOwnerEquip(owner, masked) != 0) {
                        result = 1;
                    }
                }
            } else {
                result = 1;
            }

            if (result == 1) {
                s32 mask = 0x80;
                u8 flags = action->tgt_flg;

                if ((flags & 0x40) != 0) {
                    u8 range = action->rng;
                    result = (range == 0xff) ? 2 : 1;
                } else {
                    result = (flags & mask) ? -1 : 0;
                }
            }
        }
    }

    return result;
}

#undef RenderOutput_RedrawSavedRectFar

void ItemMenu_TryBreak(void)
{
    struct InventoryMenuState *menu;

    menu = gMenuWork;
    if ((Item_GetDirectFar(0x1ff & menu->selected_items[0])->use_type == 2) && (Random16() < 0x2000U)) {
        Inventory_BreakFar(
            menu->pane_owner[0],
            menu->selected_slots[0]);
        AudioCommand_PlayFar(SOUND_ITEM_BREAK);
        InventoryMenu_ShowModalMessage((s32)MsgItemBroke, 0, -1);
#if !EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
    }
}

#undef AudioCommand_PlayFar

s32 ItemMenu_OpenDetail(s32 item_index)
{
    struct InventoryMenuState *menu = gMenuWork;

    UiWindow_UpdateOrCreate((s32 *)&menu->equip_window, 0, 0, 13, 10, 2);
    Palette_CopyObjectBankToBackground14();

    if (menu->selected_items[item_index] != 0)
        ItemMenu_DrawItemDetails(
            (s32)menu->equip_window, menu->selected_items[item_index]);

    return 1;
}

#undef UiText_DrawStringInWindowFar

#undef UiText_DrawStringInWindowFar
#undef UiText_DrawStringInWindowFar
#define UiText_DrawStringInWindowFar ((void (*)(u8*,s32,s32,s32))UiText_DrawStringInWindowFar)
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 palette);
void UiWork_PushValueSlotFar(u32 value, u32 slot);

s32 Menu_SelectQuantity(s32 value)
{
	void UiWork_FinalizeFar(s32, s32);
#if EDITION_INTERNATIONAL
    s32 changed = 1;
    struct InventoryMenuState *menu = gMenuWork;
    struct RenderOutput *confirmState = menu->selected_item_icon;
#else
    struct InventoryMenuState *menu = gMenuWork;
    s32 changed = 1;
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

    goto check_exit;

adjust:
    {
        if (gKeysRepeat & 0x40) {
            quantity -= 1;
            changed = 1;
        }
        if (gKeysRepeat & 0x80) {
            quantity += 1;
            changed = 1;
        }
    }
    WaitFrames(1);

check_exit:
    if (GameFlag_TestFar(336) != 0)
        goto done;

    if (changed != 0) {
        changed = 0;
        quantity = (quantity + 5) % 5;
        ItemMenu_DrawItemDetails(window, value);
    }

    {
        if (gKeyState & 1)
            goto done;
        if (gKeyState & 2) {
            quantity = -1;
            goto done;
        }
    }
    goto adjust;

done:
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
    WaitFrames(1);
    UiWork_FinalizeFar(window, 1);
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
    s32 i;
    s32 found;
    s32 amount;
    s32 consumable;
    s8 row;

    row = 0;
    consumable = 0;
    def = Item_GetDirectFar(item & ITEM_ID_MASK);
    if (def->type != 0) {
        found = 0;
        if (def->primary_bonus == 0 && def->secondary_bonus == 0) {
            for (i = 0; i < 4; i++) {
                if (def->effects[i].kind != 0 || def->use_type == 3) {
                    found = 1;
                    break;
                }
            }
        } else {
            found = 1;
        }
        if (found == 1) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgEquipEffectHeading, window, 16, 0);
            row = 1;
        }
        if (def->primary_bonus != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgStatLabel, window, 0, row * 8);
            amount = def->primary_bonus;
            ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
            row++;
        }
        if (def->secondary_bonus != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgDefenseLabel, window, 0, row * 8);
            amount = def->secondary_bonus;
            ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
            row++;
        }
    }
    for (i = 0; i < 4; i++) {
        if (def->effects[i].kind == 0)
            continue;
        amount = def->effects[i].amount;
        switch (def->effects[i].kind) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 26:
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
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
            UiWindow_SetTilemapEntryFar(window, (u8)((def->effects[i].kind - 15) % 4) + 1, 0, row, 2);
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)MsgEquipEffectName, window, 8, row * 8);
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
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
#if EDITION_INTERNATIONAL
            UiText_DrawStringInWindowFar(Menu_TimesString, window, 64, row * 8);
            if (amount > 9) {
                UiText_DrawNumberInWindowFar(1, 1, window, 72, row * 8);
                UiText_DrawStringInWindowFar(Menu_PointString, window, 80, row * 8);
                UiText_DrawNumberInWindowFar(amount - 10, 1, window, 88, row * 8);
            } else {
                UiText_DrawNumberInWindowFar(0, 1, window, 72, row * 8);
                UiText_DrawStringInWindowFar(Menu_PointString, window, 80, row * 8);
                UiText_DrawNumberInWindowFar(amount, 1, window, 88, row * 8);
            }
#else
            if (amount > 9) {
                UiText_DrawNumberInWindowFar(1, 1, window, 48, row * 8);
                UiText_DrawStringInWindowFar(Menu_PointString, window, 56, row * 8);
                UiText_DrawNumberInWindowFar(amount - 10, 1, window, 64, row * 8);
            } else {
                UiText_DrawNumberInWindowFar(0, 1, window, 48, row * 8);
                UiText_DrawStringInWindowFar(Menu_PointString, window, 56, row * 8);
                UiText_DrawNumberInWindowFar(amount, 1, window, 64, row * 8);
            }
            UiText_DrawCharacterAtOffsetFar((s32)MsgEffectRateSuffix, window, 72, row * 8);
#endif
            break;
        case 23:
        case 25:
        case 27:
            UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
            break;
        }
        row++;
    }
    if (def->flags & ITEM_CURSED) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgItemCursed, window, 0, row * 8);
        row++;
    }
    if (def->use_type == 3) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgBestowsPsynergy, window, 0, row * 8);
        consumable = 1;
        row++;
    }
    if (def->use_type != 4 && def->use_type != 0) {
        if (!consumable) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsesHeading, window, 16, row * 8);
            row++;
        }
        switch (def->use_type) {
        case 0:
            break;
        case 1:
            UiText_DrawCharacterAtOffsetFar((s32)MsgSingleUse, window, 0, row * 8);
            row++;
            break;
        case 2: {
            s32 m;

            if (item & INVENTORY_BROKEN) {
                m = (s32)MsgBrokenNotice;
                UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
                row++;
                m++;
                UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
                row++;
            } else {
                m = (s32)MsgMightBreak;
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
    if (def->flags & ITEM_STACKABLE) {
        if (row != 0)
            row++;
        i = (s32)MsgNumberHeading;
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
    if (row == 0) {
        if (def->flags & 4) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgRareItem, window, 0, 0);
            found = 1;
        }
        if (!found) {
            if (def->flags & ITEM_INDISPENSABLE) {
                UiText_DrawCharacterAtOffsetFar((s32)MsgImportantItem, window, 0, 0);
                found = 1;
            }
            if (!found)
                UiText_DrawCharacterAtOffsetFar((s32)MsgDetailsUnknown, window, 0, 0);
        }
    }
}

void ItemMenu_DrawStat(
    s32 delta, s32 unused, s32 window, s32 x, s32 y)
{
    s32 digits;
    s32 magnitude;

    UiText_DrawNumberInWindowFar(delta, 3, window, x, y);
    digits = 1;
    magnitude = delta;
    if (delta < 0) {
        magnitude = -delta;
    }
    if (magnitude > 9) {
        digits = 2;
    }
    magnitude = delta;
    if (delta < 0) {
        magnitude = -delta;
    }
    if (magnitude > 99) {
        digits = 3;
    }
    if (delta > 0) {
        UiText_DrawStringInWindowFar(
            &Menu_PlusSignString, window, x - digits * 8 + 16, y);
    } else {
        UiText_DrawStringInWindowFar(
            &Menu_MinusSignString, window, x - digits * 8 + 16, y);
    }
}

void ItemMenu_SetItemWin5(void)
{
    UiWindow_SetBounds(
        (struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->item_window,
        13, 5, 17, 10);
}

void ItemMenu_SetItemWin3(void)
{
    UiWindow_SetBounds(
        (struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->item_window,
        13, 3, 17, 10);
}

void ItemMenu_SetMsgWin5(void)
{
    UiWindow_SetBounds(
        (struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window,
        13, 0, 17, 5);
}

void ItemMenu_SetMsgWin6(void)
{
    UiWindow_SetBounds(
        (struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window,
        13, 0, 17, 6);
}

void ItemMenu_SetMsgWin7(void)
{
    UiWindow_SetBounds(
        (struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window,
        13, 0, 17, 7);
}

void ItemMenu_SetMsgWin3(void)
{
    UiWindow_SetBounds(
        (struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window,
        13, 0, 17, 3);
}

#undef RenderOutput_ClearListFar

void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 GameFlag_TestFar(s32 flag);

#if EDITION_INTERNATIONAL
#define GIVE_CURSOR_X 128
#define GIVE_ASK_X    32
#define GIVE_ASK_Y    0
#else
#define GIVE_CURSOR_X 88
#define GIVE_ASK_X    0
#define GIVE_ASK_Y    32
#endif

s32 ItemMenu_SelectGiveQuantity(s32 base, s32 range, s32 single)
{
    s32 quantity = base;
    s32 slot;
    s32 changed;
    s32 other_count;
    s32 count;
    struct InventoryMenuState *menu = gMenuWork;
    void *tiles = Runtime_AllocateBlock(14, 0x400);
    s32 window;
    struct RenderOutput *object;

    changed = 1;
    other_count = 0;
    window = (s32)menu->message_window;
    ItemMenu_SetMsgWin7();
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
    if (single == 0)
        other_count = InventoryMenu_GetItemQuantity(menu->pane_owner[1], menu->selected_items[0] & 0x1ff);
    count = InventoryMenu_GetItemQuantity(menu->pane_owner[0], menu->selected_items[0] & 0x1ff);
    slot = Resource_FindFreeEntry();
    if (slot != 96) {
        VramBlock_LoadCached(slot, 256, 0);
        RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 48, 32);
        object = RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 80, 32);
        object->oam.index += 4;
        UiMenu_SlideCursor(GIVE_CURSOR_X, 40);
        while (!GameFlag_TestFar(0x150)) {
            if (changed) {
                changed = 0;
                quantity = (range + quantity) % range;
                RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
                UiText_DrawCharacterAtOffsetFar((s32)MsgGiveHowMany, window, GIVE_ASK_X, GIVE_ASK_Y);
                Dma_Set(Menu_QuantityTiles, tiles, 0x84000040, REG_DMA3);
                Shop_FillSelectorFar(30, 14, tiles);
                Shop_FillSelectorFar(range + base, 0, tiles);
                Shop_FillSelectorFar(base + quantity + 1, 10, tiles);
                Shop_FillSelectorFar(base, 2, tiles);
                VramBlock_LoadCached(slot, 256, tiles);
                UiText_DrawNumberInWindowFar(quantity + 1, 2, window, 32, 32);
                UiText_DrawCharacterAtOffsetFar((menu->selected_items[0] & 0x1ff) + (s32)MsgItemName,
                    window, 16, 8);
                UiText_DrawNumberInWindowFar(count - quantity - 1, 2, window, 16, 24);
#if EDITION_INTERNATIONAL
                if (single == 0)
                    UiText_DrawNumberInWindowFar(other_count + quantity + 1, 2, window, 80, 24);
#else
                UiText_DrawCharacterAtOffsetFar((s32)MsgItemCounter, window, 32, 24);
                if (single == 0) {
                    UiText_DrawNumberInWindowFar(other_count + quantity + 1, 2, window, 80, 24);
                    UiText_DrawCharacterAtOffsetFar((s32)MsgItemCounter, window, 96, 24);
                }
#endif
                UiText_DrawStringAtOffsetFar(UnitGetFar(menu->pane_owner[0])->name, window, 16, 16);
                if (single == 0)
                    UiText_DrawStringAtOffsetFar(UnitGetFar(menu->pane_owner[1])->name, window, 80, 16);
            }
            if (gKeyState & KEY_A) {
                AudioCommand_PlayFar(112);
                break;
            }
            if (gKeyState & KEY_B) {
                quantity = -1;
                AudioCommand_PlayFar(113);
                break;
            }
            UiMenu_PositionCursor(GIVE_CURSOR_X, 40);
            if (gKeysRepeat & KEY_LEFT) {
                quantity--;
                changed = 1;
                AudioCommand_PlayFar(111);
            }
            if (gKeysRepeat & KEY_RIGHT) {
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

#define UiText_DrawCharacterAtOffsetFar ((void (*)(s32, void *, s32, s32))UiText_DrawCharacterAtOffsetFar)

s32 __modsi3(s32, s32);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);

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
        UnitGetFar(menu->pane_owner[0])->name,
        (void *)menu->message_window,
        16,
        0);
    UiText_DrawCharacterAtOffsetFar(
        (menu->selected_items[0] & ITEM_ID_MASK) +
            (s32)MsgItemName,
        (void *)menu->message_window,
        16,
        8);
}

s32 ItemMenu_ConfirmDrop(s32 a0)
{
	void UiWork_FinalizeFar(s32, s32);
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
	UiText_DrawCharacterAtOffsetFar(slot + (s32)((u8 *)MsgItemName), win, 24, 0);
	text = (s32)MsgConfirmDrop;
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
	label = (s32)MsgYes;
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
	UiWork_FinalizeFar(win, 1);
	return sel;
}

s32 ItemMenu_ConfirmEquip(s32 unused)
{
    s32 sel = 0;
    s32 changed = 1;
    struct InventoryMenuState *menu = gMenuWork;
    struct BtlUnit *state = UnitGetFar(menu->pane_owner[1]);
    struct BtlUnit *saved;
    s32 win;
    s32 item;
    s32 owner;

    item = menu->selected_slots[1];
    owner = menu->pane_owner[1];
    ItemMenu_DrawEquipPreview(owner, item, 0, owner);
    saved = MemAlloc(BATTLE_UNIT_SIZE);
    Iwram_CopyWords(saved, state, BATTLE_UNIT_SIZE);
    win = (s32)menu->message_window;
    if ((u32)(Inventory_EquipFar(menu->pane_owner[1], menu->selected_slots[1]) + 2) <= 1) {
        sel = 1;
    } else {
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar((s32)MsgYes, (void *)win, 24, 24);
        UiText_DrawCharacterAtOffsetFar((s32)MsgYes + 1, (void *)win, 72, 24);
        UiWindow_ClearInteriorTilesFar(win, 16, 16, 96, 24);
        UiText_DrawCharacterAtOffsetFar((s32)MsgEquipThisItem, (void *)win, 0, 16);
        UiMenu_SlideCursor(110, 32);
#else
        UiText_DrawCharacterAtOffsetFar((s32)MsgYes, (void *)win, 96, 0);
        UiText_DrawCharacterAtOffsetFar((s32)MsgYes + 1, (void *)win, 96, 16);
        UiWindow_ClearInteriorTilesFar(win, 16, 16, 96, 24);
        UiText_DrawCharacterAtOffsetFar((s32)MsgEquipThisItem, (void *)win, 8, 16);
        UiMenu_SlideCursor(184, 5);
#endif
        for (;;) {
            if (GameFlag_TestFar(0x150))
                break;
            if (changed) {
                changed = 0;
                sel = __modsi3(sel + 2, 2);
            }
            if (gKeyState & KEY_A) {
                AudioCommand_PlayFar(175);
                break;
            }
            if (gKeyState & KEY_B) {
                AudioCommand_PlayFar(113);
                sel = 1;
                break;
            }
#if EDITION_INTERNATIONAL
            UiMenu_PositionCursor(sel * 48 + 110, 32);
            if (gKeysRepeat & KEY_LEFT) {
#else
            UiMenu_PositionCursor(184, (sel << 4) + 5);
            if (gKeysRepeat & KEY_UP) {
#endif
                sel--;
                changed = 1;
                AudioCommand_PlayFar(111);
            }
#if EDITION_INTERNATIONAL
            if (gKeysRepeat & KEY_RIGHT) {
#else
            if (gKeysRepeat & KEY_DOWN) {
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

#undef UiText_DrawCharacterAtOffsetFar

#undef gFrameCount

#define UiWindow_DrawDividerLineFar ((int (*)())UiWindow_DrawDividerLineFar)

s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state, s32 flags);

#undef RenderOutput_ClearListFar
#undef RenderOutput_ClearListFar
#define RenderOutput_ClearListFar ((void (*)(void *))RenderOutput_ClearListFar)
#define Render_SetTilemapFlagRect ((s32 (*)(s32, s32, s32, s32, s32, s32))Render_SetTilemapFlagRect)

void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused0, s32 unused1);
#ifndef MODERN // modern GCC keeps the headers' volatile declaration
#define gFrameCount (*(u32 (*))&gFrameCount)
#endif

#define LIST_PAGE_SIZE 5

void Resource_LoadPairedBlocks(void)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 value = Resource_FindFreeEntry();

    state->resource_slots[0] = value;
    VramBlock_LoadCached(value, 128, Menu_ArrowTiles);
    value = Resource_FindFreeEntry();
    state->resource_slots[1] = value;
    VramBlock_LoadCached(value, 128, Menu_SmallDownArrowTiles);
}

s32 ItemMenu_PageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    struct InventoryMenuState *menu = gMenuWork;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    limit = ItemMenu_Count(menu->pane_owner[index]);
    encoded = (s32)UnitGetFar(menu->pane_owner[index]);
    value = menu->selected_index_by_owner[menu->pane_owner[index]];
    if ((value + 1) > limit) {
        value = limit - 1;
    }
    quotient = value / 5;
    remainder = value % 5;
    groups = limit / 5;
    if (limit % 5 != 0) {
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
    s32 page;
    s32 combined;
    s32 row;

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
    if (menu->items[combined] != 0) {
        s32 masked = (menu->items[combined] & ITEM_ID_MASK) + (s32)MsgItemPlainName;
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar(masked, (s32)menu->info_window, 0, 0);
#else
        UiText_DrawResourceFar(masked, (s32)menu->info_window, 0, 0);
#endif
    }

    row = 0;
    do {
        if (row == state->row) {
            Render_SetTilemapFlagRect((s32)menu->item_window, 1, row * 2 + 1, 14, 1, 14);
        } else {
            Render_SetTilemapFlagRect((s32)menu->item_window, 1, row * 2 + 1, 14, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}

#if EDITION_INTERNATIONAL
#define PAGE_X  116
#define ENTRY_X 24
#else
#define PAGE_X  120
#define ENTRY_X 32
#endif

s32 ItemMenu_DrawNamePage(
    s32 window,
    s32 unused,
    const struct MenuResult *state)
{
    struct InventoryMenuState *menu =
        gMenuWork;
    u32 page;
    u32 first_entry;
    u32 visible_count;
    u8 row;
    const u16 *item_id;

    (void)unused;

    RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
    UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

    page = state->page;
    first_entry = page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Menu_SetPageIcons(5, first_entry, window, PAGE_X, 34);
    Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);

    row = 0;
    if (visible_count > row) {
        item_id = &menu->items[first_entry];
        do {
            UiText_DrawCharacterAtOffsetFar(
                (item_id[0] & ITEM_ID_MASK) + (s32)MsgItemName,
                (s32)menu->item_window,
                ENTRY_X,
                row * 16 + 8
            );
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
    s32 window;
    s32 changed;
    s32 nav;
    s32 tab;
    u8 i;
    s32 result;
    s32 redraw;
    s32 done;
    struct BtlUnit *owner;
    s32 prev;
    struct RenderOutput *icon;
    s32 work[5];
    struct MenuResult state;

    menu = gMenuWork;
    result = 0;
    prev = 0;
    menu->equip_preview = result;
    UiWindow_UpdateOrCreate((s32 *)&menu->list_window, 13, 3, 17, 14, 2);
    window = (s32)menu->list_window;
    done = 0;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        owner = UnitGetFar(menu->pane_owner[pane]);
        menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        menu->selected_item_icon->active = 13;
        ItemMenu_PageResult(&state, pane);
        UiMenu_PositionCursor(98, state.selected_index * 16 + 36);
        changed = 1;
        redraw = 1;

        while (GameFlag_TestFar(0x150) == 0) {
            UiMenu_PositionCursor(98, state.row * 16 + 36);

            if (changed != 0) {
                changed = 0;
                if (owner->inventory[prev] != 0) {
                    icon = menu->entry_icons[prev];
                    UiIcon_PrepareObject(icon);
                }
                if (redraw != 0) {
                    WaitFrames(1);
                    ItemMenu_DrawNamePage(window, 0, &state);
                    if (pane == 0) {
                        UiText_DrawCharacterAtOffsetFar((s32)MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
                    }
                    redraw = 0;
                }
                ItemMenu_DrawItemDetailPage(window, work, &state);
                menu->selected_items[pane] = menu->items[state.selected_index];
                ItemMenu_DrawEquipPreview(menu->pane_owner[pane], state.selected_index, 0, menu->pane_owner[pane]);
                if (menu->items[state.selected_index] != 0) {
                    icon = menu->entry_icons[state.selected_index];
                    icon->active = 9;
                    icon->anim_step = 0;
                    icon->sentinel = 250;
                }
                for (i = 0; i < menu->party_count; i++) {
                    AnimationObjects_SelectAnimationFar(menu->owner_objects[i], 1);
                }
            }

            if ((gFrameCount & 31) == 0) {
                for (i = 0; i < menu->party_count; i++) {
                    if (Item_CanOwnerEquip(menu->owner_ids[i],
                            ITEM_ID_MASK & menu->items[state.selected_index]) != 0) {
                        AnimationObjects_SelectAnimationFar(menu->owner_objects[i], 3);
                    }
                }
            }

            WaitFrames(1);
            prev = state.selected_index;
            nav = Menu_HandlePageInput(0, state.entry_count, LIST_PAGE_SIZE,
                &state.row, &state.page);
            if (nav == 1) {
                redraw = 1;
                changed = 1;
            }
            if (nav == 0) {
                changed = 1;
            }
            if (nav == -1) {
                changed = 0;
            }

            if ((gKeyState & KEY_A) != 0
                && menu->items[state.selected_index] != 0) {
                AudioCommand_PlayFar(173);
                result = menu->items[state.selected_index];
                done = 1;
                break;
            }
            if ((gKeyState & KEY_B) != 0) {
                AudioCommand_PlayFar(113);
                result = -1;
                done = 1;
                break;
            }
            if ((gKeysRepeat & KEY_R) != 0
                || (gKeysRepeat & KEY_L) != 0) {
                if (pane == 1) {
                    AudioCommand_PlayFar(114);
                    WaitFrames(1);
                } else {
                    AudioCommand_PlayFar(111);
                    menu->selected_index_by_owner[menu->pane_owner[pane]] =
                        state.selected_index;
                    tab = menu->pane_index[pane];
                    do {
                        if ((gKeysRepeat & KEY_R) != 0) {
                            tab++;
                        } else {
                            tab--;
                        }
                        tab = (tab + menu->party_count) % menu->party_count;
                        menu->selected_owner = menu->owner_ids[tab];
                        menu->pane_owner[pane] = menu->owner_ids[tab];
                        menu->pane_index[pane] = tab;
                        menu->item_count = ItemMenu_Collect(
                            UnitGetFar(menu->pane_owner[pane]), menu->items, 0);
                    } while (menu->item_count == 0);
                    for (i = 0; i <= 3; i++) {
                        menu->owner_y[i] = 30;
                    }
                    menu->owner_y[tab] = 26;
                    break;
                }
            }
        }
    }

    UiWindow_ClearInteriorTilesFar(window, 0, 88, 120, 96);
    UiIcon_PrepareObject(menu->grid_cursor);
    menu->selected_slots[pane] = state.selected_index;
    menu->selected_index_by_owner[menu->pane_owner[pane]] = state.selected_index;
    menu->selected_items[pane] = result;
    if (GameFlag_TestFar(0x150) != 0) {
        result = -1;
    }
    WaitFrames(1);
    return result;
}

#undef UiWindow_DrawDividerLineFar
#undef Render_SetTilemapFlagRect
#undef gFrameCount

#undef Link_DrawShiftedTilePairFar
#undef RenderOutput_ClearListFar

void UnitCalcFar(s32 unit_id);

#undef RenderOutput_ClearListFar
#undef RenderOutput_ClearListFar
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)
void ItemMenu_Init(s32, s32, s32, s32);
void ItemMenu_Close(void);

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define PROMPT_SAVES_TILES 1
#define PROMPT_TILES ((void *)0x06004000)
#define PROMPT_TILES_SIZE 0x2000

typedef s32 (*WordCopyFn_2)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

#endif

s32 Menu_OpenConfirmPrompt(void)
{
	s32 Party_ListActiveOwnersFar(void *ids);
#if defined(PROMPT_SAVES_TILES)
    void *saved = Runtime_BumpAllocateAlternatePool(PROMPT_TILES_SIZE);
#endif
    struct PsynergyMenuState *state = (struct PsynergyMenuState *)Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
    s32 high;
    s32 unused;
    s32 low;
    s32 result;

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
    result = Menu_ResolveSelectedAction(
        &high, &unused, &low);
    Menu_EnsureCancelSound();
    if (result == 1) {
        struct EventWork *target = WORK_SLOT(HEAP_SLOT_EVENT);
        u16 flags;
        BtlActGet(0x3fff & state->pane_action[0]);
        flags = low | (high << 10);
        target->psynergy_request = flags;
    }
    RenderOutput_ClearListFar(state->status_window);
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

void ItemMenu_DrawMsg(s32 unused, s32 message);

s32 Menu_ResolveSelectedAction(s32 *out_owner, s32 *unused, s32 *out_action)
{
    struct PsynergyMenuState *work;
    s32 result;
    s32 state;
    s32 done;
    s32 selection;
    s32 classification;
    s32 raw;
    s32 self_flag;
    struct BtlAct *action;

    state = 0;
    done = 0;
    work = gMenuWork;
    result = 0;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        switch (state) {
        case 0:
        {
            s32 cancelled;
            s32 chosen;

            work->pane_row[0] = 0;
            ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseOwner);
            chosen = PsynergyMenu_SelectPartySlot(0);
            cancelled = -1;
            /* FAKEMATCH: the redundant cancellation copy ends the CSE block,
             * preserving the native cancellation/done register choices.
             * Without it, r2/r3 exchange and 12 instructions differ.
             * XOR equality keeps the cancellation copy into the result. */
            if (chosen == -1)
                cancelled = chosen;
            if (((u32)chosen ^ (u32)cancelled) == 0) {
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
            if (work->psynergy_count != 0) {
                {
                    struct PsynergyMenuState *saved_work = work;

                    /* FAKEMATCH: the discarded conditional restoration
                     * ends the CSE block, so the mode offset is built from
                     * its own constant instead of adding 80 to the earlier
                     * pane offset. Both conditional arms disappear. */
                    if (done)
                        work = NULL;
                    if (done)
                        work = saved_work;
                }
                switch (work->mode) {
                case 0:
                    ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseAbility);
                    break;
                case 1:
                    ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseForL);
                    break;
                case 2:
                    ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseForR);
                    break;
                }
                ItemMenu_PosCategory();
                Menu_DrawOwnerStatusPanel(work->status_window, work->owner_ids[0], 0, 0);
                selection = PsynergyMenu_RunList(0);
                state = 0;
                if (selection != -1) {
                    state = 2;
                    if (work->mode != 0) {
                        if (work->mode == 1) {
                            PsynergyMenu_SetShortcut(
                                work->owner_ids[0], selection, 0);
                            RenderOutput_ClearListFar((s32)work->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)MsgShortcutSetL, -1, -1);
#if !EDITION_INTERNATIONAL
                            RenderOutput_RedrawSavedRectFar(work->info_window);
#endif
                        } else {
                            PsynergyMenu_SetShortcut(
                                work->owner_ids[0], selection, 1);
                            RenderOutput_ClearListFar((s32)work->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)MsgShortcutSetR, -1, -1);
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
            ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseTarget);
            self_flag = PsynergyMenu_SelectTarget(0);
            state = 4;
            if (self_flag == -1) {
                work->flags |= 1;
                state = 1;
            }
            break;

        case 2:
            classification = PsynergyMenu_ClassifySelectedPsynergy();
            if (classification == 1) {
                state = 3;
                break;
            }
            if (classification == 2) {
                work->owner_ids[1] = 9;
                state = 4;
                break;
            }
            done = 1;
            result = 1;
            *out_owner = work->owner_ids[0];
            *out_action = work->pane_action[0] & 0x3fff;
            break;

        case 4:
            self_flag = 0;
            raw = work->pane_action[0];
            result = BattleEffect_ApplyToTargets(
                raw, work->owner_ids[0], work->owner_ids[1], 0);
            if (work->owner_ids[1] == 9) {
                work->owner_ids[1] = work->owner_ids[0];
                self_flag = 9;
            }
            if (result != -1) {
                action = BtlActGet(work->pane_action[0] & 0x3fff);
                UnitEpAddFar(work->owner_ids[0], -action->pp_cost);
            }
            UnitCalcFar(work->owner_ids[0]);
            if (result != -1) {
                Menu_DrawOwnerStatusPanel(work->status_window, work->owner_ids[1], 0, 0);
                Ability_PlayUseAnimation(work->pane_action[0] & 0x3fff);
                RenderOutput_ClearListFar((s32)work->info_window);
                InventoryMenu_ShowModalMessage(
                    work->use_result + (s32)MsgItemUseResult, 0, -1);
            } else {
                AudioCommand_PlayFar(114);
                RenderOutput_ClearListFar((s32)work->info_window);
                InventoryMenu_ShowModalMessage(
                    work->use_result + (s32)MsgItemUseResult,
                    result,
                    result);
            }
            if (result != -1) {
                result = 1;
                work->flags |= 1;
                state = 1;
            } else {
                work->skip_slide = 1;
                if (self_flag == 9) {
                    work->flags |= 1;
                    state = 1;
                } else {
                    state = 3;
                }
            }
            break;

        default:
            done = 1;
            break;
        }
    }

    if (GameFlag_TestFar(0x150) != 0) {
        result = -1;
    }
    return result;
}

s32 PsynergyMenu_ClassifySelectedPsynergy(void)
{
    struct BtlAct *psynergy;
    s32 diff;
    s32 ret;

    psynergy = BtlActGet(
        0x3fff & ((struct PsynergyMenuState *)gMenuWork)->pane_action[0]);
    if (BattleFx_HasMatchingEvent5Far(psynergy->type_0c) != 0) {
        return 0;
    }
    ret = 2;
    if (psynergy->rng != 0xff) {
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
    s32 tab_offset = party_slot + ((u8 *)menu->tab_index - (u8 *)menu);
    s32 icon_offset = party_slot * sizeof(menu->pane_icon[0]) +
                      ((u8 *)menu->pane_icon - (u8 *)menu);
    struct RenderOutput *icon;
    u8 byte_val;
    s32 tab;
    s32 owner_offset;
    s32 table_offset;
    s32 owner_id;
    struct BtlUnit *owner;
    u16 *actions;
    u8 *counts;
    s32 badge;
    s32 result;
    s32 icon_offset2;

    result = 0;
    icon = *(struct RenderOutput **)((u8 *)menu + icon_offset);
    icon->active = 1;
    icon->anim_step = result;
    menu->cursor_icon->active = 13;
    counts = (u8 *)menu + ((u8 *)menu->tab_counts - (u8 *)menu->tab_index);
    tab = *(s8 *)((u8 *)menu + tab_offset);
    byte_val = menu->owner_count;
    counts[tab_offset] = byte_val;

    if (tab == -1) {
        *((u8 *)menu + tab_offset) = 0;
        owner_offset = 0;
    } else {
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

    icon_offset2 = party_slot * sizeof(menu->pane_icon[0]) +
                   ((u8 *)menu->pane_icon - (u8 *)menu);
    icon = *(struct RenderOutput **)((u8 *)menu + icon_offset2);
    UiIcon_PrepareObject(icon);
    WaitFrames(1);
    return result;
}

#undef UiText_DrawStringInWindowFar

#define GameFlag_ClearBitFar ((int (*)())GameFlag_ClearBitFar)

#undef UiText_GetResourceDimensionsFar
#define UiText_GetResourceDimensionsFar ((s32 (*)(s32,s32*,s32*,s32*,s32*))UiText_GetResourceDimensionsFar)
#undef UiText_DrawStringInWindowFar
#undef UiText_DrawStringInWindowFar
#define UiText_DrawStringInWindowFar ((void (*)(const u8*,s32*,s32,s32))UiText_DrawStringInWindowFar)
#define Menu_SpawnIconEntries ((void (*)(struct PsynergyMenuState *menu, s32 window))Menu_SpawnIconEntries)
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 mode);

s32 PsynergyMenu_SetupActionIcons(u16 *owner_ids, u16 *unused)
{
    /* FAKEMATCH: the low-byte CollectActions result keeps adds r5,r0,#0; explicit narrowing of s32 selects mov r5,r0. */
    struct PsynergyMenuState *menu;
    s32 selection;
    s32 count;
    s32 pending;
    s32 result;
    struct BtlUnit *owner;
    s32 window;
    s32 shown;
    s32 i;
    u16 *actions;
    s32 action_count;
    u8 found;

    menu = (struct PsynergyMenuState *)gMenuWork;
    selection = menu->tab_index[0];
    count = (s8)menu->tab_counts[0];
    pending = 1;
    result = 0;
    shown = 0;
    menu->mode = result;
    owner = UnitGetFar(owner_ids[selection]);
    if (UiWindow_UpdateOrCreate(&menu->psynergy_window, 13, 3, 17, 10, 2))
        Menu_SpawnIconEntries(menu, menu->psynergy_window);
    if (UiWindow_UpdateOrCreate(&menu->shortcut_window, 13, 13, 17, 4, 2)) {
        menu->cursor_icon = RenderOutput_CreateFromResourceFar(2, 0, (struct RenderInput *)menu->shortcut_window, 0, result);
        menu->cursor_icon->active = 13;
    }
    while (!GameFlag_TestFar(0x150)) {
        if (pending) {
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
            if (!GameFlag_TestFar(0x151) && !shown) {
                RenderOutput_ClearListFar((s32)menu->info_window);
#if EDITION_INTERNATIONAL
                RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
#endif
                UiText_DrawWorkValueWithLabel((s32)menu->info_window);
                shown = 1;
            } else {
                GameFlag_ClearBitFar(0x151);
            }
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            if (menu->psynergy_count) {
                AudioCommand_PlayFar(112);
                result = owner_ids[selection];
                break;
            }
            AudioCommand_PlayFar(114);
        }
        if ((gKeyState & KEY_L) || (gKeyState & KEY_R)) {
            result = owner_ids[selection];
            if (gKeyState & KEY_L)
                menu->mode = 1;
            else
                menu->mode = 2;
            actions = MemAlloc(64);
            found = PsynergyMenu_CollectActions((struct BtlUnit *)owner, actions, 1);
            MemFree(actions);
            action_count = (s8)found;
            if (action_count == 0) {
                menu->mode = action_count;
                AudioCommand_PlayFar(114);
            } else {
                AudioCommand_PlayFar(112);
                break;
            }
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        if (gKeysRepeat & KEY_LEFT) {
            AudioCommand_PlayFar(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & KEY_RIGHT) {
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
#undef UiText_DrawCharacterAtOffsetFar
#undef UiText_DrawCharacterAtOffsetFar
#define UiText_DrawCharacterAtOffsetFar ((void (*)(s32 message, s32 *, s32 x, s32 y))UiText_DrawCharacterAtOffsetFar)

void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id)
{
    u16 *psynergies;
    struct PsynergyMenuState *menu;
    struct BtlUnit *owner;

    menu = gMenuWork;
    owner = (struct BtlUnit *)UnitGetFar(owner_id);
    psynergies = menu->psynergies;
    menu->psynergy_count =
        PsynergyMenu_CollectActions(owner, psynergies, 2);
    RenderOutput_RedrawSavedRectFar(menu->psynergy_window);
    PsynergyMenu_RefreshOwnerEntries(0x6c, 0x20, 8);
    PsynergyMenu_DrawPsynergyIcons(psynergies);
    if (menu->psynergy_count == 0) {
        UiText_DrawCharacterAtOffsetFar(
            (s32)MsgPsynergyMenuEmpty,
            (s32 *)menu->psynergy_window,
            0,
            0x18);
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
    s32 selection;
    s32 count;
    s32 pending;
    s32 result;
    s32 shown;
    struct RenderOutput *marker;

    menu = ((struct PsynergyMenuState *)gMenuWork);
    selection = menu->tab_index[1];
    count = menu->owner_count;
    pending = 1;
    result = 0;
    shown = 0;
    UnitGetFar(menu->owner_table[menu->tab_index[0]]);
    UiMenu_SlideCursor(selection * 24 - 10, 16);
    while (!GameFlag_TestFar(0x150)) {
        if (pending) {
            pending = 0;
            selection = (selection + count) % count;
            UnitGetFar(menu->owner_table[selection]);
            marker = menu->pane_icon[1];
            marker->oam.x = marker->x =
                ((((struct RenderInput *)menu->auxiliary_window)->x + selection * 3) << 3) - 2;
            if (mode == 0) {
                Menu_DrawOwnerStatusPanel(menu->status_window,
                                          menu->owner_table[selection], 0, 0);
                PsynergyMenu_CallIconRoutineWithValue(
                    (s32)menu, menu->owner_table[selection]);
                if (!GameFlag_TestFar(0x151) && !shown) {
#if EDITION_INTERNATIONAL
                    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
                    UiText_DrawCharacterAtOffsetFar(
                        (menu->pane_action[0] & 0x3fff) + (s32)MsgAbilityDescription,
                        (s32)menu->info_window, 0, 0);
#else
                    RenderOutput_ClearListFar((s32)menu->info_window);
                    UiText_DrawResourceFar(
                        (menu->pane_action[0] & 0x3fff) + (s32)MsgAbilityDescription,
                        menu->info_window, 0, 0);
#endif
                    shown = 1;
                } else {
                    GameFlag_ClearBitFar(0x151);
                }
            }
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            AudioCommand_PlayFar(112);
            result = menu->owner_table[selection];
            break;
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        if (gKeysRepeat & KEY_LEFT) {
            AudioCommand_PlayFar(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & KEY_RIGHT) {
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
    s32 id = psynergy & 0x3fff;
    s32 code =
        (owner << 10) | id;

    if (shortcut == 0) {
        gGameState.first_shortcut = code;
    } else {
        gGameState.second_shortcut = code;
    }
    return 1;
}

#if EDITION_INTERNATIONAL

#define UiWork_PushValueSlotFar ((void (*)(s32 value, s32 slot))UiWork_PushValueSlotFar)

s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner)
{
    s32 left;
    s32 top;
    s32 width;
    s32 height;
    s32 wide;

    if (gGameState.first_shortcut != 0 &&
        gGameState.second_shortcut != 0)
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutChangeHelp, (s32 *)window, 0, -8);
    else
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutLabel, (s32 *)window, 0, -8);

    UiText_GetResourceDimensionsFar(
        (gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName,
        &left, &top, &width, &height);
    if (width < 0 || width > 10)
        wide = 1;
    else
        wide = 0;
    if (gGameState.first_shortcut != 0) {
        UiWork_PushValueSlotFar(gGameState.first_shortcut & 0x3ff, 4);
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutNameL, (s32 *)window, 0, 0);
        if (wide == 0)
            UiText_DrawStringAtOffsetFar(
                UnitGetFar(gGameState.first_shortcut >> 10),
                (s32 *)window, 80, 0);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyL, (s32 *)window, 0, 0);
    }

    UiText_GetResourceDimensionsFar(
        (gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName,
        &left, &top, &width, &height);
    if (width < 0 || width > 10)
        wide = 1;
    else
        wide = 0;
    if (gGameState.second_shortcut != 0) {
        UiWork_PushValueSlotFar(gGameState.second_shortcut & 0x3ff, 4);
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutNameR, (s32 *)window, 0, 8);
        if (wide == 0)
            UiText_DrawStringAtOffsetFar(
                UnitGetFar(gGameState.second_shortcut >> 10),
                (s32 *)window, 80, 8);
        UiWork_SetParamNibbleFar(15);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyR, (s32 *)window, 0, 8);
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

    if (gGameState.first_shortcut != 0 &&
        gGameState.second_shortcut != 0)
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutChangeHelp, (s32 *)window, 0, -8);
    else
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutLabel, (s32 *)window, 0, -8);

    UiText_GetResourceDimensionsFar(
        (gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName,
        &left, &top, &width, &height);
    if (width < 0 || width > 11)
        wide = 1;
    else
        wide = 0;
    if (wide)
        UiText_DrawStringInWindowFar(Menu_ShortcutLString, (s32 *)window, 0, 0);
    else
        UiText_DrawStringInWindowFar(Menu_ShortcutLColonString, (s32 *)window, 0, 0);
    if (gGameState.first_shortcut != 0) {
        if (wide)
            UiText_DrawCharacterAtOffsetFar(
                (gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName,
                (s32 *)window, 8, 0);
        else
            UiText_DrawCharacterAtOffsetFar(
                (gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName,
                (s32 *)window, 16, 0);
        UiText_DrawStringAtOffsetFar(
            UnitGetFar(gGameState.first_shortcut >> 10),
            (s32 *)window, 80, 0);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyL, (s32 *)window, 24, 0);
    }

    UiText_GetResourceDimensionsFar(
        (gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName,
        &left, &top, &width, &height);
    if (width < 0 || width > 11)
        wide = 1;
    else
        wide = 0;
    if (wide)
        UiText_DrawStringInWindowFar(Menu_ShortcutRString, (s32 *)window, 0, 8);
    else
        UiText_DrawStringInWindowFar(Menu_ShortcutRColonString, (s32 *)window, 0, 8);
    if (gGameState.second_shortcut != 0) {
        if (wide)
            UiText_DrawCharacterAtOffsetFar(
                (gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName,
                (s32 *)window, 8, 8);
        else
            UiText_DrawCharacterAtOffsetFar(
                (gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName,
                (s32 *)window, 16, 8);
        UiText_DrawStringAtOffsetFar(
            UnitGetFar(gGameState.second_shortcut >> 10),
            (s32 *)window, 80, 8);
        UiWork_SetParamNibbleFar(15);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyR, (s32 *)window, 24, 8);
    }
    return 1;
}
#endif

#undef GameFlag_ClearBitFar
#undef Menu_SpawnIconEntries
#undef UiText_DrawCharacterAtOffsetFar
#undef UiWork_PushValueSlotFar

#define SideObject_CreateFar ((void * (*)(s32,s32,s32,s32,s32,s32))SideObject_CreateFar)

void PsynergyMenu_CreateEntryGrid(void)
{
    s32 window;
    struct PsynergyMenuState *menu;
    struct RenderOutput *cursor;
    s32 index;
    s32 x;
    s32 y;
    struct RenderOutput **output;

    menu = gMenuWork;
    window = UiMenu_CreateCursor(menu);
    PsynergyMenu_InitializeEntryObjects(window, 2, 2, 8, 0);

    window = UiWindow_CreateFar(0, 5, 30, 15, 2);
    menu->psynergy_window = window;
    menu->selected_column = 0;
    menu->selected_row = 0;
    menu->column_count = 8;
    menu->row_count = 2;

    cursor = (struct RenderOutput *)UiIcon_CreateWithResourceVariant(
        (struct RenderInput *)window, 0, 4);
    cursor->active = 13;
    menu->entry_grid_cursor = cursor;
    SideObject_CreateFar(0, 0, 0, window, 0, 0);

    y = 8;
    index = 0;
    output = &menu->entry_icons[0];
    x = 96;
    do {
        *output++ = RenderOutput_CreateFromResourceFar(4, index, (struct RenderInput *)window, x, y);
        index++;
        x += 16;
    } while (index <= 7);

    index = 8;
    y = 24;
    output = &menu->entry_icons[8];
    x = 96;
    do {
        *output++ = RenderOutput_CreateFromResourceFar(4, index, (struct RenderInput *)window, x, y);
        index++;
        x += 16;
    } while (index <= 15);
}

void PsynergyMenu_CloseWindows(void)
{
	void UiWork_FinalizeFar(s32 window, s32 mode);
    struct PsynergyMenuState *menu;

    menu = gMenuWork;
    Menu_ReleaseEntryObjects();
    UiWork_FinalizeFar(menu->auxiliary_window, 1);
    UiWork_FinalizeFar(menu->psynergy_window, 1);
    UiWork_FinalizeFar(menu->message_window, 1);
}
#define Resource_LoadByModeIntoSlotFar ((void (*)(s32 style, u16 action, u8 target, s32 flags))Resource_LoadByModeIntoSlotFar)

void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies)
{
    s32 remaining;
    struct RenderOutput **icons;
    u16 *p;
    s32 psynergy_id;

    icons =
        ((struct PsynergyMenuState *)gMenuWork)->entry_icons;
    p = psynergies;
    remaining = 31;
    do {
        psynergy_id = *p++;
        if (psynergy_id != 0) {
            Resource_LoadByModeIntoSlotFar(
                4, psynergy_id, (*icons)->index, 0);
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(psynergies);
}

#undef Resource_LoadByModeIntoSlotFar

#define ACTION_ID_MASK 0x3FFF

s32 PsynergyMenu_CollectActions(struct BtlUnit *owner, u16 *actions, s32 mode)
{
    /* FAKEMATCH: the doubled zero store through the byte-offset cursor makes loop.c replace the counter with the pointer (274 bytes); a single indexed clear keeps the counter (266). */
    s32 n;
    s32 passes;
    s32 count;
    s32 i;
    s32 j;

    passes = (mode != 2) ? 4 : 3;
    for (n = 62; n >= 0; n -= 2) {
        u16 *q = (u16 *)((u8 *)actions + n);
        *q = 0;
        *q = 0;
    }
    count = 0;

    if (mode == 1) {
        for (i = 0; i < 32; i++) {
            if (owner->action_slots[i].encoded_action != 0) {
                if (BtlActGet(owner->action_slots[i].encoded_action & ACTION_ID_MASK)->type_0c != 0) {
                    actions[count] = owner->action_slots[i].encoded_action;
                    count++;
                }
            }
        }
    } else {
        for (j = 0; j < passes; j++) {
            for (i = 0; i < 32; i++) {
                if (owner->action_slots[i].encoded_action != 0) {
                    struct BtlAct *ability = BtlActGet(owner->action_slots[i].encoded_action & ACTION_ID_MASK);

                    if (j == 0 && (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0)) {
                        actions[count] = owner->action_slots[i].encoded_action;
                        count++;
                    } else if (j == 1) {
                    } else if (j == 2) {
                    } else if (j == 3 && ability->type_0c == 0 && (ability->tgt_flg & 0x40) == 0) {
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
    s32 owner_state;
    struct PsynergyMenuState *menu = gMenuWork;
    s32 entry_count;
    s32 row;
    s32 page;
    s32 page_count;
    s32 selected_index;
    s32 owner;

    owner_state = (s32)UnitGetFar(menu->owner_ids[index]);
    entry_count = menu->psynergy_count;
    owner = menu->owner_ids[index];
    selected_index = menu->selected_index_by_owner[owner];
    if ((selected_index + 1) > entry_count) {
        selected_index = entry_count - 1;
    }
    page = selected_index / 5;
    row = selected_index % 5;
    page_count = entry_count / 5;
    if (entry_count % 5 != 0) {
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

#undef RenderOutput_ClearListFar
#undef RenderOutput_RedrawSavedRectFar

#if EDITION_INTERNATIONAL
#define DETAIL_ROW_X     0
#define DETAIL_ROW_WIDTH 15
#else
#define DETAIL_ROW_X     1
#define DETAIL_ROW_WIDTH 14
#endif

s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work, struct MenuResult *state)
{
    struct PsynergyMenuState *menu;
    s32 combined;
    s32 row;

    menu = gMenuWork;
    combined = state->page * 5;
    combined += state->row;
    state->selected_index = combined;

    if (GameFlag_TestFar(0x151) == 0) {
#if EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(menu->info_window);
#else
        RenderOutput_ClearListFar(menu->info_window);
#endif
        WaitFrames(1);

        combined = state->selected_index;
        if (menu->psynergies[combined] != 0) {
            s32 masked = (menu->psynergies[combined] & 0x1ff) + (s32)MsgAbilityDescription;
#if EDITION_INTERNATIONAL
            UiText_DrawCharacterAtOffsetFar(masked, (s32)menu->info_window, 0, 0);
#else
            UiText_DrawResourceFar(masked, (s32)menu->info_window, 0, 0);
#endif
        }
    } else {
        GameFlag_ClearBitFar(0x2ff);
    }

    row = 0;
    do {
        if (row == state->row) {
            Render_SetTilemapFlagRect(menu->psynergy_window, DETAIL_ROW_X, row * 2 + 1, DETAIL_ROW_WIDTH, 1, 14);
        } else {
            Render_SetTilemapFlagRect(menu->psynergy_window, DETAIL_ROW_X, row * 2 + 1, DETAIL_ROW_WIDTH, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}

#undef GameFlag_ClearBitFar
#undef Render_SetTilemapFlagRect

#undef ACTION_ID_MASK

struct BtlAct * BtlActGet(s32 action_id);

struct AnimationObject;
#undef ACTION_ID_MASK
#undef ACTION_ID_MASK
#define ACTION_ID_MASK 0x3fff

s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state)
{
    u32 first_entry;
    u32 visible_count;
    u8 row;
    s32 cursor;
    struct BtlUnit *owner;
    struct BtlAct *ability;
    struct PsynergyMenuState *menu = gMenuWork;

    (void)unused;

    RenderOutput_RedrawSavedRectFar(window);
    UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

    if (2 & menu->flags) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutHelp, window, 0, 88);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
    }

    first_entry = state->page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

#if EDITION_INTERNATIONAL
    Menu_SetPageIcons(5, first_entry, window, 0x70, 0x22);
#else
    Menu_SetPageIcons(5, first_entry, window, 0x78, 0x22);
#endif
    Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, 0x60, 0);
#else
    UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, 0x48, 0);
#endif

    row = 0;
    if (visible_count > row) {
        cursor = first_entry;
        do {
            owner = UnitGetFar(menu->owner_ids[0]);
            ability = BtlActGet(0x3fff & menu->psynergies[cursor]);

            if (ability->pp_cost > owner->ep) {
                UiWork_SetParamNibbleFar(2);
            } else if (PsynergyMenu_IsActionRestricted(0x3fff & menu->psynergies[cursor]) != 0) {
                UiWork_SetParamNibbleFar(4);
            } else {
                UiWork_SetParamNibbleFar(15);
            }

            UiText_DrawCharacterAtOffsetFar(
                (0x3fff & menu->psynergies[cursor]) + (s32)MsgAbilityName,
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
    struct BtlAct *ability;
    struct RenderOutput *icon;
    s32 window;
    s32 changed;
    s32 nav;
    s8 mode;
    s32 tab;
    u8 i;
    s32 result;
    s32 redraw;
    s32 done;
    struct BtlUnit *owner;
    s32 prev;
    s32 prompt;
    s32 work[5];
    struct MenuResult state;

    menu = ((struct PsynergyMenuState *)gMenuWork);
    result = 0;
    prev = 0;
    prompt = 0;
    menu->pane_icon[pane]->active = 13;
    UiWindow_UpdateOrCreate(&menu->list_window, 13, 3, 17, 14, 2);
    window = menu->list_window;
    done = 0;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        owner = UnitGetFar(menu->owner_ids[pane]);
        if (menu->mode != 0) {
            menu->psynergy_count =
                PsynergyMenu_CollectActions(owner, menu->psynergies, 1);
        } else {
            menu->psynergy_count =
                PsynergyMenu_CollectActions(owner, menu->psynergies, 2);
        }
        PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
        PsynergyMenu_BuildPageResult(&state, pane);
        redraw = 1;
        changed = 1;
        menu->pane_icon[pane]->active = 1;

        while (GameFlag_TestFar(0x150) == 0) {
#if EDITION_INTERNATIONAL
            UiMenu_PositionCursor(88, state.row * 16 + 36);
#else
            UiMenu_PositionCursor(98, state.row * 16 + 36);
#endif

            if (changed != 0) {
                changed = 0;
                if (menu->psynergies[prev] != 0) {
                    icon = menu->entry_icons[prev];
                    UiIcon_PrepareObject(icon);
                }
                if (redraw != 0) {
                    redraw = 0;
                    WaitFrames(1);
                    PsynergyMenu_DrawActionPage(window, 0, &state);
                }
                PsynergyMenu_DrawDetailPage(window, work, &state);
                menu->pane_action[pane] =
                    menu->psynergies[state.selected_index];
                menu->cursor_icon->active = 13;
                if (menu->psynergies[state.selected_index] != 0) {
                    icon = menu->entry_icons[state.selected_index];
                    icon->active = 9;
                    icon->anim_step = 0;
                    icon->sentinel = 250;
                }
                for (i = 0; i < menu->owner_count; i++) {
                    AnimationObjects_SelectAnimationFar(menu->tab_objects[i], 1);
                }
            }

            WaitFrames(1);
            prev = state.selected_index;

            if ((gKeysHeld & KEY_SELECT) == 0) {
                nav = Menu_HandlePageInput(
                    0, state.entry_count, LIST_PAGE_SIZE,
                    &state.row, &state.page);
            } else {
                nav = -1;
            }
            if (nav == 1) {
                redraw = 1;
                changed = 1;
            }
            if (nav == 0) {
                changed = 1;
            }
            if (nav == -1) {
                changed = 0;
            }

            if (menu->mode == 0) {
                if ((gKeyState & KEY_SELECT) != 0 && prompt == 0) {
                    ability = BtlActGet(
                        menu->psynergies[state.selected_index] &
                        ACTION_ID_MASK);
                    if (ability->type_0c == 0) {
                        AudioCommand_PlayFar(114);
                    } else {
                        AudioCommand_PlayFar(174);
                        prompt = 1;
                        menu->flags |= 2;
                        UiWindow_ClearInteriorTilesFar(window, 0, 88, 120, 96);
                        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutHelp, window, 0, 88);
                    }
                }
                if ((gKeysHeld & KEY_SELECT) == 0 && prompt == 1) {
                    prompt = 0;
                    menu->flags &= 0xfffd;
                    UiWindow_ClearInteriorTilesFar(window, 0, 88, 120, 96);
                    UiText_DrawCharacterAtOffsetFar((s32)MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
                }
            }

            if ((gKeyState & KEY_A) != 0) {
                if (menu->mode != 0) {
                    AudioCommand_PlayFar(130);
                    result = menu->psynergies[state.selected_index];
                    done = 1;
                    break;
                }
                if (menu->psynergies[state.selected_index] == 0) {
                    goto no_accept;
                }
                if (PsynergyMenu_IsActionRestricted(
                        menu->psynergies[state.selected_index]) != 0) {
                    AudioCommand_PlayFar(114);
                    continue;
                }
                ability = BtlActGet(
                    menu->psynergies[state.selected_index] & ACTION_ID_MASK);
                if (ability->pp_cost > owner->ep) {
                    AudioCommand_PlayFar(114);
                    goto no_accept;
                }
                AudioCommand_PlayFar(173);
                result = menu->psynergies[state.selected_index];
                done = 1;
                break;
            }

        no_accept:
            if ((gKeyState & KEY_B) != 0) {
                AudioCommand_PlayFar(113);
                result = -1;
                done = 1;
                break;
            }

            if (((gKeysRepeat & KEY_R) != 0 ||
                 (gKeysRepeat & KEY_L) != 0) &&
                (gKeysHeld & KEY_SELECT) == 0) {
                mode = menu->mode != 0 ? 1 : 2;
                AudioCommand_PlayFar(111);
                menu->selected_index_by_owner[menu->owner_ids[pane]] =
                    state.selected_index;
                tab = menu->tab_index[pane];
                do {
                    if ((gKeysRepeat & KEY_R) != 0) {
                        tab = tab + 1;
                    } else {
                        tab = tab - 1;
                    }
                    tab = (tab + menu->owner_count) % menu->owner_count;
                    menu->selected_owner = menu->owner_table[tab];
                    menu->owner_ids[0] = menu->owner_table[tab];
                    menu->psynergy_count = PsynergyMenu_CollectActions(
                        UnitGetFar(menu->owner_ids[0]),
                        menu->psynergies, mode);
                } while (menu->psynergy_count == 0);
                menu->tab_index[pane] = tab;
                for (i = 0; i < 4; i++) {
                    menu->row_positions[i] = 30;
                }
                menu->row_positions[tab] = 26;
                Menu_DrawOwnerStatusPanel(menu->status_window, menu->owner_table[tab], 0, 0);
                PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_table[tab]);
                break;
            }

            if ((gKeyState & KEY_L) != 0 &&
                (gKeysHeld & KEY_SELECT) != 0) {
                ability = BtlActGet(
                    menu->psynergies[state.selected_index] & ACTION_ID_MASK);
                if (ability->type_0c == 0) {
                    AudioCommand_PlayFar(114);
                } else {
                    AudioCommand_PlayFar(130);
                    if (PsynergyMenu_SetShortcut(
                            menu->owner_ids[pane],
                            menu->psynergies[state.selected_index],
                            0) != 0) {
                        result = menu->psynergies[state.selected_index];
                        menu->mode = 1;
                        done = 1;
                        break;
                    }
                }
            }

            if ((gKeyState & KEY_R) != 0 &&
                (gKeysHeld & KEY_SELECT) != 0) {
                ability = BtlActGet(
                    menu->psynergies[state.selected_index] & ACTION_ID_MASK);
                if (ability->type_0c == 0) {
                    AudioCommand_PlayFar(114);
                } else {
                    if (PsynergyMenu_SetShortcut(
                            menu->owner_ids[pane],
                            menu->psynergies[state.selected_index],
                            1) != 0) {
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
    menu->selected_index_by_owner[menu->owner_ids[pane]] =
        state.selected_index;
    menu->pane_action[pane] = result;
    if (GameFlag_TestFar(0x150) != 0) {
        result = -1;
    }
    WaitFrames(1);
    return result;
}

s32 PsynergyMenu_IsActionRestricted(s32 no)
{
    struct BtlAct *action = BtlActGet((u32)(no << 18) >> 18);
    u32 flags;

    if (action->type_0c != 0)
        goto restricted;
    flags = action->tgt_flg & 0xc0;
    no = 1;
    if (flags != 0xc0)
        goto done;
restricted:
    no = 0;
done:
    return no;
}

#undef UiWindow_DrawDividerLineFar
