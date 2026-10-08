#define OWN_AudioCommand_PlayFar
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "INVENTORY_MENU.H"
#include "SOUND_IDS.H"
#include "TBS_EDITION.H"
#include "MENU_LIST.H"
#include "text/MSG_IDS.H"

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
