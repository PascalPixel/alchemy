#include "EDITION.H"
#include "FIXED_MATH.H"
#include "INVENTORY_MENU.H"
#include "GLOBAL_CELLS.H"
#include "INVENTORY.H"
#include "ITEM.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"
extern u8 MsgItemBroke[];

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
s32 InventoryMenu_ShowModalMessage(s32, s32, s32);
s32 Audio_PlayCue(s32);
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);

void ItemMenu_TryBreak(void)
{
    struct InventoryMenuState *menu;

    menu = gMenuWork;
    if ((Item_Get(0x1ff & menu->selected_items[0])->use_type == 2) && (Random16() < 0x2000U)) {
        Inventory_BreakFar(
            menu->pane_owner[0],
            menu->selected_slots[0]);
        Audio_PlayCue(SOUND_ITEM_BREAK);
        InventoryMenu_ShowModalMessage((s32)MsgItemBroke, 0, -1);
#if !EDITION_INTERNATIONAL
        /* The Japanese menu redraws the info window after the message. */
        RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
    }
}
