#include "scene.h"
#include "item_menu.h"
#include "battle_runtime.h"
#include "gs1_edition.h"
#include "sound_ids.h"
#include "a8_state.h"
#include "object_factory.h"
#include "types.h"
#include "global_cells.h"
#include "far_runtime.h"
#include "equipment_menu.h"
#include "item.h"
#include "owner_state.h"
#include "fixed_math.h"
#include "inventory.h"
#include "m7_interfaces.h"

/* menu/item_menu/use.c */
/* menu/item_menu/use.c */
/* item_menu/use.c */
extern char Value_00000bef;
extern s32 Audio_PlayCue(s32);

extern s32 UiWindow_Commit(s32);

s32 ItemMenu_Use(void)
{
    struct ItemMenuState *menu;
    s32 result;

    menu = gIw;
    result = Item_Use(
        menu->selected_slot, menu->item_owner, menu->target_owner);

    if (result == -1) {
        Audio_PlayCue(SOUND_MENU_ERROR);
        Sys_Check(menu->info_window);
        Sys_Place(
            menu->message_offset + (s32)&Value_00000bef, result, result);
#if defined(GS1_EDITION_JA)
        UiWindow_Commit(menu->info_window);
#endif
        menu->completion_flag = 1;
        return result;
    }

    Item_PlayUseAnimation(menu->selected_item & 0x1ff);
    BattleUnit_Recalculate(menu->item_owner);
    BattleUnit_Recalculate(menu->target_owner);
    return 1;
}

/* item_menu/init.c */
s32 UiMenu_CreateCursor(void *menu);
void InitializeEntryObjects(s32 source, s32 x, s32 y, s32 spacing, s32 style);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);

void ItemMenu_Init(void)
{
    struct ItemMenuState *menu = gIw;
    s32 index;

    InitializeEntryObjects(UiMenu_CreateCursor(menu), 2, 2, 8, 0);
    for (index = 3; index >= 0; index--)
        *(u16 *)((u8 *)menu + 324 + index * 2) = 30;

    {
        s32 zero = 0;
        s32 style;
        *(s32 *)((u8 *)menu + 40) = zero;
        *(s32 *)((u8 *)menu + 36) = zero;
        style = 2;
        *(s32 *)((u8 *)menu + 44) =
            UiWindow_CreateFar(0, 17, 30, 3, style);
        *(s32 *)((u8 *)menu + 32) = zero;
        *(u8 *)((u8 *)menu + 272) = zero;
        *(u8 *)((u8 *)menu + 273) = zero;
        *(u8 *)((u8 *)menu + 274) = 8;
        *(u8 *)((u8 *)menu + 275) = style;
    }
}

/* menu/entry/spawn_icon_entries.c */
void Menu_SpawnIconEntries(struct State080a8088 *state, s32 arg1)
{
    void **output0;
    s32 index0;
    s32 fifth0;
    void **output1;
    s32 index1;
    s32 fifth1;
    void **output2;
    s32 index2;
    s32 fifth2;

    index0 = 0;
    fifth0 = 0xA8;
    output0 = &state->entries[0];
    do {
        *output0++ = Menu_Run(2, index0, arg1, 0xF8, fifth0);
        index0++;
    } while (index0 <= 7);

    index1 = 8;
    fifth1 = 0xA8;
    output1 = &state->entries[8];
    do {
        *output1++ = Menu_Run(2, index1, arg1, 0x100, fifth1);
        index1++;
    } while (index1 <= 15);

    index2 = 16;
    fifth2 = 0xA8;
    output2 = &state->entries[16];
    do {
        *output2++ = Menu_Run(2, index2, arg1, 0x100, fifth2);
        index2++;
    } while (index2 <= 31);
}

/* menu/item_menu/hide_all_icons.c */
/* item_menu/item_menu_hide_all_icons.c */
/* item_menu/icon/hide_all.c */
void ItemMenu_HideAllIcons(void)
{
    s32 hidden_state = 13;
    struct ItemMenuIcon **icons = gIw->entry_icons;
    s32 slot;

    for (slot = 31; slot >= 0; slot--) {
        struct ItemMenuIcon *icon = *icons++;
        if (icon != 0) {
            icon->state = hidden_state;
        }
    }
}

/* item_menu/icon/hide_page.c */
s32 Modulo(s32, s32);

void ItemMenu_HidePageIcons(void)
{
    struct ItemMenuState *menu = gIw;
    s32 slot = 0;
    s32 hidden_state = 13;
    struct ItemMenuIcon **icon_slot = menu->entry_icons;

    do {
        struct ItemMenuIcon *icon = *icon_slot++;

        if (icon != 0 && Modulo(slot, 5) == 0) {
            icon->state = hidden_state;
        }
        slot++;
    } while (slot <= 31);
}

/* item_menu/close.c */
void WaitFrames(s32);

void Menu_ReleaseEntryObjects(void);

void ItemMenu_Close(void)
{
    u8 *menu;
    s8 *cursor;

    menu = *(u8 **)ADDR_03001F2C;
    Menu_ReleaseEntryObjects();
    ItemMenu_HideAllIcons();
    WaitFrames(1);
    cursor = *(s8 **)(menu + 0x17C);
    cursor[5] = 0xD;
    Sys_Apply(menu + 0x10, 1);
    Sys_Apply(menu + 0x20, 1);
    Sys_Apply(menu + 0x10C, 1);
    Sys_Apply(menu + 0x24, 1);
    Sys_Apply(menu + 0x28, 1);
    Sys_Apply(menu + 0x2C, 1);
    Sys_Apply(menu + 0x30, 1);
    Sys_Apply(menu + 0x34, 1);
    Sys_Apply(menu + 0x38, 1);
    Sys_Apply(menu + 0x3C, 1);
    Sys_Apply(menu + 0x40, 1);
}

/* item_menu/prep_owner.c */
s32 Runtime_GetObject(s32);

void WaitFrames(s32);

s32 ItemMenu_PrepOwner(s32 party_slot)
{
    register void *menu = gIw;
    register s32 offset = party_slot + 28;
    s32 result = 0;
    register s32 owner_index;
    u8 *owner_slots;

    owner_index = *(s8 *)(menu + offset);
    Sys_Do(*(s32 *)(menu + 44));
    owner_slots = menu + 2;
    owner_slots[offset] = *(u8 *)(menu + 0x219);
    if (owner_index == -1) {
        *(u8 *)(menu + offset) = result;
        offset = 0;
    } else {
        offset = owner_index * 2;
        Sys_Apply(owner_index * 24 - 10, 16);
    }

    {
        s32 off = offset + 0x208;
        result = Runtime_GetObject(*(u16 *)(menu + off));
    }
    offset = (s32)(menu + 0x1C8);
    *(s8 *)(menu + 0x218) =
        (s8)ItemMenu_Collect((void *)result, (u16 *)offset, 0);
    result = Sys_Apply2(menu + 0x208, (void *)offset);
    {
        s32 cursor_offset = party_slot * 4 + 20;
        UiIcon_PrepareObject(*(s32 *)(menu + cursor_offset));
    }
    WaitFrames(1);
    return result;
}

/* item_menu/open_category.c */
void ItemMenu_OpenCategory(s32 owner_id)
{
    struct ItemMenuState *menu;

    menu = gIw;
    ItemMenu_PosCategory();
    UiWindow_Commit(menu->item_window);
    ItemMenu_DrawCategory(menu->item_window, owner_id, 0);
}

/* equipment_menu/equipment_menu_update_compatibility_indicators.c */
/* equipment_menu/update_compatibility_indicators.c */
extern u8 *gIw;
extern s32 gIw2;
void Object_InitializeMode(void *, s32);

void EquipmentMenu_UpdateCompatibilityIndicators(void)
{
    u8 *menu;
    s8 member_index;

    menu = gIw;
    if ((gIw2 & 31) == 0 && menu[0x219] != 0) {
        member_index = 0;
        do {
            s32 owner_offset = member_index * 2 + 520;

            if (Item_CanOwnerEquip(*(u16 *)(menu + owner_offset),
                                   *(u16 *)(menu + 376) & 0x1FF) != 0) {
                s32 off = member_index * 4 + 276;
                void *indicator = *(void **)(menu + off);
                Object_InitializeMode(indicator, 3);
            } else {
                s32 off = member_index * 4 + 276;
                void *indicator = *(void **)(menu + off);
                Object_InitializeMode(indicator, 1);
            }
            member_index++;
        } while (member_index < menu[0x219]);
    }
}

/* equipment_menu/start_compatibility_indicators.c */
extern u8 *gIw;
void Object_InitializeMode(void *, s32);
s32 ScheduleCallback(s32);

void EquipmentMenu_StartCompatibilityIndicators(void)
{
    u8 *menu;
    s8 member_index;

    menu = gIw;
    if (menu[0x219] != 0) {
        member_index = 0;
        do {
            s32 off = member_index * 4 + 276;
            void *indicator = *(void **)(menu + off);
            Object_InitializeMode(indicator, 1);
            member_index++;
        } while (member_index < menu[0x219]);
    }
    ScheduleCallback((s32)&EquipmentMenu_CompatibilityUpdateEntry);
}

/* item_menu/item_menu_is_special.c */
/* item_menu/is_special.c */
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

/* item_menu/draw/msg.c */
s32 UiText_DrawAt(s32, s32, s32, s32);

void ItemMenu_DrawMsg(s32 unused, s32 message)
{
    struct ItemMenuState *menu;

    menu = gIw;
    UiWindow_Commit(menu->message_window);
    UiText_DrawAt(message, menu->message_window, 0, 0);
}

/* item_menu/icon/hide_empty.c */
void Menu_HideEmptyEntryIcons(const u16 *items)
{
    struct ItemMenuState *menu = gIw;
    s32 slot;

    for (slot = 0; slot < 32; slot++) {
        if (items[slot] == 0) {
            Sys_DoItemMenuIsSpecial(menu->entry_icons[slot]);
            menu->entry_icons[slot]->state = 13;
        }
    }
}

/* item_menu/count.c */
s32 ItemMenu_Count(s32 owner_id)
{
    s32 item_id;
    s32 remaining;
    s32 count;
    u16 *slots;

    count = 0;
    slots = OwnerState_GetFar(owner_id)->inventory;
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

/* item_menu/collect.c */
s32 ItemMenu_Collect(struct OwnerInventoryState *owner, u16 *items)
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

/* item_menu/draw/icons.c */
void ItemMenu_DrawIcons(u16 *items, s32 style)
{
    s32 remaining;
    u16 *entries;
    struct ItemMenuIcon **icons;
    s32 item_id;

    icons = gIw->entry_icons;
    entries = items;
    remaining = 14;
    do {
        item_id = *entries++;
        if (item_id != 0) {
            if (style == 0) {
                ItemMenu_DrawIcon(
                    2, item_id, (*icons)->render_target, 0);
            } else {
                ItemMenu_DrawIcon(
                    7, item_id, (*icons)->render_target, 0);
            }
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(items);
}

/* item_menu/refresh_owner.c */
void UiWindow_Commit(s32 window);

void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);

void ItemMenu_RefreshOwner(s32 owner_id, s32 mode)
{
    struct ItemMenuState *menu;
    struct OwnerInventoryState *owner;
    u16 *items;

    menu = gIw;
    owner = OwnerState_GetFar(owner_id);
    items = menu->items;
    menu->item_count = ItemMenu_Collect(owner, items, 0);
    UiWindow_Commit(menu->item_window);
    Sys_DoItemMenuIsSpecial(mode);
    ItemMenu_DrawIcons(items, 0);
    if (ItemMenu_Count(owner_id) == 0)
        UiText_DrawAt(
            (s32)&ItemMenu_EmptyMsg, menu->item_window, 8, 24);
}

void ItemMenu_NoOp(void)
{
}

/* item_menu/cmd/cursor_x.c */
extern s32 ItemMenu_CommandCursorXTable[];

s32 ItemMenu_CmdCursorX(s32 column, s32 row)
{
    if (column > 2 || row > 2 || column < 0 || row < 0)
        return 0;
    return ItemMenu_CommandCursorXTable[row * 3 + column];
}

/* item_menu/cmd/cursor_y.c */
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

/* item_menu/cmd/states.c */
#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1

void ItemMenu_BuildCmd(s8 *command_states)
{
    struct ItemMenuState *menu;
    struct ItemDefinition *item;

    menu = gIw;
    item = Item_Get(0x1ff & menu->selected_item);

    if (item->type == 0) {
        command_states[0] = COMMAND_AVAILABLE;
        command_states[1] = COMMAND_DISABLED;
    } else {
        command_states[0] = COMMAND_DISABLED;
        command_states[1] = COMMAND_AVAILABLE;
    }

    if (Sys_ApplyItemMenuIsSpecial(menu->item_owner, menu->selected_item) != -1)
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

    if (Sys_CheckHideAllIcons(menu->selected_item & 0x1ff) != 0)
        command_states[0] = COMMAND_AVAILABLE;

    if (menu->party_count <= 1)
        command_states[3] = COMMAND_DISABLED;

    if (item->flags & 8)
        command_states[5] = COMMAND_DISABLED;
}

/* item_menu/cmd/labels.c */
#if defined(GS1_EDITION_JA)
#define ITEM_TEXT_X 0x28
#else
#define ITEM_TEXT_X 0x20
#endif

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void UiText_DrawAt(s32, s32, s32, s32);
extern char Value_00000b33;
#define ItemMenu_CommandUseMessage Value_00000b33

void ItemMenu_DrawCmd(void *command_states, s32 window)
{
    s32 disabled;
    s32 value;
    u32 message;

    UiPalette_SetColor(0xf);
    value = FIELD(command_states, s8 *, 0);
    disabled = -1;
    if (value == disabled)
        UiPalette_SetColor(0xe);

    message = (u32)&ItemMenu_CommandUseMessage;
    UiText_DrawAt(message, window, 0, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 1) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 1, window, ITEM_TEXT_X, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 3) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 2, window, 0, 0x20);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 5) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 3, window, 0x50, 0x20);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 2) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 4, window, 0x50, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 4) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 5, window, ITEM_TEXT_X, 0x20);
    UiPalette_SetColor(0xf);
}

/* item/classify_use_mode.c */
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct ItemDefinition *Item_GetData(s32);

void *Ability_GetData(s32);

s32 Item_ClassifyUseMode(s32 owner, s32 itemId)
{
    s32 masked = itemId;
    void *itemData;
    s32 result = -1;

    masked &= 0x1ff;
    itemData = Item_GetData(masked);

    if (Sys_CheckClassifyUseMode(masked)!= 0) {
        return 0;
    }

    {
        void *abilityData = Ability_GetData(FIELD_AT_OFFSET(itemData, u16, 40) & 0x3fff);

        if (FIELD_AT_OFFSET(itemData, u16, 40) != 0) {
            if (FIELD_AT_OFFSET(itemData, u8, 2) != 0) {
                if (FIELD_AT_OFFSET(itemData, u8, 12) != 3) {
                    if (Sys_ApplyClassifyUseMode(owner, masked) != 0) {
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

/* menu/item_menu/try_break.c */
/* menu/item_menu/try_break.c */
/* item_menu/try_break.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

u32 Random16();
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16

s32 Audio_PlayCue(s32);

void ItemMenu_TryBreak(void)
{
    void *menu;

    menu = *(void **)ADDR_03001F2C;
    if ((FIELD(Item_Get(0x1FF & FIELD(menu, u16 *, 0x178)), u8 *, 0xC) == 2) && (Rand() < 0x2000U)) {
        Inventory_BreakFar(
            FIELD(menu, u8 *, 0x21A),
            FIELD(menu, u16 *, 0x174));
        Audio_PlayCue(SOUND_ITEM_BREAK);
        Sys_Place(0xB86, 0, -1);
    }
}

/* item_menu/open_detail.c */
struct ItemMenuDetailsState {
    u8 unknown_000[0x30];
    s32 details_window;
    u8 unknown_034[0x144];
    u16 item_ids[0x100];
};

s32 ItemMenu_OpenDetail(s32 item_index)
{
    struct ItemMenuDetailsState *menu =
        (struct ItemMenuDetailsState *)gIw;

    UiWindow_UpdateOrCreate(&menu->details_window, 0, 0, 13, 10, 2);
    Sys_RunTryBreak();

    if (menu->item_ids[item_index] != 0)
        ItemMenu_DrawItemDetails(
            menu->details_window, menu->item_ids[item_index]);

    return 1;
}

/* menu/selection/select_quantity.c */
#define MENU_SUBOBJECT(menu, offset) (*(u8 **)((u8 *)(menu) + (offset)))

struct ItemMenuState;
extern struct ItemMenuState *gIw;
extern volatile s32 gIw2TryBreak;
extern volatile u32 gIw3;

s32 Menu_RunTryBreak(s32 value)
{
    s32 changed = 1;
    u8 *menu = (u8 *)gIw;
    u8 *confirmState = MENU_SUBOBJECT(menu, 540);
    s32 window;
    s32 quantity = 0;

    confirmState[5] = 13;
    window = UiWindow_CreateFar(0, 0, 30, 10, 2);
    ScheduleCallback(Menu_UpdateEntryObjectTransforms);

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
        iconState[5] = 13;
    }
    Menu_unk3_3();
    WaitFrames(1);

    goto check_exit;

adjust:
    {
        volatile s32 *keys = &gIw2TryBreak;

        if (*keys & 0x40) {
            quantity -= 1;
            changed = 1;
        }
        if (*keys & 0x80) {
            quantity += 1;
            changed = 1;
        }
    }
    WaitFrames(1);

check_exit:
    if (GameFlag_IsSet(336) != 0)
        goto done;

    if (changed != 0) {
        changed = 0;
        quantity = Modulo(quantity + 5, 5);
        Menu_Apply2(window, value);
    }

    {
        volatile u32 *keys = &gIw3;

        if (*keys & 1)
            goto done;
        if (*keys & 2) {
            quantity = -1;
            goto done;
        }
    }
    goto adjust;

done:
    UiWindow_Commit(window);
    WaitFrames(1);
    UiWindow_Close(window, 1);
    UiWindow_Commit(*(s32 *)(menu + 16));
    Menu_unk3_2(14);
    {
        s32 delay = 0xc80;

        ScheduleCallbackAfterFrames((const void *)Menu_UpdateEntryObjectTransforms, delay);
    }

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
        iconState[5] = 1;
    }
    Menu_SetMode(13, 0, 17, 10);

    return quantity;
}

/* menu/item_menu/draw_stat.c */
/* item_menu/draw/stat_delta.c */
extern u8 ItemMenu_IncreaseGlyph;
extern u8 ItemMenu_DecreaseGlyph;

void UiNumber_DrawAt(s32, s32, s32, s32, s32);

void ItemMenu_DrawStat(
    s32 delta, s32 unused, s32 window, s32 x, s32 y)
{
    s32 digits;
    s32 magnitude;

    UiNumber_DrawAt(delta, 3, window, x, y);
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
        Sys_SetMode(
            &ItemMenu_IncreaseGlyph, window, x - digits * 8 + 16, y);
    } else {
        Sys_SetMode(
            &ItemMenu_DecreaseGlyph, window, x - digits * 8 + 16, y);
    }
}

/* item_menu/win/item_5.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void ItemMenu_SetItemWin5(void)
{
    void *menu = *(void **)ADDR_03001F2C;
    struct WindowBounds *window =
        FIELD(menu, struct WindowBounds **, 0x20);

    WindowBounds_Set(window, 0xD, 5, 0x11, 0xA);
}

/* item_menu/win/item_3.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void ItemMenu_SetItemWin3(void)
{
    void *menu = *(void **)ADDR_03001F2C;
    struct WindowBounds *window =
        FIELD(menu, struct WindowBounds **, 0x20);

    WindowBounds_Set(window, 0xD, 3, 0x11, 0xA);
}

/* item_menu/win/msg_5.c */
void ItemMenu_SetMsgWin5(void)
{
    WindowBounds_Set(
        (struct WindowBounds *)gIw->message_window,
        13, 0, 17, 5);
}

/* item_menu/win/msg_6.c */
void ItemMenu_SetMsgWin6(void)
{
    WindowBounds_Set(
        (struct WindowBounds *)gIw->message_window,
        13, 0, 17, 6);
}

/* item_menu/win/msg_7.c */
void ItemMenu_SetMsgWin7(void)
{
  int state_cell;
  state_cell = ADDR_03001F2C;
  WindowBounds_Set(*((struct WindowBounds **) (((u8 *)(*((void **)state_cell))) + 0x10C)), 0xD, 0, 0x11, 7);
}

/* item_menu/win/msg_3.c */
void ItemMenu_SetMsgWin3(void)
{
    WindowBounds_Set(
        (struct WindowBounds *)gIw->message_window,
        13, 0, 17, 3);
}

/* item_menu/draw/item_head.c */
extern u8 Value_00000182;
#define ItemMenu_ItemNameMessages Value_00000182

extern s32 Runtime_GetObject(s32);

extern void UiText_DrawAt(s32, void *, s32, s32);

void ItemMenu_DrawItemHead(void)
{
    struct ItemMenuState *menu = gIw;

    ItemMenu_DrawIcon(
        2, menu->selected_item, menu->selected_item_icon->render_target, 0);
    menu->selected_item_icon->state = 1;
    menu->selected_item_icon->x = 112;
    menu->selected_item_icon->y = 8;
    UiIcon_PrepareObject(menu->selected_item_icon);
    Sys_SetMode(
        Runtime_GetObject(menu->item_owner),
        (void *)menu->message_window,
        16,
        0);
    UiText_DrawAt(
        (menu->selected_item & 0x1FF) +
            (s32)&ItemMenu_ItemNameMessages,
        (void *)menu->message_window,
        16,
        8);
}
