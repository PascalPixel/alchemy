#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "tbs_edition.h"
#include "inventory_menu.h"
#include "btlsys.h"
#include "ui.h"
#include "text/msg_ids.h"

#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)

#if EDITION_INTERNATIONAL
#define SHOP_PAGE_ICONS_X 119
#define SHOP_ITEM_NAME_X  128
#define SHOP_STAT_X       80
#else
#define SHOP_PAGE_ICONS_X 123
#define SHOP_ITEM_NAME_X  136
#define SHOP_STAT_X       72
#endif

void UiText_DrawStringAtOffsetFar(s32 text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);

s32 Shop_DrawItemPage(s32 window, s32 unused, struct MenuResult *state)
{
    struct InventoryMenuState *menu = gMenuWork;
    struct BtlUnit *unit;
    u16 *item;
    s32 first;
    u32 count;
    u32 row;
    s32 message;

    unit = UnitGetFar(menu->pane_owner[0]);
    UiWindow_ClearInteriorTilesFar(window, 128, 8, 224, 96);
    first = state->page * PAGE_ROWS;
    count = (u8)(state->entry_count - first);
    if (count > PAGE_ROWS)
        count = PAGE_ROWS;
    Menu_SetPageIcons(PAGE_ROWS, first, window, SHOP_PAGE_ICONS_X, 52);
    Menu_DrawPageIndicator(window, state->entry_count, PAGE_ROWS, state->page, 28);
    if (menu->item_count == 0) {
#if defined(TBS_EDITION_DE)
        UiText_DrawCharacterAtOffsetFar((s32)MsgItemMenuEmpty, window, 112, 8);
#else
        UiText_DrawCharacterAtOffsetFar((s32)MsgItemMenuEmpty, window, 120, 8);
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
    UiText_DrawNumberInWindowFar(unit->atk, 3, window, SHOP_STAT_X, 16);
    UiText_DrawNumberInWindowFar(unit->def, 3, window, SHOP_STAT_X, 24);
    return 1;
}
