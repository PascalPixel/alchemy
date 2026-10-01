#include "EQUIPMENT_MENU.H"
#include "ITEM.H"
#include "FAR_RUNTIME.H"
#include "INVENTORY_MENU.H"
#include "OWNER_STATE.H"

extern s32 gFrameCount;
void AnimationObjects_SelectAnimationFar(void *, s32);


s32 UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
void UiIcon_PrepareObject(void *icon);

void EquipmentMenu_UpdateCompatibilityIndicators(void)
{
    u8 *menu;
    s8 member_index;

    menu = ((u8 *)gMenuWork);
    if ((gFrameCount & 31) == 0 && menu[0x219] != 0) {
        member_index = 0;
        do {
            s32 owner_offset = member_index * 2 + 520;

            if (Item_CanOwnerEquip(*(u16 *)(menu + owner_offset),
                                   *(u16 *)(menu + 376) & 0x1FF) != 0) {
                s32 off = member_index * 4 + 276;
                void *indicator = *(void **)(menu + off);
                AnimationObjects_SelectAnimationFar(indicator, 3);
            } else {
                s32 off = member_index * 4 + 276;
                void *indicator = *(void **)(menu + off);
                AnimationObjects_SelectAnimationFar(indicator, 1);
            }
            member_index++;
        } while (member_index < menu[0x219]);
    }
}

void EquipmentMenu_StartCompatibilityIndicators(void)
{
    u8 *menu;
    s8 member_index;

    menu = ((u8 *)gMenuWork);
    if (menu[0x219] != 0) {
        member_index = 0;
        do {
            s32 off = member_index * 4 + 276;
            void *indicator = *(void **)(menu + off);
            AnimationObjects_SelectAnimationFar(indicator, 1);
            member_index++;
        } while (member_index < menu[0x219]);
    }
    Scheduler_RemoveCallback((u32)((s32)&EquipmentMenu_UpdateCompatibilityIndicators));
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
    UiText_DrawCharacterAtOffsetFar(message, menu->message_window, 0, 0);
}

void Menu_HideEmptyEntryIcons(const u16 *items)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 slot;

    for (slot = 0; slot < 32; slot++) {
        if (items[slot] == 0) {
            UiIcon_PrepareObject(menu->entry_icons[slot]);
            menu->entry_icons[slot]->state = 13;
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
    slots = Owner_GetStateFar(owner_id)->inventory;
    remaining = 0xE;
    do {
        item_id = 0x1FF & *slots;
        slots += 1;
        if (item_id != 0) {
            count += 1;
        }
        remaining -= 1;
    } while (remaining >= 0);
    return count;
}
