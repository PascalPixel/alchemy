#define OWN_AudioCommand_PlayFar
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
#include "fixed_math.h"
#include "inventory_menu.h"
#include "global_cells.h"
#include "inventory.h"
#include "item.h"
#include "sound_ids.h"
#include "system.h"
#include "menu_list.h"
#include "text/msg_ids.h"
struct ItemDefinition *Item_GetDirectFar(s32 item);

#define InventoryMenu_ShowModalMessage ((s32 (*)(s32,s32,s32))InventoryMenu_ShowModalMessage)

s32 AudioCommand_PlayFar(s32);

void ItemMenu_TryBreak(void)
{
    struct InventoryMenuState *menu;

    menu = gMenuWork;
    if ((Item_GetDirectFar(0x1ff & menu->selected_items[0])->use_type == 2) && (Random16() < 0x2000U)) {
        Inventory_BreakFar(
            menu->pane_owner[0],
            menu->selected_slots[0]);
        AudioCommand_PlayFar(SOUND_ITEM_BREAK);
        InventoryMenu_ShowModalMessage((s32)MsgItemBroke, 0, -1);
#if !EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
    }
}
