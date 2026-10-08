#include "m7_interfaces.h"
#include "inventory_menu.h"
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
#include "io_reg.h"
#include "ui.h"
#include "item.h"

void ItemMenu_RefreshEntry(s32 layout)
{
    struct RenderOutput **slot;
    struct RenderOutput **scan;
    struct RenderOutput *object;
    s32 index;
    s32 origin_y;
    struct InventoryMenuState *menu;

    menu = gMenuWork;
    origin_y = 0x38;
    if (layout != 1) {
        origin_y = 0x28;
    }
    slot = menu->entry_icons;
    index = 0;
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            ItemMenu_PosOwner(slot, index, 0x74, origin_y, 5);
        }
        index++;
        slot++;
    } while (index <= 0xE);
}

#define gKeyState (*(volatile u32 *)&gKeyState)
#define RenderOutput_ClearListFar ((void (*)(void *))RenderOutput_ClearListFar)

#define UiText_GetResourceDimensionsFar ((void (*)(s32,s32*,s32*,s32*,s32*))UiText_GetResourceDimensionsFar)

void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
void UiWindow_CloseIfOpen();

void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 window;
    s32 height;
    s32 width;
    s32 top;
    s32 left;

    menu->pane_icons[0]->active = 13;
    if (y != -1) {
        UiText_GetResourceDimensionsFar(message, &left, &top, &width, &height);
        if (UiWindow_UpdateOrCreate((s32 *)&menu->modal_window, x, y, width, height, 0x102) == 0)
            UiWindow_SetBounds((struct RenderInput *)menu->modal_window, x, y, width, height);
        window = (s32)menu->modal_window;
    } else {
        window = (s32)menu->info_window;
    }
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
    RenderOutput_ClearListFar((void *)window);
#if EDITION_INTERNATIONAL
    if (y == -1)
        UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
    else
        UiText_DrawResourceFar(message, window, 0, 0);
#else
    UiText_DrawResourceFar(message, window, 0, 0);
#endif
    if (x != -1) {
        WaitFrames(1);
        do {
            WaitFrames(1);
        } while (!(gKeyState & KEY_A) && !(gKeyState & KEY_B) && !(gKeyState & KEY_START));
#if EDITION_INTERNATIONAL
        if (y == -1)
            RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
#endif
        RenderOutput_ClearListFar((void *)window);
    } else {
        GameFlag_SetBitFar(0x151);
    }
    menu->completion_flag = 1;
    menu->pane_icons[0]->active = 1;
#if EDITION_INTERNATIONAL
    if (y != -1)
        UiWindow_CloseIfOpen((s32 *)&menu->modal_window, 1);
#endif
}

struct ItemDefinition *Item_GetDirectFar(s32 item);

s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode)
{
    u16 sorted[16];
    u16 items[16];
    u8 order[32];
    struct ItemDefinition *item;
    s32 pass;
    s32 i;
    s32 count;
    s32 best;
    s32 best_slot;
    s32 sorted_count;
    s32 value;

    best_slot = 0;
    sorted_count = 0;
    Menu_CopyListOrder(mode, order);

    for (i = 0; i < 15; i++)
        items[i] = inventory[i];

    count = 0;
    for (i = 0; i < 15; i++) {
        if (items[i] != 0)
            count++;
    }

    for (i = count; i < 15; i++)
        sorted[i] = 0;

    pass = 0;
    while (order[pass] != 0xff) {
        for (;;) {
            best = 0;
            for (i = 0; i < count; i++) {
                if (items[i] == 0)
                    continue;
                item = Item_GetDirectFar(items[i]);
                if ((order[pass] & 0x7f) != item->type)
                    continue;
                if ((order[pass] & 0x80) && (items[i] & 0x200) == 0)
                    continue;
                value = items[i] & 0x1ff;
                if (best < value) {
                    best_slot = i;
                    best = value;
                }
            }
            if (best == 0)
                break;
            sorted[sorted_count] = items[best_slot];
            items[best_slot] = 0;
            sorted_count++;
        }
        pass++;
    }

    for (i = 0; i < count; i++)
        inventory[i] = sorted[i];
    return 1;
}
