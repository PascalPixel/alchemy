#include "types.h"
#include "shop.h"

struct ShopMenuState_080b1868 {
    u8 unknown_00[0x20];
    s32 value_20;
};

struct BattleActorDefinition *Func_08077008(s32 actor_id);
s32 Func_08077218(s32 unit_id, s32 item_id);
s32 Func_08077228(s32 unit_id, u8 kind);
void Func_08077050(s32 unit_id, s32 slot);
void Func_08015120(u32 unit_id, u32 mode);
void Func_080b04dc(s32 message);
s32 Func_080b0634(s32 value);
void Func_080b1dec(s32 value, s32 unit_id);
void Func_080f9010(s32 cue);
void Func_08015140(void);
s32 Func_08015038(s32 a, s32 b, s32 c, s32 d);
s32 Func_08015048(void);
void Func_080b0574(s32 message);
void WaitFrames(s32 frames);

extern struct ShopMenuState_080b1868 *Data_03001f2c;
extern char Value_00000ca2;
extern char Value_00000ca3;
extern u8 Value_00000ad0[];

s32 Shop_ConfirmEquipItem(s32 unit_id, s32 slot)
{
    struct ShopMenuState_080b1868 *menu = Data_03001f2c;
    u8 *unit = (u8 *)Func_08077008(unit_id);
    s32 slot_offset = slot * 2 + 216;
    s32 masked = *(volatile u16 *)(unit + slot_offset) & 0x1ff;
    struct ItemDefinition *info = Item_Get(masked);
    s32 replaced;
    s32 menu_value;

    if (*(volatile u16 *)(unit + slot_offset) & 0x200)
        return 0;

    if (Func_08077218(unit_id, masked) == 0)
        return 0;

    replaced = Func_08077228(unit_id, info->type);
    if (replaced != -1) {
        s32 old_offset = replaced * 2 + 216;
        u16 old_raw = *(u16 *)(unit + old_offset);
        struct ItemDefinition *old_info = Item_Get(old_raw);

        if (old_info->flags & 2)
            return 0;
    }

    Func_08015120(unit_id, 1);
    Func_080b04dc((s32)&Value_00000ca2);
    if (Func_080b0634(0) != 0)
        return 0;

    Func_08077050(unit_id, slot);
    menu_value = menu->value_20;
    if (menu_value != 0)
        Func_080b1dec(menu_value, unit_id);

    if (info->flags & 1) {
        Func_080f9010(103);
        Func_08015140();
        Func_08015038((s32)Value_00000ad0, 8, 4, 2);
        while (Func_08015048() == 0) {
            WaitFrames(1);
        }
    }

    Func_080b0574((s32)&Value_00000ca3);
    return 1;
}
