#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "GLOBAL_CELLS.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"

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

extern struct ItemListWork *gMenuWork;

void AnimationObjects_SelectAnimationFar(s32 object, s32 mode);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
struct BattleUnit *Owner_GetStateFar(s32 owner);
struct BattleAction *BattleAction_Get(s32 action);
s32 GameFlag_TestFar(s32 message);
void UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused0, s32 unused1);
void UiIcon_PrepareObject(struct MenuEntryIcon *icon);
void PsynergyMenu_CallIconRoutineWithValue(void *work, s32 value);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 Unnamed_080a1fd4(s32 mode, s32 count, s32 page_size, s32 *row, s32 *page);
void Audio_PlayCue(s32 cue);

/*
 * types.h already supplies WaitFrames, Math_Mod, Audio_PlayCue, GameFlag_IsSet,
 * Ability_GetData, UiText_DrawAt, UiIcon_PrepareObject and
 * Object_InitializeMode; only the names it does not carry are declared here.
 */

#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_HELD_KEYS (*(volatile u32 *)ADDR_03001AE8)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

#define KEY_A 1
#define KEY_B 2
#define KEY_SELECT 4
#define KEY_R 0x100
#define KEY_L 0x200

extern u32 gFrameCount;
extern u8 Value_00000b89;

u8 ItemMenu_Collect(struct BattleUnit *owner, u16 *items, s32 mode);
void ItemMenu_DrawIcons(u16 *items, s32 style);
/* Takes a fourth argument; this caller passes the owner there as well. */
s32 ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 arg3);
s32 ItemMenu_PageResult(struct MenuResult *result, s32 pane);
s32 ItemMenu_DrawItemDetailPage(s32 window, s32 *work, struct MenuResult *result);
s32 ItemMenu_DrawNamePage(s32 window, s32 unused, struct MenuResult *result);
s32 Item_CanOwnerEquip(s32 owner, s32 item);

#define ITEM_ID_MASK 0x1ff
#define LIST_PAGE_SIZE 5

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

    menu = gMenuWork;
    result = 0;
    prev = 0;
    *((u8 *)menu + 0x25c) = result;
    UiWindow_UpdateOrCreate(&menu->list_window, 13, 3, 17, 14, 2);
    window = menu->list_window;
    done = 0;

    while (done == 0 && GameFlag_IsSet(0x150) == 0) {
        owner = Owner_GetStateFar(menu->owner_ids[pane]);
        menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
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
                        UiText_DrawAt((s32)&Value_00000b89, window, 0, 88);
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
            nav = Unnamed_080a1fd4(0, state.entry_count, LIST_PAGE_SIZE,
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

            if ((INPUT_NEW_KEYS & KEY_A) != 0
                && menu->items[state.selected_index] != 0) {
                Audio_PlayCue(173);
                result = menu->items[state.selected_index];
                done = 1;
                break;
            }
            if ((INPUT_NEW_KEYS & KEY_B) != 0) {
                Audio_PlayCue(113);
                result = -1;
                done = 1;
                break;
            }
            if ((INPUT_REPEAT_KEYS & KEY_R) != 0
                || (INPUT_REPEAT_KEYS & KEY_L) != 0) {
                if (pane == 1) {
                    Audio_PlayCue(114);
                    WaitFrames(1);
                } else {
                    Audio_PlayCue(111);
                    menu->selected_index_by_owner[menu->owner_ids[pane]] =
                        state.selected_index;
                    tab = menu->tab_index[pane];
                    do {
                        if ((INPUT_REPEAT_KEYS & KEY_R) != 0) {
                            tab++;
                        } else {
                            tab--;
                        }
                        tab = Math_Mod(tab + menu->owner_count, menu->owner_count);
                        menu->field_008 = menu->owner_table[tab];
                        menu->owner_ids[pane] = menu->owner_table[tab];
                        menu->tab_index[pane] = tab;
                        menu->item_count = ItemMenu_Collect(
                            Owner_GetStateFar(menu->owner_ids[pane]), menu->items, 0);
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
