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
#include "FIXED_MATH.H"
#include "INVENTORY_MENU.H"
#include "GLOBAL_CELLS.H"
#include "INVENTORY.H"
#include "ITEM.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"
#include "MENU_LIST.H"
#include "text/MSG_IDS.H"
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
