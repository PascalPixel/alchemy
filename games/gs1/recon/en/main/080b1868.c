#include "types.h"

#define UNIT_ITEM_SLOT(unit, idx) (*(u16 *)((u8 *)(unit) + (idx) * 2 + 216))

struct ShopMenuState_080b1868 {
    u8 unknown_00[0x20];
    s32 value_20;
};

struct BattleActorDefinition *Func_08077008(s32 actor_id);
void *Func_08077018(s32 item_id);
s32 Func_08077218(s32 unit_id, s32 item_id);
s32 Func_08077228(s32 unit_id, u8 kind);
void Func_08077050(s32 unit_id, s32 slot);
void Func_08015120(u32 unit_id, u32 mode);
void Func_080b04dc(s32 message);
s32 Func_080b0634(s32 value);
void Func_080b1dec(s32 value, s32 unit_id);
void Func_080f9010(s32 cue);
void Func_08015140(void);
void Func_08015038(s32 a, s32 b, s32 c, s32 d);
s32 Func_08015048(void);
void Func_080b0574(s32 message);
void WaitFrames(s32 frames);

extern struct ShopMenuState_080b1868 *Data_03001f2c;
extern char Value_00000ca2;
extern char Value_00000ca3;

s32 Func_080b1868(s32 unit_id, s32 slot)
{
    struct ShopMenuState_080b1868 *menu = Data_03001f2c;
    struct BattleActorDefinition *unit = Func_08077008(unit_id);
    u16 raw = UNIT_ITEM_SLOT(unit, slot);
    s32 masked = raw & 0x1ff;
    u8 *info = Func_08077018(masked);
    s32 replaced;
    s32 menu_value;

    if (raw & 0x200)
        return 0;

    if (Func_08077218(unit_id, masked) == 0)
        return 0;

    replaced = Func_08077228(unit_id, info[2]);
    if (replaced != -1) {
        u16 old_raw = UNIT_ITEM_SLOT(unit, replaced);
        u8 *old_info = Func_08077018(old_raw);

        if (old_info[3] & 2)
            return 0;
    }

    Func_08015120(unit_id, 1);
    Func_080b04dc((s32)&Value_00000ca2);
    if (Func_080b0634(0) == 0)
        return 0;

    Func_08077050(unit_id, slot);
    menu_value = menu->value_20;
    if (menu_value != 0)
        Func_080b1dec(menu_value, unit_id);

    if (info[3] & 1) {
        Func_080f9010(103);
        Func_08015140();
        Func_08015038(0xad0, 8, 4, 2);
        do {
            WaitFrames(1);
        } while (Func_08015048() == 0);
    }

    Func_080b0574((s32)&Value_00000ca3);
    return 1;
}
