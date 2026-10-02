#include "EDITION.H"
#include "TYPES.H"
#include "INVENTORY_MENU.H"
#include "SOUND_IDS.H"
#include "TBS_EDITION.H"

/* menu/item_menu/use.c */
extern char MsgItemUseResult;
s32 Audio_PlayCue(s32);
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
s32 Item_Use(s32 slot, s32 owner, s32 target);
void Item_PlayUseAnimation(u32 item);
void RenderOutput_ClearListFar(void *window);
s32 InventoryMenu_ShowModalMessage(s32 message, s32 a, s32 b);
void Owner_RecalculateStatsFar(s32 owner);

s32 ItemMenu_Use(void)
{
    struct InventoryMenuState *menu;
    s32 result;

    menu = gMenuWork;
    result = Item_Use(
        menu->selected_slots[0], menu->pane_owner[0], menu->pane_owner[1]);

    if (result == -1) {
        Audio_PlayCue(SOUND_MENU_ERROR);
        RenderOutput_ClearListFar(menu->info_window);
        InventoryMenu_ShowModalMessage(
            menu->message_offset + (s32)&MsgItemUseResult, result, result);
#if !EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
        menu->completion_flag = 1;
        return result;
    }

    Item_PlayUseAnimation(menu->selected_items[0] & 0x1ff);
    Owner_RecalculateStatsFar(menu->pane_owner[0]);
    Owner_RecalculateStatsFar(menu->pane_owner[1]);
    return 1;
}
