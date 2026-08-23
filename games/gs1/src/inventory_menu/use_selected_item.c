#include "inventory_menu.h"
#include "battle_runtime.h"
#include "gs1_edition.h"
#include "sound_ids.h"
extern char Value_00000bef;
extern s32 Func_080f9010(s32);
extern s32 Func_08015278(s32);
extern s32 Func_08015270(s32);
extern s32 Func_080a1d08(s32, s32, s32);
extern s32 Func_080aa448(u32);

s32 InventoryMenu_UseSelectedItem(void)
{
    struct InventoryMenuState *menu;
    s32 result;

    menu = Data_03001f2c;
    result = InventoryMenu_ApplyItemAtSlot(
        menu->selected_slot, menu->item_owner, menu->target_owner);

    if (result == -1) {
        Func_080f9010(SOUND_MENU_ERROR);
        Func_08015278(menu->info_window);
        Func_080a1d08(
            menu->message_offset + (s32)&Value_00000bef, result, result);
#if defined(GS1_EDITION_JA)
        Func_08015270(menu->info_window);
#endif
        menu->completion_flag = 1;
        return result;
    }

    Func_080aa448(menu->selected_item & 0x1ff);
    BattleUnit_Recalculate(menu->item_owner);
    BattleUnit_Recalculate(menu->target_owner);
    return 1;
}
