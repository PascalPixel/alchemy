#include "scene.h"
#include "item_menu.h"
#include "battle_runtime.h"
#include "gs1_edition.h"
#include "sound_ids.h"

/* item_menu/use.c */
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

/* item_menu/init.c */
s32 UiMenu_CreateCursor(void *menu);
void InitializeEntryObjects(s32 source, s32 x, s32 y, s32 spacing, s32 style);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);

void ItemMenu_Init(void)
{
    struct ItemMenuState *menu = gIw;
    s32 index;

    InitializeEntryObjects(UiMenu_CreateCursor(menu), 2, 2, 8, 0);
    for (index = 3; index >= 0; index--)
        *(u16 *)((u8 *)menu + 324 + index * 2) = 30;

    {
        s32 zero = 0;
        s32 style;
        *(s32 *)((u8 *)menu + 40) = zero;
        *(s32 *)((u8 *)menu + 36) = zero;
        style = 2;
        *(s32 *)((u8 *)menu + 44) =
            UiWindow_CreateFar(0, 17, 30, 3, style);
        *(s32 *)((u8 *)menu + 32) = zero;
        *(u8 *)((u8 *)menu + 272) = zero;
        *(u8 *)((u8 *)menu + 273) = zero;
        *(u8 *)((u8 *)menu + 274) = 8;
        *(u8 *)((u8 *)menu + 275) = style;
    }
}
