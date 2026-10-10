#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "character_menu.h"
#include "inventory_menu.h"
#include "battle_unit.h"
#include "item.h"
#include "edition.h"
#include "types.h"
#include "btlsys.h"
#include "ui.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "vram_block.h"
#include "render_input.h"
#include "fixed_math.h"

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

struct UiIconAttributes {
    u8 y;
    u8 affine_mode : 2;
    u8 flags : 6;
    u16 x : 9;
    u16 matrix : 5;
    u16 size : 2;
};

void UiIcon_PrepareObject(struct RenderOutput *object)
{
    if (object != NULL) {
        object->active = 1;
        ((struct UiIconAttributes *)&object->packed)->x = object->x;
        ((struct UiIconAttributes *)&object->packed)->y = object->y;
        ((struct UiIconAttributes *)&object->packed)->matrix = 0;
        ((struct UiIconAttributes *)&object->packed)->affine_mode = 0;
    }
}
