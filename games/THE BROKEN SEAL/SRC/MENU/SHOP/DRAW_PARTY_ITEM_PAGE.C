#include "EDITION.H"
#include "TYPES.H"
#include "TBS_EDITION.H"
#include "INVENTORY_MENU.H"
#include "BATTLE_RUNTIME.H"

/* The Japanese item names and stats sit closer, and the page icons a little
   further right. */
#if EDITION_INTERNATIONAL
#define SHOP_PAGE_ICONS_X 119
#define SHOP_ITEM_NAME_X  128
#define SHOP_STAT_X       80
#else
#define SHOP_PAGE_ICONS_X 123
#define SHOP_ITEM_NAME_X  136
#define SHOP_STAT_X       72
#endif

extern u8 MsgItemName[];
extern u8 MsgStatLabel[];
extern char MsgItemMenuEmpty;
extern struct InventoryMenuState *gMenuWork;
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(s32 text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 page_size, s32 page, s32 style);
void Menu_SetPageIcons(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);

s32 Shop_DrawItemPage(s32 window, s32 unused, struct MenuResult *state)
{
    struct InventoryMenuState *menu = gMenuWork;
    struct BattleUnit *unit;
    u16 *item;
    s32 first;
    u32 count;
    u32 row;
    s32 message;

    unit = Owner_GetStateFar(menu->pane_owner[0]);
    UiWindow_ClearInteriorTilesFar(window, 128, 8, 224, 96);
    first = state->page * PAGE_ROWS;
    count = (u8)(state->entry_count - first);
    if (count > PAGE_ROWS)
        count = PAGE_ROWS;
    Menu_SetPageIcons(PAGE_ROWS, first, window, SHOP_PAGE_ICONS_X, 52);
    Menu_DrawPageIndicator(window, state->entry_count, PAGE_ROWS, state->page, 28);
    if (menu->item_count == 0) {
#if defined(TBS_EDITION_DE)
        UiText_DrawCharacterAtOffsetFar((s32)&MsgItemMenuEmpty, window, 112, 8);
#else
        UiText_DrawCharacterAtOffsetFar((s32)&MsgItemMenuEmpty, window, 120, 8);
#endif
    } else {
        row = 0;
        if (count > row) {
            item = &menu->items[first];
            do {
                UiText_DrawCharacterAtOffsetFar((*item & 0x1ff) + (s32)MsgItemName,
                    window, SHOP_ITEM_NAME_X, (row << 4) + 8);
                row = (u8)(row + 1);
                item++;
            } while (count > row);
        }
    }
    UiText_DrawStringAtOffsetFar((s32)unit->name, window, 40, 0);
    message = (s32)MsgStatLabel;
    UiText_DrawCharacterAtOffsetFar(message, window, 32, 16);
    UiText_DrawCharacterAtOffsetFar(message + 1, window, 32, 24);
    UiText_DrawNumberInWindowFar(unit->attack, 3, window, SHOP_STAT_X, 16);
    UiText_DrawNumberInWindowFar(unit->defense, 3, window, SHOP_STAT_X, 24);
    return 1;
}
