#include "TYPES.H"
#include "ITEM_MENU.H"
#include "SOUND_IDS.H"
#include "TBS_EDITION.H"

/* menu/item_menu/use.c */
extern char Value_00000bef;
extern struct ItemMenuState *gMenuWork;
s32 Audio_PlayCue(s32);
void RenderOutput_RedrawSavedRectFar(s32 window);
s32 Item_Use(s32 slot, s32 owner, s32 target);
void Item_PlayUseAnimation(u32 item);
s32 RenderOutput_ClearListFar(s32 window);
s32 InventoryMenu_ShowModalMessage(s32 message, s32 a, s32 b);
void Owner_RecalculateStatsFar(s32 owner);

s32 ItemMenu_Use(void)
{
    struct ItemMenuState *menu;
    s32 result;

    menu = gMenuWork;
    result = Item_Use(
        menu->selected_slot, menu->item_owner, menu->target_owner);

    if (result == -1) {
        Audio_PlayCue(SOUND_MENU_ERROR);
        RenderOutput_ClearListFar(menu->info_window);
        InventoryMenu_ShowModalMessage(
            menu->message_offset + (s32)&Value_00000bef, result, result);
#if defined(TBS_EDITION_JA)
        RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
        menu->completion_flag = 1;
        return result;
    }

    Item_PlayUseAnimation(menu->selected_item & 0x1ff);
    Owner_RecalculateStatsFar(menu->item_owner);
    Owner_RecalculateStatsFar(menu->target_owner);
    return 1;
}
