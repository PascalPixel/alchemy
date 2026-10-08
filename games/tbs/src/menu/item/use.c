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
#include "types.h"
#include "inventory_menu.h"
#include "sound_ids.h"
#include "tbs_edition.h"
#include "menu_list.h"
#include "text/msg_ids.h"

#define InventoryMenu_ShowModalMessage ((s32 (*)(s32,s32,s32))InventoryMenu_ShowModalMessage)

s32 AudioCommand_PlayFar(s32);

s32 ItemMenu_Use(void)
{
    struct InventoryMenuState *menu;
    s32 result;

    menu = gMenuWork;
    result = Item_Use(
        menu->selected_slots[0], menu->pane_owner[0], menu->pane_owner[1]);

    if (result == -1) {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
        RenderOutput_ClearListFar(menu->info_window);
        InventoryMenu_ShowModalMessage(
            menu->message_offset + (s32)MsgItemUseResult, result, result);
#if !EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
        menu->completion_flag = 1;
        return result;
    }

    Item_PlayUseAnimation(menu->selected_items[0] & 0x1ff);
    UnitCalcFar(menu->pane_owner[0]);
    UnitCalcFar(menu->pane_owner[1]);
    return 1;
}
