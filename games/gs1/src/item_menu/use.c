#include "scene.h"
#include "abi/item_menu/use.h"
#include "item_menu.h"
#include "battle_runtime.h"
#include "gs1_edition.h"
#include "sound_ids.h"
extern char Value_00000bef;
extern s32 Audio_PlayCue(s32);

extern s32 UiWindow_Commit(s32);

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
