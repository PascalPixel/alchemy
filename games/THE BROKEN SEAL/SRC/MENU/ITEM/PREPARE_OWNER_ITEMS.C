#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "INVENTORY_MENU.H"
#include "BATTLE_RUNTIME.H"
#include "SYSTEM.H"
#include "MENU_LIST.H"


s32 ItemMenu_PrepOwner(s32 pane)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 result = 0;
    s32 index;
    struct BtlUnit *owner;

    index = menu->pane_index[pane];
    RenderOutput_ClearListFar(menu->info_window);
    menu->pane_count[pane] = menu->party_count;
    if (index == -1) {
        menu->pane_index[pane] = result;
        index = 0;
    } else {
        UiMenu_SlideCursor(index * 24 - 10, 16);
    }
    owner = UnitGetFar(menu->owner_ids[index]);
    menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
    result = ItemMenu_RunOwnerSelection(menu->owner_ids, menu->items);
    UiIcon_PrepareObject(menu->pane_icons[pane]);
    WaitFrames(1);
    return result;
}
