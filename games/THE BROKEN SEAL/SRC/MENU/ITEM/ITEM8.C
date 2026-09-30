#include "TYPES.H"
#include "INVENTORY_MENU.H"
#include "ITEM.H"
#include "TBS_EDITION.H"

#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1
s32 Item_ClassifyUseMode(s32 owner, s32 item);
s32 BattleFx_HasTriggerFar(s32 item);
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
extern char MsgItemCommandUse;

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
struct ItemDefinition *Item_Get(s32);
s32 BattleFx_HasTriggerFar(s32);
void *BattleAction_Get(s32);
s32 Item_CanOwnerEquip(s32, s32);

void ItemMenu_BuildCmd(s8 *command_states)
{
    struct InventoryMenuState *menu;
    struct ItemDefinition *item;

    menu = gMenuWork;
    item = Item_Get(0x1ff & menu->selected_item);

    if (item->type == 0) {
        command_states[0] = COMMAND_AVAILABLE;
        command_states[1] = COMMAND_DISABLED;
    } else {
        command_states[0] = COMMAND_DISABLED;
        command_states[1] = COMMAND_AVAILABLE;
    }

    if (Item_ClassifyUseMode(menu->item_owner, menu->selected_item) != -1)
        command_states[0] = COMMAND_AVAILABLE;
    else
        command_states[0] = COMMAND_DISABLED;

    if (menu->selected_item & 0x400)
        command_states[0] = COMMAND_DISABLED;

    if (Item_CanOwnerEquip(
            menu->item_owner,
            menu->selected_item & 0x1ff) == 0) {
        command_states[1] = COMMAND_DISABLED;
    }

    command_states[3] = COMMAND_AVAILABLE;
    command_states[5] = COMMAND_AVAILABLE;
    command_states[2] = COMMAND_AVAILABLE;

    if (menu->selected_item & 0x200) {
        command_states[4] = COMMAND_AVAILABLE;
        command_states[1] = COMMAND_DISABLED;
    } else {
        command_states[4] = COMMAND_DISABLED;
    }

    if (item->flags & 2) {
        command_states[4] = COMMAND_DISABLED;
        if (menu->selected_item & 0x200) {
            command_states[3] = COMMAND_DISABLED;
            command_states[5] = COMMAND_DISABLED;
        }
    }

    if (BattleFx_HasTriggerFar(menu->selected_item & 0x1ff) != 0)
        command_states[0] = COMMAND_AVAILABLE;

    if (menu->party_count <= 1)
        command_states[3] = COMMAND_DISABLED;

    if (item->flags & 8)
        command_states[5] = COMMAND_DISABLED;
}

#if defined(TBS_EDITION_JA)
#define ITEM_TEXT_X 0x28
#else
#define ITEM_TEXT_X 0x20
#endif

void ItemMenu_DrawCmd(void *command_states, s32 window)
{
    s32 disabled;
    s32 value;
    u32 message;

    UiWork_SetParamNibbleFar(0xf);
    value = FIELD(command_states, s8 *, 0);
    disabled = -1;
    if (value == disabled)
        UiWork_SetParamNibbleFar(0xe);

    message = (u32)&MsgItemCommandUse;
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 0x18);
    UiWork_SetParamNibbleFar(0xf);
    if (FIELD(command_states, s8 *, 1) == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 1, window, ITEM_TEXT_X, 0x18);
    UiWork_SetParamNibbleFar(0xf);
    if (FIELD(command_states, s8 *, 3) == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 2, window, 0, 0x20);
    UiWork_SetParamNibbleFar(0xf);
    if (FIELD(command_states, s8 *, 5) == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 3, window, 0x50, 0x20);
    UiWork_SetParamNibbleFar(0xf);
    if (FIELD(command_states, s8 *, 2) == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 4, window, 0x50, 0x18);
    UiWork_SetParamNibbleFar(0xf);
    if (FIELD(command_states, s8 *, 4) == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 5, window, ITEM_TEXT_X, 0x20);
    UiWork_SetParamNibbleFar(0xf);
}

s32 Item_ClassifyUseMode(s32 owner, s32 itemId)
{
    s32 masked = itemId;
    void *itemData;
    s32 result = -1;

    masked &= 0x1ff;
    itemData = Item_Get(masked);

    if (BattleFx_HasTriggerFar(masked)!= 0) {
        return 0;
    }

    {
        void *abilityData = BattleAction_Get(FIELD_AT_OFFSET(itemData, u16, 40) & 0x3fff);

        if (FIELD_AT_OFFSET(itemData, u16, 40) != 0) {
            if (FIELD_AT_OFFSET(itemData, u8, 2) != 0) {
                if (FIELD_AT_OFFSET(itemData, u8, 12) != 3) {
                    if (Item_CanOwnerEquip(owner, masked) != 0) {
                        result = 1;
                    }
                }
            } else {
                result = 1;
            }

            if (result == 1) {
                s32 mask = 0x80;
                u8 field1 = FIELD_AT_OFFSET(abilityData, u8, 1);

                if ((field1 & 0x40) != 0) {
                    u8 field8 = FIELD_AT_OFFSET(abilityData, u8, 8);
                    result = (field8 == 0xff) ? 2 : 1;
                } else {
                    result = (field1 & mask) ? -1 : 0;
                }
            }
        }
    }

    return result;
}
