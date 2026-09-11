#include "types.h"
#include "scene.h"
#include "abi/shop/sel/equip.h"
#include "shop.h"

struct ShopMenuState_080b1868 {
    u8 unknown_00[0x20];
    s32 value_20;
};

struct BattleActorDefinition *Sys_Run(s32 actor_id);

void WaitFrames(s32 frames);

extern struct ShopMenuState_080b1868 *gIw;
extern char Value_00000ca2;
extern char Value_00000ca3;
extern u8 Value_00000ad0[];

s32 Shop_ConfirmEquip(s32 unit_id, s32 slot)
{
    struct ShopMenuState_080b1868 *menu = gIw;
    u8 *unit = (u8 *)Sys_Run(unit_id);
    s32 slot_offset = slot * 2 + 216;
    s32 masked = *(volatile u16 *)(unit + slot_offset) & 0x1ff;
    struct ItemDefinition *info = Item_Get(masked);
    s32 replaced;
    s32 menu_value;

    if (*(volatile u16 *)(unit + slot_offset) & 0x200)
        return 0;

    if (Sys_Apply(unit_id, masked) == 0)
        return 0;

    replaced = Sys_Apply2(unit_id, info->type);
    if (replaced != -1) {
        s32 old_offset = replaced * 2 + 216;
        u16 old_raw = *(u16 *)(unit + old_offset);
        struct ItemDefinition *old_info = Item_Get(old_raw);

        if (old_info->flags & 2)
            return 0;
    }

    Sys_Apply3(unit_id, 1);
    Sys_Do((s32)&Value_00000ca2);
    if (Sys_Check(0) != 0)
        return 0;

    Sys_Apply4(unit_id, slot);
    menu_value = menu->value_20;
    if (menu_value != 0)
        Sys_Apply5(menu_value, unit_id);

    if (info->flags & 1) {
        Sys_Do2(103);
        Sys_Run2();
        Sys_SetMode((s32)Value_00000ad0, 8, 4, 2);
        while (Sys_Check2() == 0) {
            WaitFrames(1);
        }
    }

    Sys_Do3((s32)&Value_00000ca3);
    return 1;
}
