#include "EDITION.H"
#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "ITEM.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"
#include "UI.H"
#include "TBS_EDITION.H"

/* The equipment slots' highlight: the Japanese slot names are narrower. */
#if EDITION_INTERNATIONAL
#define EQUIP_SLOT_X     1
#define EQUIP_SLOT_WIDTH 12
#else
#define EQUIP_SLOT_X     2
#define EQUIP_SLOT_WIDTH 9
#endif

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
    u8 unknown_028[4];
    void *info_window;                        /* 0x02c */
    void *equip_window;                       /* 0x030 */
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

extern u8 MsgItemPlainName;

void RenderOutput_RedrawSavedRectFar(s32 window);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void RenderOutput_ClearListFar(s32 window);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 icon, s32 x, s32 y, s32 palette);
void Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);

#define ITEM_ID_MASK 0x1ff

/* The item page for equipment: names the selected item, draws each row's
   element icon and highlight, then marks the equipment slot the selected
   item fits when it is equipped (bit 9). */
s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state)
{
    struct ItemListWork *menu;
    struct ItemDefinition *item;
    s32 base;
    s32 row;

    menu = gMenuWork;
    base = state->page * PAGE_ROWS;
    state->selected_index = base + state->row;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    /* Spanish and Italian clear the info window's top row instead. */
    UiWindow_ClearInteriorTilesFar((s32)menu->info_window, 0, 0, 224, 8);
#else
    /* Japanese clears the info window's list and names the item with
       UiText_DrawMessageAt. */
    RenderOutput_ClearListFar((s32)menu->info_window);
#endif
    WaitFrames(1);
    if (menu->items[state->selected_index] != 0) {
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar((menu->items[state->selected_index] & ITEM_ID_MASK)
                + (s32)&MsgItemPlainName,
            (s32)menu->info_window, 0, 0);
#else
        UiText_DrawMessageAt((menu->items[state->selected_index] & ITEM_ID_MASK)
                + (s32)&MsgItemPlainName,
            (s32)menu->info_window, 0, 0);
#endif
    }

    for (row = 0; row < PAGE_ROWS; row++) {
        if (row == state->row) {
            item = Item_Get(menu->items[state->selected_index] & ITEM_ID_MASK);
            if (item->element != 4) {
                UiWindow_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1, 0);
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 14);
            } else {
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 14);
            }
        } else {
            item = Item_Get(menu->items[base + row] & ITEM_ID_MASK);
            if (item->element != 4) {
                UiWindow_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1, 4);
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 15);
            } else {
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 15);
            }
        }
    }

    for (row = 0; row <= 3; row++) {
        Render_SetTilemapFlagRect((s32)menu->equip_window, EQUIP_SLOT_X, row * 2 + 1, EQUIP_SLOT_WIDTH, 1, 15);
    }

    if (menu->items[state->selected_index] & 0x200) {
        item = Item_Get(menu->items[state->selected_index] & ITEM_ID_MASK);
        switch (item->type) {
        case 1:
            Render_SetTilemapFlagRect((s32)menu->equip_window, EQUIP_SLOT_X, 1, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        case 4:
            Render_SetTilemapFlagRect((s32)menu->equip_window, EQUIP_SLOT_X, 3, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        case 3:
            Render_SetTilemapFlagRect((s32)menu->equip_window, EQUIP_SLOT_X, 5, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        case 2:
            Render_SetTilemapFlagRect((s32)menu->equip_window, EQUIP_SLOT_X, 7, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        }
    }
    WaitFrames(1);
    return 1;
}
