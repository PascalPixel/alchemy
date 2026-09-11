#include "scene.h"
#include "psynergy_menu.h"
#include "global_cells.h"
#include "types.h"
#include "far_runtime.h"
#include "owner_state.h"
#include "battle_types.h"

/* psynergy_menu/classify_selected_psynergy.c */
u8 *Ability_GetData(s32 action);

s32 PsynergyMenu_ClassifySelectedPsynergy(void)
{
    u8 *psynergy;
    s32 diff;
    s32 ret;

    psynergy = Ability_GetData(
        (s32)(0x3fff &
              (*(struct PsynergyMenuState **)ADDR_03001F2C)
                  ->selected_psynergy));
    if (Sys_Check(psynergy[0x0c]) != 0) {
        return 0;
    }
    ret = 2;
    if (psynergy[8] != 0xff) {
        u8 kind = psynergy[0];
        diff = kind ^ 2;
        ret = (0 - diff) | diff;
        ret = (s32)((u32)ret >> 0x1f);
        ret = 1 - ret;
    }
    return ret;
}

/* psynergy_menu/select_party_slot.c */
struct Rec5 { u8 pad[5]; unsigned int flag : 8; };
struct Cur { unsigned short mark : 8; };

extern void *gIw;

void *Sys_Run(s32);

void WaitFrames(s32);

s32 PsynergyMenu_SelectPartySlot(s32 party_slot)
{
    void *menu = gIw;
    s32 offset = party_slot + 28;
    s32 cursor_offset = party_slot * 4 + 20;
    void *icon;
    u8 byte_val;
    s32 owner_index;
    s32 combined_offset;
    s32 obj_off;
    s32 obj_id;
    void *obj_ptr;
    void *p456;
    u8 *p2;
    s32 badge;
    s32 result;
    s32 cursor_offset2;

    result = 0;
    icon = *(void **)(menu + cursor_offset);
    *(u8 *)(icon + 5) = 1;
    *(u16 *)(icon + 12) = result;
    ((struct Rec5 *)(*(u8 **)(menu + 540)))->flag = 13;
    p2 = (u8 *)menu + 2;
    owner_index = *(s8 *)(menu + offset);
    byte_val = ((struct Cur *)(menu + 537))->mark;
    p2[offset] = byte_val;

    if (owner_index == -1) {
        *(u8 *)(menu + offset) = 0;
        combined_offset = 0;
    } else {
        combined_offset = owner_index * 2;
        Sys_Apply(owner_index * 24 - 10, 16);
    }

    obj_off = combined_offset + 520;
    obj_id = *(u16 *)(menu + obj_off);
    obj_ptr = Sys_Run(obj_id);
    p456 = menu + 456;
    badge = Sys_Place(obj_ptr, p456, 2);
    *(u8 *)(menu + 536) = (u8)badge;
    result = Sys_Apply2(menu + 520, p456);

    cursor_offset2 = party_slot * 4 + 20;
    icon = *(void **)(menu + cursor_offset2);
    Sys_Do(icon);
    WaitFrames(1);
    return result;
}

/* psynergy_menu/refresh_owner_psynergy.c */
void UiText_DrawAt(s32 message, s32 *, s32 x, s32 y);

void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id)
{
    u16 *psynergies;
    struct PsynergyMenuState *menu;
    struct OwnerActionState *owner;

    menu = gIw;
    owner = (struct OwnerActionState *)OwnerState_GetFar(owner_id);
    psynergies = menu->psynergies;
    menu->psynergy_count =
        PsynergyMenu_CollectActions(owner, psynergies, 2);
    UiWindow_Commit(menu->psynergy_window);
    Sys_Place(0x6c, 0x20, 8);
    PsynergyMenu_DrawPsynergyIcons(psynergies);
    if (menu->psynergy_count == 0) {
        UiText_DrawAt(
            (s32)&PsynergyMenu_EmptyMessage,
            (s32 *)menu->psynergy_window,
            0,
            0x18);
    }
}

/* psynergy_menu/return_true.c */
s32 PsynergyMenu_ReturnTrue(void)
{
    return 1;
}

/* psynergy_menu/set_shortcut.c */
s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut)
{
    s32 id = psynergy & 0x3fff;
    s32 code =
        (s32)(((u32)owner << 10) | (u32)id);

    if (shortcut == 0) {
        gCell.psynergy_shortcuts[0] = code;
    } else {
        gCell.psynergy_shortcuts[1] = code;
    }
    return 1;
}

/* psynergy_menu/create_entry_grid.c */
s32 InitializeEntryObjects(s32, s32, s32, s32, s32);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
struct PsynergyMenuIcon *Sys_Run(s32, s32, s32);
void *Sys_unk2_4(s32, s32, s32, s32, s32, s32);
struct PsynergyMenuIcon *Sys_unk3_4(s32, s32, s32, s32, s32);

void PsynergyMenu_CreateEntryGrid(void)
{
    s32 window;
    struct PsynergyMenuState *menu;
    struct PsynergyMenuIcon *cursor;
    s32 index;
    s32 x;
    s32 y;
    struct PsynergyMenuIcon **output;

    menu = *(struct PsynergyMenuState **)ADDR_03001F2C;
    window = Sys_Check(menu);
    InitializeEntryObjects(window, 2, 2, 8, 0);

    window = UiWindow_CreateFar(0, 5, 30, 15, 2);
    menu->psynergy_window = window;
    menu->selected_column = 0;
    menu->selected_row = 0;
    menu->column_count = 8;
    menu->row_count = 2;

    cursor = Sys_Run(window, 0, 4);
    cursor->state = 13;
    menu->entry_grid_cursor = cursor;
    Sys_unk2_4(0, 0, 0, window, 0, 0);

    y = 8;
    index = 0;
    output = &menu->entry_icons[0];
    x = 96;
    do {
        *output++ = Sys_unk3_4(4, index, window, x, y);
        index++;
        x += 16;
    } while (index <= 7);

    index = 8;
    y = 24;
    output = &menu->entry_icons[8];
    x = 96;
    do {
        *output++ = Sys_unk3_4(4, index, window, x, y);
        index++;
        x += 16;
    } while (index <= 15);
}

/* psynergy_menu/close_windows.c */
s32 UiWindow_Close(s32 window, s32 mode);
void Menu_ReleaseEntryObjects(void);

void PsynergyMenu_CloseWindows(void)
{
    struct PsynergyMenuState *menu;

    menu = *(struct PsynergyMenuState **)ADDR_03001F2C;
    Menu_ReleaseEntryObjects();
    UiWindow_Close(menu->auxiliary_window, 1);
    UiWindow_Close(menu->psynergy_window, 1);
    UiWindow_Close(menu->message_window, 1);
}

/* psynergy_menu/draw_psynergy_icons.c */
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies)
{
    s32 remaining;
    struct PsynergyMenuIcon **icons;
    u16 *p;
    s32 psynergy_id;

    icons =
        (*(struct PsynergyMenuState **)ADDR_03001F2C)->entry_icons;
    p = psynergies;
    remaining = 31;
    do {
        psynergy_id = *p++;
        if (psynergy_id != 0) {
            Sys_SetMode(
                4, psynergy_id, (*icons)->render_target, 0);
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(psynergies);
}

/* psynergy_menu/collect_actions.c */
u8 *Ability_GetData(s32 action);

#define ACTION_ID_MASK 0x3FFF

/* PsynergyMenu_CollectActions per games/gs1/include/psynergy_menu.h. */
s32 PsynergyMenu_CollectActions(struct OwnerActionState *owner, u16 *actions, s32 mode)
{
    s32 n;
    u16 *out;
    s32 outerCount;
    s32 count;
    s32 i;
    s32 j;
    s32 off;

    outerCount = (mode != 2) ? 4 : 3;
    for (n = 62; n >= 0; n -= 2) {
        u16 *q = (u16 *)((u8 *)actions + n);
        *q = 0;
        *q = 0;
    }
    count = 0;

    if (mode == 1) {
        for (i = 0, off = 88, out = actions; i <= 31; i++, off += 4) {
            if (*(u16 *)(off + (s32)owner) != 0) {
                if (Ability_GetData(*(u16 *)(off + (s32)owner) & ACTION_ID_MASK)[12] != 0) {
                    *out = *(u16 *)((s32)owner + off);
                    out++;
                    count++;
                }
            }
        }
    } else {
        for (j = 0; j < outerCount; j++) {
            out = (u16 *)(count * 2 + (s32)actions);

            for (i = 0; i < 32; i++) {
                if (owner->action_slots[i].encoded_action != 0) {
                    u8 *ability = Ability_GetData(owner->action_slots[i].encoded_action & ACTION_ID_MASK);

                    if (j == 0 && (ability[12] != 0 || (ability[1] & 0x40) != 0)) {
                        *out = owner->action_slots[i].encoded_action;
                        out++;
                        count++;
                    } else if (j == 1) {
                    } else if (j == 2) {
                    } else if (j == 3 && ability[12] == 0 && (ability[1] & 0x40) == 0) {
                        *out = owner->action_slots[i].encoded_action;
                        out++;
                        count++;
                    }
                }
            }
        }
    }

    return count;
}

/* psynergy_menu/build_page_result.c */
s32 Runtime_GetObject(s32);
s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 owner_state;
    u8 *base = *(u8 **)ADDR_03001F2C;
    s32 offset = index + 0x218;
    u8 *owners = base + 2;
    s32 entry_count;
    s32 row;
    s32 page;
    s32 page_count;
    s32 selected_index;
    s32 owner_slot;

    owner_state = Runtime_GetObject(owners[offset]);
    entry_count = base[0x218];
    owner_slot = owners[offset] + 0x260;
    selected_index = ((s8 *)base)[owner_slot];
    if ((s32)(selected_index + 1) > entry_count) {
        selected_index = entry_count - 1;
    }
    page = FixedPoint_Ratio(selected_index, 5);
    row = Modulo(selected_index, 5);
    page_count = FixedPoint_Ratio(entry_count, 5);
    if (Modulo(entry_count, 5) != 0) {
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

/* psynergy_menu/draw_action_page.c */
void UiWindow_Commit(s32 window);

void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);

s32 UiPalette_SetColor(s32 color);

struct BattleUnit *Sys_Run(s32 owner);
struct BattleAction *Ability_GetData(s32 action);
extern u8 Value_00000333;

s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state)
{
    u32 first_entry;
    u32 visible_count;
    u8 row;
    s32 cursor;
    struct BattleUnit *owner;
    struct BattleAction *ability;
    struct PsynergyMenuState *menu = gIw;

    (void)unused;

    UiWindow_Commit(window);
    Sys_SetRange(window, 0, 11, 16, 11);

    if (2 & *(u16 *)((u8 *)menu + 0x220)) {
        UiText_DrawAt(0xae1, window, 0, 88);
    } else {
        UiText_DrawAt(0xb89, window, 0, 88);
    }

    first_entry = state->page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Sys_unk2_5(5, first_entry, window, 0x70, 0x22);
    Sys_unk3_5(window, state->entry_count, 5, state->page, 15);
    UiText_DrawAt(0xaed, window, 0x60, 0);

    row = 0;
    if (visible_count > row) {
        cursor = first_entry * 2 + 0x1c8;
        do {
            owner = Sys_Run(menu->owner_ids[0]);
            ability = Ability_GetData(0x3fff & *(const u16 *)(cursor + (s32)menu));

            if (ability->pp_cost > owner->pp) {
                UiPalette_SetColor(2);
            } else if (Sys_unk2_4(0x3fff & *(const u16 *)(cursor + (s32)menu)) != 0) {
                UiPalette_SetColor(4);
            } else {
                UiPalette_SetColor(15);
            }

            UiText_DrawAt(
                (0x3fff & *(const u16 *)(cursor + (s32)menu)) + (s32)&Value_00000333,
                window, 16, row * 16 + 8);
            Sys_unk4_2(ability->pp_cost, 2, window, 104, row * 16 + 8);
            UiPalette_SetColor(15);

            row++;
            cursor += 2;
        } while (visible_count > row);
    }

    return 1;
}

/* psynergy_menu/is_action_restricted.c */
u8 *Ability_GetData(u32 action);

s32 PsynergyMenu_IsActionRestricted(s32 no)
{
    u8 *action = Ability_GetData((u32)(no << 18) >> 18);
    u32 flags;

    if (action[12] != 0)
        goto restricted;
    flags = action[1] & 0xc0;
    no = 1;
    if (flags != 0xc0)
        goto done;
restricted:
    no = 0;
done:
    return no;
}
