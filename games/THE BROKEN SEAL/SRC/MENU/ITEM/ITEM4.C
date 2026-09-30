#include "TYPES.H"
#include "SCENE.H"
#include "LAYOUT_GUARD.H"
#include "GLOBAL_CELLS.H"
#include "ITEM_MENU.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "UI.H"
#include "INVENTORY_MENU.H"
#include "TBS_EDITION.H"
#include "BATTLE_TYPES.H"
#include "MENU_RESULT.H"

struct State_080a5534 {
    u8 padding[0x392];
    s16 values[2];
};

LAYOUT_OFFSET_GUARD(
    State_080a5534_values_offset, struct State_080a5534, values, 0x392);
LAYOUT_SIZE_GUARD(State_080a5534_size, struct State_080a5534, 0x398);
extern u8 Data_080aebcc[];
extern u8 Data_080aeb4c[];
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32, s32, const u8 *);

extern u8 Data_03001f2c[];

/* menu/item_menu/page_result.c */
s32 Owner_GetStateFar(s32);
s32 ItemMenu_Count(s32 owner);

static __inline__ u8 LoadByte(s32 base, s32 offset)
{
    return *(u8 *)(base + offset);
}

static __inline__ s8 LoadSignedByte(s32 base, s32 offset)
{
    return *(s8 *)(base + offset);
}

extern u8 MsgItemPlainName;
void RenderOutput_RedrawSavedRectFar(s32);
void RenderOutput_ClearListFar(s32);
void UiText_DrawMessageAt(s32, s32, s32, s32);
s32 Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);
extern u8 MsgItemName;
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void RenderOutput_RedrawSavedRectFar(s32 window);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 page_size, s32 page, s32 style);
void Menu_SetPageIcons(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);
s32 GameFlag_IsSet(s32 message);
void Object_InitializeMode(s32 object, s32 mode);
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;

struct MenuEntryIcon {
    u8 unknown_00[5];
    u8 state;                    /* 0x05 */
    u8 unknown_06[6];
    u16 field_0c;                /* 0x0c */
    u8 unknown_0e;
    u8 field_0f;                 /* 0x0f */
};

struct ItemListWork {
    u8 unknown_000[8];
    s32 field_008;                            /* 0x008 */
    u8 unknown_00c[8];
    struct MenuEntryIcon *pane_icon[2];       /* 0x014 */
    s8 tab_index[2];                          /* 0x01c */
    u8 unknown_01e[6];
    s32 field_024;                            /* 0x024 */
    u8 unknown_028[0x0c];
    s32 list_window;                          /* 0x034 */
    u8 unknown_038[0x0c];
    struct MenuEntryIcon *entry_grid_cursor;  /* 0x044 */
    struct MenuEntryIcon *entry_icons[32];    /* 0x048 */
    u8 unknown_0c8[0x4c];
    s32 tab_objects[4];                       /* 0x114 */
    u8 unknown_124[0x20];
    u16 tab_colors[4];                        /* 0x144 */
    u8 unknown_14c[0x28];
    u16 pane_row[2];                          /* 0x174 */
    u16 pane_action[2];                       /* 0x178 */
    u8 unknown_17c[0x4c];
    u16 items[32];                            /* 0x1c8 */
    u16 owner_table[8];                       /* 0x208 */
    u8 item_count;                            /* 0x218 */
    u8 owner_count;                           /* 0x219 */
    u8 owner_ids[2];                          /* 0x21a */
    struct MenuEntryIcon *cursor_icon;        /* 0x21c */
    u16 flags;                                /* 0x220 */
    u8 unknown_222[0x3e];
    s8 selected_index_by_owner[8];            /* 0x260 */
    u8 mode;                                  /* 0x268 */
};

void AnimationObjects_SelectAnimationFar(s32 object, s32 mode);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
struct BattleAction *BattleAction_Get(s32 action);
s32 GameFlag_TestFar(s32 message);
void UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused0, s32 unused1);
void UiIcon_PrepareObject(struct MenuEntryIcon *icon);
void PsynergyMenu_CallIconRoutineWithValue(void *work, s32 value);
void UiMenu_PositionCursor(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
#define KEY_A 1
#define KEY_B 2
#define KEY_SELECT 4
#define KEY_R 0x100
#define KEY_L 0x200
extern u32 gFrameCount;
extern u8 MsgChangeCharacterHelp;
void ItemMenu_DrawIcons(u16 *items, s32 style);

/* Takes a fourth argument; this caller passes the owner there as well. */
s32 ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 arg3);
s32 ItemMenu_PageResult(struct MenuResult *result, s32 pane);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
#define ITEM_ID_MASK 0x1ff
#define LIST_PAGE_SIZE 5
s32 ItemMenu_DrawNamePage( s32 window, s32 unused, const struct MenuResult *state);
s32 ItemMenu_DrawItemDetailPage(s32 arg0, s32 arg1, void *state);

void Resource_LoadPairedBlocks(void)
{
    struct State_080a5534 *state = *(struct State_080a5534 **)Data_03001f2c_a;
    s32 value = Resource_FindFreeEntry();

    state->values[0] = value;
    VramBlock_LoadCached(value, 128, Data_080aebcc);
    value = Resource_FindFreeEntry();
    state->values[1] = value;
    VramBlock_LoadCached(value, 128, Data_080aeb4c);
}

s32 ItemMenu_PageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    s32 base = *(s32 *)((u32)&Data_03001f2c);
    s32 offset = index + 0x218;
    s32 entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    limit = ItemMenu_Count(LoadByte(entries, offset));
    encoded = Owner_GetStateFar(LoadByte(entries, offset));
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    quotient = Math_Div(value, 5);
    remainder = Math_Mod(value, 5);
    groups = Math_Div(limit, 5);
    if (Math_Mod(limit, 5) != 0) {
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

s32 ItemMenu_DrawItemDetailPage(s32 arg0, s32 arg1, void *state)
{
    void *menu;
    s32 page;
    s32 combined;
    s32 off;
    s32 row;

    page = *(s32 *)(state + 8);
    menu = ((void *)gMenuWork);
    combined = page * 5;
    combined += *(s32 *)(state + 16);
    *(s32 *)(state + 24) = combined;

#if defined(TBS_EDITION_JA)
    RenderOutput_ClearListFar(*(s32 *)(menu + 44));
#else
    RenderOutput_RedrawSavedRectFar(*(s32 *)(menu + 44));
#endif
    WaitFrames(1);

    combined = *(s32 *)(state + 24);
    off = combined * 2 + 456;
    if (*(u16 *)((char *)menu + off) != 0) {
        s32 masked = (*(u16 *)((char *)menu + off) & 0x1ff) + (s32)&MsgItemPlainName;
#if defined(TBS_EDITION_JA)
        UiText_DrawMessageAt(masked, *(s32 *)(menu + 44), 0, 0);
#else
        UiText_DrawCharacterAtOffsetFar(masked, *(s32 *)(menu + 44), 0, 0);
#endif
    }

    row = 0;
    do {
        if (row == *(s32 *)(state + 16)) {
            Render_SetTilemapFlagRect(*(s32 *)(menu + 32), 1, row * 2 + 1, 14, 1, 14);
        } else {
            Render_SetTilemapFlagRect(*(s32 *)(menu + 32), 1, row * 2 + 1, 14, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}

#if defined(TBS_EDITION_JA)
#define PAGE_X  120
#define ENTRY_X 32
#else
#define PAGE_X  116
#define ENTRY_X 24
#endif

s32 ItemMenu_DrawNamePage(
    s32 window,
    s32 unused,
    const struct MenuResult *state)
{
    struct InventoryMenuState *menu =
        *(struct InventoryMenuState **)((u32)&Data_03001f2c);
    u32 page;
    u32 first_entry;
    u32 visible_count;
    u8 row;
    const u16 *item_id;

    (void)unused;

    RenderOutput_RedrawSavedRectFar(window);
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
                (item_id[0] & 0x1ff) + (s32)&MsgItemName,
                menu->item_window,
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

/* An empty routine after the name page; nothing in the ROM refers to it. */
void InventoryMenu_ItemNamePageNoOp(void)
{
}

/*
 * types.h already supplies WaitFrames, Math_Mod, Audio_PlayCue, GameFlag_IsSet,
 * Ability_GetData, UiText_DrawCharacterAtOffsetFar, UiIcon_PrepareObject and
 * Object_InitializeMode; only the names it does not carry are declared here.
 */

/* The item list loop, the counterpart of PsynergyMenu_RunList: browse the
   owner's items, cycle owners with L and R in the first pane, and return
   the chosen item or -1. */
s32 ItemMenu_RunList(s32 pane)
{
    struct ItemListWork *menu;
    s32 window;
    s32 changed;
    s32 nav;
    s32 tab;
    u8 i;
    s32 result;
    s32 redraw;
    s32 done;
    struct BattleUnit *owner;
    s32 prev;
    struct MenuEntryIcon *icon;
    s32 work[5];
    struct MenuResult state;

    menu = ((struct ItemListWork *)gMenuWork);
    result = 0;
    prev = 0;
    *((u8 *)menu + 0x25c) = result;
    UiWindow_UpdateOrCreate(&menu->list_window, 13, 3, 17, 14, 2);
    window = menu->list_window;
    done = 0;

    while (done == 0 && GameFlag_IsSet(0x150) == 0) {
        owner = (struct BattleUnit *)Owner_GetStateFar(menu->owner_ids[pane]);
        menu->item_count = (u8)ItemMenu_Collect(owner, menu->items, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        menu->cursor_icon->state = 13;
        ItemMenu_PageResult(&state, pane);
        UiMenu_PositionCursor(98, state.selected_index * 16 + 36);
        changed = 1;
        redraw = 1;

        while (GameFlag_IsSet(0x150) == 0) {
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
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
                    }
                    redraw = 0;
                }
                ItemMenu_DrawItemDetailPage(window, work, &state);
                menu->pane_action[pane] = menu->items[state.selected_index];
                ItemMenu_DrawEquipPreview(menu->owner_ids[pane], state.selected_index, 0, menu->owner_ids[pane]);
                if (menu->items[state.selected_index] != 0) {
                    icon = menu->entry_icons[state.selected_index];
                    icon->state = 9;
                    icon->field_0c = 0;
                    icon->field_0f = 250;
                }
                for (i = 0; i < menu->owner_count; i++) {
                    Object_InitializeMode(menu->tab_objects[i], 1);
                }
            }

            if ((gFrameCount & 31) == 0) {
                for (i = 0; i < menu->owner_count; i++) {
                    if (Item_CanOwnerEquip(menu->owner_table[i],
                            ITEM_ID_MASK & menu->items[state.selected_index]) != 0) {
                        Object_InitializeMode(menu->tab_objects[i], 3);
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
                Audio_PlayCue(173);
                result = menu->items[state.selected_index];
                done = 1;
                break;
            }
            if ((gKeyState & KEY_B) != 0) {
                Audio_PlayCue(113);
                result = -1;
                done = 1;
                break;
            }
            if ((gKeysRepeat & KEY_R) != 0
                || (gKeysRepeat & KEY_L) != 0) {
                if (pane == 1) {
                    Audio_PlayCue(114);
                    WaitFrames(1);
                } else {
                    Audio_PlayCue(111);
                    menu->selected_index_by_owner[menu->owner_ids[pane]] =
                        state.selected_index;
                    tab = menu->tab_index[pane];
                    do {
                        if ((gKeysRepeat & KEY_R) != 0) {
                            tab++;
                        } else {
                            tab--;
                        }
                        tab = Math_Mod(tab + menu->owner_count, menu->owner_count);
                        menu->field_008 = menu->owner_table[tab];
                        menu->owner_ids[pane] = menu->owner_table[tab];
                        menu->tab_index[pane] = tab;
                        menu->item_count = (u8)ItemMenu_Collect(
                            (struct BattleUnit *)Owner_GetStateFar(menu->owner_ids[pane]), menu->items, 0);
                    } while (menu->item_count == 0);
                    for (i = 0; i <= 3; i++) {
                        menu->tab_colors[i] = 30;
                    }
                    menu->tab_colors[tab] = 26;
                    break;
                }
            }
        }
    }

    UiWindow_ClearInteriorTilesFar(window, 0, 88, 120, 96);
    UiIcon_PrepareObject(menu->entry_grid_cursor);
    menu->pane_row[pane] = state.selected_index;
    menu->selected_index_by_owner[menu->owner_ids[pane]] = state.selected_index;
    menu->pane_action[pane] = result;
    if (GameFlag_IsSet(0x150) != 0) {
        result = -1;
    }
    WaitFrames(1);
    return result;
}
