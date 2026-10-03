#include "EDITION.H"
#include "TYPES.H"
#include "INVENTORY_MENU.H"
#include "ITEM.H"
#include "TBS_EDITION.H"
#include "BATTLE_RUNTIME.H"

#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1
s32 Item_ClassifyUseMode(s32 owner, s32 item);
s32 BattleFx_HasTriggerFar(s32 item);
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
extern char MsgItemCommandUse;


void ItemMenu_BuildCmd(s8 *command_states)
{
    struct InventoryMenuState *menu;
    struct ItemDefinition *item;

    menu = gMenuWork;
    item = Item_Get(ITEM_ID_MASK & menu->selected_items[0]);

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

    message = (u32)&MsgItemCommandUse;
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
    def = Item_Get(masked);

    if (BattleFx_HasTriggerFar(masked)!= 0) {
        return 0;
    }

    {
        struct BattleAction *action = BattleAction_Get(def->action_id & 0x3fff);

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
                u8 flags = action->target_flags;

                if ((flags & 0x40) != 0) {
                    u8 range = action->range;
                    result = (range == 0xff) ? 2 : 1;
                } else {
                    result = (flags & mask) ? -1 : 0;
                }
            }
        }
    }

    return result;
}
