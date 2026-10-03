#include "CHARACTER_MENU.H"
#include "INVENTORY_MENU.H"
#include "BATTLE_UNIT.H"
#include "ITEM.H"
#include "EDITION.H"
#include "TYPES.H"

/* Where the name and ailment labels start: the Japanese edition moves them right. */
#if EDITION_INTERNATIONAL
#define STATUS_X 32
#else
#define STATUS_X 40
#endif

/* The status panel beside the item and ability lists: the owner's name,
   ailments or level, then a page chosen by the low byte of mode (class and
   stats, the stat change of equipping the selected item, whether its
   Psynergy is already known, or the four current stats). Bit 8 redraws the
   page without the window, name and side portrait. */

extern u8 IwramCopyWords[];
extern u8 Data_080af20c[];
extern u8 MsgClassName[], MsgAbilityName[];
extern u8 MsgPoisonLabel[], MsgVenomLabel[], MsgCurseLabel[], MsgHauntLabel[];
extern u8 MsgExpLabel[], MsgCannotEquip[], MsgWillLearn[], MsgLearned[];
extern u8 MsgPanelStatLabel[];

struct BattleUnit *Owner_GetStateFar(s32 owner);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void WaitFrames(s32 frames);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(const void *text, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void RenderOutput_ClearListFar(s32 window);
struct RenderOutput *SideObject_CreateFar(s32 owner, s32 position, s32 side, s32 window, s32 x, s32 y);
void Ui_DrawValuePairRows(struct BattleUnit *unit, s32 window);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
void *Runtime_BumpAllocate(s32 size);
s32 _call_via_r3(void *, void *, s32, void *);
void Inventory_EquipFar(s32 owner, s32 slot);
void Owner_RecalculateStatsFar(s32 owner);
void UiText_DrawStatComparison(const struct BattleUnit *unit, const struct BattleUnit *backup, s32 window);
void Runtime_BumpFree(void *block);

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
    struct BattleUnit *unit;
    s32 window;
    s32 cnt;
    s32 value;
    u8 avail[8];

    created = 0;
    state = gMenuWork;
    unit = Owner_GetStateFar(owner);
    item = unit->inventory[slot];
    def = Item_Get(item & 0x1ff);
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
            UiText_DrawStringAtOffsetFar(Data_080af20c, window, 40, 16);
#else
            UiText_DrawStringInWindowFar(Data_080af20c, window, 40, 16);
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
        backup = Runtime_BumpAllocate(0x14c);
        Owner_Copy(backup, unit);
        if (state->equip_preview)
            unit->inventory[slot] &= 0xfdff;
        else
            Inventory_EquipFar(owner, slot);
        Owner_RecalculateStatsFar(owner);
        UiText_DrawStatComparison(unit, backup, window);
        Owner_Copy(unit, backup);
        Runtime_BumpFree(backup);
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
        /* The Spanish verdict sits above the Psynergy's name. */
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
        value = unit->attack;
        UiText_DrawNumberInWindowFar(value, 3, window, 64, 40);
        UiText_DrawCharacterAtOffsetFar((s32)base + 1, window, 0, 48);
        value = unit->defense;
        UiText_DrawNumberInWindowFar(value, 3, window, 64, 48);
        UiText_DrawCharacterAtOffsetFar((s32)base + 4, window, 0, 56);
        value = unit->agility;
        UiText_DrawNumberInWindowFar(value, 3, window, 64, 56);
        UiText_DrawCharacterAtOffsetFar((s32)base + 3, window, 0, 64);
        value = unit->luck;
        UiText_DrawNumberInWindowFar(value, 3, window, 64, 64);
        break;
    }
    }
}
