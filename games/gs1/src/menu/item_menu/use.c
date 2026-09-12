#include "types.h"
#include "item_menu.h"
#include "sound_ids.h"
#include "gs1_edition.h"

/* menu/item_menu/use.c */
extern char Value_00000bef;
extern struct ItemMenuState *gIw;
s32 Audio_PlayCue(s32);
void UiWindow_Commit(s32 window);
s32 Item_Use(s32 slot, s32 owner, s32 target);
void Item_PlayUseAnimation(u32 item);
s32 Sys_Check(s32 window);
s32 Sys_Place(s32 message, s32 a, s32 b);
void BattleUnit_Recalculate(s32 owner);

s32 ItemMenu_Use(void)
{
    struct ItemMenuState *menu;
    s32 result;

    menu = gIw;
    result = Item_Use(
        menu->selected_slot, menu->item_owner, menu->target_owner);

    if (result == -1) {
        Audio_PlayCue(SOUND_MENU_ERROR);
        Sys_Check(menu->info_window);
        Sys_Place(
            menu->message_offset + (s32)&Value_00000bef, result, result);
#if defined(GS1_EDITION_JA)
        UiWindow_Commit(menu->info_window);
#endif
        menu->completion_flag = 1;
        return result;
    }

    Item_PlayUseAnimation(menu->selected_item & 0x1ff);
    BattleUnit_Recalculate(menu->item_owner);
    BattleUnit_Recalculate(menu->target_owner);
    return 1;
}
